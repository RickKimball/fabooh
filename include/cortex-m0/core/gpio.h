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

#ifdef __cplusplus
#include <stdint.h>
#include <LPC11xx.h>

#include <logic.h>

#define GPIO_VERSION 0x0100 /* 1.0 */
#define GPIO_BUILD   0x0005 /* build #*/

enum portnos {
  DUMMY_PORT=-1
  ,P0_PORTNO=0
  ,P1_PORTNO=1
};

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
};

enum pin_value {
  LOW=0x00
  ,HIGH=0x01
};

#if 0
__IO uint32_t DATA;                    /*!< Offset: 0x8000 Data direction Register (R/W) */
__IO uint32_t DIR;                    /*!< Offset: 0x8000 Data direction Register (R/W) */
__IO uint32_t IS;                     /*!< Offset: 0x8004 Interrupt sense Register (R/W) */
__IO uint32_t IBE;                    /*!< Offset: 0x8008 Interrupt both edges Register (R/W) */
__IO uint32_t IEV;                    /*!< Offset: 0x800C Interrupt event Register  (R/W) */
__IO uint32_t IE;                     /*!< Offset: 0x8010 Interrupt mask Register (R/W) */
__IO uint32_t RIS;                    /*!< Offset: 0x8014 Raw interrupt status Register (R/ ) */
__IO uint32_t MIS;                    /*!< Offset: 0x8018 Masked interrupt status Register (R/ ) */
__IO uint32_t IC;                     /*!< Offset: 0x801C Interrupt clear Register (R/W) */
#endif

typedef volatile uint32_t & u32_SFR;
typedef uint32_t pin_width_t;

/*
 * gpio_pincaps_t - gpio_pin capabilities
 *
 * @desc: provide boolean tests that describe the capabilities
 *       of a GPIO_PIN
 */
template<
  bool active_low = false
  ,bool uart_tx_pin = false
  ,bool uart_rx_pin = false
  ,bool spi_sclk_pin = false
  ,bool spi_mosi_pin = false
  ,bool spi_mis0_pin = false
>
struct gpio_pincaps_t {
  static const bool is_active_low = active_low;
  static const bool canbe_uart_tx = uart_tx_pin;
  static const bool canbe_uart_rx = uart_rx_pin;
};

typedef gpio_pincaps_t<true> gpio_pin_button;
typedef gpio_pincaps_t<true,true> gpio_pin_uart_tx;
typedef gpio_pincaps_t<true,false,true> gpio_pin_uart_rx;
typedef gpio_pincaps_t<false,false,false,true> gpio_pin_spi_sclk;

/*
 * GPIO_PORT_BASE<> - port template for basic ports
 *
 */
template <const portnos portno>
struct GPIO_PORT_BASE {
    static const portnos _portno = portno;

    ALWAYS_INLINE static LPC_GPIO_TypeDef  & portdef() {
        if (_portno == P0_PORTNO ) {
            return *LPC_GPIO0;
        }
        else if ( _portno == P1_PORTNO ) {
            return *LPC_GPIO1;
        }
        return *LPC_GPIO1;
    }

    ALWAYS_INLINE static volatile uint32_t & PIN() { return portdef().DATA; }
    ALWAYS_INLINE static volatile uint32_t & POUT() { return portdef().DATA; }
    ALWAYS_INLINE static volatile uint32_t & PDIR() { return portdef().DIR; }

  /**
   * pin direction configuration methods
   */
  ALWAYS_INLINE static void set_mode(const pin_width_t mask, pin_mode mode) {
    if ( 0 ) {
    }
    else if (mode == OUTPUT ) {
      setmode_output(mask);
    }
    else if ( mode == INPUT ) {
      setmode_input(mask);
    }
    else if (mode == INPUT_PULLUP) {
        setmode_inputpullup(mask);
    }
    else if (mode == INPUT_PULLDOWN) {
        setmode_inputpulldown(mask);
    }
  }

  ALWAYS_INLINE static void setmode_input(const pin_width_t mask) {
  }

  ALWAYS_INLINE static void setmode_inputpullup(const pin_width_t mask) {
  }

  ALWAYS_INLINE static void setmode_inputpulldown(const pin_width_t mask) {

  }

  ALWAYS_INLINE static void setmode_output(const pin_width_t mask ) {
      PDIR() |= mask;
  }

