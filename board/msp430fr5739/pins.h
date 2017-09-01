/*
  * pins.h - msp430fr5739 GPIO port and pins
 *
 * Created: Nov 20, 2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-04-2012
 * Version: 1.0.3
 *
 * Note: this file is a work in progress don't expect it to be final ...
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
 */

#ifndef MSP430FR5739_NOBOARD_H_
#define MSP430FR5739_NOBOARD_H_

/*
 * P1 port and P1.X shorthand typedefs
 */
#ifdef __MSP430_HAS_PORT1_R__
 typedef GPIO_PORT2<
         1
         ,PAIN_L
         ,PAOUT_L
         ,PADIR_L
         ,PAIFG_L
         ,PAIES_L
         ,PAIE_L
         ,PASEL0_L
         ,PASEL1_L
         ,PAREN_L
         > P1;

 typedef GPIO_PIN<BIT0,P1> P1_0;
 typedef GPIO_PIN<BIT1,P1> P1_1;
 typedef GPIO_PIN<BIT2,P1> P1_2;
 typedef GPIO_PIN<BIT3,P1> P1_3;
 typedef GPIO_PIN<BIT4,P1> P1_4;
 typedef GPIO_PIN<BIT5,P1> P1_5;
 typedef GPIO_PIN<BIT6,P1> P1_6;
 typedef GPIO_PIN<BIT7,P1> P1_7;

#endif

/*
 * P2 port and P2.X shorthand typedefs
 */
#ifdef __MSP430_HAS_PORT2_R__

 typedef GPIO_PORT2<
         2
         ,PAIN_H
         ,PAOUT_H
         ,PADIR_H
         ,PAIFG_H
         ,PAIES_H
         ,PAIE_H
         ,PASEL0_H
         ,PASEL1_H
         ,PAREN_H
         > P2;

 typedef GPIO_PIN<BIT0,P2> P2_0;
 typedef GPIO_PIN<BIT1,P2> P2_1;
 typedef GPIO_PIN<BIT2,P2> P2_2;
 typedef GPIO_PIN<BIT3,P2> P2_3;
 typedef GPIO_PIN<BIT4,P2> P2_4;
 typedef GPIO_PIN<BIT5,P2> P2_5;
 typedef GPIO_PIN<BIT6,P2> P2_6;
 typedef GPIO_PIN<BIT7,P2> P2_7;

#endif

 /*
  * P3 port and P3.X shorthand typedefs
  */
#ifdef __MSP430_HAS_PORT3_R__
 typedef GPIO_PORT2<
         3
         ,PBIN_L
         ,PBOUT_L
         ,PBDIR_L
         ,PBIFG_L
         ,PBIES_L
         ,PBIE_L
         ,PBSEL0_L
         ,PBSEL1_L
         ,PBREN_L
         > P3;

 typedef GPIO_PIN<BIT0,P3> P3_0;
 typedef GPIO_PIN<BIT1,P3> P3_1;
 typedef GPIO_PIN<BIT2,P3> P3_2;
 typedef GPIO_PIN<BIT3,P3> P3_3;
 typedef GPIO_PIN<BIT4,P3> P3_4;
 typedef GPIO_PIN<BIT5,P3> P3_5;
 typedef GPIO_PIN<BIT6,P3> P3_6;
 typedef GPIO_PIN<BIT7,P3> P3_7;

#endif

 /*
  * P4 port and P4.X shorthand typedefs
  */
#ifdef __MSP430_HAS_PORT4_R__
 typedef GPIO_PORT2<
         4
         ,PBIN_H
         ,PBOUT_H
         ,PBDIR_H
         ,PBIFG_H
         ,PBIES_H
         ,PBIE_H
         ,PBSEL0_H
         ,PBSEL1_H
         ,PBREN_H
         > P4;

 typedef GPIO_PIN<BIT0,P4> P4_0;
 typedef GPIO_PIN<BIT1,P4> P4_1;

#endif

 /*
  * PJ port and PJ.X shorthand typedefs
  */
#ifdef __MSP430_HAS_PORTJ_R__
 typedef GPIO_PORT_BASE2<
         5
         ,PJIN_L
         ,PJOUT_L
         ,PJDIR_L
         ,PJSEL0_L
         ,PJSEL1_L
         ,PJREN_L
         > PJ;

 typedef GPIO_PIN<BIT0,PJ> PJ_0;
 typedef GPIO_PIN<BIT1,PJ> PJ_1;
 typedef GPIO_PIN<BIT2,PJ> PJ_2;
 typedef GPIO_PIN<BIT3,PJ> PJ_3;
 typedef GPIO_PIN<BIT4,PJ> PJ_4;
 typedef GPIO_PIN<BIT5,PJ> PJ_5;

#endif

#ifdef __MSP430_HAS_PORTJ_R__
 typedef PJ_0 RED_LED;
 typedef PJ_1 GREEN_LED;

 typedef PJ_0 LED1;
 typedef PJ_1 LED2;
 typedef PJ_2 LED3;
 typedef PJ_3 LED4;

 typedef RED_LED LED_BUILTIN;
#endif

#ifdef __MSP430_HAS_PORT3_R__
 typedef P3_4 LED5;
 typedef P3_5 LED6;
 typedef P3_6 LED7;
 typedef P3_7 LED8;
#endif

#ifdef __MSP430_HAS_PORT4_R__
 typedef GPIO_PIN<BIT0,P4,gpio_pin_button> PUSH1;
 typedef GPIO_PIN<BIT1,P4,gpio_pin_button> PUSH2;
#endif

#ifdef __MSP430_HAS_PORT2_R__
 typedef P2_0 TX_PIN;
 typedef P2_1 RX_PIN;

 typedef P2_2 SCLK;
#endif

 typedef P1_6 MOSI;
 typedef P1_7 MIS0;

#endif /* MSP430FR5739_NOBOARD_H_ */
