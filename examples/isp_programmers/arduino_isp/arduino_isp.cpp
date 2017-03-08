/**
 * arduino_isp.cpp - stk500v1 isp avr programmer
 *
 * Copyright (c) 2008-2011 Randall Bohn
 * If you require a license, see
 * http://www.opensource.org/licenses/bsd-license.php
 *
 * same as arduino_isp.ino ported to fabooh
 */

#include <fabooh.h>
#include <main.h>

#include "configuration.h"
#include "bitbangedspi.h"
#include "stk500k.h"

#define beget16(addr) ((*addr) << 8 | *(addr+1) )

int error = 0;        // flag indicates error(s) count
int pmode = 0;        // true/false flag indicating in programming mode
bool rst_active_high; // device reset mode (atmega/attiny reset when low) (at89sx high)
unsigned int here;    // address for reading and writing, set by 'U' command
uint8_t buff[256];    // global block storage
parameter param;      // device parameter structure values

//HardwareTimer external_clk_timer(1);  // optional rescue external clock PA8 is on TIMER1
_PIN_SCK    PIN_SCK;    /* these 4 pins were selected because they are 5V tolerant */
_PIN_MISO   PIN_MISO;   /* we use bitbanged spi to make life simpler */
_PIN_MOSI   PIN_MOSI;
_PIN_RESET  PIN_RESET;

_LED_HB     LED_HB;    /* (led builtin) shows the programmer is running */
_LED_ERR    LED_ERR;   /* (red led) Lights up if something goes wrong */
_LED_PMODE  LED_PMODE; /* (blue led) In communication with the slave */

_CLK_PIN    CLK_PIN;   /* ~1MHz rescue clock */
                       /* useful to recover bad fuse setting */

BitBangedSPI<_PIN_SCK,_PIN_MISO,_PIN_MOSI> SPI;  // use a bitbang SPI use the 5v tolerant pins SPI

namespace {
  typedef serial_usart_isr_t<BAUD_RATE, CPU::frequency, TX1_PIN, RX1_PIN > serial;
  serial Serial;
}

USART_IRQHandler(1, Serial)

// function prototypes
void avrisp(void);
void commit(unsigned int addr);
unsigned current_page();
char eeprom_read_page(unsigned length);
void end_pmode(void);
void external_clock_begin(void);
char flash_read_page(unsigned length);
uint8_t flash_read(uint8_t hilo, unsigned int addr);
void flash(uint8_t hilo, unsigned int addr, uint8_t data);
void fill(int n);
uint8_t getch(void);
void get_parameter(uint8_t c);
unsigned get_u16_hilo(void);
unsigned get_u16_lohi(void);
void heartbeat(void);
void prog_lamp(int state, bool useDelay=false);
void program_page(void);
void read_page(void);
void read_signature(void);
void reply(void);
void reply_u8(uint8_t c);
void reset_target(bool reset);
void set_parameters(void);
uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
void start_pmode(void);
void universal(void);
uint8_t write_eeprom_chunk(unsigned start, unsigned length);
uint8_t write_eeprom(unsigned length);
uint8_t write_flash_pages(unsigned length);
void write_flash(unsigned length);

//----------------------------------------------------------------------
template<typename PIN>
void pulse(int times) {
  PIN pin;
  do {
    pin.high();
    delay(PULSE_TIME);
    pin.low();
    delay(PULSE_TIME);
  } while (--times);
}

//---------------------------------------------------------------------------
void setup() {
  // configure UART using a IRQ and ringbuffer .. TBD move to Serial.h

  Serial.begin(BAUD_RATE);                  // configure serial device

  NVIC_EnableIRQ(USART1_IRQn);              // NVIC usart setup
  Serial.enable_rx_isr();

  // configure status lights
  LED_PMODE.setmode_output();
  pulse<_LED_PMODE>(3);

  LED_ERR.setmode_output();
  pulse<_LED_ERR>(3);

  LED_HB.setmode_output();
  pulse<_LED_HB>(3);
}

