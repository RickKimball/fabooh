/*
 * 
 * File   : 910.c
 * Author : Minifloat 
 * Project: Launchprog
 * Version: 1.2 on 10 May 2012
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "devcodes.h"
#include "910.h"

inline void usdelay(void)
{
   uint8_t us = DELAYTIME;
   while (us--)
      asm(";");
}

#if 1
inline void delay910(uint8_t cnt)
{
   uint16_t i;
   
   while (cnt--)
   {
      i = 64 * DELAYTIME;
      while (i--)
         asm(";");
   }
}
#endif

inline void pulse_sck(void)
{
   usdelay();
   set_sck();
   usdelay();
   clr_sck();
   usdelay();
}

#if 0
uint8_t uart_getc(void)
{
   while (SoftSerial_empty())
      ;

   return((uint8_t)SoftSerial_read());
}

inline void uart_putc(uint8_t data)
{
   SoftSerial_xmit(data);
}

void uart_puts(const uint8_t *data)
{
   while (*data)
   {
      uart_putc(*data);
      data++;
   }
}
#endif

uint8_t wrser(uint8_t send)
{
   uint8_t rcv  = 0x00;
   uint8_t mask;
   
   for (mask = 0x80; mask != 0x00; mask /= 2)
   {
      if(send & mask)
      {
         set_mosi();
      }
      else
      {
         clr_mosi();
      }
      
      
      if(rd_miso())
         rcv |= mask;
      
      pulse_sck();
   }
   return(rcv);
}

inline void spiinit(uint8_t device)
{
   uint8_t count;
   
   set_rst();
   clr_sck();
   port_get();
   delay910(0xff);
   
   clr_rst();
   delay910(0xff);
   
   wrser(0xac);
   wrser(0x53);
   //only these devices show the sync fail?!
   if ((device >= 20) && (device <= 0x7F))
   {
      count = 32;
      
      do 
      {
         if (rdser() == 0x53)
            break;
         
         wrser(0x00);
         pulse_sck();
         wrser(0xac);
         wrser(0x53);
      } 
      while (--count);
         
   }
   else
   {
      wrser(0x00);
   }
   
   wrser(0x00);
   delay910(0x10);
}

inline void show_id(void)
{
   const static uint8_t id[] = "AVR ISP";
   uart_puts(id);
}

inline void init910(void)
{
   set_rst();
   led_init();
   ledr_off();
   ledg_off();
   port_release();
}

inline void waitcmd(void)
{
   // device specific stuff
   uint8_t device = 0, pgm_mode = 0, dev_ok = 1;
   
   // switch command var
   uint8_t sw;
   
   // counting or GP variables
   uint8_t i, j, k, l;
   
   // r/w address
   uint16_t addr = 0;
   
   // the outer while loop
   while (1)
   {
      //ready for some action
      ledg_on();
      
      
      dev_ok = 1;
      
      // 0x1b is 'ESC'
      while ((sw = uart_getc()) == 0x1b)
         ;
      
      //busy becaus of some action
      ledg_off();
      
      //commands taken from Serasidis' AVR910
      //commands from other sources are marked separately
      switch (sw)
      {
         case 'T': //device type
            device = uart_getc();
            
            //dev_ok = 0;
            
            //look up device in list
            //i do this as the device is selected
            //so i do not have to prove everytime
            //a command after 'P' is executed
            //as in Serasidis' original AVR910 v3.3
            for (i = 0; device_codes[i][0] != 0xFF ; i++)
            {
               if(device == device_codes[i][0]) 
               {
                  pgm_mode = device_codes[i][1]; 
                  dev_ok = 1;
                  break;
               }
               else 
               {
                  dev_ok = 0;
               }

            }
            if (dev_ok)
            {
               // found selected dev in list
               uart_put_ret();
            }
            else
            {
               // didnt find device in list
               uart_put_err();
            }
            break;
            
         case 'S': //Return software id
            show_id();
            break;
            
         case 'V': //Return SW version
            uart_putc((uint8_t)SW_MAJOR);
            uart_putc((uint8_t)SW_MINOR);
            break;
            
         case 'v': //Return HW version
            uart_putc((uint8_t)HW_MAJOR);
            uart_putc((uint8_t)HW_MINOR);
            break;
            
         case 't': //show supported devices
            for (i = 0; device_codes[i][0] != 0xFF ; i++)
            {
               uart_putc(device_codes[i][0]);
            }
            uart_putc(0);
            break;
         
         case 'p': //Return programmer type
            uart_putc((uint8_t)'S');
            break;
         
         case 'a': //Return address auto-increment
            uart_putc((uint8_t)'Y');
            break;
         
         case 'b':
            //Return Blockmode capability(from Leidingers AVR910 v3.8)
            //this device is not capable of block mode(no buffer avail)
            //avrdude needs this info, otherwise the dude
            //crashes with report "programmer not answering"
            uart_putc((uint8_t)'N');
            break;
            
         case 'x': //set LED ignored
            uart_getc();
            uart_put_ret();
            break;
         
         case 'y': //clear LED ignored
            uart_getc();
            uart_put_ret();
            break;

         default:
            break;
      }
      

      //without a valid device
      //commands below this won't work
      if (!dev_ok) 
      {
         // no or no valid device was selected
         // or errorneous opcode was given
         uart_put_err();
         // continues the outer while loop
         continue;
      }
      else
      {
         // valid device was selected
         switch (sw)
         {
            case 'P': //enter programming mode
               
               spiinit(device);
               uart_put_ret();
               ledr_on();
               break;
         
            case 'C': //write program memory hi-Byte
               i = uart_getc();
               wrser(0x48);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               wrser(i);
               
               addr++;
               
               if (!pgm_mode)
                  delay910(0x20);
               
               uart_put_ret();
               break;
         
            case 'c': //write program memory lo-Byte
               i = uart_getc();
               wrser(0x40);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               wrser(i);
               
               if (!pgm_mode)
                  delay910(0x20);
               
               uart_put_ret();
               break;
         
            case 'R': //read program memory
               wrser(0x28);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               uart_putc(rdser());
               wrser(0x20);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               uart_putc(rdser());
               addr++;
               break;
            
            case 'A': //load address
               addr  = 256 * uart_getc(); //read addr.hi
               addr += uart_getc();       //read addr.lo
               uart_put_ret();
               break;
         
            case 'D': //write data memory
               i = uart_getc();
               wrser(0xc0);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               wrser(i);
               delay910(0x20);
               addr++;
               uart_put_ret();
               break;
         
            case 'd': //read data memory
               wrser(0xa0);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               uart_putc(rdser());
               addr++;
               break;
         
            case 'L': //leave programming mode
               port_release();
               set_rst();
               ledr_off();
               uart_put_ret();
               break;
         
            case 'e': //chip erase
               wrser(0xac);
               wrser(0x80);
               wrser(0x04);
               wrser(0x00);
               delay910(0x30);
               uart_put_ret();
               break;
         
            case 'l' : //write lock bits
               i = uart_getc();
               wrser(0xac);
               wrser((i & 0x06) | 0xe0);
               wrser(0x00);
               wrser(0x00);
               delay910(0x30);
               uart_put_ret();
               break;
            
            case 's': //read signature bytes
               for(i = 0x02; i != 0xFF; i--)
               {
                  wrser(0x30);
                  wrser(0x00);
                  wrser(i);
                  uart_putc(rdser());
               }
               break;
         
            case 'm': //write program memory page
               wrser(0x4c);
               wrser((uint8_t) (addr >>   8)); //addr.hi
               wrser((uint8_t) (addr & 0xFF)); //addr.lo
               wrser(0x00);
               delay910(0xFF);
               uart_put_ret();
               break;
               
            case ':': //universal command ":"
            case '.': //universal command "."
               i = uart_getc();
               j = uart_getc();
               k = uart_getc();
               l = (sw == ((uint8_t)':'))?0:uart_getc();
               wrser(i);
               wrser(j);
               wrser(k);
               uart_putc(wrser(l));
               delay910(0xFF);
               uart_put_ret();
               break;
         
            default:
               //or errorneous opcode was given
               //do we need this?
               //uart_putc('?');
               break;
         }
      }
   }
}
