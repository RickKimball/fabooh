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

      __attribute__ ((noinline))
      static void _write(register const uint8_t *data, register uint16_t datalen) {
        //register unsigned mask=WS2811_DIN::pin_mask, bitcnt=7, bits=0;
        // TODO: implement this
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
