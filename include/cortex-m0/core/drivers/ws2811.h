/*
 * ws2811.h - basic ws2811 driver
 *
 *  Created: Mar 1, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: 3-1-2013
 *  Version: 1.0.0
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

#ifndef WS2811_H_
#define WS2811_H_

#include <fabooh.h>

namespace led {

  template<const uint16_t _led_count, typename WS2811_DIN=MOSI>
  struct WS2811 {

  public:
      static void begin() {
        LPC_SYSCON->PRESETCTRL    |= (1 << 0);            // clear SPI reset, SPI held in reset by default (3.5.2)
        LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 11 | 1 << 16); // enable clock to SPI0 block (3.5.14)

        // reset the ws2811
        // make MOSI a pull-down pin
        LPC_IOCON->PIO0_9 = (LPC_IOCON->PIO0_9 & ~(0b11<<3)) | (0b01 << 3);
        delay(2);

        // SPI configuration
        // MOSI pull-down resistor,
        LPC_IOCON->PIO0_9 = (LPC_IOCON->PIO0_9 & ~(0x3)) | 0b01;  // MOSI function (7.4.23)

      #if 0
        // (optional) we don't really need to output the clock, it is useful for debugging SPI speed
        LPC_IOCON->SCK_LOC = 0x02;  // Selects SCK0 function in pin location PIO0_6/SCK0
        LPC_IOCON->PIO0_6  = 0x02;  // SCK function (sec. 7.4.9)
      #endif

        // 800kHz = F_CPU=48000000/SSP0CLKDIV=1/CPSR=2/(SCR=1)+1/BITS=15
        // 400kHz = F_CPU=24000000/SSP0CLKDIV=1/CPSR=2/(SCR=1)+1/BITS=15
        // 400kHz = F_CPU=12000000/SSP0CLKDIV=1/CPSR=2/(SCR=0)+1/BITS=15
        LPC_SYSCON->SSP0CLKDIV = 1; // enable SPI clk by writing non-zero clk divisor (3.5.15)
        LPC_SSP0->CPSR = 0x2;       // spi clock prescaler (14.6.5)
        static const unsigned SCR=(F_CPU == 12000000UL) ? 0 : 1;
        // 15 bit transfer, SPI frame format, CPOL=0 + CPHA=1, SCR=1 (14.6.1)
        LPC_SSP0->CR0 = (15-1) | (0b00 << 4) | (0b10 << 6) | (SCR << 8);
        LPC_SSP0->CR1 |= 0x01 << 1; // enable SPI (14.6.2)
      }

      static void end() {
        LPC_IOCON->PIO0_9 = (LPC_IOCON->PIO0_9 & ~(0x3));  // revert it back to GPIO pin
      }

      static void _write(register const uint8_t *data, register uint16_t datalen) {
        unsigned int bit_mask, bits;
          do {
            bit_mask = 1 << 7;
            do {
              if (*data & bit_mask ) {
                // ~600ns high / ~625ns low at 800kHz
                bits = 0b111111100000000; //transmit data upper 8 bits of conversion result (section 14.6.3)
              }
              else {
                // ~250ns high / ~1000ns low at 800kHz
                bits = 0b111000000000000; //transmit data upper 8 bits of conversion result (section 14.6.3)
              }
              while ( !(LPC_SSP0->SR & (1<<1)) ); // wait until !TNF (Transmit Not Full) (14.6.4)
              LPC_SSP0->DR = bits;
              bit_mask >>= 1;
            } while(bit_mask);
            data++;
          } while(--datalen);
          while ( !(LPC_SSP0->SR & (1<<4)) ); // wait until ! BSY (Busy) (14.6.4)
          delay(1);
      }

      __attribute__ ((noinline))
      static void write(register const uint8_t *data, register uint16_t led_count) {
        _write(data, led_count*3);
      };

      __attribute__ ((noinline))
      static void write(register const uint8_t *data) {
        register uint16_t cnt=_led_count*3;
        _write(data, cnt);
      };
  };

} // end led namespace

#endif /* WS2811_H */
