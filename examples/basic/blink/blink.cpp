/*
 * blink.cpp - simple fabooh blink
 */

#include <fabooh.h>
#include <main.h>

// instance the built-in LED
LED_BUILTIN LED;

void setup(void) {
  LED.setmode_output();
}

void loop(void) {
  static const uint32_t Hz = 1;
  static const uint32_t msec_delay0 = 1000 / Hz / 2;

  while (1) {
    LED.high();
    delay(msec_delay0);
    LED.low();
    delay(msec_delay0);
  }
}
