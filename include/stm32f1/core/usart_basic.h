/*
 * blink.cpp - simple fabooh blink
 */

#ifndef USART_BASIC_H_
#define USART_BASIC_H_

#include "fabooh.h"
#include <stm32f103xb.h>

/*
 * usart_basic_t - blocking read, blocking write low memory usage usart
 *
 * This Writer class implementation uses the minimal amount of system
 * resource. It uses no buffer memory and does blocking reads and writes
 * using the USART peripheral.
 *
 * It has a lower bound baud rate speed of ~600bps. (36MHz/sizeof(uint16_t))
 * it fails silently with lower speeds.
 */
template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct usart_basic_t {
  USART_TypeDef * USARTx() {
    switch(TXPIN::pin()) {
    case 9:
      return USART1;
      break;

    case 2:
      return USART2;
      break;

    case 10:
      return USART3;
      break;

    default:
      while(1); // pick a valid port_no
      break;
    }
  }

  void begin(const uint32_t bps = BAUD) {
    (void)bps;
    switch(TXPIN::pin()) {
    case 9:
      GPIOA->CRH = (GPIOA->CRH & ~(0xF << 4)) | (0b01001011 << 4); // PA9-TX1, PA10-RX1
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/1; // BUS SPEED is MCLK/1
      USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      break;

    case 2:
      GPIOA->CRL = (GPIOA->CRL & ~(0xF << 8)) | (0b01001011 << 8); // PA2-TX2, PA3-RX2
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/2; // BUS SPEED is MCLK/2
      USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      break;

    case 10:
      GPIOB->CRH = (GPIOB->CRH & ~(0xF << 8)) | (0b01001011 << 8); // PB10-TX1, PB11-RX1
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/2; // BUS SPEED is MCLK/2
      USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      break;

    default:
      while(1); // pick a valid port_no
      break;
    }
  }

  /*
   * end - wait for all bytes to finish sending
   */
  void end(void) {
    while(!(USARTx()->SR & USART_SR_TXE));
  }

  /*
   * read - blocking read function
   */
  int read(void) {

    while(!(USARTx()->SR & USART_SR_RXNE));

    return USARTx()->DR;
  }

  /*
   * write_impl - Writer write(uint8_t c) implementation
   *
   * This function called by the print_t class
   */
  int write_impl(uint8_t c) {

    while(!(USARTx()->SR & USART_SR_TXE));
    USARTx()->DR = c;

    return 1;
  }
};

#endif
