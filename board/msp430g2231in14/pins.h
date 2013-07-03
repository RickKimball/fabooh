/*
 * pins.h - msp430g2231 GPIO port and pins
 *
 * Created: Nov 20, 2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-02-2012
 * Version: 1.0.1
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

#ifndef MSP430G2231_NOBOARD_H_
#define MSP430G2231_NOBOARD_H_

/*
 * P1 port and P1.X shorthand typedefs
 */
#ifdef __MSP430_HAS_PORT1_R__
 typedef GPIO_PORT0<
         1
         ,P1IN
         ,P1OUT
         ,P1DIR
         ,P1IFG
         ,P1IES
         ,P1IE
         ,P1SEL
         ,P1REN
         > P1;

 typedef GPIO_PIN<BIT0,P1> P1_0;
 typedef GPIO_PIN<BIT1,P1> P1_1;
 typedef GPIO_PIN<BIT2,P1> P1_2;
 typedef GPIO_PIN<BIT3,P1> P1_3;
 typedef GPIO_PIN<BIT4,P1> P1_4;
 typedef GPIO_PIN<BIT5,P1> P1_5;
 typedef GPIO_PIN<BIT6,P1> P1_6;
 typedef GPIO_PIN<BIT7,P1> P1_7;

 typedef GPIO_PIN<BIT0,P1> RED_LED;
 typedef GPIO_PIN<BIT6,P1> GREEN_LED;
 typedef GPIO_PIN<BIT3,P1,gpio_pin_button> PUSH2;
 typedef P1_1 TX_PIN;
 typedef P1_2 RX_PIN;
 typedef P1_5 SCLK;
 typedef P1_6 MOSI;
 typedef P1_7 MISO;

#endif

/*
 * P2 port and P2.X shorthand typedefs
 */
#ifdef __MSP430_HAS_PORT2_R__

 typedef GPIO_PORT0<
        2
        ,P2IN
        ,P2OUT
        ,P2DIR
        ,P2IFG
        ,P2IES
        ,P2IE
        ,P2SEL
        ,P2REN
        > P2;

 typedef GPIO_PIN<BIT6,P2> P2_6;
 typedef GPIO_PIN<BIT7,P2> P2_7;
 typedef GPIO_PIN<BIT6,P2> XIN_PIN;
 typedef GPIO_PIN<BIT7,P2> XOUT_PIN;

 /**
  * P2.6 and P2.7 configured as XTAL pins by default
  * override setmode methods so they can be used as
  * GPIO pins
  */

/*
 * setmode_input() - P2.6 XIN specialization
 */
template<>
ALWAYS_INLINE void P2_6::setmode_input(void)
{
    P2SEL &= ~BIT6;
    P2DIR &= ~BIT6;
}

/*
 * setmode_output() - P2.6 XIN specialization
 */
template<>
ALWAYS_INLINE void P2_6::setmode_output()
{
    P2SEL &= ~BIT6;
    P2DIR |= BIT6;
}

/*
 * setmode_input() - P2.7 XOUT specialization
 */
template<>
ALWAYS_INLINE void P2_7::setmode_input(void)
{
    P2SEL &= ~BIT7;
    P2DIR &= ~BIT7;
}

/*
 * setmode_output() - P2.7 XOUT specialization
 */
template<>
ALWAYS_INLINE void P2_7::setmode_output(void)
{
    P2SEL &= ~BIT7;
    P2DIR |= BIT7;
}

#endif

#endif /* MSP430G2231_NOBOARD_H_ */
