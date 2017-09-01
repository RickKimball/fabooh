/*
 * pins.h
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef PINS_H_
#define PINS_H_

#include <LPC11xx.h>

typedef GPIO_PORT_BASE<P0_PORTNO> P0;
typedef GPIO_PIN<(1 << 0),P0> P0_0;
typedef GPIO_PIN<(1 << 1),P0> P0_1;
typedef GPIO_PIN<(1 << 2),P0> P0_2;
typedef GPIO_PIN<(1 << 3),P0> P0_3;
typedef GPIO_PIN<(1 << 4),P0> P0_4;
typedef GPIO_PIN<(1 << 5),P0> P0_5;
typedef GPIO_PIN<(1 << 6),P0> P0_6;
typedef GPIO_PIN<(1 << 7),P0> P0_7;
typedef GPIO_PIN<(1 << 8),P0> P0_8;
typedef GPIO_PIN<(1 << 9),P0> P0_9;
typedef GPIO_PIN<(1 << 10),P0> P0_10;
typedef GPIO_PIN<(1 << 11),P0> P0_11;

typedef GPIO_PORT_BASE<P1_PORTNO> P1;
typedef GPIO_PIN<(1 << 0),P1> P1_0;
typedef GPIO_PIN<(1 << 1),P1> P1_1;
typedef GPIO_PIN<(1 << 2),P1> P1_2;
typedef GPIO_PIN<(1 << 3),P1> P1_3;
typedef GPIO_PIN<(1 << 4),P1> P1_4;
typedef GPIO_PIN<(1 << 5),P1> P1_5;
typedef GPIO_PIN<(1 << 6),P1> P1_6;
typedef GPIO_PIN<(1 << 7),P1> P1_7;
typedef GPIO_PIN<(1 << 8),P1> P1_8;
typedef GPIO_PIN<(1 << 9),P1> P1_9;

typedef P1_8 RED_LED;
typedef RED_LED LED_BUILTIN;
typedef P1_7 TX_PIN;
typedef P1_6 RX_PIN;

typedef P0_6 SCLK;
typedef P0_9 MOSI;
typedef P0_8 MISO;
typedef P0_2 SS;

#endif /* PINS_H_ */
