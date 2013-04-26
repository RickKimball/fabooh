/*
 * ws2811_basic.cpp - demo ws2811_basic,
 *
 * Created: Mar 1, 2013
 *  Author: rick@kimballsoftware.com
 *    Date: 03-01-2013
 * Version: 1.0.0
 *
 * $ msp430-size ws2811_basic.elf
 *  text     data     bss     dec     hex filename
 *   300        0       0     300     134 ws2811_basic.elf
 */

#include <fabooh.h>
#include <drivers/ws2811.h>
#include <main.h>

typedef led::WS2811<4, P1_0> ws2811; // this routine only works @16MHz

const uint8_t frames[][12] = {
    { 0x00,0x0f,0x00, /* red 50% */
      0x0f,0x00,0x00, /* green 50% */
      0x00,0x00,0x0f, /* blue 50% */
      0x0f,0x0f,0x0f  /* white 50% */
    }
   ,{ 0x00,0x7f,0x00, /* red 75% */
      0x7f,0x00,0x00, /* green 75% */
      0x00,0x00,0x7f, /* blue 75% */
      0x7f,0x7f,0x7f  /* white 75% */
    }
};

inline void setup() {

  ws2811::begin();

  do {
    ws2811::write(&frames[0][0]);
    delay(300);
    ws2811::write(&frames[1][0]);
    delay(300);
  } while (1);

}

void loop() {}
