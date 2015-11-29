/*
 * notopossum.cpp - a fabooh version to compare with opossum.c
 *
 *  Created on: Mar 15, 2013
 *      Author: kimballr
 */
#include <fabooh.h>
#define SMALL_INIT4 /* don't initialize .bss and .data sections */
#include <main.h>
#include <serial.h>

namespace {
  const uint32_t BAUD_RATE=9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial;
  serial Serial;
}

void setup() {
  PUSH2::set_mode(INPUT_PULLUP);
  Serial.begin();

  for (;;) {
    int n = 1;

    do (PUSH2::read()) ? --n : (n = 5000); while (n);
    do (PUSH2::read()) ? n = 5000 : --n; while (n);
    Serial << "Poke me with a Stick\r\n";
    for (n = -5; n <= 5;) Serial << (n++) << " "; Serial << endl;

    n =  0xDEAD; Serial << _HEX(n) << " ";
    n += 0xE042; Serial << _HEX(n) << endl << "\n";
  }
}

void loop() {}
