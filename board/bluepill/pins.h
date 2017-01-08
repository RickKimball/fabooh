#ifndef PINS_H_
#define PINS_H_

typedef GPIO_PIN_LED<3,13,false> LED_BUILTIN_;

typedef GPIO_PIN<1,9> TX1_PIN;
typedef GPIO_PIN<1,10> RX1_PIN;

typedef GPIO_PIN<1,2> TX2_PIN;
typedef GPIO_PIN<1,3> RX2_PIN;

typedef GPIO_PIN<2,10> TX3_PIN;
typedef GPIO_PIN<2,11> RX3_PIN;

typedef GPIO_PIN<0,0xFFFF> NO_PIN;

typedef TX2_PIN TX_PIN;
typedef RX2_PIN RX_PIN;

#endif