  /*
   * port wide (8bits) get/set methods
   */
  ALWAYS_INLINE static void set_value(const pin_width_t value) {
    POUT() = value;
  }

  ALWAYS_INLINE static pin_width_t get_value() {
    return PIN();
  }

  ALWAYS_INLINE static void set_pins(const pin_width_t pin_mask) {
    POUT() |= pin_mask;
  }

  ALWAYS_INLINE static void clear_pins(const pin_width_t pin_mask) {
    POUT() &= ~pin_mask;
  }

  ALWAYS_INLINE static void toggle_pins(const pin_width_t pin_mask) {
    POUT() ^= pin_mask;
  }

};

/*
 * GPIO_PIN<> - pin template
 */

template <const pin_width_t MASK, typename PORT, typename CAPS_T = gpio_pincaps_t<> >
struct GPIO_PIN {
  typedef GPIO_PIN<MASK,PORT> T;
  static const pin_width_t pin_mask=MASK;
  static const int _portno = PORT::_portno;
  typedef CAPS_T pin_cap;

  static u32_SFR PIN()  { return PORT::PIN();  }
  static u32_SFR POUT()  { return PORT::POUT(); }
  static u32_SFR PDIR()  { return PORT::PDIR(); }
#if 0
  static u8_SFR PSEL()  { return PORT::PSEL(); }
  static u8_SFR PSEL2() { return PORT::PSEL2();}
  static u8_SFR PREN()  { return PORT::PREN(); }

  static u8_SFR PIFG()  { return PORT::PIFG(); }
  static u8_SFR PIES()  { return PORT::PIES(); }
  static u8_SFR PIE()   { return PORT::PIE(); }
#endif
  /*
   * pin direction functions
   */
  ALWAYS_INLINE static void setmode_input() {
    PORT::PDIR() &= ~MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_inputpullup() {
    PORT::PDIR() &= ~MASK;
    high();
    PORT::PREN() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_inputpulldown() {
    PORT::PDIR() &= ~MASK;
    low();
    PORT::PREN() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void setmode_output() {
    PORT::PDIR() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void set_mode(const pin_mode mode) {
    if (0) {
    }
    else if (mode == OUTPUT) {
      setmode_output();
    }
    else if (mode == INPUT) {
      setmode_input();
    }
    else if (mode == INPUT_PULLUP) {
      setmode_inputpullup();
    }
    else if (mode == INPUT_PULLDOWN) {
      setmode_inputpulldown();
    }
  }

  /*
   * pin query functions
   */
  ALWAYS_INLINE static pin_width_t read() {
    return (PORT::PIN() & MASK ) != 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static int is_low() {
    return (PORT::PIN() & MASK ) == 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static int is_high() {
    return (PORT::PIN() & MASK ) != 0;
  }

  /*
   *
   */
  ALWAYS_INLINE static int is_pushed() {
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
  ALWAYS_INLINE static void write(pin_value value) {
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
  ALWAYS_INLINE static void high() {
    PORT::POUT() |= MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void low() {
    if ( MASK )
      PORT::POUT() &= ~MASK;
  }

  /*
   *
   */
  ALWAYS_INLINE static void toggle() {
    if ( MASK )
      PORT::POUT() ^= MASK;
  }

  /*
   * port wide (8bits) set direction using pin as port
   */
  ALWAYS_INLINE static void set_modes(const uint8_t pins_mask, pin_mode mode) {
    PORT::set_mode(pins_mask, mode);
  }

  /*
   * port wide (8bits) modifications using pin as port
   */
  ALWAYS_INLINE static void set_pins(const uint8_t pins_mask) {
    PORT::POUT() |= pins_mask;
  }

  /*
   *
   */
  ALWAYS_INLINE static void clear_pins(const uint8_t pins_mask) {
    PORT::POUT() &= ~pins_mask;
  }

  /*
   *
   */
  ALWAYS_INLINE static void toggle_pins(const uint8_t pins_mask) {
    PORT::POUT() ^= pins_mask;
  }

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
    static uint8_t PINMASK() { return 0; }

    static void set_mode(const uint8_t, pin_mode) {}
    static const int _portno = DUMMY_PORT;
  } port;

  static const uint8_t pin_mask=0;
  static const int _portno = DUMMY_PORT;

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

typedef DummyGPIO<DUMMY_PORT> NO_PIN;

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

#endif /* __cplusplus */

#endif /* GPIO_H_ */
