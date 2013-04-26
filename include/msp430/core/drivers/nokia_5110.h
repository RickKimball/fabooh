/*
 * nokia_5110.h - provide API for Nokia 5110
 *
 * fabooh adaptation of:
 *    http://forum.43oh.com/topic/1769-nokia-5110-c-template-class/
 *
 *  Date: Dec 12, 2012
 *  Author: rick@kimballsoftware.com
 *  Version: 1.0.0
 *
 * =========================================================================
 *  Copyright © 2012 Kevin Timmerman
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
 */

#include <fabooh.h>

#ifndef USE_SPI
#define USE_SPI 1
#endif

// lcd::nokia::Nokai5110 {}
namespace lcd {
namespace nokia {

typedef enum {
    lcd_command = 0,        // Array of one or more commands
    lcd_data = 1,           // Array of one or more bytes of data
    lcd_data_repeat = 2     // One byte of data repeated
} lcd_cmd_type;

template <
  typename _SCLK,   /* serial clock */
  typename _DIN,    /* serial data in */
  typename _DC,     /* data/command */
  typename _CE,     /* chip select */
  unsigned _RST=90  /* reset pulse duration in msecs */
>
struct Nokia5110
{
    void begin() {
    }

    void write(const unsigned char *cmd, unsigned len, const lcd_cmd_type type = lcd_data);
    void reset(void);
    void init(void);
    void vert(void);
    void home(void);
    void pos(unsigned char x, unsigned char y);
    void clear(unsigned char x = 0);
    void bitmap(const unsigned char *bmp, signed char x, signed char y, unsigned char w, unsigned char h);
    inline void bitmap(const unsigned char *bmp, signed char x, signed char y) { bitmap(bmp + 2, x, y, bmp[0], bmp[1]); };
    void print(char c);
    inline void print(unsigned char x, unsigned char y, char c) { pos(x, y); print(c); };
    void print(const char *s);
    inline void print(unsigned char x, unsigned char y, char *s) { pos(x, y); print(s); };
    void printv(unsigned char x, unsigned char y, char *s);

