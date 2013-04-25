/*
 * main.h
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <lpc1100l.h>

#if 0
static void cpu_speed_irc(void) {
     LPC_SYSCON->SYSAHBCLKDIV             = 0x1;       //set clock divider for core to 1
     LPC_SYSCON->MAINCLKSEL               &= ~(0x03);  //set “main clock” to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)
     LPC_SYSCON->MAINCLKUEN               &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->MAINCLKUEN               |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->SYSPLLCLKSEL             = 0x00;      //connect system oscillator to SYSTEM PLL (sec. 3.5.9)
     LPC_SYSCON->SYSPLLCLKUEN             &= ~(1);     //write a zero to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
     LPC_SYSCON->SYSPLLCLKUEN             |= 1;        //write a one to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
     LPC_SYSCON->PDRUNCFG                 |= (1<<7);   //power down the PLL before changing divider values (sec 3.5.35)
     LPC_SYSCON->SYSPLLCTRL               = 0x23;      //set MSEL = 0x00011 and PSEL = 0x01 (sec 3.5.3 and table 46 of sec. 3.11.4.1)
     LPC_SYSCON->PDRUNCFG                 &= ~(1<<7);  //power up PLL after divider values changed (sec. 3.5.35)
     while((LPC_SYSCON->SYSPLLSTAT & 1) == 0);         //wait for PLL to lock
     LPC_SYSCON->MAINCLKSEL               = 0x03;      //set system oscillator to the output of the PLL (sec. 3.5.11)
     LPC_SYSCON->MAINCLKUEN               &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->MAINCLKUEN               |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12)
}

static void cpu_speed_xtal(void) {
    //USE EXTERNAL CRYSTAL TO GENERATE INTERNAL SYSTEM CLOCK
     LPC_SYSCON->SYSAHBCLKDIV             = 0x1;       //set clock divider for core to 1
     LPC_SYSCON->MAINCLKSEL               &= ~(0x03);  //set “main clock” to IRC oscillator, if not system will lock up when PLL turns off!(sec. 3.5.11)
     LPC_SYSCON->MAINCLKUEN               &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->MAINCLKUEN               |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->SYSPLLCLKSEL             = 0x01;      //connect system oscillator to SYSTEM PLL (sec. 3.5.9)
     LPC_SYSCON->SYSPLLCLKUEN             &= ~(1);     //write a zero to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
     LPC_SYSCON->SYSPLLCLKUEN             |= 1;        //write a one to SYSPLLUEN register (sec. 3.5.10), necessary for SYSPLLCLKSEL to update
     LPC_SYSCON->PDRUNCFG                 |= (1<<7);   //power down the PLL before changing divider values (sec 3.5.35)
     LPC_SYSCON->SYSPLLCTRL               = 0x23;      //set MSEL = 0x00011 and PSEL = 0x01 (sec 3.5.3 and table 46 of sec. 3.11.4.1)
     LPC_SYSCON->PDRUNCFG                 &= ~(1<<7);  //power up PLL after divider values changed (sec. 3.5.35)
     while((LPC_SYSCON->SYSPLLSTAT & 1) == 0);         //wait for PLL to lock
     LPC_SYSCON->MAINCLKSEL               = 0x03;      //set system oscillator to the output of the PLL (sec. 3.5.11)
     LPC_SYSCON->MAINCLKUEN               &= ~(1);     //write a zero to the MAINCLKUEN register (sec. 3.5.12), necessary for MAINCLKSEL to update
     LPC_SYSCON->MAINCLKUEN               |= 1;        //write a one to the MAINCLKUEN register (sec. 3.5.12)
}
#endif


extern "C" {
  volatile uint32_t tickcount;

  void SysTick_Handler(void) { tickcount++; }

  void setup(void);
  void loop(void);
  int main(void);
}

int main(void)
{
    SysTick_Config(F_CPU/1000);
    CPU::enable_clkout();
    CPU::flash_access_time(F_CPU);

    setup();

    while(1) {
        loop();
    }
}

#endif /* MAIN_H_ */
