/*
 * gpio.h - C++ template based GPIO port and pin methods
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-27-2013
 * Version: 1.0.4
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
 *
 */

#ifndef GPIO_H_
#define GPIO_H_

#ifdef __GNUC__
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE __attribute__((noinline))
#ifndef __always_inline
#define __always_inline __attribute__((always_inline))
#endif
#ifndef __noinline
#define __noinline __attribute__((noinline))
#endif
#else
#define ALWAYS_INLINE inline
#define NEVER_INLINE
#warning This code takes advantage of features only found in msp430-gcc!!!
#endif

#include <logic.h>

#define GPIO_VERSION 0x0100 /* 1.0 */
#define GPIO_BUILD   0x0004 /* build #*/

typedef volatile uint8_t & u8_SFR;        /* 8 bit unsigned Special Function Register reference */
typedef const volatile uint8_t & u8_CSFR; /* 8 bit unsigned Constant Special Function Register reference */

/*
 * pin_mode - settings for port/pin direction
 * Note: values used coincide with the optimized constant
 *       generator values that result in faster msp430 instructions
 */
enum pin_mode {
  INPUT=0x00
  ,OUTPUT=0x01
  ,INPUT_PULLUP=0x02
  ,INPUT_PULLDOWN=0x04

  ,TIMER_INPUT=0x1000
  ,TIMER_OUTPUT=0x1100
  ,UART_TX=0x1200
  ,UART_RX=0x1400
};

enum pin_value {
  LOW=0x00
  ,HIGH=0x01
};

/*
 * GPIO_PORT_BASE0<> - port template for basic ports
 *
 */
template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR psel
  ,u8_SFR pren
>
struct GPIO_PORT_BASE0 {
    static const int _portno = portno;

    static ALWAYS_INLINE u8_SFR POUT() { return pout; }
    static ALWAYS_INLINE u8_CSFR PIN() { return pin; }
    static ALWAYS_INLINE u8_SFR PSEL() { return psel; }

    /**
     * pin direction configuration methods
     */
    static ALWAYS_INLINE void setmode_input(const uint8_t pins_mask) {
        pdir &= ~pins_mask;
    }

    static ALWAYS_INLINE void setmode_inputpullup(const uint8_t pins_mask) {
        pdir &= ~pins_mask;
        set(pins_mask);
        pren |= pins_mask;
    }

    static ALWAYS_INLINE void setmode_inputpulldown(const uint8_t pins_mask) {
        pdir &= ~pins_mask;
        clear(pins_mask);
        pren |= pins_mask;
    }

    static ALWAYS_INLINE void setmode_output(const uint8_t pins_mask) {
        pdir |= pins_mask;
    }

    static ALWAYS_INLINE void set_mode(const uint8_t pins_mask, pin_mode mode) {
        if (0) {
        }
        else if (mode == OUTPUT) {
            setmode_output(pins_mask);
        }
        else if (mode == INPUT) {
            setmode_input(pins_mask);
        }
        else if (mode == INPUT_PULLUP) {
            setmode_inputpullup(pins_mask);
        }
        else if (mode == INPUT_PULLDOWN) {
            setmode_inputpulldown(pins_mask);
        }
    }

    /*
     * port wide (8bits) get/set methods
     */
    static ALWAYS_INLINE void set_value(const uint8_t value) {
        pout = value;
    }

    static ALWAYS_INLINE uint8_t get_value() {
        return pin;
    }

    static ALWAYS_INLINE void set(const uint8_t pins_mask) {
        pout |= pins_mask;
    }

    static ALWAYS_INLINE void clear(const uint8_t pins_mask) {
        pout &= ~pins_mask;
    }

    static ALWAYS_INLINE void toggle(const uint8_t pins_mask) {
        pout ^= pins_mask;
    }

    /*
     * pin query functions
     */
    static ALWAYS_INLINE unsigned read(const uint8_t pin_mask) {
        return (pin & pin_mask);
    }

    /*
     *
     */
    static ALWAYS_INLINE bool test(const uint8_t pin_mask) {
        return (pin & pin_mask);
    }

    /*
     *
     */
    static ALWAYS_INLINE unsigned is_low(const uint8_t pins_mask) {
        return (pin & pins_mask) == 0 ? 1 : 0;
    }

    /*
     *
     */
    static ALWAYS_INLINE unsigned is_high(const uint8_t pin_mask) {
        return (pin & pin_mask) == pin_mask ? 1 : 0;
    }
    /* TODO: devise generic scheme for setting alternate pin functions */
};

/*
 * GPIO_PORT_BASE2<> - port base template with psel2
 *
 */

