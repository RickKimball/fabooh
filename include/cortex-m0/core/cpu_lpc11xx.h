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

#ifndef CPU_LPC11XX_H_
#define CPU_LPC11XX_H_

#include "cpu.h"

#define FLASHCFG                            (*(volatile uint32_t*)0x4003C010)
#define FLASHCFG_FLASHTIM_bit               0
#define FLASHCFG_FLASHTIM_0_bit             0
#define FLASHCFG_FLASHTIM_1_bit             1

#define FLASHCFG_FLASHTIM_0                 (1 << FLASHCFG_FLASHTIM_0_bit)
#define FLASHCFG_FLASHTIM_1                 (1 << FLASHCFG_FLASHTIM_1_bit)

#define FLASHCFG_FLASHTIM_1CLK_value        0
#define FLASHCFG_FLASHTIM_2CLK_value        1
#define FLASHCFG_FLASHTIM_3CLK_value        2
#define FLASHCFG_FLASHTIM_mask              3

#define FLASHCFG_FLASHTIM_1CLK              (FLASHCFG_FLASHTIM_1CLK_value << FLASHCFG_FLASHTIM_bit)
#define FLASHCFG_FLASHTIM_2CLK              (FLASHCFG_FLASHTIM_2CLK_value << FLASHCFG_FLASHTIM_bit)
#define FLASHCFG_FLASHTIM_3CLK              (FLASHCFG_FLASHTIM_3CLK_value << FLASHCFG_FLASHTIM_bit)

extern "C" volatile uint32_t tickcount;

template <uint32_t FREQ=F_CPU>
struct cpu_lpc11xx_t
    : cpu_base_t<FREQ> {
  static const uint32_t MCLK_FREQ=FREQ;

  static void init_clock(void) {
#if   F_CPU == 48000000L
#elif F_CPU == 36000000L
#elif F_CPU == 24000000L
#elif F_CPU == 12000000L
    // do nothing this is the default
#else
#warning F_CPU is not a know frequency value
#endif
  }

  static void flash_access_time(const uint32_t frequency = MCLK_FREQ) {
    (void) frequency;
    uint32_t access_time, flashcfg_register;

    if (frequency > 24000000UL)
      access_time = FLASHCFG_FLASHTIM_3CLK;
    else if (frequency > 12000000UL)
      access_time = FLASHCFG_FLASHTIM_2CLK;
    else
      access_time = FLASHCFG_FLASHTIM_1CLK;

    // do not modify reserved bits in FLASHCFG register
    flashcfg_register = FLASHCFG;           // read register
    flashcfg_register &= ~(FLASHCFG_FLASHTIM_mask << FLASHCFG_FLASHTIM_bit); // mask the FLASHTIM field
    flashcfg_register |= access_time << FLASHCFG_FLASHTIM_bit; // use new FLASHTIM value
    FLASHCFG = flashcfg_register;     // save the new value back to the register
  }

  static void enable_clkout(void) {
    // tweak the trim register of the internal clock
    //LPC_SYSCON->IRCCTRL += 23; // using a scope determined that this generates 50MHz for my chip

    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6) | (1 << 16); // Enable clocks GPIO, IOCON

    LPC_IOCON->PIO0_1 |= 0x01;
    LPC_SYSCON->CLKOUTDIV = 1;      // Divided by 1

    LPC_SYSCON->CLKOUTCLKSEL = 0x03; // output Main clock
                                      // 0x00 IRC oscillator
                                      // 0x01 System oscillator
                                      // 0x02 Watchdog
                                      // 0x03 Main clock

    LPC_SYSCON->CLKOUTUEN = 0x01;  // Update clock
    LPC_SYSCON->CLKOUTUEN = 0x00;  // Toggle update register once
    LPC_SYSCON->CLKOUTUEN = 0x01;
    while (!(LPC_SYSCON ->CLKOUTUEN & 0x01))  ; // Wait until updated

  }

  static void delay_msec(const uint32_t msec) {
    volatile uint32_t t = tickcount + msec;

    while (tickcount < t)
      ;
  }

};

typedef cpu_lpc11xx_t<F_CPU> CPU;

#define delay(msec) CPU::delay_msec((msec))

#endif /* CPU_LPC11XX_H_ */
