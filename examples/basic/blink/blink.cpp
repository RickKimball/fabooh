/*
 * blink.cpp - simple fabooh blink
 */

#include <fabooh.h>
#define SMALL_INT4
#include <main.h>

// instance the built-in LED
LED_BUILTIN LED;

void setup(void) {
  LED.setmode_output();
  LED.off();
}

void loop(void) {
  static const uint32_t Hz = 1;
  static const uint32_t msec_delay0 = 1000 / Hz / 2;

  while (1) {
    LED.on();
    delay(msec_delay0);
    LED.off();
    delay(msec_delay0);
  }
}
