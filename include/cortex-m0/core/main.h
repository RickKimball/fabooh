/*
 * main.h
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <LPC11xx.h>

extern "C" {
  volatile uint32_t tickcount;

  void SysTick_Handler(void) { tickcount++; }

  void setup(void);
  void loop(void);
  int main(void);
}

int main(void)
{
    CPU::init_clock();
    //CPU::enable_clkout();
    SysTick_Config(F_CPU/1000);

    setup();

    while(1) {
        loop();
    }
}

#endif /* MAIN_H_ */
