/**
 * blink.cpp - blink test fabooh style
 *
 * Pins Used: RED_LED, GREEN_LED, PUSH2
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

extern "C" {
#include "910.h"
}

namespace {
  const uint32_t BAUD_RATE=9600;
  typedef serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> serial;
  serial Serial;
}

uint8_t uart_getc(void)
{
  return Serial.read();
}

void uart_putc(uint8_t data)
{
  Serial.write(data);
}

void uart_puts(const uint8_t *data)
{
   while (*data)
   {
      uart_putc(*data);
      data++;
   }
}

void setup() {
  Serial.begin(9600);
  init910();
  waitcmd(); //has intrinsic loop
}

void loop() {}
