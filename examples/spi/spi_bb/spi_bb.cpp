/**
 * spi_bb.cpp - simple SPI bitbang shiftout test
 *
 * Pins Used: P2_0 (CS), P2_1(SCLK), P2_2(MOSI)
 */

#include <fabooh.h>
#include <main.h>
#include <spi.h>
#include <drivers/spi_bitbang.h>

#if defined(__MSP430G2553__)

spi_bitbang_t<P2_0, P2_1, P2_2, NO_PIN,
  spi_attr_t<SPI_MODE0, MSBLSB, SPI_CLOCK_DIV16>
> SPI;

void setup() {
  SPI.begin();
}

void loop() {
  P2_0::low(); // _cs_ enable SPI SELECT
  if ( 1 ) {
    // test multiple bytes
    SPI.shiftout((const uint8_t []){0xf0,0x0f},2);
  }
  else {
    // test one byte
    SPI.shiftout(0b11110000);
  }
  P2_0::high(); // -CS- disable SPI UNSELECT

  delay_msecs(10);
}
#else

void setup() {}

void loop() {}

#warning this code is just for msp430g2553 now and not really ready

#endif
