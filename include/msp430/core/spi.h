/*
 * spi.h - spi_base_t serial peripheral interface base template
 *
 *  Created: Mar 13, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: Mar 13, 2013
 *  Version: 1.0.0
 *
 *  See: http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus
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

#ifndef SPI_H_
#define SPI_H_

#include <msp430.h>


/*
 * spi_mode_e -
 */
enum spi_mode_e {
   SPI_MODE0=0 // CPOL=0, CHPA=0
  ,SPI_MODE1=1 // CPOL=0, CHPA=1
  ,SPI_MODE2=2 // CPOL=1, CHPA=0
  ,SPI_MODE3=4 // CPOL=1, CHPA=1
};

/*
 * spi_bitorder_e -
 */
enum spi_bitorder_e {
   MSBLSB=0x00 // send most significant bits first
  ,LSBMSB=0x01 // send least significant bits first
};

/*
 * spi_clockdiv_e =
 */
enum spi_clockdiv_e {
    SPI_CLOCK_DIV1=1
   ,SPI_CLOCK_DIV2=2
   ,SPI_CLOCK_DIV4=4
   ,SPI_CLOCK_DIV8=8
   ,SPI_CLOCK_DIV16=16
   ,SPI_CLOCK_DIV32=32
   ,SPI_CLOCK_DIV64=64
   ,SPI_CLOCK_DIV128=128
};

/*
 * spi_attr_t - configuration for mode, bit order and speed
 *
 * expectation is that each device will set the proper pins
 */
template<
  spi_mode_e MODE=SPI_MODE0,
  spi_bitorder_e BITORDER=MSBLSB,
  spi_clockdiv_e DIVISOR=SPI_CLOCK_DIV4
>
struct spi_attr_t {
  static const spi_mode_e mode=MODE;
  static const spi_bitorder_e bitorder=BITORDER;
  static const spi_clockdiv_e divisor=DIVISOR;
};

template<typename CS, typename SCLK, typename MOSI, typename MISO, typename ATTR_T>
struct spi_base_t {

  static const ATTR_T & attributes() {
    static const ATTR_T _attr;
    return _attr;
  }

#if 0
  static void begin();
  static void end();

  static void set_bitorder(uint8_t);
  static void set_datamode(uint8_t);
  static void set_speed(uint8_t);

  static uint8_t transfer(uint8_t _data);
  static void shiftout(uint8_t _data);
  static uint8_t shiftin();
#endif

};

#include <drivers/spi_usi.h>
#include <drivers/spi_usci.h>
#include <drivers/spi_eusci.h>
#include <drivers/spi_bitbang.h>

#endif /* SPI_H_ */
