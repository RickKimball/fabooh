/*
 * pins.h
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef PINS_H_
#define PINS_H_

#include <lpc1100l.h>

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
typedef P1_7 TX_PIN;
typedef P1_6 RX_PIN;

#endif /* PINS_H_ */
