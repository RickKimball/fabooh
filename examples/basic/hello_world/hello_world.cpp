/*
 * hello_world.cpp - simple fabooh serial
 */

#include <fabooh.h>
#include <main.h>

serial_default_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;

void setup(void) {
  Serial.begin();
}

void loop() {
  Serial.print("Hello world!\n");
  while(1);
}

