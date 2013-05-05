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
#include <main.h>
#include <drivers/ws2811.h>

static const unsigned num_leds=4; // configured for a strip of 4 leds * low power can run from usb
typedef led::WS2811<num_leds, MOSI> ws2811; // this routine only works @16MHz on g2553, 12/24,48 on lpc1114

static const unsigned num_frames=4;
const uint8_t frames[num_frames][12] = {
    {
       0x00,0x00,0x00 /* turn them all off */
      ,0x00,0x00,0x00
      ,0x00,0x00,0x00
      ,0x00,0x00,0x00
    }
    ,{
      0x05,0x05,0x05  /* white 20% */
      ,0x00,0x05,0x00 /* red 20% */
      ,0x05,0x00,0x00 /* green 20% */
      ,0x00,0x00,0x05 /* blue 20% */
    }
    ,{
      0x5f,0x5f,0x5f  /* white 37d% */
      ,0x00,0x5f,0x00 /* red 37% */
      ,0x5f,0x00,0x00 /* green 37% */
      ,0x00,0x00,0x5f /* blue 37% */
    }
    ,{
      0xff,0xff,0xff  /* white 100% */
      ,0x00,0xff,0x00 /* red 100% */
      ,0xff,0x00,0x00 /* green 100% */
      ,0x00,0x00,0xff /* blue 100% */
    }
};

inline void setup() {
  ws2811::begin();

  do {
    unsigned framecnt=0;
    do {
      ws2811::write(&frames[framecnt][0]);
      delay(500);
    } while(++framecnt < num_frames);
  } while (1);

}

void loop() {}
