/*
 * blink.cpp - fabooh blink
 */
#include <fabooh.h>
#include <main.h>

void setup(void) {
  RED_LED::setmode_output();
}

void loop(void) {
  const uint32_t freq = 2;
  const uint32_t msec_delay = 1000 / freq / 2;

  while (1) {
    RED_LED::toggle();
    delay(msec_delay);
  }
}
