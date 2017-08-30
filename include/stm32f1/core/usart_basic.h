/*
 * usart_basic.h - Hardware USART Serial templates for stm32f1
 *
 * Created: 12-18-2016
 * Author: rick@kimballsoftware.com
 * Vesion: 1.0.1
 */

#ifndef USART_BASIC_H_
#define USART_BASIC_H_

#include "usart_core.h"

/*
 * usart_basic_t - blocking read, blocking write, low memory usage USART
 *
 * This Writer class implementation uses the minimal amount of system
 * resource. It uses no buffer memory and does blocking reads and writes
 * using the USART peripheral registers.
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
