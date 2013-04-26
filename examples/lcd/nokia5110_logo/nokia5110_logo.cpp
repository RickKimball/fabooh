/*
 * nokia5110_logo.cpp - demo xbmto5110
 *
 * Created: Feb-3-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 02-28-2013
 * Version: 1.0.0
 *
 */

#include <fabooh.h>
#include <main.h>

#include <drivers/nokia_5110.h>

#include "bitmaps/lp_logo.inc"

typedef P1_0 LCD_DC;
typedef P1_4 LCD_CE;
//typedef P1_5 SCLK; // varies depending on the board selected
typedef MOSI SDI;    // varies depending on the board selected
typedef P1_6 LCD_BACKLIGHT;

inline void init_spi(void) {
  LCD_CE::setmode_output();
  LCD_CE::high(); // active low, so this disables

  SCLK::setmode_output();
  SDI::setmode_output();
  SCLK::high();
}

void setup() {
  LCD_DC::setmode_output();
  LCD_BACKLIGHT::setmode_output();
  LCD_DC::high();

  init_spi();

  lcd::nokia::Nokia5110<SCLK, SDI, LCD_DC, LCD_CE, 100> lcd;

  lcd.reset();
  lcd.init();
  lcd.clear();

  LCD_BACKLIGHT::high(); // turn off backlight

  do {
    lcd.clear();
    lcd.bitmap(lp_logo, (84 - 56) / 2, 0);
    while(1);
  } while (1);
}

void loop() {}