template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR psel
  ,u8_SFR psel2
  ,u8_SFR pren
>
struct GPIO_PORT_BASE2 :
  GPIO_PORT_BASE0<portno,pin,pout,pdir,psel,pren>
{
  static u8_SFR PSEL2() { return psel2; }
};

/*
* GPIO_PORT0<> - port template for interrupt capable ports
*
*/

template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR pifg
  ,u8_SFR pies
  ,u8_SFR pie
  ,u8_SFR psel
  ,u8_SFR pren
>
struct GPIO_PORT0 :
GPIO_PORT_BASE0<portno,pin,pout,pdir,psel,pren>
{
  static volatile uint8_t _fakepsel;

  static u8_SFR PIFG()  { return pifg; }
  static u8_SFR PIES()  { return pies; }
  static u8_SFR PIE()   { return pie; }

  /* TODO: implement decent pin interrupt ISR handlers () */
};

/*
 * GPIO_PORT<> - port template for interrupt capable ports
 *
 */

template <
  const int portno
  ,u8_CSFR pin
  ,u8_SFR pout
  ,u8_SFR pdir
  ,u8_SFR pifg
  ,u8_SFR pies
  ,u8_SFR pie
  ,u8_SFR psel
  ,u8_SFR psel2
  ,u8_SFR pren
>
struct GPIO_PORT :
  GPIO_PORT_BASE2<portno, pin,pout,pdir,psel,psel2,pren>
{
  static u8_SFR PIFG()  { return pifg; }
  static u8_SFR PIES()  { return pies; }
  static u8_SFR PIE()   { return pie; }

  /* TODO: implement decent pin interrupt ISR handlers () */
};

/*
 * gpio_pincaps_t - gpio_pin capabilities
 *
 * @desc: provide boolean tests that describe the capabilities
 *       of a GPIO_PIN
 */
template<
   bool active_low   = false
  ,bool uart_tx_pin  = false
  ,bool uart_rx_pin  = false
  ,bool spi_sclk_pin = false
  ,bool spi_mosi_pin = false
  ,bool spi_mis0_pin = false
  ,bool timer_out_pin = false
  ,bool timer_in_pin = false
>
struct gpio_pincaps_t {
  static const bool is_active_low = active_low;
  static const bool canbe_uart_tx = uart_tx_pin;
  static const bool canbe_uart_rx = uart_rx_pin;
  static const bool canbe_timer_out = timer_out_pin;
  static const bool can_be_timer_in = timer_in_pin;
};

typedef gpio_pincaps_t<true> gpio_pin_button;
typedef gpio_pincaps_t<true,true> gpio_pin_uart_tx;
typedef gpio_pincaps_t<true,false,true> gpio_pin_uart_rx;
typedef gpio_pincaps_t<false,false,false,true> gpio_pin_spi_sclk;
typedef gpio_pincaps_t<false,false,false,false,false,false,true> gpio_pin_timer_tx;
typedef gpio_pincaps_t<false,false,false,false,false,false,false,true> gpio_pin_timer_rx;

/*
 * GPIO_PIN<> - pin template
 */

template <const uint8_t MASK, typename PORT, typename CAPS_T = gpio_pincaps_t<> >
struct GPIO_PIN {
  typedef GPIO_PIN<MASK,PORT> T;
  static const uint8_t pin_mask=MASK;
  static const int _portno = PORT::_portno;
  typedef CAPS_T pin_cap;

  static ALWAYS_INLINE u8_SFR POUT() { return PORT::POUT(); }
  static ALWAYS_INLINE u8_CSFR PIN()  { return PORT::PIN();  }
  static ALWAYS_INLINE u8_SFR PSEL() { return PORT::PSEL(); }

