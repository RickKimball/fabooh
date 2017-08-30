/*
 * usart_core.h - Hardware USART Serial templates for stm32f1
 *
 * Created: 12-18-2016
 * Author: rick@kimballsoftware.com
 * Vesion: 1.0.1
 */

#ifndef USART_CORE_H_
#define USART_CORE_H_

#include "fabooh_common.h"

/*
 * usart_core_t - common core usart methods
 *
 */
template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct usart_core_t {

  //
  // USARTx() - return PORT from pin

  USART_TypeDef * USARTx() {
    // try TXPIN first
    switch(TXPIN::port_pin()) {
    case (1<< 16) | 9: // PA9-TX1
      return USART1;
      break;

    case (1<<16) | 2:  // PA2-TX2
      return USART2;
      break;

    case (2<<16) | 10: // PB10-TX3
      return USART3;
      break;
    }

    switch(RXPIN::port_pin()) {
    case (1<< 16) | 10: // PA10-RX1
      return USART1;
      break;

    case (1<<16) | 3:   // PA3-RX2
      return USART2;
      break;

    case (2<<16) | 11:  // PB11-RX3
      return USART3;
      break;

    default:
      while(1); // error trap: you need to pick a valid port_no
      break;
    }
  }

  // configure port pins as a USART
  // TBD deal with TX only and RX only
  void begin(const uint32_t bps = BAUD) {
    (void)bps;

    // Setup the USART pins RM0008 ~page 171-172
    // RX pin configured as Floating input
    // TX pin configured as Alternate function output Push-Pull

    // See GPIO_CRH_CNFx GPIO_CRH_MODEx defines in stm32f103xb.h

    if ( USARTx() == USART1) {
      GPIOA->CRH = (GPIOA->CRH & ~(0xF << 4)) | (0b01001011 << 4); // PA9-TX1, PA10-RX1
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/1; // BUS SPEED is MCLK/1

      if ( RXPIN::port_pin() == ((1<<16) | 10) )
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      else
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE;
    }

    else if ( USARTx() == USART2) {
      GPIOA->CRL = (GPIOA->CRL & ~(0xF << 8)) | (0b01001011 << 8); // PA2-TX2, PA3-RX2
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/2; // BUS SPEED is MCLK/2

      if ( RXPIN::port_pin() == ((1<<16) | 3) )
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      else
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE;
    }

    else  if ( USARTx() == USART3) {
      GPIOB->CRH = (GPIOB->CRH & ~(0xF << 8)) | (0b01001011 << 8); // PB10-TX1, PB11-RX1
      USARTx()->BRR = ((SystemCoreClock)/BAUD)/2; // BUS SPEED is MCLK/2

      if ( RXPIN::port_pin() == ((2<<16) | 11) )
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
      else
        USARTx()->CR1 = USART_CR1_UE | USART_CR1_TE;
    }

    else {
      while(1); // error trap: you need to pick a valid port_no
    }

  }

  /*
   * end - wait for all bytes to finish sending
   */
  void end(void) {
    while(!(this->USARTx()->SR & USART_SR_TC));
  }

  /*
   * bit twiddling routines
   */
  void enable_rx_isr() {
    bb_peri_set_bit(&this->USARTx()->CR1, USART_CR1_RXNEIE_Pos, 1);
  }

  void disable_rx_isr() {
    bb_peri_set_bit(&this->USARTx()->CR1, USART_CR1_RXNEIE_Pos, 0);
  }

  void enable_tx_isr() {
    bb_peri_set_bit(&this->USARTx()->CR1, USART_CR1_TXEIE_Pos, 1);
  }

  void disable_tx_isr() {
    bb_peri_set_bit(&this->USARTx()->CR1, USART_CR1_TXEIE_Pos, 0);
  }

  bool is_rx_isr_enabled() {
    return bb_peri_get_bit(&this->USARTx()->CR1, USART_CR1_RXNEIE_Pos);
  }

  bool is_tx_isr_enabled() {
    return bb_peri_get_bit(&this->USARTx()->CR1, USART_CR1_TXEIE_Pos);
  }

};

#endif
