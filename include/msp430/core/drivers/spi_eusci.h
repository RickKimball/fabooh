/*
 * spi_eusci.h - eusci spi implemenation
 *
 * Created: 09-14-2014
 *  Author: rick@kimballsoftware.com
 *    Date: 09-14-2014
 * Version: 1.0.0
 *
 * =========================================================================
 *  Copyright © 2014 Rick Kimball
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
 */
#ifndef _SPI_EUSCI_H_
#define _SPI_EUSCI_H_

#include <msp430.h>
//#include <msp430fr5969.h>
#include <stdint.h>
#include "spi.h"

#ifndef __MSP430_HAS_EUSCI_B0__
#error "Error! This MCU doesn't have a USCI peripheral"
#endif

#define SPI_MODE_0 (0      | UCCKPH | UCSYNC) /* CPOL=0 CPHA=0 */
#define SPI_MODE_1 (0      | 0      | UCSYNC) /* CPOL=0 CPHA=1 */
#define SPI_MODE_2 (UCCKPL | UCCKPH | UCSYNC) /* CPOL=1 CPHA=0 */
#define SPI_MODE_3 (UCCKPL | 0      | UCSYNC) /* CPOL=1 CPHA=1 */

#include "spi.h"

#define SPI_62_5kHz 256 /* 16MHz = 62500 */
#define SPI_125kHz  128 /* 16MHz = 125000 */
#define SPI_250kHz   64 /* 16MHz = 250000 */
#define SPI_400kHz   40 /* 16MHz = 400000 */
#define SPI_1MHz     16 /* 16MHz = 1MHz */
#define SPI_2MHz      8 /* 16MHz = 2MHz */
#define SPI_4MHz      4 /* 16MHz = 4MHz */
#define SPI_8MHz      2 /* 16MHz = 8MHz */
#define SPI_16MHz     1 /* 16MHz = 16Mhz */

template<typename CS, typename SCLK, typename MOSI, typename MISO, typename ATTR >
struct spi_eusci_b0_t
	:spi_base_t<CS, SCLK, MOSI, MISO, ATTR>
{
	typedef spi_base_t<CS, SCLK, MOSI, MISO, ATTR> base;

	/**
	 * spi_initialize() - Initialize and enable the SPI module
	 *
	 * P2.4 - CS (active low)
	 * P2.2 - SCLK
	 * P1.7 - MISO
	 * P1.6 - MOSI
	 *
	 */
	static void begin(void) {
		CS::high();			 				 // P2.4
		CS::setmode_output();

		UCB0CTLW0 = UCSWRST;	// source USCI clock from SMCLK, put USCI in reset mode

		if ( 0 ) {
			;
		}
		else if (base::attributes().mode == SPI_MODE0)
			UCB0CTLW0 |= (SPI_MODE_0 | base::attributes().bitorder << 13 | UCMST); // SPI MODE 0 - CPOL=0 CPHA=0
		else if (base::attributes().mode == SPI_MODE1)
			UCB0CTLW0 |= (SPI_MODE_1 | base::attributes().bitorder << 13 | UCMST); // SPI MODE 1 - CPOL=0 CPHA=1
		else if (base::attributes().mode == SPI_MODE2)
			UCB0CTLW0 |= (SPI_MODE_2 | base::attributes().bitorder << 13 | UCMST); // SPI MODE 2 - CPOL=1 CPHA=0
		else if ( base::attributes().mode == SPI_MODE3 )
			UCB0CTLW0 |= (SPI_MODE_3 | base::attributes().bitorder << 13 | UCMST); // SPI MODE 3 - CPOL=1 CPHA=1
		else
			;

    UCB0BR0 = base::attributes().divisor & 0xff;  // set initial speed
    UCB0BR1 = base::attributes().divisor >> 8;

    // configure SCK-P2.2, MOSI-P1.6, MISO-P1.7 for eUSCI
		P2SEL1 |= BIT2;	P2SEL0 &= ~BIT2;

		P1SEL0 &= ~(BIT6 | BIT7);
    P1SEL1 |= (BIT6 | BIT7);

    UCB0CTLW0 |= UCSSEL_3;      // SMCLK
    UCB0CTLW0 &= ~UCSWRST;      // release for operation
	}

	/**
	 * spi_receive() - send dummy byte and get response
	 */
	static uint8_t receive(void) {
		return send(0xff);
	}

	/**
	 * spi_send() - send a byte and get response
	 */
	static uint8_t send(const uint8_t c) {
		while (!(UCB0IFG & UCTXIFG))
			; 							// wait for SPI RX to finish
		UCB0TXBUF = c; 		// setting TXBUF clears the TXIFG flag

		while (!(UCB0IFG & UCRXIFG))
			; 							// wait for SPI RX to finish

		return UCB0RXBUF;	// reading clears RXIFG flag
	}

	/**
	 * spi_send() - send a buffer of length
	 */
	static void shiftout(const uint8_t *buffer, unsigned length)
	{
		do {
			unsigned c = *buffer;

			++buffer;

			while (!(UCB0IFG & UCTXIFG))
				; 							// wait for SPI RX to finish

			UCB0TXBUF = c; // setting TXBUF clears the TXIFG flag

			--length;
			//if ( length > 1600 ) { while(1); }

		} while(length);
	}

	/**
	 * spi_setspeed() - set new clock divider for USCI
	 *
	 * USCI speed is based on the SMCLK divided by BR0 and BR1
	 * initially we start slow (400kHz) to conform to SDCard
	 * specifications then we speed up once initialized (16Mhz)
	 *
	 */
	static void set_divisor(const uint16_t clkdiv) {
		UCB0CTLW0 |= UCSWRST;		// go into reset state
		UCB0BR0 = clkdiv & 0xff;
		UCB0BR1 = clkdiv >> 8;
		UCB0CTLW0 &= ~UCSWRST;		// release for operation
	}

	static void select() {
		CS::low();
	}

	static void deselect() {
		CS::high();
	}

	static uint8_t transfer(const uint8_t c) {
		return send(c);
	}

	static uint8_t transfer() {
		return receive();
	}

};

#endif /* _SPI_EUSCI_H_ */