  /*
   * pin direction functions
   */
  static ALWAYS_INLINE void setmode_input() {
      mode_gpio();
      PORT::setmode_input(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void setmode_inputpullup() {
      mode_gpio();
      PORT::setmode_inputpullup(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void setmode_inputpulldown() {
      mode_gpio();
      PORT::setmode_inputpulldown(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void setmode_output() {
      mode_gpio();
      PORT::setmode_output(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void set_mode(const pin_mode mode) {
      // TBD provide core alt switch
      PORT::set_mode(MASK,mode);
  }

  /*
   * alt_functions - overrideable
   */
  static ALWAYS_INLINE void mode_gpio() {}

  static ALWAYS_INLINE void mode_timer_out() {}
  static ALWAYS_INLINE void mode_timer_in() {}

  static ALWAYS_INLINE void mode_uart() {}
  static ALWAYS_INLINE void mode_uart_rx() {}
  static ALWAYS_INLINE void mode_uart_tx() {}

  static ALWAYS_INLINE void mode_spi() {}
  static ALWAYS_INLINE void mode_spi_clk() {}
  static ALWAYS_INLINE void mode_spi_mosi() {}
  static ALWAYS_INLINE void mode_spi_miso() {}

  /*
   * pin query functions
   */

  static ALWAYS_INLINE bool is_low() {
      return !(PORT::test(MASK));
  }

  static ALWAYS_INLINE bool is_high() {
      return (PORT::test(MASK));
  }

  static ALWAYS_INLINE bool read() {
      return PORT::read(MASK) ? 1 : 0;
  }

  /*
   *
   */
  static ALWAYS_INLINE unsigned is_pushed() {
    if ( pin_cap::is_active_low ) {
      return is_low();
    }
    else {
      return is_high();
    }
  }

  /*
   * pin modification functions
   */
  static ALWAYS_INLINE void write(pin_value value) {
    if (value == HIGH) {
      high();
    }
    else if (value == LOW) {
      low();
    }
  }

  /*
   *
   */
  static ALWAYS_INLINE void high() {
      PORT::set(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void low() {
      PORT::clear(MASK);
  }

  /*
   *
   */
  static ALWAYS_INLINE void toggle() {
      PORT::toggle(MASK);
  }

  /*
   * port wide (8bits) set direction using pin as port
   */
  static ALWAYS_INLINE void set_modes(const uint8_t pins_mask, pin_mode mode) {
    PORT::set_mode(pins_mask, mode);
  }

  /*
   * port wide (8bits) modifications using pin as port
   */
  static ALWAYS_INLINE void set_pins(const uint8_t pins_mask) {
      PORT::set(pins_mask);
  }

  /*
   *
   */
  static ALWAYS_INLINE void clear_pins(const uint8_t pins_mask) {
      PORT::clear(pins_mask);
  }

  /*
   *
   */
  static ALWAYS_INLINE void toggle_pins(const uint8_t pins_mask) {
      PORT::toggle(pins_mask);
  }

  /* TODO: fix this, not all pins have interrupt capability */

  #if 0
  /* neat idea, however, this requires an instance which means we use data when we don't have to */
  ALWAYS_INLINE T operator =(int value) {
      if ( value ) {
          high();
      }
      else {
          low();
      }
      return *this;
  }

  ALWAYS_INLINE T operator =(pin_value value) {
      write(value);
      return *this;
  }
  #endif
};

/*
 * DummyGPIO - minimal template that looks like a GPIO but does nothing
 *
 * Useful to provide NOP pin as in the case of an output only serial port.
 * This will satisfy the compiler but not add to the code size.
 *
 */
template<uint8_t PORTPIN>
struct DummyGPIO {
  struct {
    static uint16_t PIN() { return 0; }
    static uint16_t POUT() { return 0; }
    static uint16_t PSEL() { return 0; }
    static uint16_t PSEL2() { return 0; }
    static uint16_t PREN() { return 0; }
    static uint8_t PINMASK() { return 0; }

    static void set_mode(const uint8_t, pin_mode) {}
    //static const int _portno = 0;
  } port;
  static volatile uint8_t _psel;
  static volatile uint8_t _fake_pin;

  static u8_SFR PSEL() { return (u8_SFR)_psel; }
  static u8_SFR PSEL2() { return (u8_SFR)_psel; }
  static u8_SFR PIN() { return (u8_SFR)_fake_pin; }

  static const uint8_t pin_mask=0;
  static const int _portno = 0;

  static void setmode_input() {}
  static void setmode_inputpullup() {}
  static void setmode_inputpulldown() {}
  static void setmode_output() {}
  static void set_mode(pin_mode) {}

  static void high() { }
  static void low() { }
  static void toggle() { }

  static int read() { return 0; }
  static void write(pin_value) {}
};

typedef DummyGPIO<0> NO_PIN;

template<typename P1, typename P2>
struct is_same_port {
  enum  { yes_no = (P1::_portno == P2::_portno) ? 1 : 0 };
};

/*
 * port helper macros
 * assumes all pins are from the same port, no check performed , user tasked with being smart
 */
#define port_mode(B0,B1, _mode) B0::set_modes((const uint8_t)(B0::pin_mask|B1::pin_mask),_mode);
#define port_toggle(B0,B1) B0::toggle_pins(B0::pin_mask|B1::pin_mask);

/*
 * nod to Arduino API
 */
#define digitalRead(PIN_T) PIN_T::read()
#define digitalWrite(PIN_T,_value) PIN_T::write(_value)
#define pinMode(PIN_T,_mode) PIN_T::set_mode(_mode)

#endif /* GPIO_H_ */
