/*
 * blink.cpp - simple fabooh blink
 */

#include <fabooh.h>
#include <main.h>

LED_BUILTIN_ LED_BUILTIN;

void setup(void) {
  LED_BUILTIN.setmode_output();
  LED_BUILTIN.high();
}

void loop(void) {
  const uint32_t freq = 2;
  const uint32_t msec_delay0 = 1000 / freq / 2 ;
  
  while (1) {
    LED_BUILTIN.toggle();
    delay(msec_delay0);
  }
}
