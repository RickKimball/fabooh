/*
 * timerserial430.h - TimerA based Serial UART template for msp430
 *
 * Created: Apr-24-2013
 *  Author: rick@kimballsoftware.com
 *    Date: 04-24-2013
 * Version: 1.0.0
 *
 * Sometimes you just want to work with interrupts enabled. This UART code
 * give you a better chance of achieving that.
 *
 * Acknowledgments:
 *  Serial API inspired by Arduino.
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIMERSERIAL_430_H_
#define TIMERSERIAL_430_H_
#include <msp430.h>
#include <stdint.h>
#include <ringbuffer.h>

/*
 * tiny_rbuffer_t - really small ring buffer
 */
typedef ringbuffer_t<uint8_t, 16> tiny_rbuffer_t;

/*
 * localize these variables to the compilation unit that includes it
 */
namespace {
  volatile unsigned int USARTTXBUF; // 1 character tx buffer, used by class and ISR
  static uint8x2_t rx_bits;         // persistent storage for data and mask. fits in one 16 bit register
  enum { RX_MASK=0, RX_DATA=1 };    // index into rx_bits.data[]

  tiny_rbuffer_t rx_buffer;         // instance of ring buffer can be overridden by user code and ignored
}

/*
 * timer_base_timer_t - TimerA based UART
 */
template <uint32_t BAUD, uint32_t MCLK_HZ, typename TXPIN, typename RXPIN, typename RB_T, RB_T& RB>
struct timer_base_timer_t {
  static const uint16_t TICKS_PER_BIT = (MCLK_HZ + (BAUD >> 1))/BAUD;
  static const uint16_t TICKS_PER_BIT_DIV2 = TICKS_PER_BIT >> 1;

  // compact type for volatile SFR that are 16 bit
  typedef volatile uint16_t & u16_SFR;

  template <const uint16_t ccindx, u16_SFR _CCTL, u16_SFR _CCR, u16_SFR _CTL, u16_SFR _TAR>
  struct timer_t {
    static u16_SFR CCTL() { return _CCTL; }
    static u16_SFR CCR()  { return _CCR; }
    static u16_SFR CTL()  { return _CTL; }
    static u16_SFR TAR()  { return _TAR; }
  };

  // TODO: this is more flexible but still needs work to allow switching the ISR
  static const timer_t<0, TA0CCTL0, TA0CCR0, TA0CTL, TA0R> tx_timer; // lowest ISR overhead P1.2
  static const timer_t<1, TA0CCTL1, TA0CCR1, TA0CTL, TA0R> rx_timer; // higher ISR overhead P1.1

  /*
   * ringbuffer - access routines
   */
  static void rb_push(uint8_t c) { RB.push_back(c); }
  static size_t rb_available() { return RB.available(); }
  static int rb_pop_front() { return RB.pop_front_nc(); }

  /**
   * begin( baud rate ) - initialize TX/RX pins
   * Note: we assume a software pin setup like the original examples provided
   *       from TI. P1.1 is TX and P1.2 is RX.

   *       On an msp430 launchpad v1.5 you will have to rotate the J3
   *       RX/TX jumpers 90 degrees.
   */
  void begin(const uint32_t baud_ignored=BAUD /* not used */) {
    (void)baud_ignored; // warning avoidance

    if ( TXPIN::pin_mask ) {
      TXPIN::setmode_output();
      // User controlled timer pin output
      tx_timer.CCTL() = OUT;
      (void)timera_tx_isr;           // reference ISR so it isn't garbage collected
    }

    if ( RXPIN::pin_mask ) {
      RXPIN::setmode_input();
      // Sync Latch TA0.0, Synchronize TACLK and MCLK, Detect Negative Edge, Enable Capture mode and RX Interrupt
      rx_timer.CCTL() = CCIS_0 | SCS | CM_2 | CAP | CCIE;
      (void)timera_rx_isr;           // reference ISR so it isn't garbage collected
    }

    // allow for output only and input only UART configs
    if ( RXPIN::pin_mask && TXPIN::pin_mask ) {
      TXPIN::PSEL() |= (TXPIN::pin_mask | RXPIN::pin_mask);
    }
    else if ( TXPIN::pin_mask ) {
      TXPIN::PSEL() |= TXPIN::pin_mask;
    }
    else if ( RXPIN::pin_mask ) {
      RXPIN::PSEL() |= RXPIN::pin_mask;
    }

    // Clock TIMERA from SMCLK, run in continuous mode counting from to 0-0xFFFF, set TAR to 0
    tx_timer.CTL() = TASSEL_2 | MC_2 | TACLR;

    __enable_interrupt();

  }

  ALWAYS_INLINE void flush() {
    while (tx_timer.CCTL() & CCIE) { ; } // wait for previous transmission to finish
  }

