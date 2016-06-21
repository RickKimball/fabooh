/**
 * feed_the_dog.cpp
 */

#include <fabooh.h>
#include <main.h>
#include <serial.h>

//------- file space globals ------
namespace {
  const uint32_t BAUD_RATE=9600;

  // pick only 1 serial implementation
#if 0
  usci_serial_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#elif 1
  sw_serial_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#elif 0
  // you need to swap the jumpers to use timer serial on a 1.5 launchpad
  // P1_1 becomes TX_PIN and  P1_2 becomes RX_PIN
  timer_serial_t<BAUD_RATE, CPU::frequency, P1_1, NO_PIN> Serial; // xmit only serial
#else
  serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial
#endif

  volatile unsigned int count = 0;
}

inline void setup(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  // Initialize serial port pins. Note: speed is ignored here just for documentation
  // change the baud rate in the template definition
  Serial.begin(BAUD_RATE);

  /* ADC */
  P1SEL |= BIT5;
  
  ADC10CTL0 &= ~ENC;
  ADC10CTL1 = INCH_5 | ADC10DIV_3 ;
  ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;
  ADC10AE0 |= BIT5;

  /* Timer1 A0*/
  TA1CCR0 = (CPU::frequency*.01)/8;       // Count overflow frequency ~10ms
                                          // NOTE: this is a compile time calc
                                          // it won't use floating point
  TA1CTL = TASSEL_2 | ID_3 | MC_1;        // SMCLK / 8  upmode
  TA1CCTL0 = CCIE;

  P1_6::setmode_output();
  P1_6::high();
  
  __enable_interrupt();
  WDTCTL = WDTPW | WDTCNTCL;              // Enable WDT
  Serial.print("\r\nreset...\n");

}

void loop()
{
  unsigned sample = 0;

  while(1){
    LPM0;
    WDTCTL = WDTPW | WDTCNTCL;

    if (count > 100) {                      // ~1 Second
      P1_6::toggle();
      count = 1;
      __delay_cycles(128);
      ADC10CTL0 |= ENC + ADC10SC;           // Sampling and conversion start
      while (ADC10CTL1 & ADC10BUSY) {       // sleep and wait for completion
        __bis_SR_register(CPUOFF + GIE);    // LPM0 with interrupts enabled
	WDTCTL = WDTPW | WDTCNTCL;
      }

      sample = ADC10MEM;

      Serial.print(sample,DEC);
      Serial.print("     \r");    }
  }
  
}

__attribute__( (interrupt (TIMER1_A0_VECTOR)) )
void TIMER1_A0_ISR(void)
{
  LPM0_EXIT;
  count++;
}

