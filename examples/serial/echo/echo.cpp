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
#if 0
  // pick one serial implementation
#elif 1 && defined(__MSP430G2231__) || defined(__MSP430G2452__)
  timer_serial_t<BAUD_RATE, CPU::frequency, TX_PIN, RX_PIN> Serial;
#elif 1 && defined(STM32F1)
  serial_usart_isr_t<BAUD_RATE, CPU::frequency, TX_PIN, RX_PIN> Serial;
  USART_IRQHandler(1,Serial)
#else
  serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, RX_PIN> Serial;
#endif

  const char *prompt = "\r\ntype here:\r\n";
}

void setup() {
  Serial.begin(BAUD_RATE);	// configure serial device for Timer or SW only serial
}

void loop() {
  Serial.print(prompt);

  for (;;) {
    int c;
    
    c = Serial.read();
    switch ( c ) {
    case '\r':
      return;
      
    case -1:
      break;
      
    default:
      Serial.write(c);
      break;
    }
  }
}

