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

  template<const uint16_t _led_count, typename WS2811_DIN>
  struct WS2811 {

  public:
      static void begin() {
        WS2811_DIN::setmode_output();
      }

      static void end() {
        WS2811_DIN::setmode_input();
      }

      /*
       * Thanks to Kevin Timmeran's awesome code
       * see: http://forum.43oh.com/topic/2852-ws2811-led-controller-driver/
       *
       */
      __attribute__ ((noinline))
      static void _write(register const uint8_t *data, register uint16_t datalen) {
        register unsigned mask=WS2811_DIN::pin_mask, bitcnt=7, bits=0;

        __asm__ volatile (
            "1:                                      ;\n"
            "        mov     #7, %[bitcnt]           ; Do 7 bits in a loop\n"
            "        mov.b   @%[data]+, %[bits]      ; Get next byte from buffer\n"
            "2:                                      ; - Bit loop - 20 cycles per bit\n"
            "        rla.b   %[bits]                 ; Get next bit\n"
            "        jc      3f                      ; Jump if one...\n"
            "        bis.b   %[mask], %[WS2811_POUT] ; Output high - 4 cycle\n"
            "        bic.b   %[mask], %[WS2811_POUT] ; Output low - 4 cycles elapsed\n"
            "        bic.b   %[mask], %[WS2811_POUT] ; 4 cycle nop\n"
            "        jmp     4f                      ; Next bit...\n"
            "3:                                      ;\n"
            "        bis.b   %[mask], %[WS2811_POUT] ; 4 cycle Output high\n"
            "        bis.b   %[mask], %[WS2811_POUT] ; 4 cycle nop\n"
            "        jmp     $ + 2                   ; 2 cycle nop\n"
            "        bic.b   %[mask], %[WS2811_POUT] ; Output low - 10 cycles elapsed\n"
            "4:                                      ;\n"
            "        dec     %[bitcnt]               ; Decrement bit count\n"
            "        jne     2b                      ; Do next bit of not zero...\n"
            "                                        ;\n"
            "        rla.b   %[bits]                 ; Get final bit of byte\n"
            "        jc      5f                      ; Jump if one...\n"
            "        bis.b   %[mask], %[WS2811_POUT] ; Output high\n"
            "        bic.b   %[mask], %[WS2811_POUT] ; Output low - 4 cycles elapsed\n"
            "        jmp     $ + 2                   ; 2 cycle nop\n"
            "        dec     %[bytecnt]              ; Decrement byte count\n"
            "        jne     1b                      ; Next byte if count not zero...\n"
            "        jmp     6f                      ; All bytes done, reset...\n"
            "5:                                      ;\n"
            "        bis.b   %[mask], %[WS2811_POUT] ; Output high\n"
            "        jmp     $ + 2                   ; 2 cycle nop\n"
            "        mov     #7, %[bitcnt]           ; Reset bit counter\n"
            "        mov.b   @%[data]+, %[bits]      ; Get next byte from buffer\n"
            "        bic.b   %[mask], %[WS2811_POUT] ; Output low - 10 cycles elapsed\n"
            "        dec     %[bytecnt]              ; Decrement byte count\n"
            "        jne     2b                      ; Do next byte if count is not zero...\n"
            "                                        ;\n"
            "6:                                      ;\n"
            "        mov     #800 / 3, %[bits]       ; 800 cycle delay (50usec) for reset\n"
            "        dec     %[bits]                 ;\n"
            "        jne     $ - 2                   ;\n"

            :  [bitcnt]  "+r" (bitcnt)
              ,[bits]    "+r" (bits)
              ,[bytecnt] "+r" (datalen)
              ,[data]    "+r" (data)

            :  [mask]        "r" (mask)
              ,[WS2811_POUT] "m" (WS2811_DIN::POUT())

            : "cc"
            );
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
