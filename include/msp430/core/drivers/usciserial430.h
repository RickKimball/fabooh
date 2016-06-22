/*
 * usciserial430.h - USCI hardware Serial UART template for msp430
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

#ifndef USCISERIAL_430_H_
#define USCISERIAL_430_H_

#ifdef __MSP430_HAS_USCI__

template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct serial_base_usci_t {

  /**
   * begin( baudrate ) - initialize TX/RX pins
   */
  void begin(const uint32_t baud_ignored=BAUD /* not used */) {
      const unsigned long baud_rate_20_bits = (MCLK_HZ + (BAUD >> 1)) / BAUD; // Bit rate divisor
                                                      // Configure TXPIN and RXPIN as UART controlled pins
      P1SEL |= (TXPIN::pin_mask | RXPIN::pin_mask);   // msp430g2553 uses P1.2=TXD P1.1=RXD
      P1SEL2 |= (TXPIN::pin_mask | RXPIN::pin_mask);  //

      UCA0CTL1 = UCSWRST;                             // Hold USCI in reset to allow configuration
      UCA0CTL0 = 0;                                   // No parity, LSB first, 8 bits, one stop bit, UART (async)
      UCA0BR1 = (baud_rate_20_bits >> 12) & 0xFF;     // High byte of whole divisor
      UCA0BR0 = (baud_rate_20_bits >> 4) & 0xFF;      // Low byte of whole divisor
      UCA0MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
      UCA0CTL1 = UCSSEL_2;                            // Use SMCLK for bit rate generator, then release reset

      (void)baud_ignored;
  }

  void flush() {
    while (UCA0STAT & UCBUSY) ;
  }

  /*
   * end() - set TX/RX pin back to default
   */
  void end() {
    // wait for TX/RX to finish
    flush();

    // put USCI in reset
    UCA0CTL1 = UCSWRST;

    // Revert to default to GPIO input
    if ( TXPIN::pin_mask && RXPIN::pin_mask ) {
      P1DIR  &= ~(TXPIN::pin_mask);
      P1SEL  &= ~(TXPIN::pin_mask | RXPIN::pin_mask);
      P1SEL2 &= ~(TXPIN::pin_mask | RXPIN::pin_mask);
    }
    else if ( TXPIN::pin_mask ) {
      P1DIR  &= ~(TXPIN::pin_mask);
      P1SEL  &= ~(TXPIN::pin_mask);
      P1SEL2 &= ~(TXPIN::pin_mask);
    }
    else if ( RXPIN::pin_mask ) {
      P1SEL  &= ~(RXPIN::pin_mask);
      P1SEL2 &= ~(RXPIN::pin_mask);
    }
  }

  /*
   * read() - blocking read
   */
  int read(void) {
      while(!(IFG2 & UCA0RXIFG));                  // wait for something in the RX buffer
      return UCA0RXBUF;
  }

  /*
   * write_impl() - blocking write CRTP function used by print<> template
   *
   * this template implements enough code to be considered a Writer pattern
   * for use with the print_t<Writer>
   *
   */
  int write_impl(uint8_t c) {
      while(!(IFG2 & UCA0TXIFG));                  // wait for TX buffer to be empty
      UCA0TXBUF = c;
      return 1;
  }

};

// typical usage
template <uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct usci_serial_t:
  serial_base_usci_t<BAUD, MCLK_HZ, TXPIN, RXPIN>,
  print_t<usci_serial_t<BAUD, MCLK_HZ, TXPIN, RXPIN> >
{
    // make it easier to instance in user code
};

#endif /* __MSP430_HAS_USCI__ */

#endif /* USCISERIAL_430_H_ */
