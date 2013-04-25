/**
 * echo.cpp - echo serial characters back
 *
 * connect using putty/hyperterm/minicom .. 9600-8-N-1
 * what ever you type is echoed back
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

namespace {
  const uint32_t BAUD_RATE = 9600;
  serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, RX_PIN> Serial; // TX=varies, RX=varies
  const char *prompt = "\r\ntype here: ";
}

void setup() {
  Serial.begin(BAUD_RATE);                 // configure serial device for Timer or SW only serial
}

void loop() {
  Serial.print(prompt);

  for (;;) {
    int c;
    // use a blocking read to get a character
    switch ((c = Serial.read())) {

    case '\r':
      return;

    default:
      Serial.write(c);
      break;
    }
  }
}
