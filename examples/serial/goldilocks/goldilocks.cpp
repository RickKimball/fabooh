/*
 * goldilocks.cpp - An iterative approach to a more accurate DCO clock using the serial port
 *
 * Desc: This code uses the serial port as an accurate time reference to allow you to
 *       calibrate your DCO clock.
 *
 *       Use a terminal program (putty/screen/hypterminal) to connect this program.
 *       This code waits for you to type a single 'U' character. When received,
 *       it calculates the bit period for each edge transition. It then suggests
 *       value adjustments to your DCOCTL to achieve a more accurate DCO clock.
 *       This technique doesn't require you to solder any XTALS to achieve better
 *       accuracy.
 *
 *       The 'U' ASCII character is special because its value is 0x55 or binary
 *       0b01010101. We use it to capture each edge transition using Timera's
 *       edge capture feature. Serial data is transmitted least significant bit to
 *       most significant. So a 'U' starts with a '1' (lsb of 'U') and then shifts
 *       each bit in until it has sent 8 bits. The serial pins are normally high.
 *       A new RX byte can be detected when the pin goes from high to low. This
 *       is the called the start bit.  Our serial frame is 1 start bit + 8 data
 *       bits + one stop bit (goes back to idling high).
 *
 *       The timera peripheral detects changes in the pin state and triggers an
 *       interrupt providing you the time the transition occurred. This code
 *       keeps track of the transition times for each bit. We average the
 *       duration and decide if our clock is accurate. If your host is sending
 *       bits accurately, the duration of the ones to zeros should be
 *       DCO frequency / baud rate = time period between transitions.
 *
 *       For example: 16000000 Hz / 9600 bps == 1666 cycles or bit period
 *
 *       We ignore the fractional part and just average the duration
 *       for all bits. The actual frequency of your DCO is computed
 *       by multiplying this averaged bit_duration * bps:
 *
 *       1666*9600 = 15993600 Hz
 *
 *       You can tweak the DCOCTL value up or down by setting it in your setup()
 *       function and achieve a more accurate DCO clock speed using this technique.
 *
 *       This code assumes the host serial port is configured for 9600-8-N-1.
 *
 * Author: rick@kimballsoftware.com
 * Created: 03/30/2013
 *
 * Note: A special thanks to g0rdon from IRC. This code was inspired by his questions
 *       about edge capture for IR communications using the timerA peripheral. Not
 *       having an IR capture IC handy, I used the serial port to generate some wave forms
 *       using 'U' to test his code. This made me think about using the serial port as a no
 *       setup, quality reference clock to calibrate the DCO. The result is this code.
 */
#include <fabooh.h>
#include <main.h>
#include <serial.h>

#ifndef __MSP430G2553__

 void setup() {} // only works for msp430g2553
 void loop() {}

#else

const unsigned bps=9600;
const unsigned bit_duration = CPU::frequency/bps; // clock cycles per bit
const unsigned howmany=1+8+1; // 1 start bit + 8 data bits + 1 stop bit

volatile uint16_t cnts[howmany];  // cycle count for each bit
volatile uint16_t bits;           // received bits
volatile unsigned bit_indx;

/*
 *
 */
void setup(void) {
  serial_default_t<bps,CPU::frequency,TX_PIN,NO_PIN> Serial;

  BCSCTL1 = *(uint8_t *)(0x10F9);
  DCOCTL  = *(uint8_t *)(0x10F8);

#define TWEAK_DCO 1 /* to adjust DCO frequency */
#if defined(TWEAK_DCO) && TWEAK_DCO == 1
  DCOCTL = 0xAA; // this is best 16MHz setting for my chip
#endif

  Serial.begin(bps);

  P1_1::setmode_inputpullup();
  P1_1::PSEL() |= P1_1::pin_mask; // configure as CCI0A timera input

  TA0CCTL0 = CM_3 | CCIS_0 | SCS | CAP | CCIE; /* Both edges, CCI0A (P1.1), synchronous, capture, interrupt enable */
  TA0CTL = TASSEL_2 | ID_0 | MC_2 | TACLR;     /* SMCLK,  / by 1, continuous, clear TAR */

  __enable_interrupt();
}

/*
 *
 */
