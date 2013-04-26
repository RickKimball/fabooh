/*
 * uartserial.h - USART hardware Serial UART template for lpc1114
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-08-2013
 * Version: 1.0.3
 *
 * 11-12-2012: 0.0.001 rick@kimballsoftware.com - initial version
 * 02-05-2013: 0.0.002 rick@kimballsoftware.com - tweaks to remove TX RX instances
 *
 * Acknowledgments:
 *  Serial API inspired by Arduino.
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef UARTSERIAL_H
#define UARTSERIAL_H

#include <lpc1100l.h>

template <const uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct serial_base_uart_t {
  static const uint32_t bit_cycles = (((MCLK_HZ / 1) / 2) / 16) / BAUD; // cycles/bit

  /**
   * begin(bps)  - initialize TX/RX pins
   */
  void begin(const uint32_t bps = BAUD) {
    //SET UP UART (sec. 13.2 in datasheet "BASIC CONFIGURATION")

    if ( TXPIN::_portno != DUMMY_PORT ) {
      LPC_IOCON->PIO1_7 &= ~0x07;  // clear all functions
      LPC_IOCON->PIO1_7 |= 0x01;   // enable as UART pin
    }

    if ( RXPIN::_portno != DUMMY_PORT ) {
      LPC_IOCON->PIO1_6 &= ~0x07;  // clear all functions
      LPC_IOCON->PIO1_6 |= 0x01;   // enable as UART pin
    }

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12); //enable clock to UART (sec. 3.5.14)
    LPC_SYSCON->UARTCLKDIV = 0x2;       // divided by 1

    LPC_UART->LCR = 0x83;               //DLB=1, 8 bits, no Parity, 1 Stop bit

    /* TODO: figure out fractional dividers */
    LPC_UART->DLM = bit_cycles >> 8;    // bc / 256
    LPC_UART->DLL = bit_cycles & 0xff;  // bc mod 256
    LPC_UART->FCR = 0x07;               // Enable and reset TX and RX FIFO. (sec. 13.5.6)
    LPC_UART->LCR = 0x03;               // DLAB = 0 ,Disable access to divisor latches

    unsigned int temp = LPC_UART->LSR;  // Read to clear the line status. (sec. 13.5.9)

    (void)bps;
    (void)temp;

    delay(200);                 /* TODO: figure out why I need a delay */
  }

  /*
   * end() - set TX/RX pin back to default
   */
  void end() {
  }

  /*
   * read() - blocking read
   */
  int read(void) {
#define LSR_RDR (1<<0) /* 0x01 */

    while ( !(LPC_UART->LSR & LSR_RDR) ); // wait for if Receiver Data Ready bit set (sec 13.5.9)
    return LPC_UART->RBR & 0xff;          // return byte received
  }

  /*
   * write_impl() - blocking write CRTP function used by print<> template
   *
   * this template implements enough code to be considered a Writer pattern
   * for use with the print_t<Writer>
   *
   */
  int write_impl(uint8_t c) {
#define LSR_TEMT (1 << 6) /* 0x40 */

    LPC_UART->THR |= c;                // insert into FIFO (sec. 13.5.2)
    while ( !(LPC_UART->LSR & LSR_TEMT) ); // wait until Transmitter Empty
    return 1;
  }

};

// typical usage
template <uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct uart_serial_t:
    serial_base_uart_t<BAUD, MCLK_HZ, TXPIN, RXPIN>,
  print_t<uart_serial_t<BAUD, MCLK_HZ, TXPIN, RXPIN> >
{
    // make it easier to instance in user code
};

#endif /* UARTSERIAL_H */
