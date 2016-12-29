/*
 * blink.cpp - simple fabooh blink
 */

#include <fabooh.h>
#include <main.h>

// instance the builtin LED
LED_BUILTIN_ LED_BUILTIN; // GPIO_PIN_LED has on()/off() in addtion to low()/high()

void setup(void) {
  LED_BUILTIN.setmode_output();
  LED_BUILTIN.off();
}

void loop(void) {
  const uint32_t Hz = 2;
  const uint32_t msec_delay0 = 1000 / Hz / 2 ;
  
  while (1) {
    LED_BUILTIN.on();
    delay(msec_delay0);
    LED_BUILTIN.off();
    delay(msec_delay0);
  }
}
