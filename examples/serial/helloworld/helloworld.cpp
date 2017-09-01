/**
 * helloworld.cpp - an msp430g2xxx specific watchdog timer example
 *
 * msp430-size msp430g2553in20_release/helloworld.elf
 *  text	   data	    bss	    dec	    hex	filename
 *   252	      0	      0	    252	     fc	msp430g2553in20_release/helloworld.elf
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>
#include <stdlib.h>

#ifdef __MSP430__
 extern "C" void __do_copy_data () {; }
 extern "C" void __do_clear_bss () {; }

 #if 0
   sw_serial_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;
 #else
   usci_serial_t < 9600, CPU::frequency, TX_PIN, NO_PIN > Serial;
 #endif

#elif defined(STM32F1)

 #if 1
   serial_usart_isr_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;
   USART_IRQHandler(1,Serial)
 #else
   serial_default_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;
 #endif
#else
   serial_default_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;
#endif

inline void setup(void)
{
  Serial.begin();
}

inline void loop(void)
{
  Serial.print("\r\nHello world\r\n");
  Serial.print(1234,DEC);

  while(1);
}

