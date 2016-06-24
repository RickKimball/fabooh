/**
 * fixmath_table.cpp - print a table of angles and sine using a fix16_t
 *                     data type from libfixmath and its trigonometry functions
 *
 * see: http://en.wikipedia.org/wiki/Libfixmath
 *
 * $ msp430-size fixmath_table.elf
 *    text    data     bss     dec     hex filename
 *    1370       0       0    1370     55a fixmath_table.elf
 */

#include <fabooh.h>
#define SMALL_INIT4 // no data or bss? if so we can get rid of the c runtime lowlevel init functions
#include <main.h>
#include <serial.h>

namespace {
  const uint32_t BAUD_RATE = 9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial; /* TX=P?.? varies, RX=NO_PIN */
  serial Serial;
};

#if defined(__MSP430G2231__)

inline void setup() {

  Serial.begin(BAUD_RATE);

  Serial << "Sorry, this chip doesn't have enough flash memory to run the sin_table sample" << endl;

  while(1);
}

void loop() {}

#else

inline void setup() {

  Serial.begin(BAUD_RATE);
  Fix16 angle; // use fix16_t for calculations

  Serial << "Table of Sin(a)" << endl;

  for (angle = (int16_t)-90; angle <= (int16_t)90; angle += (int16_t)1) {
    fix16_t sin_value = angle.sind(); // calc sine from degrees

    Serial << "angle = " << _FIX16(angle, 0)
           << " sin =  " << _FIX16(sin_value, 4) /* 5 is max with fix16_t */
           << endl;
  }

  while(1); // stop here when done., press reset button to see again
}

void loop() {}

#endif /* weed out small chips */
