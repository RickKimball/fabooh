/*
 * andyd293.cpp - simple fabooh msp430g2231 program to turn on a relay for 10 seconds
 *                when the user button on the launchpad is pressed
 *
 *  Created on: Feb 13th, 2017
 *      Author: kimballr
 * Pinout:
 *   P1.3 is button on launchpad board
 *   P1.6 is relay output (also lights led)
 */
#include <fabooh.h>
#define SMALL_INIT4 /* don't initialize .bss and .data sections */
#include <main.h>
#include <serial.h>
#include "button_debounce.h"

namespace {
  const uint32_t BAUD_RATE=9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial;
  serial Serial;
  static const unsigned on_cycle=10000;
}

void setup() {
  PUSH2::set_mode(INPUT_PULLUP);
  P1_6::set_mode(OUTPUT);
  P1_6::low();
  //Serial.begin();

}

void loop() {
  Debouncer button_debouncer(BUTTON_PIN_3);

  for (;;) {
    delay(1);
    button_debouncer.ButtonProcess(P1IN);
    if ( button_debouncer.ButtonPressed(BUTTON_PIN_3) ) {
      //Serial << "turning on relay\r\n";
      P1_6::high();
      //Serial << "wait " on_cycle / 1000 << " seconds\r\n";
      delay(on_cycle);
      //Serial << "turning off relay\r\n";
      P1_6::low();
    }
  }
}