void loop() {
  serial_default_t<bps,CPU::frequency,TX_PIN,NO_PIN> Serial;

    bits = bit_indx = 0; // reset and try again

    Serial << endl
           << "Press a single 'U' to test the factory calibrated DCO value for " << CPU::frequency << " Hz" << endl
           << "(press reset and try multiple times)" << endl;

    // sleep until user presses the 'U' keyboard key
    LPM0;

    // display results
    uint16_t i, avg=0, mask=0x01;

    Serial << "bit val ts  duration" << endl;
    for (i = 0; i < bit_indx; i++) {
      Serial << "[" << i << "] = " <<  (bits & mask ? "1 " : "0 ") << cnts[i];
      mask <<= 1;
      if ( i > 0 ) {
        unsigned bdur = cnts[i]-cnts[i-1];
        Serial << " " << bdur;
        avg += bdur;
      }
      if ( i == 0 ) Serial << " (start bit)";
      if ( i == 9 ) Serial << " (stop bit)";
      Serial << endl;
    }

    // compute tweaks to the DCO to make it more accurate
    // you will probably run this multiple times to get it right

    avg /= howmany-1;

    Serial << endl
           << "ideal bit duration is: " << bit_duration << endl;
    Serial << " your bit duration is: " << avg << " which is " << (uint32_t)avg*bps << " Hz" << endl
           << endl;

    if ( avg == bit_duration ) {
      Serial << "Your DCO is just right!" << endl;
    }
    else if ( avg < bit_duration ) {
        Serial << "Your DCO is too cold, add this line to your setup():\n" << endl
               << "  DCOCTL=0x" << _HEX(DCOCTL+1) << "; // slightly faster" << endl;
    }
    else { /* avg > bit_duration */
        Serial << "Your DCO is too hot, add this line to your setup():\n" << endl
               << "  DCOCTL=0x" << _HEX(DCOCTL-1) << "; // slightly slower" << endl;
    }
    Serial << "  // DCOCTL=0x" << _HEX(DCOCTL) << "; // your current DCOCTL setting" << endl;
}

/*
 * capture edge change times from P1_1
 */
__attribute__ ((interrupt(TIMER0_A0_VECTOR)))
void timer_capture_isr(void) {
  static unsigned bitmask;
  static unsigned start_ts;

  if ( !bit_indx ) {      // bit_indx == 0 ? save start time
    start_ts=TA0CCR0;     // save TAR time at start bit
    bitmask = 0x0001;
  }

  cnts[bit_indx] = TA0CCR0-start_ts; // save TAR time at change

  if ( TA0CCTL0 & CCI ) { // if bit value is high
    bits |= bitmask;
  }

  if ( ++bit_indx == howmany ) { // last one? wake main line
    LPM0_EXIT;
  }

  bitmask <<= 1; // capture from LSB to MSB;
}

#endif

#if 0

# After you have determined your optimum values you might want to save them
# permanently to a specific chip ... do this:

$ mspdebug rf2500

... dump your original infoa segment to a file

(mspdebug) hexout 0x10c0 64 originfoa.hex

...
edit and change the CALDCO_XX_MHZ bytes and save as infoa.hex
address 0x10F8 is the DCOCTL value (a6) below .. changed to (aa) here

(mspdebug) md 0x10c0
    010c0: 17 8f fe 16 ff ff ff ff ff ff ff ff ff ff ff ff |................|
    010d0: ff ff ff ff ff ff ff ff ff ff 10 10 f1 7f 00 00 |................|
    010e0: 93 7f f0 02 00 00 1e 80 c1 01 00 00 fe 08 ff ff |................|
    010f0: ff ff ff ff ff ff 01 08 a6 8f af 8e 9e 8d dd 86 |................|

... erase infoa segment
... WARNING!! make sure you have a hex backup of the original infoa before doing this!!!

(mspdebug) opt enable_locked_flash_access true
(mspdebug) erase segment 0x10c0
Erasing...
Enabling locked flash access

... make sure it is erased (0xff is an erased flash byte)

(mspdebug) md 0x10c0
    010c0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
    010d0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
    010e0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
    010f0: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|

... load new values into flash, adjust CRC value in the file and resave after it
... complains about crc being wrong, mspdebug will tell you what the crc should be

(mspdebug) load infoa.hex
Writing   64 bytes at 10c0...
Done, 64 bytes total

... verify your new values ( i changed CALDCO_16MHz from a6 to aa )
(mspdebug) md 0x10c0
    010c0: 17 8f fe 16 ff ff ff ff ff ff ff ff ff ff ff ff |................|
    010d0: ff ff ff ff ff ff ff ff ff ff 10 10 f1 7f 00 00 |................|
    010e0: 93 7f f0 02 00 00 1e 80 c1 01 00 00 fe 08 ff ff |................|
    010f0: ff ff ff ff ff ff 01 08 aa 8f af 8e 9e 8d dd 86 |................|

(mspdebug) reset
(mspdebug) run

#endif
