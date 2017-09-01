/*
 * 
 * File   : 910.h
 * Author : Minifloat 
 * Project: Launchprog
 * Version: 1.2 on 10 May 2012
 *
 */

#ifndef HEADER910
#define HEADER910

//version info
#define SW_MAJOR '3'
#define SW_MINOR '3'
#define HW_MAJOR '1'
#define HW_MINOR '0'

//delay val
#define DELAYTIME 16

//portpin access macros
//  LEDR  = P1.0 the red led
// ~RESET = P1.3 there's also a button S2 to GND.
//  MOSI  = P1.4
//  MISO  = P1.5
//  LEDG  = P1.6 the green led
//  SCK   = P1.7

#define LEDRPIN  BIT0
#define RESETPIN BIT3
#define MOSIPIN  BIT4
#define MISOPIN  BIT5
#define LEDGPIN  BIT6
#define SCKPIN   BIT7

#define set_rst() P1OUT|=RESETPIN
#define clr_rst() P1OUT&=~RESETPIN

#define ledr_on()  P1OUT|=LEDRPIN
#define ledr_off() P1OUT&=~LEDRPIN

#define ledg_on()  P1OUT|=LEDGPIN
#define ledg_off() P1OUT&=~LEDGPIN

#define led_init() P1DIR|=LEDGPIN+LEDRPIN

#define port_get()     P1DIR|=MOSIPIN+RESETPIN+SCKPIN
#define port_release() P1DIR&=~(MOSIPIN+RESETPIN+SCKPIN+MISOPIN)

#define set_sck()  P1OUT|=SCKPIN
#define clr_sck()  P1OUT&=~SCKPIN
#define set_mosi() P1OUT|=MOSIPIN
#define clr_mosi() P1OUT&=~MOSIPIN
#define rd_miso()  (P1IN&MISOPIN)

//global variables


//delay func
inline void usdelay(void);
#if 1
inline void delay910(uint8_t dly);
#endif

//pulse sck one time
inline void pulse_sck(void);

//uart functions compatibility
extern uint8_t uart_getc(void);
extern void uart_putc(uint8_t data);
extern void uart_puts(const uint8_t *data);
//send CR
#define uart_put_ret() uart_putc(0x0d)
//send '?'
#define uart_put_err() uart_putc(0x3f)

//write read soft spi
uint8_t wrser(uint8_t send);
//read soft spi
#define rdser() wrser(0)

//enter programming mode
inline void spiinit(uint8_t device);

//show "AVR ISP" on serial line
inline void show_id(void);

//RESET marker of 910
void init910(void);

//wait for a command
void waitcmd(void);

#endif //HEADER910
