/*
 * cpu.h - abstract CPU clock initialization, timing and sleeping
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 08-23-2014
 * Version: 1.0.3
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

#ifndef CPU430_FRAM_H_
#define CPU430_FRAM_H_

#include <msp430.h>

#if defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ == 6
#define FRCTLPW 0xA500
#define NWAITS_1 0x10
#endif

#if !defined(F_CPU)
#define F_CPU 8000000UL
#endif

#if 0
/*
 * cpu_base_t - provide CPU related constants
 *
 * uint32_t FREQ - cpu frequency in Hz
 */
template<uint32_t MCLK_FREQ>
struct cpu_base_t {
    static const uint32_t frequency = MCLK_FREQ;
    static const uint32_t msec_cycles = frequency/1000;
    static const uint32_t usec_cycles = frequency/1000000;
};
#endif

template<const unsigned long FREQ = F_CPU>
struct cpu430_cs_t: cpu_base_t<F_CPU> {
	//static const unsigned long MCLK_FREQ = FREQ;

	static void init_clock(void) {
		if ( 0 ) {
			// zero wait states for < 8MHz
		}
		else if ( FREQ > 16160000L ) {
			FRCTL0 = FRCTLPW | NWAITS_2;				// two wait cycles for > 16MHz operation
		}
		else if ( FREQ > 8080000L ) {
			FRCTL0 = FRCTLPW | NWAITS_1;				// one wait cycle for > 8MHz operation
		}

		CSCTL0_H = CSKEY >> 8;                    		// Unlock CS registers
		CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; // Set SMCLK = MCLK = DCO,
																									// ACLK = VLOCLK
		if ( 0 ) {
		}
		else if (FREQ > 990000L && FREQ < 1010000L ) {
			CSCTL1 = DCOFSEL_0; // Set DCO to 1MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
		else if (FREQ > 3940000L && FREQ < 4040000L ) {
			CSCTL1 = DCOFSEL_0 | DCOFSEL_3; // Set DCO to 4MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
    else if (FREQ > 5400000L && FREQ < 6600000L ) {
      CSCTL1 = DCORSEL | DCOFSEL_6; // Set DCO to 6MHz
      CSCTL3 = DIVA__1 | DIVS__4 | DIVM__4;
    }
    else if (FREQ > 6895000L && FREQ < 7105000L ) {
      CSCTL1 = DCOFSEL_0 | DCOFSEL_5; // Set DCO to 7MHz
      CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    }
		else if (FREQ > 7920000L && FREQ < 8080000L ) {
			CSCTL1 = DCORSEL | DCOFSEL_3; // Set DCO to 8MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
		else if (FREQ > 11800000L && FREQ < 12120000L ) {
			CSCTL1 = DCORSEL | DCOFSEL_6; // Set DCO to 12MHz
			CSCTL3 = DIVA__1 | DIVS__2 | DIVM__2;
		}
		else if (FREQ > 15840000L && FREQ < 16160000 ) {
			CSCTL1 = DCORSEL | DCOFSEL_4; // Set DCO to 16MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
#if 0
		else if (FREQ > 20580000L && FREQ < 21420000 ) {
			CSCTL1 = DCORSEL | DCOFSEL_5; // Set DCO to 21MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
		else if (FREQ > 23520000L && FREQ < 24480000L ) {
			CSCTL1 = DCORSEL | DCOFSEL_6; // Set DCO to 24MHz
			CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
		}
#endif
		else {
			//#warning FREQ is not a known frequency value
		}

		CSCTL0_H = 0; 	// Lock CS registers
	}

	/* enable_mclkout() - output the resulting MCLK on PJ.1 */
	static void enable_mclkout(void)
	{
		PJDIR |= BIT1;
		PJSEL0 |= BIT1;		// Output MCLK, not pinned out
	}

	/* enable_smclkout() - output the resulting SMCLK on P3.4	 */
	static void enable_smclkout(void)
	{
		P3DIR |= BIT4;
		P3SEL1 |= BIT4;		// Output SMCLK
	}

};

typedef cpu430_cs_t<F_CPU> CPU;

#endif /* CPU430_FRAM_H_ */
