/*
 *   bitbangedspi.h - use bitbanged spi using delayMicroseconds for timing
 */

#ifndef _BITBANGEDSPI_H_
#define _BITBANGEDSPI_H_

#define SPI_MODE0 0x00
#define MSBFIRST 0x00

#if 0
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
#endif

template< typename PIN_SCK, typename PIN_MISO, typename PIN_MOSI >
struct BitBangedSPI {
  public:
    void begin() {
      PIN_SCK SCK;
      PIN_MOSI MOSI;
      PIN_MISO MISO;

      SCK.low();
      MOSI.low();
      SCK.setmode_output();
      MOSI.setmode_output();
      pinMode(MISO, INPUT);
    }

    void beginTransaction(SPISettings settings) {
      pulseWidth = (500000 + settings.clock - 1) / settings.clock;
      if (pulseWidth == 0)
        pulseWidth = 1;
    }

    void end() {
      PIN_SCK SCK;
      PIN_MOSI MOSI;
      // revert output pins back to input
      SCK.setmode_input();
      MOSI.setmode_input();
    }

    uint8_t transfer (uint8_t b) {
      extern void delayMicroseconds(uint32_t us);

      for (unsigned int i = 0; i < 8; ++i) {
        PIN_SCK SCK;
        PIN_MOSI MOSI;
        PIN_MISO MISO;

        digitalWrite(MOSI, (b & 0x80) ? HIGH : LOW);
        digitalWrite(SCK, HIGH);
        delayMicroseconds(pulseWidth);
        b = (b << 1) | digitalRead(MISO);
        digitalWrite(SCK, LOW); // slow pulse
        delayMicroseconds(pulseWidth);
      }
      return b;
    }

    uint8_t get_pulseWidth() {
      return (uint8_t)pulseWidth;
    }

  private:
    unsigned long pulseWidth; // in microseconds
};

#endif
