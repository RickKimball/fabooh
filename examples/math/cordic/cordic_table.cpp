/**
 * cordic_table.cpp - print a table of angles and sine using fixed point
 *                    cordic sin/cos functions
 *
 * $ msp430-size cordic_table.elf
 *  text    data     bss     dec     hex filename
 *  1264       0       2    1266     4f2 cordic_table.elf
 *
 */

#include <fabooh.h>
#include <main.h>

#include <serial.h>
#include "cordic_wrapper.h"

namespace {
  const uint32_t BAUD_RATE = 9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial; /* TX=P?.? varies, RX=NO_PIN */
  serial Serial;
};

inline void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  int angle;

  Serial << "Table of Sin(a)" << endl;

  for (angle = -90; angle <= 90; angle++) {
    int q15_sin_value = sin_q15(angle);
    d15<3> decimal(q15_sin_value);

    Serial  << "angle = " << angle
            << " sin = " << ((decimal.sign < 0) ? "-0." : "0.") << (int)decimal.r
            //<< " , " << q15_sin_value
            << endl;
  }

  while(1); // stop here when done., press reset button to see again
}