//---------------------------------------------------------------------------
void loop(void) {
  // is pmode active?
  digitalWrite(LED_PMODE, pmode ? HIGH : LOW );

  // is there an error?
  digitalWrite(LED_ERR, error ? HIGH : LOW );

  // light the heartbeat LED
  heartbeat();

  if ( Serial.available() ) {
    avrisp();
  }
}

//---------------------------------------------------------------------------
//   S T K 5 0 0 V 1   P r o t o c o l

void avrisp() {
  uint8_t ch = getch();

  switch (ch) {
    case Cmnd_STK_GET_SYNC:      // 0x30 '0'
      error = 0;
      reply();
      break;

    case Cmnd_STK_GET_SIGN_ON:   // 0x31 '1'
      if (getch() == CRC_EOP) {
        Serial.print((char)STK_INSYNC);
        Serial.print("AVR ISP");
        Serial.print((char)STK_OK);
      }
      else {
        error++;
        Serial.write(STK_NOSYNC);
      }
      break;

    case Cmnd_STK_SET_PARAMETER: // 0x40 '@'
      fill(2); // parm, value TBD ... do something
      reply();
      break;

    case Cmnd_STK_GET_PARAMETER: // 0x41 'A':
      get_parameter(getch());
      break;

    case Cmnd_STK_SET_DEVICE:    // 0x42 'B':
      fill(20);
      set_parameters();
      reply();
      break;

    case Cmnd_STK_SET_DEVICE_EXT: // 0x45 'E': // extended parameters - ignore for now
      fill(5);
      reply();
      break;

    case Cmnd_STK_ENTER_PROGMODE: // 0x50 'P':
      if (!pmode)
        start_pmode();
      reply();
      break;

    case Cmnd_STK_LEAVE_PROGMODE: // 0x51 'Q'
      error = 0;
      end_pmode();
      reply();
      break;

    case Cmnd_STK_LOAD_ADDRESS:   // 0x55 'U': // set address (word)
      here = get_u16_lohi();
      reply();
      break;

    case Cmnd_STK_UNIVERSAL:      // 0x56 'V'
      universal();
      break;

    case Cmnd_STK_PROG_FLASH:     // 0x60: '`'
      getch(); // low addr
      getch(); // high addr
      reply();
      break;

    case Cmnd_STK_PROG_DATA:      // 0x61:  'a'
      getch(); // data
      reply();
      break;

    case Cmnd_STK_PROG_PAGE:      // 0x64: 'd'
      program_page();
      break;

    case Cmnd_STK_READ_PAGE:      // 0x74: 't'
      read_page();
      break;

    case Cmnd_STK_READ_SIGN:      // 0x75: 'u'
      read_signature();
      break;

    // expecting a command, not CRC_EOP
    // this is how we can get back in sync
    case CRC_EOP:
      error++;
      Serial.write(STK_NOSYNC);
      break;

    // anything else we will return STK_UNKNOWN
    default:
      error++;
      if (CRC_EOP == getch())
        Serial.write(STK_UNKNOWN);
      else
        Serial.write(STK_NOSYNC);
  }
}

//----------------------------------------------------------------------
// spew a ~1MHz square wave, useful as an external rescue xtal

void external_clock_begin() {
#if 0
  pinMode(PA8, PWM);
  external_clk_timer.pause();
  external_clk_timer.setPrescaleFactor(1);
  external_clk_timer.setOverflow(71); /* 72MHz/72 = 1MHz period clock */
  pwmWrite(PA8, 35);                 /* 50% duty cycle (500ns bit clock) */
  external_clk_timer.resume();
#endif
}

//----------------------------------------------------------------------
// this provides a heartbeat, so you can tell the software is running.

