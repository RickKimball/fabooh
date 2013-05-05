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

#ifndef F_XTAL
#define F_XTAL 12000000UL
#endif

enum cpu_clksrc_e { CLK_FROM_IRC=0, CLK_FROM_XTAL=1 };

template <const uint32_t FREQ=F_CPU, const uint32_t FREQ_XTAL=F_XTAL, cpu_clksrc_e clk_src=CLK_FROM_IRC>
struct cpu_lpc11xx_t
    : cpu_base_t<FREQ> {
  static const uint32_t MCLK_FREQ=FREQ;

  static void clock_from_xtal(const uint32_t PSEL_MSEL) {
    LPC_SYSCON->SYSAHBCLKDIV = 0x1;           // set clock divider for core to 1

    LPC_SYSCON->MAINCLKSEL &= ~(0x03);        // set “main clock” to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)
    LPC_SYSCON->MAINCLKUEN = 0;               // write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
    LPC_SYSCON->MAINCLKUEN = 1;               // write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
    while (!(LPC_SYSCON ->MAINCLKUEN & 0x01));

    LPC_SYSCON->PDRUNCFG &= ~(1<<5);          // power xtal oscillator
    volatile unsigned cnt=255;
    do { __NOP(); } while(--cnt);

    LPC_SYSCON->SYSPLLCLKSEL = 0x01;         // 0x01 set pll source to xtal oscillator
    LPC_SYSCON->SYSPLLCLKUEN = 0;
    LPC_SYSCON->SYSPLLCLKUEN = 1;
    while ( !(LPC_SYSCON->SYSPLLCLKUEN & 0x01) );

    LPC_SYSCON->SYSPLLCTRL = PSEL_MSEL;
    LPC_SYSCON->PDRUNCFG &= ~(1 << 7);        // Power-up SYSPLL
    while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)); // Wait Until PLL Locked

    // 3.5.11 Main clock source select register
    LPC_SYSCON ->MAINCLKSEL = 0x03;           // System PLL clock out
    LPC_SYSCON ->MAINCLKUEN = 0x0;
    LPC_SYSCON ->MAINCLKUEN = 0x1;
    while (!(LPC_SYSCON ->MAINCLKUEN & 0x01));
  }

  static void clock_from_irc(const uint32_t PSEL_MSEL) {
    LPC_SYSCON->SYSAHBCLKDIV  = 0x1;          // set clock divider for core to 1

    LPC_SYSCON->MAINCLKSEL &= ~(0x03);        // set “main clock” to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)
    LPC_SYSCON->MAINCLKUEN = 0;               // write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
    LPC_SYSCON->MAINCLKUEN = 1;               // write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
    while (!(LPC_SYSCON ->MAINCLKUEN & 0x01));

    LPC_SYSCON->SYSPLLCLKSEL = 0x0;           // 0x0 internal osc
    LPC_SYSCON->SYSPLLCLKUEN = 0;
    LPC_SYSCON->SYSPLLCLKUEN = 1;
    while ( !(LPC_SYSCON->SYSPLLCLKUEN & 0x01) );

    LPC_SYSCON->SYSPLLCTRL = PSEL_MSEL;

    LPC_SYSCON->PDRUNCFG &= ~(1 << 7);        // Power-up SYSPLL
    while (!(LPC_SYSCON->SYSPLLSTAT & 0x01)); // Wait Until PLL Locked

    LPC_SYSCON ->MAINCLKSEL = 0x03;           // System PLL clock out
    LPC_SYSCON ->MAINCLKUEN = 0x0;
    LPC_SYSCON ->MAINCLKUEN = 0x1;
    while (!(LPC_SYSCON ->MAINCLKUEN & 0x01));
  }

  static void init_clock(void) {
    if (FREQ_XTAL > 20000000) {
      LPC_SYSCON->SYSOSCCTRL = (1 << 1);  // "higher speed" crystals
    }
    else {
      LPC_SYSCON->SYSOSCCTRL = 0;     // "lower speed" crystals
    }

    if ( FREQ_XTAL == 12000000 ) {
      if ( F_CPU == 50000000L ) {
        static const uint32_t PSEL=0x20, MSEL=0x03;      // 48MHz
        ( clk_src ==  CLK_FROM_IRC ) ?  clock_from_irc(PSEL|MSEL) : clock_from_xtal(PSEL|MSEL);

        LPC_SYSCON->IRCCTRL += 22;    // TODO: tweak the trim register of the internal clock
                                      // using a scope to generates 50MHz. This only works for
                                      // my chip, you will need to calibrate this yourself
                                      // for your chip
      }

      if ( F_CPU == 48000000L ) {
        static const uint32_t PSEL=0x20, MSEL=0x03;      // 48MHz
        ( clk_src ==  CLK_FROM_IRC ) ?  clock_from_irc(PSEL|MSEL) : clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 36000000L ) {
        static const uint32_t PSEL=0x40, MSEL=0x02;      // 36MHz
        ( clk_src ==  CLK_FROM_IRC ) ?  clock_from_irc(PSEL|MSEL) : clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 24000000L ) {
        static const uint32_t PSEL=0x40, MSEL=0x01;      // 24MHz
        ( clk_src ==  CLK_FROM_IRC ) ?  clock_from_irc(PSEL|MSEL) : clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 12000000L ) {
        static const uint32_t PSEL=0x60, MSEL=0x00;      // 12MHz
        ( clk_src ==  CLK_FROM_IRC ) ?  clock_from_irc(PSEL|MSEL) : clock_from_xtal(PSEL|MSEL);
      }
    }
    else if ( FREQ_XTAL == 14745600 ) {
      if (F_CPU == 44236800UL ) {
        static const uint32_t PSEL=0x20, MSEL=0x02;      // 14.7456MHz MSEL=3,PSEL=1
        clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 29491200UL ) {
        static const uint32_t PSEL=0x40, MSEL=0x01;      // 14.7456MHz MSEL=2,PSEL=2
        clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 14745600UL ) {
        static const uint32_t PSEL=0x60, MSEL=0x00;      // 14.7456MHz MSEL=1,PSEL=2
        clock_from_xtal(PSEL|MSEL);
      }
    }
    else if ( FREQ_XTAL == 20000000 ) {
      if (F_CPU == 40000000L ) {
        static const uint32_t PSEL=0x20, MSEL=0x01;      // 40MHz
        clock_from_xtal(PSEL|MSEL);
      }
      else if (F_CPU == 20000000L ) {
        static const uint32_t PSEL=0x40, MSEL=0x00;      // 20MHz
        clock_from_xtal(PSEL|MSEL);
      }
    }

    // set flash wait states
    flash_access_time(F_CPU);
  }

  static void flash_access_time(const uint32_t frequency = MCLK_FREQ) {
    (void) frequency;
    uint32_t access_time, flashcfg_register;

    if (frequency > 40000000UL)
      access_time = FLASHCFG_FLASHTIM_3CLK;
    else if (frequency > 20000000UL)
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
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6) | (1 << 16); // Enable clocks GPIO, IOCON

    LPC_IOCON->PIO0_1 |= 0x01;
    LPC_SYSCON->CLKOUTDIV = 1;        // Divided by 1

    LPC_SYSCON->CLKOUTCLKSEL = 0x03;  // output Main clock
                                      // 0x00 IRC oscillator
                                      // 0x01 System oscillator
                                      // 0x02 Watchdog
                                      // 0x03 Main clock

    LPC_SYSCON->CLKOUTUEN = 0x01;     // Update clock
    LPC_SYSCON->CLKOUTUEN = 0x00;     // Toggle update register once
    LPC_SYSCON->CLKOUTUEN = 0x01;
    while (!(LPC_SYSCON ->CLKOUTUEN & 0x01))  ; // Wait until updated

  }

  static void delay_msec(const uint32_t msec) {
    volatile uint32_t t = tickcount + msec;

    while (tickcount < t) ;
  }

};

typedef cpu_lpc11xx_t<F_CPU, F_XTAL, CLK_FROM_IRC> CPU;

#define delay(msec) CPU::delay_msec((msec))
#define delay_msecs(msec) CPU::delay_msec((msec))


#endif /* CPU_LPC11XX_H_ */
