/*
 * pins.h - configuration for msp430fr5969 launchpad FRAM
 *
 * Created: 08-27-2014
 *  Author: rick@kimballsoftware.com
 *    Date: 08-27-2014
 * Version: 1.0.0
 *
 * =========================================================================
 *  Copyright © 2014 Rick Kimball
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

#ifndef PINS_H_
#define PINS_H_

#include <msp430.h>

#ifdef __MSP430_HAS_PORT1_R__
 typedef GPIO_PORT2<
         1
         ,PAIN_L
         ,PAOUT_L
         ,PADIR_L
         ,PASEL0_L
         ,PASEL1_L
         ,PAREN_L
         ,PAIFG_L
         ,PAIES_L
         ,PAIE_L
         > P1;

 typedef GPIO_PIN<BIT0,P1> P1_0;
 typedef GPIO_PIN<BIT1,P1> P1_1;
 typedef GPIO_PIN<BIT2,P1> P1_2;
 typedef GPIO_PIN<BIT3,P1> P1_3;
 typedef GPIO_PIN<BIT4,P1> P1_4;
 typedef GPIO_PIN<BIT5,P1> P1_5;
 typedef GPIO_PIN<BIT6,P1> P1_6;
 typedef GPIO_PIN<BIT7,P1> P1_7;

 typedef P1_0 GREEN_LED;

#endif /* __MSP430_HAS_PORT1_R__ */

#ifdef __MSP430_HAS_PORT2_R__
 typedef GPIO_PORT2<
         2
         ,PAIN_H
         ,PAOUT_H
         ,PADIR_H
         ,PASEL0_H
         ,PASEL1_H
         ,PAREN_H
         ,PAIFG_H
         ,PAIES_H
         ,PAIE_H
         > P2;

 typedef GPIO_PIN<BIT0,P2> P2_0;
 typedef GPIO_PIN<BIT1,P2> P2_1;
 typedef GPIO_PIN<BIT2,P2> P2_2;
 typedef GPIO_PIN<BIT3,P2> P2_3;
 typedef GPIO_PIN<BIT4,P2> P2_4;
 typedef GPIO_PIN<BIT5,P2> P2_5;
 typedef GPIO_PIN<BIT6,P2> P2_6;
 typedef GPIO_PIN<BIT7,P2> P2_7;

#endif /* __MSP430_HAS_PORT2_R__ */

 #ifdef __MSP430_HAS_PORT3_R__
 typedef GPIO_PORT2<
         3
         ,PBIN_L
         ,PBOUT_L
         ,PBDIR_L
         ,PBSEL0_L
         ,PBSEL1_L
         ,PBREN_L
         ,PBIFG_L
         ,PBIES_L
         ,PBIE_L
         > P3;

 typedef GPIO_PIN<BIT0,P3> P3_0;
 typedef GPIO_PIN<BIT1,P3> P3_1;
 typedef GPIO_PIN<BIT2,P3> P3_2;
 typedef GPIO_PIN<BIT3,P3> P3_3;
 typedef GPIO_PIN<BIT4,P3> P3_4;
 typedef GPIO_PIN<BIT5,P3> P3_5;
 typedef GPIO_PIN<BIT6,P3> P3_6;
 typedef GPIO_PIN<BIT7,P3> P3_7;

#endif /* __MSP430_HAS_PORT3_R__ */

#ifdef __MSP430_HAS_PORT4_R__
 typedef GPIO_PORT2<
         4
         ,PBIN_H
         ,PBOUT_H
         ,PBDIR_H
         ,PBSEL0_H
         ,PBSEL1_H
         ,PBREN_H
         ,PBIFG_H
         ,PBIES_H
         ,PBIE_H
         > P4;

 typedef GPIO_PIN<BIT0,P4> P4_0;
 typedef GPIO_PIN<BIT1,P4> P4_1;
 typedef GPIO_PIN<BIT2,P4> P4_2;
 typedef GPIO_PIN<BIT3,P4> P4_3;
 typedef GPIO_PIN<BIT4,P4> P4_4;
 typedef GPIO_PIN<BIT5,P4> P4_5;
 typedef GPIO_PIN<BIT6,P4> P4_6;
 typedef GPIO_PIN<BIT7,P4> P4_7;

 typedef P4_6 RED_LED;

#endif /* __MSP430_HAS_PORT4_R__ */

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
 typedef GPIO_PIN<BIT6,PJ> PJ_6;
 typedef GPIO_PIN<BIT7,PJ> PJ_7;

 #endif /* __MSP430_HAS_PORTJ_R__ */

 typedef RED_LED LED_BUILTIN;
 typedef P1_0 LED2;
 typedef P4_6 LED1;
 typedef GPIO_PIN<BIT1, P4, gpio_pin_button > S1;
 typedef GPIO_PIN<BIT1, P1, gpio_pin_button > S2;

 typedef P3_4 CS;
 typedef P2_2 SCLK;
 typedef P1_7 MISO;
 typedef P1_6 MOSI;

 typedef P2_0 TX_PIN;
 typedef P2_1 RX_PIN;

 typedef P2_6 TX1_PIN;
 typedef P2_5 RX1_PIN;

#endif /* PINS_H_ */
