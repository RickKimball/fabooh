/*
 * blink.cpp - simple fabooh blink
 */

#include <fabooh.h>
#include <main.h>

LED_BUILTIN_ LED_BUILTIN;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // off
}

void loop(void) {
  const uint32_t freq = 2;
  const uint32_t msec_delay0 = 1000 / freq / 10 ;
  const uint32_t msec_delay1 = msec_delay0*9;
  
  while (1) {
    digitalWrite(LED_BUILTIN, LOW);  // on
    delay(msec_delay0);
    digitalWrite(LED_BUILTIN, HIGH); // off
    delay(msec_delay1);
  }
}