void heartbeat() {
  static unsigned long last_time = 0;
  static unsigned hb_state = 1;

  unsigned long now = millis();

  if ( (now - last_time) < 250 )
    return;

  last_time = now;
  digitalWrite(LED_HB, hb_state);
  hb_state ^= 1;
}

//----------------------------------------------------------------------
void prog_lamp(int state, bool useDelay) {
  if (PROG_FLICKER) {
    if ( useDelay )
      delay(PULSE_TIME);

    digitalWrite(LED_PMODE, state);
  }
}

//----------------------------------------------------------------------
void start_pmode() {

  // Reset target before driving PIN_SCK or PIN_MOSI

  reset_target(true); // force the pin low

  // SPI.begin() will configure SS as output,
  // so SPI master mode is selected.
  // We have defined RESET as pin 10,
  // which for many arduino's is not the SS pin.
  // So we have to configure RESET as output here,
  // (reset_target() first sets the correct level)

  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));

  // See avr datasheets, chapter "SERIAL_PRG Programming Algorithm":

  // Pulse RESET after PIN_SCK is low:
  digitalWrite(PIN_SCK, LOW);
  delay(50); // discharge PIN_SCK, value arbitrarily chosen
  reset_target(false); // let it float high
  // Pulse must be minimum 2 target CPU clock cycles
  // so 500 usec is overkill for CPU speeds above 20KHz
  delayMicroseconds(100);
  reset_target(true); // force it low

  // Send the enable programming command:
  delay(30); // datasheet: must be > 20 msec
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
  pmode = 1;
  delay(30); // datasheet: must be > 20 msec
}

//----------------------------------------------------------------------
void end_pmode() {
  SPI.end();

  // We're about to take the target out of reset
  // so configure SPI pins as input
  reset_target(false); // switch to input mode, let pin float high to run

  pmode = 0;
}

//----------------------------------------------------------------------
uint8_t spi_transaction(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  SPI.transfer(a);
  SPI.transfer(b);
  SPI.transfer(c);
  return SPI.transfer(d);
}

//--------------------------------------------------------------------------------
uint8_t getch(void) {
  int c;

  while ( (c = Serial.read()) < 0 ) {
    __WFI();
  }

  return c;
}

//----------------------------------------------------------------------
unsigned get_u16_hilo() {
  unsigned hi=getch();
  unsigned lo=getch();

  return (hi << 8) | lo;
}

//----------------------------------------------------------------------
unsigned get_u16_lohi() {
  unsigned lo=getch();
  unsigned hi=getch();

  return (hi << 8) | lo;
}

//----------------------------------------------------------------------
void fill(int n) {
  for (int x = 0; x < n; x++) {
    buff[x] = getch();
  }
}

//----------------------------------------------------------------------
// reply() with empty response

void reply(void) {
  if (CRC_EOP == getch()) {
    Serial.write(STK_INSYNC);
    Serial.write(STK_OK);
  }
  else {
    error++;
    Serial.write(STK_NOSYNC);
  }
}

//----------------------------------------------------------------------
// reply with single byte

void reply_u8(uint8_t c) {
  if (CRC_EOP == getch()) {
    Serial.write(STK_INSYNC);
    Serial.write(c);
    Serial.write(STK_OK);
  }
  else {
    error++;
    Serial.write(STK_NOSYNC);
  }
}

//----------------------------------------------------------------------
void get_parameter(uint8_t c) {
  switch (c) {
    case Parm_STK_HW_VER: // 0x80:
      reply_u8(HWVER);
      break;

    case Parm_STK_SW_MAJOR: // 0x81:
      reply_u8(SWMAJ);
      break;

    case Parm_STK_SW_MINOR: // 0x82:
      reply_u8(SWMIN);
      break;

    case Parm_STK_VTARGET: // 0x84:
      reply_u8(33); // 3.3 volts TBD read actual voltage * 10
      break;

    case Parm_STK_SCK_DURATION: // 0x89
      reply_u8((uint8_t)SPI.get_pulseWidth());
      break;

    case Parm_STK_PROGMODE: // 0x93:
      reply_u8('S'); // Serial Programmer
      break;

    default:
      reply_u8(0);
  }
}