  /*
   * end() - set TX/RX pin back to default
   */
  void end() {
    // wait for TX/RX to finish
    flush();
    tx_timer.CTL() = 0; // stop timer

    if ( RXPIN::pin_mask && TXPIN::pin_mask ) {
      P1SEL &= ~(BIT1|BIT2);
      P1DIR &= ~(BIT1|BIT2);
    }
    else if ( TXPIN::pin_mask ) {
      P1SEL &= ~BIT2;
      P1DIR &= ~BIT2;
    }
    else if ( RXPIN::pin_mask ) {
      P1SEL &= ~BIT1;
      P1DIR &= ~BIT1;
    }
  }

  /*
   * read() - blocking read
   */
  int read(void) {
    if ( RXPIN::pin_mask ) {
      do {
        if ( rb_available() ) {
          return rb_pop_front();
        }
      } while (1);

      return -1;
    }
  }

  /*
   * write_impl() - blocking write CRTP function used by print<> template
   *
   * this template implements enough code to be considered a Writer pattern
   * for use with the print_t<Writer>
   *
   */
  int write_impl(uint8_t c) {
    if ( TXPIN::pin_mask ) {
      while (tx_timer.CCTL() & CCIE) {;}  // wait for previous transmission to finish
      tx_timer.CCR() = tx_timer.TAR();    // re-sync with current TimerA clock
      tx_timer.CCR() += TICKS_PER_BIT;    // setup the next timer tick
      tx_timer.CCTL() = OUTMOD0 + CCIE;   // set TX_PIN HIGH and re-enable interrupts

      register unsigned value = c | 0x300;  // add stop bit '11'
      value <<= 1;                          // add the start bit '0'
      USARTTXBUF=value;                     // queue up the byte for send
    }

    return 1;
  }

  /*
   * timera_rx_isr - P1.2 TA0.1 handler
   */

  __attribute__((interrupt(TIMER0_A1_VECTOR)))
  static void timera_rx_isr(void)
  {
    volatile uint16_t resetTAIVIFG;   // just reading TAIV will reset the interrupt flag
    resetTAIVIFG = TA0IV;
    (void)resetTAIVIFG;

    if ( RXPIN::pin_mask ) {
      register uint16_t regCCTL = rx_timer.CCTL();  // using a register variable provides a slight performance improvement

      rx_timer.CCR() += TICKS_PER_BIT;              // Setup next time to sample
      if (regCCTL & CAP) {                          // start bit?
        rx_timer.CCR() += TICKS_PER_BIT_DIV2;       // adjust sample time, so next sample is in the middle of the bit width
        rx_bits.as_u16 = 0x0001;                    // initialize both values, set data to 0x00 and mask to 0x01
        rx_timer.CCTL() = regCCTL & ~(CAP|COV);     // Switch from capture mode to compare mode
      }
      else {
        if (regCCTL & SCCI) {                       // check latched bit
          rx_bits.as_u8[RX_DATA] |= rx_bits.as_u8[RX_MASK];
        }

        if (!(rx_bits.as_u8[RX_MASK] <<= 1)) {
          rb_push(rx_bits.as_u8[RX_DATA]);
          rx_timer.CCTL() = regCCTL | CAP;
        }
      }
    }
  }

  /*
   * timera_tx_isr - P1.1 TA0.0 handler
   */
  __attribute__((interrupt(TIMER0_A0_VECTOR)))
  static void timera_tx_isr(void)
  {
    if ( TXPIN::pin_mask ) {
      tx_timer.CCR() += TICKS_PER_BIT;  // setup next time to send a bit, OUT will be set then

      tx_timer.CCTL() |= OUTMOD2;       // reset OUT (set to 0) OUTMOD2|OUTMOD0 (0b101)
      if ( USARTTXBUF & 0x01 ) {        // look at LSB if 1 then set OUT high
        tx_timer.CCTL() &= ~OUTMOD2;    // set OUT (set to 1) OUTMOD0 (0b001)
      }

      if (!(USARTTXBUF >>= 1)) {        // All bits transmitted ?
        tx_timer.CCTL() &= ~CCIE;       // disable interrupt, indicates we are done
      }
    }
  }

};

template <uint32_t BAUD, uint32_t MCLK_HZ,
          typename TXPIN, typename RXPIN,
          typename RB_T=tiny_rbuffer_t, RB_T& RB=rx_buffer>
struct timer_serial_t:
  timer_base_timer_t<BAUD, MCLK_HZ, TXPIN, RXPIN, RB_T, RB>,
  print_t<timer_serial_t<BAUD, MCLK_HZ, TXPIN, RXPIN, RB_T, RB> >
{
    // make it easier to instance in user code
};

#endif /* TIMERSERIAL_430_H_ */
