/*
 * armmath_table.cpp - print a table of angles and sine using a float32_t data type
 *                     from libcortexM3l_math and its trigonometry functions
 *
 * CMSIS-DSP library results in code size ~5.1K vs ~7.5k for libc floating point
 * that runs the sinf() function in ~20us vs ~40us
 *
 * see: www.arm.com CMSIS-DSP
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

namespace {
  const uint32_t BAUD_RATE = 9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial;
  serial Serial;

  LED_BUILTIN_ LED;

  float deg_rad_const = M_PI/180.0f;
  float f_1=1.0f;
}

/* use ARM_MATH_CM3 sine code */
inline float sinf(float x) {
    return arm_sin_f32(x);
}

float sind(float deg_angle) {
  float radians = deg_angle * deg_rad_const;
  float sin_f = sinf(radians);
  return sin_f;
}

void setup() {
  Serial.begin(BAUD_RATE);
  LED.setmode_output();
}

void loop()
{
  float angle=-90.0f;
  int x=-90;

  Serial << "Table of Sin(a)" << endl;

  for (x = -90; x <= 90; x++, angle += f_1 ) {
    LED = 1;
    float sin_value=sind(angle);
    LED = 0;

    Serial << "angle = " << _FLOAT(angle,2)
           << " sin =  " << _FLOAT(sin_value, 5) 
           << endl;
  }

  while(1); // stop here when done., press reset button to see again
}
