/**
 * feed_the_dog.cpp - an msp430g2xxx specific watchdog timer example
 *
 * msp430-size msp430g2553in20_release/feed_the_dog.elf
 *   text     data      bss      dec      hex  filename
 *    432        0        6      438      1b6  msp430g2553in20_release/feed_the_dog.elf
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

volatile unsigned count;

sw_serial_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;

inline void setup(void)
{
  Serial.begin();
  Serial << "\r\nRESET!\n";

  /* ADC */
  P1_5::PSEL();
  ADC10CTL0 &= ~ENC;
  ADC10CTL1 = INCH_5 | ADC10DIV_3 ;
  ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;
  ADC10AE0 |= BIT5;

  /* Timer0 A0*/
  TA0CCR0 = (CPU::frequency*.010 )/8;     // Count overflow frequency ~10ms
  TA0CTL = TASSEL_2 | ID_3 | MC_1;        // SMCLK / 8  upmode
  TA0CCTL0 = CCIE;

  __bis_SR_register(LPM0_bits|GIE);
}

void loop()
{
  WDTCTL = WDTPW | WDTCNTCL;
  if (count > 99) {                      // ~1 Second
    count = 0;
    __delay_cycles(125);                 // Let the pins settle
    ADC10CTL0 |= (ENC|ADC10SC);          // Sampling and conversion start
    while(ADC10CTL1 & ADC10BUSY) {
      LPM0;                              // Sleep until ADC done
   }
    unsigned sample = ADC10MEM;

    Serial << sample << "    \r";
  }
  LPM0;
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void TIMER0_A0_ISR(void)
{
 ++count;

  if ( !(ADC10CTL1 & ADC10BUSY) ) {
  	LPM0_EXIT;
  }
}

__attribute__((interrupt(ADC10_VECTOR)))
void ADC10_ISR(void)
{
  LPM0_EXIT;
}
