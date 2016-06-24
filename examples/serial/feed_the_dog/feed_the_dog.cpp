/**
 * feed_the_dog.cpp - an msp430g2xxx specific watchdog timer example
 *
 * msp430-size msp430g2553in20_release/feed_the_dog.elf
 *   text     data      bss      dec      hex  filename
 *    428        0        6      434      1b2  msp430g2553in20_release/feed_the_dog.elf
 *
 * msp430-size msp430g2231in14_release/feed_the_dog.elf
 *   text     data      bss      dec      hex  filename
 *    428        0        6      434      1b2  msp430g2231in14_release/feed_the_dog.elf
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

#define SYSTICK (1000/10)  /* ~10ms */

volatile unsigned count;

sw_serial_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial;

inline void setup(void)
{
  Serial.begin();
  Serial << "\r\nRESET!\n";

#ifdef __MSP430_HAS_AD10__
  /* ADC on P1.5 */
  P1_5::PSEL();
  ADC10CTL0 &= ~ENC;
  ADC10CTL1 = INCH_5 | ADC10DIV_3 ;          // Select A5, ADC10CLK/4
  ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON; // Vr+ = Vcc / Vr- = Vss, 64 x ADC10CLKs, ADC10 On
  ADC10AE0 |= BIT5;                          // Enable ADC input P1.5
#endif

  /* Timer0 A0*/
  TA0CCR0 = (CPU::frequency/SYSTICK)/8;  // Count overflow frequency ~10ms
  TA0CTL = TASSEL_2 | ID_3 | MC_1;       // SMCLK/8 in upmode
  TA0CCTL0 = CCIE;

  __bis_SR_register(LPM0_bits|GIE);      // Sleep with interrupts enabled
}

void loop()
{
  WDTCTL = WDTPW|WDTCNTCL;               // feed the wdt, ~32ms timeout from SMCLK

#ifdef __MSP430_HAS_ADC10__
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
#else
  Serial << "ADC10 not supported on this chip\r\n";
#endif
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
