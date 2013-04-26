/**
 * main.cpp - fabooh default main
 *
 * Desc: initialize clock, call user setup and loop.
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 02-28-2013
 * Version: 1.0.0
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

#include "fabooh.h"

#ifndef _MAIN_H_
#define _MAIN_H_

int main(void)
{
#if defined(ENERGIA)
    /* warning: optimal size reduction is achieved with -mdisable-watchdog compile time flag*/
    WDTCTL = WDTPW | WDTHOLD;
#endif

    CPU::init_clock();

    setup();

    while(1) {
        loop();
    }

    return 0;
}

/*
 * Here we provide a way to shrink the size of the exe if the code
 * doesn't actually use the .data or .bss sections. There is no
 * need to include code used to initialize sections when they have
 * a zero size.
 *
 * Use: $ msp430-size foo.elf to see the size of the .data and .bss sections.
 *
 */

#ifdef SMALL_INIT4
  #define NO_DATA_INIT
  #define NO_BSS_INIT
#endif

extern "C"
{
  #ifndef RAM_START
    #define RAM_START 0x0200
  #endif

  #ifndef STACK_COLOR
    #define STACK_COLOR 0xFEEE
  #endif

  /*
   * define STACK_COLOR to enable this feature. RAM is marked with a
   * known value. This is useful to help discover how much stack is
   * being used at runtime.
   *
   * Use mspdebug, run your program, CTRL-C to break after you
   * have exercised all your functions. Use the eXamine gdb cmd:
   *
   *  > x/4xw 0x200
   *
   * press return repeatedly to see the next 4 words. When you
   * stop seeing '0xfeee' that is where the stack ends. Of
   * course if the last thing on the stack has the value of
   * 0xfeee you will get a false reading. In general this
   * is useful.
   *
   * Note: RAM_START 0x200 will hold the data values, followed by
   * allocated heap memory, then the stack. So you can also use
   * this feature to see how much heap memory is used. It doesn't
   * however detect heap - stack collisions.
   *
   */

  #if defined(COLOR_STACK)
    void __low_level_init(void) __attribute__((section(".init3"),naked));
    void __low_level_init() {
     __asm__(
         "\n"
         " mov #__stack,r15\n" // ; start at stack and proceed down to RAM_START\n"
         "1:\n"
         " decd r15            ; double decr, a word at a time\n"
         " mov %1,0(r15)       ; 0xfree - virgin untouched stack\n"
         " cmp %0,r15          ; are we at the beginning of RAM?\n"
         " jne 1b              ; __low_level_init()\n"
         : : "i" (RAM_START), "i" (STACK_COLOR) : "r15"
         );
    }
  #endif /* COLOR_STACK */

  #if defined(NO_DATA_INIT)
    void __do_copy_data(void) __attribute__((section(".init4"),naked));
    void __do_copy_data() {
      __asm__ (
          "\n"
          //"nop; empty __do_copy_data() {} \n" // someplace to put a breakpoint
          );
    }
  #endif /* defined(NO_DATA_INIT) */

  #if defined(NO_BSS_INIT)
    void __do_clear_bss(void) __attribute__((section(".init4"),naked));
    void __do_clear_bss() {
      __asm__(
          "\n"
          //"nop; empty __do_clear_bss() {} \n" // someplace to put a breakpoint
          );
    }
  #endif /* defined(NO_BSS_INIT) */

} /* extern C */
#endif
