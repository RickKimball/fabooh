/*
 * adc_temp - read temperature from ADC10 and convert to Fahrenheit and Celsius
 *            This code uses libfixmath to convert adc samples from millivolts
 *            to F and C. It also provides some reference code that uses
 *            straight integer math to accomplish the same thing.
 *
 * $ msp430-size adc_temp.elf
 *    text    data     bss     dec     hex filename
 *    1032       0       0    1032     408 adc_temp.elf
 *
 */

#include <fabooh.h>
#define SMALL_INIT4
#include <main.h>
#include <serial.h>

typedef serial_default_t<9600,CPU::frequency,TX_PIN,NO_PIN> serial_t;

// conditional compiles let you compare sizes and code generated
#undef USE_FIX16
#define USE_INTEGER_MATH

#if defined(__MSP430G2553__) || defined(__MSP430G2231__) || defined(__MSP430G2452__)
void setup() {
  serial_t Serial;

  // configure ADC to take temperature samples see [2]
  ADC10CTL0 = 0;
  ADC10CTL1 = INCH_10 | ADC10DIV_3;
  ADC10CTL0 = SREF_1 | ADC10SHT_3 | REFON | ADC10ON | ADC10IE;

  Serial.begin(9600);


  unsigned sample;

  while(1) {
    // enable ADC sample, sleep till complete
    ADC10CTL0 |= (ENC |ADC10SC);
    __bis_SR_register(LPM3_bits | GIE);
    __nop(); // make debugger happy see [1]

    sample = ADC10MEM;

    //Serial << "RAW=" << sample << " ";

    // output F and C temps
  #ifdef USE_FIX16
    // working variables for calculations

    // convert sample to C = sample*0.413 - 277.75
    Fix16 c = (Fix16(int16_t(sample)) * Fix16(0.14663/0.355)) - Fix16(277.75);
    Serial << _FIX16(c + 0.005f, 2) << "C" << " ";

    // convert sample to F = C*9/5 + 32
    Fix16 f = (c * Fix16(9.0/5.0)) + Fix16(32);
    Serial << _FIX16(f + 0.0005f, 3) << "F" << endl;

  #endif

  #ifdef USE_INTEGER_MATH
    int conversion;
    conversion = ((27069L * sample) -  18169625L) >> 16;
    Serial << conversion << "C ";

    conversion = ((48724L * sample ) - 30634388L) >> 16;
    Serial << conversion << "F" << endl;
  #endif

    delay(5000);
  }
}

/*
 * when sample is complete, wake up main thread
 */
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
  LPM3_EXIT;
  __nop(); // make debugger happy see [1]
}

void loop() {}
#else
void setup() { }
void loop() { }

#endif

/*
 * References:
 *
 * [1] http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/182673.aspx
 * [2] http://forum.43oh.com/topic/1954-using-the-internal-temperature-sensor/
 *
 */
