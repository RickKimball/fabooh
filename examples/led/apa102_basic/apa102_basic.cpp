/**
 * APA-102 - test of 5 leds
 *
 * Author: rick@kimballsoftware.com
 * Date: 09-11-2014
 *
 */

#include <fabooh.h>
#include <main.h>

namespace {
  typedef spi_attr_t<SPI_MODE3> SPI_MODE3_A;

#if defined(__MSP430__)
  typedef spi_eusci_b0_t<CS, SCLK, MOSI, NO_PIN, SPI_MODE3_A > spi_t;
#elif defined(STM32F1)
  typedef spi_bitbang_t<CS, SCLK, MOSI, NO_PIN, SPI_MODE3_A > spi_t;
#endif

  spi_t spi;
}

static const unsigned char buff0[] = {
  0x00, 0x00, 0x00, 0x00, // start frame
  (0b111 << 5 | 31), 0x00, 0x00, 0xFF, // red
  (0b111 << 5 | 31), 0x00, 0xFF, 0x00, // green  
  (0b111 << 5 | 31), 0xFF, 0x00, 0x00, // blue
  (0b111 << 5 | 31), 0xFF, 0xFF, 0xFE, // white
  (0b111 << 5 | 31), 0x00, 0x00, 0x00, // off
  0xFF, 0xFF, 0xFF, 0xFF  // end frame
};

static const unsigned char buff1[] = {
  0x00, 0x00, 0x00, 0x00, // start frame
  (0b111 << 5 | 31), 0x00, 0x00, 0x00,
  (0b111 << 5 | 31), 0x00, 0x00, 0x00,
  (0b111 << 5 | 31), 0x00, 0x00, 0x00,
  (0b111 << 5 | 31), 0x00, 0x00, 0x00,
  (0b111 << 5 | 31), 0x00, 0x00, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF  // end frame
};

void setup(void)
{
  spi.begin();
  spi.set_divisor(8); // 1MHz spi
}

void loop(void)
{
  while(1) {
    spi.select();
    spi.shiftout(buff0, sizeof(buff0));
    spi.deselect();
    delay(1000);

    spi.select();
    spi.shiftout(buff1, sizeof(buff1));
    spi.deselect();
    delay(1000);

  }
}

