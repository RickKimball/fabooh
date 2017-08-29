/*
 * usart_basic.h - Hardware USART Serial templates for stm32f1
 *
 * Created: 12-18-2016
 * Author: rick@kimballsoftware.com
 * Vesion: 1.0.1
 */

#ifndef USART_BASIC_H_
#define USART_BASIC_H_

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
struct usart_basic_t : 
  public usart_core_t<BAUD, MCLK_HZ, TXPIN, RXPIN>
{
  typedef usart_core_t<BAUD, MCLK_HZ, TXPIN, RXPIN> base;

  /*
   * available -
   */
  size_t available() {
    if ( this->USARTx()-> SR & USART_SR_RXNE )
      return 1;
    else
      return 0;
  }

  /*
   * read - non blocking read function
   */
  int read(void) {
    // blocking read
    if ( available() ) {
      return this->USARTx()->DR & 0xff;
    }
    else {
      return -1;
    }
  }

  /*
   * write_impl - Writer write(uint8_t c) implementation
   *
   * This function called by the print_t class
   */
  int write_impl(uint8_t c) {
    // blocking write
    while(!(this->USARTx()->SR & USART_SR_TXE));  // wait until TX is clear
    this->USARTx()->DR = c;
    return 1;
  }

};

#endif