//----------------------------------------------------------------------
void set_parameters() {
  // call this after reading paramter packet into buff[]
  param.devicecode = buff[0];
  param.revision   = buff[1];
  param.progtype   = buff[2];
  param.parmode    = buff[3];
  param.polling    = buff[4];
  param.selftimed  = buff[5];
  param.lockbytes  = buff[6];
  param.fusebytes  = buff[7];
  param.flashpoll  = buff[8];
  // ignore buff[9] (= buff[8])
  // following are 16 bits (big endian)
  param.eeprompoll = beget16(&buff[10]);
  param.pagesize   = beget16(&buff[12]);
  param.eepromsize = beget16(&buff[14]);

  // 32 bits flashsize (big endian)
  param.flashsize = buff[16] * 0x01000000
                    + buff[17] * 0x00010000
                    + buff[18] * 0x00000100
                    + buff[19];

  // avr devices have active low reset, at89sx are active high
  rst_active_high = (param.devicecode >= 0xe0);
}

/*----------------------------------------------------------------------
   reset_target(bool reset)

   toogle reset pin high or low, 5V tolerant using external pull up

   reset action when:
    true  - sets pin low for chips that run when the reset pin is high
            and high for chips that run when the reset pin is low

    false - goes into HI-Z for chips that run when reset pin is high
            sets pin low for chips that run when reset pin is low
*/

void reset_target(bool reset) {
  if ( rst_active_high ) { // only at89sx chips
    digitalWrite(PIN_RESET, (reset) ? HIGH : LOW);
  }
  else {                   // atmega/attiny chips
    if ( reset ) {
      //pinMode(RESET, OUTPUT_OPEN_DRAIN);
      pinMode(PIN_RESET, OUTPUT);
      digitalWrite(PIN_RESET, LOW);
    }
    else {
      digitalWrite(PIN_RESET, HIGH);
      pinMode(PIN_RESET, INPUT);  // let the pin float so we can use either 3.3 or 5v
    }
  }
}

//----------------------------------------------------------------------
void universal() {
  uint8_t ch;

  fill(4);
  ch = spi_transaction(buff[0], buff[1], buff[2], buff[3]);
  reply_u8(ch);
}

//----------------------------------------------------------------------
void flash(uint8_t hilo, unsigned int addr, uint8_t data) {
  spi_transaction(0x40 + 8 * hilo,
                  (addr >> 8) & 0xFF,
                  addr & 0xFF,
                  data);
}

//----------------------------------------------------------------------
void commit(unsigned int addr) {
  prog_lamp(LOW);

  spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);

  prog_lamp(HIGH);
}

//----------------------------------------------------------------------
unsigned current_page() {
  if (param.pagesize == 32) {
    return here & 0xFFFFFFF0;
  }
  else if (param.pagesize == 64) {
    return here & 0xFFFFFFE0;
  }
  else if (param.pagesize == 128) {
    return here & 0xFFFFFFC0;
  }
  else if (param.pagesize == 256) {
    return here & 0xFFFFFF80;
  }
  return here;
}

//----------------------------------------------------------------------
void write_flash(unsigned length) {
  fill(length);
  if (CRC_EOP == getch()) {
    Serial.write(STK_INSYNC);
    Serial.write(write_flash_pages(length));
  }
  else {
    error++;
    Serial.write(STK_NOSYNC);
  }
}

//----------------------------------------------------------------------
uint8_t write_flash_pages(unsigned length) {
  unsigned x = 0;
  unsigned int page = current_page();

  while (x < length) {
    if (page != current_page()) {
      commit(page);
      page = current_page();
    }
    flash(LOW, here, buff[x]); x++;
    flash(HIGH, here, buff[x]); x++;
    here++;
  }

  commit(page);

  return STK_OK;
}

