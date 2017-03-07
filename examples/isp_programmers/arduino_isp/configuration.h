#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// pin configurations
typedef PB9  _PIN_SCK;    /* these 4 pins were selected because they are 5V tolerant */
typedef PB8  _PIN_MISO;   /* we use bitbanged spi to make life simpler */
typedef PB7  _PIN_MOSI;
typedef PB6  _PIN_RESET;

typedef PC13 _LED_HB;    /* (led builtin) shows the programmer is running */
typedef PC14 _LED_ERR;   /* (red led) Lights up if something goes wrong */
typedef PC15 _LED_PMODE; /* (blue led) In communication with the slave */

typedef PA8  _CLK_PIN;   /* ~1MHz rescue clock */
                        /* useful to recover bad fuse setting */

#define SERIAL Serial
// Serial1 pins
//      TX1  PA9
//      RX1  PA10

// Configure the baud rate:
#define BAUD_RATE 19200

// Configure SPI clock (in Hz).
#define SPI_CLOCK (19200*4) /* 125k conservative speed even for 1MHz */


#define HWVER 2
#define SWMAJ 1
#define SWMIN 18

#define PROG_FLICKER false

#endif
