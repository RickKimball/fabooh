/*
 * main.h
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef MAIN_H_
#define MAIN_H_

extern "C" {
  void setup(void);
  void loop(void);
  int main(void);
}

int main(void)
{
    CPU::enable_clkout();

    SysTick_Config(CPU::cycles_per_msec);

    setup();

    while(1) {
        loop();
    }
}

#endif /* MAIN_H_ */