//----------------------------------------------------------------------
uint8_t write_eeprom(unsigned length) {
  // here is a word address, get the byte address
  unsigned start = here * 2;
  unsigned remaining = length;

  if (length > param.eepromsize) {
    error++;
    return STK_FAILED;
  }

  while (remaining > EECHUNK) {
    write_eeprom_chunk(start, EECHUNK);
    start += EECHUNK;
    remaining -= EECHUNK;
  }
  write_eeprom_chunk(start, remaining);

  return STK_OK;
}

//----------------------------------------------------------------------
// write (length) bytes, (start) is a byte address

uint8_t write_eeprom_chunk(unsigned start, unsigned length) {
  // this writes byte-by-byte,
  // page writing may be faster (4 bytes at a time)
  fill(length);
  prog_lamp(LOW);
  for (unsigned int x = 0; x < length; x++) {
    unsigned int addr = start + x;
    spi_transaction(0xC0, (addr >> 8) & 0xFF, addr & 0xFF, buff[x]);
    delay(45);
  }
  prog_lamp(HIGH);

  return STK_OK;
}

//----------------------------------------------------------------------
void program_page() {
  char result = (char) STK_FAILED;
  unsigned int length = get_u16_hilo();
  char memtype = getch();
  // flash memory @here, (length) bytes
  if (memtype == 'F') {
    write_flash(length);
    return;
  }
  else if (memtype == 'E') {
    result = (char)write_eeprom(length);
    if (CRC_EOP == getch()) {
      Serial.write(STK_INSYNC);
      Serial.write(result);
    } else {
      error++;
      Serial.write(STK_NOSYNC);
    }
    return;
  }
  else {
    Serial.write(STK_FAILED);
    return;
  }
}

//----------------------------------------------------------------------
uint8_t flash_read(uint8_t hilo, unsigned int addr) {
  return spi_transaction(0x20 + hilo * 8,
                         (addr >> 8) & 0xFF,
                         addr & 0xFF,
                         0);
}

//----------------------------------------------------------------------
char flash_read_page(unsigned length) {
  if ( length > sizeof(buff)) {
    while(1); // trap if length greater than buffer
  }

  for (unsigned x = 0; x < length; x += 2) {
    uint8_t low = flash_read(LOW, here);
    uint8_t high = flash_read(HIGH, here);
    Serial.write(low);
    Serial.write(high);
    here++;
  }

  return STK_OK;
}

//----------------------------------------------------------------------
char eeprom_read_page(unsigned length) {
  // here again we have a word address
  unsigned start = here * 2;
  for (unsigned x = 0; x < length; x++) {
    unsigned addr = start + x;
    uint8_t ee = spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
    Serial.write(ee);
  }
  return STK_OK;
}

//----------------------------------------------------------------------
void read_page() {
  char result = STK_FAILED;
  unsigned length = get_u16_hilo();
  char memtype = getch();
  if (CRC_EOP != getch()) {
    error++;
    Serial.write(STK_NOSYNC);
    return;
  }
  Serial.write(STK_INSYNC);
  if (memtype == 'F') result = flash_read_page(length);
  if (memtype == 'E') result = eeprom_read_page(length);
  Serial.write(result);
}

//----------------------------------------------------------------------
void read_signature() {
  if (CRC_EOP != getch()) {
    error++;
    Serial.write(STK_NOSYNC);
    return;
  }
  Serial.write(STK_INSYNC);
  uint8_t high = spi_transaction(0x30, 0x00, 0x00, 0x00);
  Serial.write(high);
  uint8_t middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
  Serial.write(middle);
  uint8_t low = spi_transaction(0x30, 0x00, 0x02, 0x00);
  Serial.write(low);
  Serial.write(STK_OK);
}
