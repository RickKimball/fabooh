/*
 *  cpu430_bc2.h - msp430 Basic Clock Module+ implementation
 *
 * Created: Dec 12, 2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-16-2013
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

#ifndef CPU430_BC2_H
#define CPU430_BC2_H
#include <msp430.h>
#include "cpu.h"

template<const uint32_t MCLK_FREQ>
struct cpu430_bc2_t: cpu_base_t<MCLK_FREQ> {
  typedef cpu_base_t<MCLK_FREQ> cpu_base;

  static void init_clock(void) {
    if ( cpu_base::frequency == 16000000UL) {
#if !defined(__MSP430G2231__)
      BCSCTL1 = CALBC1_16MHZ;
      DCOCTL = CALDCO_16MHZ;
#else
      BCSCTL1 = 0x8F; // we provide some default values for an msp430g2231
      DCOCTL = 0x7C;  // TODO: your msp430g2231 chip needs to be calibrated
                      // see the goldilocks example
#endif
    }
    else if (cpu_base::frequency == 12000000UL) {
#if !defined(__MSP430G2231__)
      BCSCTL1 = CALBC1_12MHZ;
      DCOCTL = CALDCO_12MHZ;
#else
      BCSCTL1 = 0x8E; // we provide some default values for an msp430g2231
      DCOCTL = 0x82;  // TODO: your msp430g2231 chip needs to be calibrated
#endif
    }
    else if (cpu_base::frequency == 8000000UL) {
#if !defined(__MSP430G2231__)
      BCSCTL1 = CALBC1_8MHZ;
      DCOCTL = CALDCO_8MHZ;
#else
      BCSCTL1 = 0x8D; // we provide some default values for an msp430g2231
      DCOCTL = 0x74;  // TODO: your msp430g2231 chip needs to be calibrated
#endif
    }
    else if (cpu_base::frequency == 6400000UL) {
#if defined(__MSP430G2231__)
      BCSCTL1 = 0x8C; // we provide some default values for an msp430g2231
      DCOCTL = 0x95;  // TODO: your msp430g2231 chip needs to be calibrated
#endif
    }
    else if (cpu_base::frequency == 4000000UL) {
#if defined(__MSP430G2231__)
      BCSCTL1 = 0x8B; // we provide some default values for an msp430g2231
      DCOCTL = 0x4D;  // TODO: your msp430g2231 chip needs to be calibrated
#endif
    }
    else if (cpu_base::frequency == 1000000UL) {
      BCSCTL1 = CALBC1_1MHZ;
      DCOCTL = CALDCO_1MHZ;
    }
    else {
      // use default clock .. varies on each chip
    }
  }

  static void enable_clkout(void) {
    P1SEL |= BIT4;
    P1DIR |= BIT4;
  }
};

typedef cpu430_bc2_t<F_CPU> CPU; // F_CPU from build environment

#endif /* CPU430_BC2_H */
