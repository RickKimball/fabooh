/*
 * usart_isr.h - Hardware USART Serial templates for stm32f1
 *
 * Created: 12-18-2016
 * Author: rick@kimballsoftware.com
 * Vesion: 1.0.1
 */

#ifndef USART_ISR_H_
#define USART_ISR_H_

#include "usart_core.h"

/*
 * usart_isr_t - non-blocking USART using ISR handler
 *
 */
template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct usart_isr_t :
  public usart_basic_t<BAUD, MCLK_HZ, TXPIN, RXPIN>
{
  ringbuffer_t<64> rx_buffer;
  ringbuffer_t<64> tx_buffer;
  volatile unsigned flag_tx_primed;
  typedef usart_core_t<BAUD, MCLK_HZ, TXPIN, RXPIN> base;

  // begin - override base::begin() but call it manually
  void begin(const uint32_t bps = BAUD) {
    base::begin(bps);
    rx_buffer.clear();
    tx_buffer.clear();
    flag_tx_primed=0;

    if (base::USARTx() == USART1 ) {
      NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (base::USARTx() == USART2 ) {
      NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (base::USARTx() == USART3 ) {
      NVIC_EnableIRQ(USART3_IRQn);
    }
    
    base::enable_rx_isr();
  }

  void end(void) {
    while(!available()); // let the data finish
    base::end();         // wait for usart to clear
  }

  /*
   * available - return count of bytes available to read
   */
  size_t available() {
    return rx_buffer.available();
  }

  /*
   * read - non blocking read function
   */
  int read(void) {
    return rx_buffer.pop_front(); // returns element,  or -1 if buffer is empty
  }

  /*
   * write_impl - Writer write(uint8_t c) implementation
   *
   * This function called by the print_t class
   */
  int write_impl(uint8_t c) {
    while( tx_buffer.isFull() );  // block till space available

    tx_buffer.push_back_nc(c);
    if ( !flag_tx_primed ) {
      flag_tx_primed=1;
      this->enable_tx_isr();
    }
    return 1;
  }

};

/*
 * USART_IRQHandler - 'C' language macro for USART[123]_IRQHandler
 *
 * Note: The user must instance this macro once in their program
 */
#define USART_IRQHandler(N, SERIAL_) \
extern "C" void USART##N##_IRQHandler(void)                                      \
{									                                                               \
  if ( SERIAL_.is_rx_isr_enabled() && (SERIAL_.USARTx()->SR & USART_SR_RXNE) ) { \
    SERIAL_.rx_buffer.push_back((SERIAL_.USARTx()->DR & 0xff));		               \
  }									                                                             \
									                                                               \
  if (  SERIAL_.is_tx_isr_enabled() && (SERIAL_.USARTx()->SR & USART_SR_TXE) ) { \
    if ( SERIAL_.tx_buffer.isEmpty()) {					                                 \
      SERIAL_.disable_tx_isr();						                                       \
      SERIAL_.flag_tx_primed = 0;					                                       \
    }									                                                           \
    else {								                                                       \
      SERIAL_.USARTx()->DR = SERIAL_.tx_buffer.get() & 0xff;		                 \
    }                                                                            \
  }                                                                              \
}

#endif
