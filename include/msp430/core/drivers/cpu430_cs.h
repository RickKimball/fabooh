/*
 *  cpu430_cs.h - msp430x2 cpu using CS clock implementation (msp430fr57xxx)
 *
 * Created: Dec 12, 2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-02-2013
 * Version: 1.0.2
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

#ifndef CPU430_CS_H
#define CPU430_CS_H

#ifdef __MSP430_HAS_CS__

#include "cpu.h"

template <unsigned long FREQ=F_CPU>
struct cpu430_cs_t
    : cpu_base_t<F_CPU> {
	static const unsigned long MCLK_FREQ=FREQ;

	static void init_clock(void) {

	  CSCTL0 = CSKEY;                // Enable Access to CS Registers
    CSCTL2 &= ~SELM_7;             // Clear selected Main CLK Source
    CSCTL2 |= SELM__DCOCLK;        // Use DCO as Main Clock Source
    CSCTL3 &= ~(DIVM_3 | DIVS_3);  // clear DIVM Bits

#if F_CPU == 24000000L
    CSCTL1 = DCOFSEL0 | DCOFSEL1 | DCORSEL; // Level 2 / Range 1 : 24.0MHz
#elif F_CPU == 16000000L
    CSCTL1 = DCORSEL;                       // Level 0 / Range 1 : 16.0MHz
#elif F_CPU == 12000000L
    CSCTL1 = DCOFSEL0 | DCOFSEL1 | DCORSEL; // Level 2 / Range 1 : 24.0MHz
    CSCTL3 |= DIVM_1;                       // Div = 2
#elif F_CPU == 8000000L
    CSCTL1 = DCOFSEL0 | DCOFSEL1;           // Level 2 / Range 0 : 8.0MHz
#elif F_CPU == 1000000L
    CSCTL1 = DCOFSEL0 | DCOFSEL1;           // Level 2 / Range 0 : 8.0MHz
    CSCTL3 |= DIVM_3;                       // Div = 8
#else
#warning F_CPU is not a know frequency value
#endif
  }

	static void enable_clkout(void) {
	  P3DIR |= BIT4; P3SEL0 |= BIT4; P3SEL1 |= BIT4; /* clockout */
	}
};

typedef cpu430_cs_t<F_CPU> CPU;

#endif /* __MSP430_HAS_CS__ */
#endif /* CPU430_CS_H */
