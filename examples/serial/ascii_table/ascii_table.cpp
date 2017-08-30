/**
 * ascii_table.cpp - ASCII table print using USCI serial template
 *
 * Note: the size of .elf is determined by typeof(counter_t)
 *
 * 468 bytes unsigned thisByte
 *   Writing  370 bytes at c000 [section: .text]...
 *   Writing   66 bytes at c172 [section: .rodata]...
 *   Writing   32 bytes at ffe0 [section: .vectors]...
 *
 * 498 bytes int8_t thisByte
 * 468 bytes uint8_t thisByte
 * 498 bytes int thisByte
 * 562 bytes unsigned long thisByte
 * 606 bytes long thisByte
 *
 */

#include <fabooh.h>
#define SMALL_INIT4
#include <main.h>
#include <serial.h>

typedef int counter_t; /* type we use for counting from ' ' -> '~' */

//------- file space globals ------
namespace {
  static const uint32_t BAUD_RATE=9600;
#if 0
  usci_serial_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#elif 0
  sw_serial_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#elif 0
  sw_serial_t<BAUD_RATE, CPU::frequency, P1_1, NO_PIN> Serial; // xmit only serial
#elif 0
  timer_serial_t<BAUD_RATE, CPU::frequency, P1_1, NO_PIN> Serial; // xmit only serial
#else
  serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#endif
  counter_t thisByte; // use the standard __do_data_copy() routines from libc
}

inline void setup(void)
{
  Serial.begin();

  // prints title with ending line break
  Serial.print("\nASCII Table ~ Character Map\n");
  thisByte=' ';
}

void loop()
{
  // prints value unaltered, i.e. the raw binary version of the
  // byte. The serial monitor interprets all bytes as
  // ASCII, so 32, the first number,  will show up as ' ' AKA a space

  Serial.print((char)thisByte);

  // base 10
  Serial.print(", dec: "); Serial.print(thisByte);

  // base 8
  Serial.print(", oct: \\0"); Serial.print(thisByte,OCT);

  // base 16
  Serial.print(", hex: 0x"); Serial.print(thisByte,HEX);

  // base 2
  Serial.print(", bin: 0b"); Serial.print(thisByte,BIN);

  // new line
  Serial.println();

  // if printed last visible character '~' or 126, stop:
  if( thisByte == '~') {
    // this powers down the chip and sleeps forever
    while(1);
  }
  // go on to the next character
  thisByte++;
}
