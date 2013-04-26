/*
 * spi_bitbang.h - SPI implementation using port pin toggling
 *
 *  Created: Mar 13, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: Mar 13, 2013
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
 *
 *  TODO: work in progress many things missing and may be wrong
 */

#ifndef SPI_BITBANG_H_
#define SPI_BITBANG_H_

template<typename CS, typename SCLK, typename MOSI, typename MISO, typename ATTR >
struct spi_bitbang_t: spi_base_t<CS, SCLK, MOSI, MISO, ATTR > {
  typedef spi_base_t<CS, SCLK, MOSI, MISO, ATTR > base;

  static void begin() {
    CS::high();
    CS::setmode_output();
    MOSI::setmode_output();
    MISO::setmode_input();
    // set CPOL
    if (base::attributes().mode == SPI_MODE0 || base::attributes().mode == SPI_MODE1) {
      SCLK::low();
    }
    else {
      SCLK::high();
    }
    SCLK::setmode_output();
  }

  static void end() {
    CS::low();
    CS::setmode_input();
    MOSI::setmode_input();
    SCLK::low();
    SCLK::setmode_input();
  }

  // TODO: implement
  static uint8_t shiftin() {
    return 0;
  }

  static void shiftout(uint8_t c) {
    register unsigned mask = ((base::attributes().bitorder == MSBLSB) ? 0x80 : 0x01);

    do {
      if (c & mask) {
        MOSI::high();
      }
      else {
        MOSI::low();
      }

      SCLK::toggle();
      if (base::attributes().bitorder == MSBLSB) {
        mask >>= 1;
      }
      else {
        mask <<= 1;
      }
      //__delay_cycles(10); // make a square clock signal
      SCLK::toggle();

    } while (mask & 0x00ff);
  }

  ALWAYS_INLINE
  static void shiftout(register uint8_t const *c, register unsigned cnt) {
    if (!cnt) return;

    do {
      shiftout(*c++);
    } while (--cnt);
  }

  // TODO: implement
  static uint8_t transfer(uint8_t _data) {
    return 0;
  }

};

#endif /* SPI_BITBANG_H_ */