    ALWAYS_INLINE void shiftout(const unsigned char c) {
#if defined(USE_SPI) && USE_SPI
        UCB0TXBUF = c;             // setting TXBUF clears the TXIFG flag
        while (UCB0STAT & UCBUSY); // wait for SPI TX/RX to finish
#else
      register unsigned mask = 0x0080;

      do {
          if (c & mask) {
              _DIN::high();
          } else {
              _DIN::low();
          }
          _SCLK::low();
          mask >>= 1; // could be below this line just making the duty cycle longer
          _SCLK::high();
      } while(mask);
#endif
    }
};

template <typename _SCLK, typename _DIN, typename _DC, typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN, _DC,  _CE, _RST>::write(const unsigned char *cmd, unsigned len, const lcd_cmd_type type)
{
    _CE::low();

    do {
        if ( !type ) {
          _DC::low();
        }

        shiftout(*cmd);

        _DC::high();

        if(!(type & lcd_data_repeat)) ++cmd;
    } while(--len);

    _CE::high();
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::reset(void)
{
    // --- Set initial state of CLK, DC and CE as needed
    _SCLK::high();
    _CE::high();

    // --- Reset pulse on DC
    _DC::low();
    delay_msecs(_RST);
    _DC::high();
    delay_msecs(_RST);
}
 
template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::init(void)
{
    static const unsigned char init[] = {
        0x20 | 0x01,        // Function set - extended instructions enabled
        //0x80 | 64,            // Set vop (contrast) 0 - 127
        0x80 | 70,          // Higher contrast improves animation
        0x04 | 2,           // Temperature control
        0x10 | 3,           // Set bias system
        0x20 | 0x00,        // Function set - chip active, horizontal addressing, basic instructions
        0x08 | 0x04         // Display control - normal mode
    };
    
    write(init, sizeof(init), lcd_command);
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::vert(void)
{
    static const unsigned char v[] = {
        0x20 | 0x02,        // Function set - chip active, vertical addressing, basic instructions
    };
    
    write(v, sizeof(v), lcd_command);
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::home(void)
{
    static const unsigned char home[] = { 0x40, 0x80 };
    write(home, sizeof(home), lcd_command);
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::pos(unsigned char x, unsigned char y)
{
    unsigned char c[2];
    c[0] = 0x80 | x;
    c[1] = 0x40 | y;
    write(c, sizeof(c), lcd_command);
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::clear(unsigned char x)
{
    home();
    write(&x, 504 /*84*(48/8)*/, lcd_data_repeat);
    home();
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::bitmap(const unsigned char *bmp, signed char x, signed char y, unsigned char w, unsigned char h)
{
    unsigned char c[2];
    unsigned char ww;
    if(x < 0) {
        ww = w + x;
        bmp -= x;
        x = 0;
    } else if(x + w >= 84) {
        ww = (84 - x);
    } else {
        ww = w;
    }
    c[0] = 0x80 | x;
    c[1] = 0x40 | y;
    while(h--) {
        write(c, sizeof(c), lcd_command);
        ++c[1];
        write(bmp, ww);
        bmp += w;
    }
}

static const unsigned char font[96][5] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, //
    { 0x00, 0x00, 0x5F, 0x00, 0x00 }, // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // "
    { 0x14, 0x7F, 0x14, 0x7F, 0x14 }, // #
    { 0x24, 0x2A, 0x7F, 0x2A, 0x12 }, // $
    { 0x23, 0x13, 0x08, 0x64, 0x62 }, // %
    { 0x36, 0x49, 0x56, 0x20, 0x50 }, // &
    { 0x00, 0x08, 0x07, 0x03, 0x00 }, // '
    { 0x00, 0x1C, 0x22, 0x41, 0x00 }, // (
    { 0x00, 0x41, 0x22, 0x1C, 0x00 }, // )
    { 0x2A, 0x1C, 0x7F, 0x1C, 0x2A }, // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 }, // +
    { 0x00, 0x40, 0x38, 0x18, 0x00 }, // ,
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // -
    { 0x00, 0x00, 0x60, 0x60, 0x00 }, // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
    { 0x21, 0x41, 0x49, 0x4D, 0x33 }, // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
    { 0x41, 0x21, 0x11, 0x09, 0x07 }, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
    { 0x00, 0x00, 0x14, 0x00, 0x00 }, // :
    { 0x00, 0x00, 0x40, 0x34, 0x00 }, // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41 }, // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 }, // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 }, // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?
    { 0x3E, 0x41, 0x5D, 0x59, 0x4E }, // @
    { 0x7C, 0x12, 0x11, 0x12, 0x7C }, // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 }, // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 }, // C
    { 0x7F, 0x41, 0x41, 0x41, 0x3E }, // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 }, // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 }, // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A }, // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F }, // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 }, // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 }, // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 }, // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 }, // L
    { 0x7F, 0x02, 0x1C, 0x02, 0x7F }, // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F }, // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E }, // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 }, // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E }, // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 }, // R
    { 0x26, 0x49, 0x49, 0x49, 0x32 }, // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 }, // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F }, // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F }, // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F }, // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, // X
    { 0x03, 0x04, 0x78, 0x04, 0x03 }, // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x41 }, // [
    { 0x02, 0x04, 0x08, 0x10, 0x20 }, // '\'
    { 0x00, 0x41, 0x41, 0x41, 0x7F }, // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^
    { 0x80, 0x80, 0x80, 0x80, 0x80 }, // _
    { 0x00, 0x03, 0x07, 0x08, 0x00 }, // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
    { 0x7F, 0x28, 0x44, 0x44, 0x38 }, // b
    { 0x38, 0x44, 0x44, 0x44, 0x28 }, // c
    { 0x38, 0x44, 0x44, 0x28, 0x7F }, // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 }, // e
    { 0x00, 0x08, 0x7E, 0x09, 0x02 }, // f
    { 0x18, 0xA4, 0xA4, 0xA4, 0x7C }, // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 }, // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 }, // i
    { 0x00, 0x20, 0x40, 0x40, 0x3D }, // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44 }, // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 }, // l
    { 0x7C, 0x04, 0x78, 0x04, 0x78 }, // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 }, // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 }, // o
    { 0xFC, 0x18, 0x24, 0x24, 0x18 }, // p
    { 0x18, 0x24, 0x24, 0x18, 0xFC }, // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 }, // r
    { 0x48, 0x54, 0x54, 0x54, 0x24 }, // s
    { 0x04, 0x04, 0x3F, 0x44, 0x24 }, // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C }, // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C }, // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C }, // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 }, // x
    { 0x4C, 0x90, 0x90, 0x90, 0x7C }, // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 }, // z
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // {
    { 0x00, 0x00, 0x77, 0x00, 0x00 }, // |
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // }
    { 0x02, 0x01, 0x02, 0x04, 0x02 }, // ~
    { 0x00, 0x06, 0x09, 0x09, 0x06 }  // degrees
};

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::print(char c)
{
    write(&font[c - 32][0], 5);
    write(&font[0][0], 1);
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::print(const char *s)
{
    while(*s) {
        write(&font[*s - 32][0], 5);
        write(&font[0][0], 1);
        ++s;
    }
}

template <typename _SCLK, typename _DIN,  typename _DC,  typename _CE, unsigned _RST>
void Nokia5110<_SCLK, _DIN,  _DC,  _CE, _RST>::printv(unsigned char x, unsigned char y, char *s)
{
    while(*s) {
        pos(x, y);
        ++y;
        write(&font[*s - 32][0], 5);
        write(&font[0][0], 1);
        ++s;
    }
}

  } // namespace
} // namespace
