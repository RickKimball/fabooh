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


class SPISettings {
  public:
    // clock is in Hz
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) : clock(clock) {
      (void) bitOrder;
      (void) dataMode;
    };

  public:
    uint32_t clock;
};

template<typename CS, typename SCLK, typename MOSI, typename MISO, typename ATTR >
struct spi_bitbang_t: spi_base_t<CS, SCLK, MOSI, MISO, ATTR > {
  typedef spi_base_t<CS, SCLK, MOSI, MISO, ATTR > base;

  CS _CS;
  SCLK _SCLK;
  MOSI _MOSI;
  MISO _MISO;

  void beginTransaction(SPISettings settings) {
    pulseWidth = (500000 + settings.clock - 1) / settings.clock;
    if (pulseWidth == 0)
      pulseWidth = 1;
    pulseWidth = 3;
  }

  void begin() {
    _CS.high();
    _CS.setmode_output();
    _MOSI.setmode_output();
    _MISO.setmode_input();
    // set CPOL
    if (base::attributes().mode == SPI_MODE0 || base::attributes().mode == SPI_MODE1) {
      _SCLK.low();
    }
    else {
      _SCLK.high();
    }
    _SCLK.setmode_output();
  }

  void end() {
    _CS.low();
    _CS.setmode_input();
    _MOSI.setmode_input();
    _SCLK.low();
    _SCLK.setmode_input();
  }

  void set_divisor(unsigned div) {
    ; // we have it hard coded for ~75kHz
  }

  void select(void) {
    _CS.low();
  }

  void deselect(void) {
    _CS.high();
  }

  // TODO: implement
  uint8_t shiftin() {
    return 0;
  }

  void shiftout(uint8_t c) {
    register unsigned mask = ((base::attributes().bitorder == MSBLSB) ? 0x80 : 0x01);

    // TBD: fix speed, his works at ~75kHz
    do {
      if (c & mask) {
        _MOSI.high();
      }
      else {
        _MOSI.low();
      }

      _SCLK.toggle();
      delay_us(3);
      if (base::attributes().bitorder == MSBLSB) {
        mask >>= 1;
      }
      else {
        mask <<= 1;
      }
      _SCLK.toggle();
      delay_us(2);

    } while (mask & 0x00ff);
  }

  __always_inline
  void shiftout(register uint8_t const *c, register unsigned cnt) {
    if (!cnt) return;

    do {
      shiftout(*c++);
    } while (--cnt);
  }

  uint8_t transfer (uint8_t b) {
    for (unsigned int i = 0; i < 8; ++i) {
      digitalWrite(_MOSI, (b & 0x80) ? HIGH : LOW);
      digitalWrite(_SCLK, HIGH);
      delay_us(3);
      b = (b << 1) | digitalRead(_MISO);
      digitalWrite(_SCLK, LOW); // slow pulse
      delay_us(2);
    }
    return b;
  }

  unsigned get_pulseWidth() {
	return pulseWidth;
  }

private:
  unsigned pulseWidth;
};

#endif /* SPI_BITBANG_H_ */
