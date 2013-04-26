/**
 * sin_table.cpp - print a table of angles and sine
 *
 * $ msp430-size sin_table.elf
 *   text    data     bss     dec     hex filename
 *  11960       0       0   11960    2eb8 sin_table.elf
 *
 */

#include <fabooh.h>
#include <main.h>

#include <math.h>
#include <serial.h>

namespace {
  const uint32_t BAUD_RATE = 9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial; /* TX=P?.? varies, RX=NO_PIN */
  serial Serial;
};

#if defined(__MSP430G2231__) || defined(__MPS430G2452__)

inline void setup() {

  Serial.begin(BAUD_RATE);

  Serial << "Sorry, this chip doesn't have enough flash memory to run the sin_table sample" << endl;

  while(1);
}

void loop() {}

#else

float deg2rad(float deg) {
  return (deg * M_PI) / 180.0f;
}

inline void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  unsigned angle;
  float f, sin_value;

  Serial << "Table of Sin(a)" << endl;

  for (angle = 0; angle < 360; angle++) {
    f = (float)angle;
    sin_value = sinf(deg2rad(f));

    Serial << "angle=" << angle << " sin=" << _FLOAT(sin_value,5) << endl;
  }

  while(1); // stop here when done., press reset button to see again
}

#endif
