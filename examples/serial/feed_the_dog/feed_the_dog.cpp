/**
 * feed_the_dog.cpp - an msp430g2xxx specific watchdog timer example
 *
 * msp430-size msp430g2553in20_release/feed_the_dog.elf
 *  text     data      bss      dec      hex  filename
 *   406        0        6      412      19c  msp430g2553in20_release/feed_the_dog.elf
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

volatile unsigned int count = 0;
serial_default_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;

inline void setup(void)
{
  Serial.begin();

  /* ADC */
  P1_5::PSEL();
  ADC10CTL0 &= ~ENC;
  ADC10CTL1 = INCH_5 | ADC10DIV_3 ;
  ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;
  ADC10AE0 |= BIT5;

  /* Timer1 A0*/
  TA1CCR0 = (CPU::frequency*.010 )/8;     // Count overflow frequency ~10ms
  TA1CTL = TASSEL_2 | ID_3 | MC_1;        // SMCLK / 8  upmode
  TA1CCTL0 = CCIE;

  __enable_interrupt();
  WDTCTL = WDTPW | WDTCNTCL;              // Enable WDT
  Serial << "\r\nreset...\n";
}

void loop()
{
  unsigned sample;

  LPM0;
  WDTCTL = WDTPW | WDTCNTCL;
  
  if (count > 100) {                      // ~1 Second
    count = 1;
    __delay_cycles(128);                  // let the pins settle
    ADC10CTL0 |= ENC | ADC10SC;           // Sampling and conversion start
    while (ADC10CTL1 & ADC10BUSY) {       // sleep and wait for completion
      asm("nop");
    }
    sample = ADC10MEM;
    
    Serial << sample << "    \r";
  }
}

__attribute__( (interrupt(TIMER1_A0_VECTOR)) )
void TIMER1_A0_ISR(void)
{
  LPM0_EXIT;
  count++;
}

