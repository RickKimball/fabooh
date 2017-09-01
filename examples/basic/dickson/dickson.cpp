/*
 * dickson.cpp - fabooh dickson pump
 */

#include <fabooh.h>
#include <main.h>

// instance the builtin LED
LED_BUILTIN_ myLED;  

void blink(uint8_t times, uint16_t ms, bool level = true);

void setup() {
  unsigned x = DCOCTL;
  x += 50;
  x &= 0xFFF0;
  DCOCTL = x;
  P1OUT = 0;
  delay(1000);
  P1DIR |= BIT0|BIT1|BIT2|BIT3|BIT4;
  P1OUT = BIT0 | /*BIT1 | */ BIT2| /*BIT3 |*/ BIT4;
}

void loop() {
  unsigned x=5;
  {
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    delay(1000);
  } while(--x);

  while(1) {
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
    P1OUT ^= (BIT0|BIT1|BIT2|BIT3|BIT4);
  }
}

