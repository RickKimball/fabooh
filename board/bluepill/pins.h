/*
 * pins.h - bluepill pin map
 */

#ifndef PINS_H_
#define PINS_H_

typedef GPIO_PIN<1,0> PA0;
typedef GPIO_PIN<1,1> PA1;
typedef GPIO_PIN<1,2> PA2;
typedef GPIO_PIN<1,3> PA3;
typedef GPIO_PIN<1,4> PA4;
typedef GPIO_PIN<1,5> PA5;
typedef GPIO_PIN<1,6> PA6;
typedef GPIO_PIN<1,7> PA7;
typedef GPIO_PIN<1,8> PA8;
typedef GPIO_PIN<1,9> PA9;
typedef GPIO_PIN<1,10> PA10;
typedef GPIO_PIN<1,11> PA11;
typedef GPIO_PIN<1,12> PA12;
typedef GPIO_PIN<1,13> PA13;
typedef GPIO_PIN<1,14> PA14;
typedef GPIO_PIN<1,15> PA15;

typedef GPIO_PIN<2,0> PB0;
typedef GPIO_PIN<2,1> PB1;
typedef GPIO_PIN<2,2> PB2;
typedef GPIO_PIN<2,3> PB3;
typedef GPIO_PIN<2,4> PB4;
typedef GPIO_PIN<2,5> PB5;
typedef GPIO_PIN<2,6> PB6;
typedef GPIO_PIN<2,7> PB7;
typedef GPIO_PIN<2,8> PB8;
typedef GPIO_PIN<2,9> PB9;
typedef GPIO_PIN<2,10> PB10;
typedef GPIO_PIN<2,11> PB11;
typedef GPIO_PIN<2,12> PB12;
typedef GPIO_PIN<2,13> PB13;
typedef GPIO_PIN<2,14> PB14;
typedef GPIO_PIN<2,15> PB15;

typedef GPIO_PIN<3,13> PC13;
typedef GPIO_PIN<3,14> PC14;
typedef GPIO_PIN<3,15> PC15;

typedef GPIO_PIN_LED<3,13,false> LED_BUILTIN_;

typedef PA9 TX1_PIN;
typedef PA10 RX1_PIN;

typedef PA2 TX2_PIN;
typedef PA3 RX2_PIN;

typedef PB10 TX3_PIN;
typedef PB11 RX3_PIN;

typedef GPIO_PIN<0,0xFFFF> NO_PIN;

typedef TX1_PIN TX_PIN;
typedef RX1_PIN RX_PIN;

#endif
