/*
 * gpio.h - C++ template based GPIO port and pin methods
 *
 *  Created on: Apr 16, 2013
 *      Author: kimballr
 */

#ifndef GPIO_H_
#define GPIO_H_
#include <stm32f1xx.h>

/**
 * Specifies a GPIO pin behavior.
 * @see pinMode()
 */
enum pin_mode_e {
    OUTPUT, /**< Basic digital output: when the pin is HIGH, the
               voltage is held at +3.3v (Vcc) and when it is LOW, it
               is pulled down to ground. */

    OUTPUT_OPEN_DRAIN, /**< In open drain mode, the pin indicates
                          "low" by accepting current flow to ground
                          and "high" by providing increased
                          impedance. An example use would be to
                          connect a pin to a bus line (which is pulled
                          up to a positive voltage by a separate
                          supply through a large resistor). When the
                          pin is high, not much current flows through
                          to ground and the line stays at positive
                          voltage; when the pin is low, the bus
                          "drains" to ground with a small amount of
                          current constantly flowing through the large
                          resistor from the external supply. In this
                          mode, no current is ever actually sourced
                          from the pin. */

    INPUT, /**< Basic digital input. The pin voltage is sampled; when
              it is closer to 3.3v (Vcc) the pin status is high, and
              when it is closer to 0v (ground) it is low. If no
              external circuit is pulling the pin voltage to high or
              low, it will tend to randomly oscillate and be very
              sensitive to noise (e.g., a breath of air across the pin
              might cause the state to flip). */

    INPUT_ANALOG, /**< This is a special mode for when the pin will be
                     used for analog (not digital) reads.  Enables ADC
                     conversion to be performed on the voltage at the
                     pin. */

    INPUT_PULLUP, /**< The state of the pin in this mode is reported
                     the same way as with INPUT, but the pin voltage
                     is gently "pulled up" towards +3.3v. This means
                     the state will be high unless an external device
                     is specifically pulling the pin down to ground,
                     in which case the "gentle" pull up will not
                     affect the state of the input. */

    INPUT_PULLDOWN, /**< The state of the pin in this mode is reported
                       the same way as with INPUT, but the pin voltage
                       is gently "pulled down" towards 0v. This means
                       the state will be low unless an external device
                       is specifically pulling the pin up to 3.3v, in
                       which case the "gentle" pull down will not
                       affect the state of the input. */

    INPUT_FLOATING, /**< Synonym for INPUT. */

    PWM, /**< This is a special mode for when the pin will be used for
            PWM output (a special case of digital output). */

    PWM_OPEN_DRAIN, /**< Like PWM, except that instead of alternating
                       cycles of LOW and HIGH, the voltage on the pin
                       consists of alternating cycles of LOW and
                       floating (disconnected). */
};

enum pin_value_e {
  LOW,
  HIGH,
  ON,
  OFF
};

enum pin_crx_speed_e {
  PIN_SPEED_10MHZ=0b0001,
  PIN_SPEED_MED=0b0001,

  PIN_SPEED_2MHZ=0b0010,
  PIN_SPEED_LOW=0b0010,

  PIN_SPEED_50MHZ=0b0011,
  PIN_SPEED_HIGH=0b0011,

  PIN_CRX_CNF_MASK=GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE0_Msk
};

// TODO: figure out why print_t whines when i make this a real enum
#if 0
 enum pin_cnf_mode_in_e {
   PIN_INPUT_ANALOG 0b0000
  ,PIN_INPUT_FLOATING 0b0100
  ,PIN_INPUT_PULLDOWN 0b1000
  ,PIN_INPUT_PULLUP 0b1000
 }
#else
 typedef uint32_t pin_cnf_mode_in_e;
  #define PIN_INPUT_ANALOG 0b0000
  #define PIN_INPUT_FLOATING 0b0100
  #define PIN_INPUT_PULLDOWN 0b1000
  #define PIN_INPUT_PULLUP 0b1000
#endif

enum pin_cnf_mode_out_e {
  PIN_PUSHPULL_OUT=0b0000,
  PIN_OPENDRAIN_OUT=0b0100,
  PIN_ALT_PUSHPULL_OUT=0b1000,
  PIN_ALT_OPENDRAIN_OUT=0b1100,
};

/*
 *  GPIO_PIN -
 */

template<const uint32_t gpio_port_no, const uint32_t pin_no>
struct GPIO_PIN : GPIO_TypeDef {

  static const uint32_t port_pin(void) { return (gpio_port_no << 16) | pin_no; }

  static const uint32_t port(void) { return gpio_port_no; }

  static const uint32_t pin(void) { return pin_no; }

  static const uint32_t pin_mask(void) { return 1 << pin_no; }

  static GPIO_TypeDef & GPIOx(void) {
    switch(gpio_port_no) {
    case 1:
      return *GPIOA;

    case 2:
      return *GPIOB;

    case 3:
      return *GPIOC;

    case 4:
      return *GPIOD;
    }
  }

  void pin_mode(pin_mode_e mode) {
    switch(mode) {
    case INPUT:
    case INPUT_FLOATING:
      setmode_input();
      break;

    case INPUT_PULLUP:
      setmode_input(PIN_INPUT_PULLUP);
      break;

    case INPUT_PULLDOWN:
      setmode_input(PIN_INPUT_PULLDOWN);
      break;

    case OUTPUT:
      setmode_output();
      break;

    default:
      while(1); // TBD: finish the rest of the modes
      break;
    }
  }

  void setmode_input(pin_cnf_mode_in_e cnf_mode = PIN_INPUT_FLOATING) {
    const uint32_t offset = (pin_no < 8) ? (pin_no) << 2 : (pin_no - 8) << 2; // CRx+CNF is 4 bits
    volatile uint32_t * const crx_reg = (pin_no < 8) ? &GPIOx().CRL : &GPIOx().CRH;

    uint32_t temp = *crx_reg;
    temp = (temp & ~(PIN_CRX_CNF_MASK << offset)) | ((cnf_mode) << offset);
    *crx_reg = temp;

    if (cnf_mode == PIN_INPUT_PULLUP) {
      GPIOx().ODR |= pin_mask();
    }
    else if (cnf_mode == PIN_INPUT_PULLDOWN) {
      GPIOx().ODR &= ~(pin_mask());
    }
  }

  /*
   * setmode_output - defaults to Push/Pull @ 50MHz
   *
   * modifies both CNF and CRX (CRL/CRH) register settings
   */
  void setmode_output(pin_crx_speed_e speed=PIN_SPEED_50MHZ, pin_cnf_mode_out_e cnf_mode=PIN_PUSHPULL_OUT ) {
    const uint32_t offset = (pin_no < 8 ) ? (pin_no) << 2 : (pin_no - 8) << 2; // CRx+CNF is 4 bits
    volatile uint32_t * const crx_reg = (pin_no < 8) ? &GPIOx().CRL : &GPIOx().CRH;

    uint32_t temp = *crx_reg;
    temp = (temp & ~(PIN_CRX_CNF_MASK << offset)) | ( (cnf_mode | speed) << offset);
    *crx_reg = temp;
  }

  void clear(void) {
    low();
  }

  void high(void) {
    GPIOx().BSRR = pin_mask();
  }

  void low(void) {
    GPIOx().BRR = pin_mask();
  }

  void reset(void) {
    low();
  }

  void set(void) {
    high();
  }

  void toggle(void) {
    GPIOx().ODR ^= pin_mask();
  }

  int value(void) {
    return (GPIOx().IDR & pin_mask()) ? HIGH : LOW;
  }

  void operator=(const int value) {
    (value) ? high() : low();
  }

  operator int() {
    return value();
  }

};

/*
 * GPIO_PIN_LED - LED specialized GPIO_PIN
 */

template<const uint32_t gpio_port_no, const uint32_t pin_no, const bool active_high=true >
struct GPIO_PIN_LED :
    GPIO_PIN<gpio_port_no, pin_no > {
      typedef GPIO_PIN<gpio_port_no, pin_no> BASE;

      void off(void) {
        active_high ? this->low() : this->high();
      }

      void on(void) {
        active_high ? this->high() : this->low();
      }
};

#define pinMode(p,m) ((p).pin_mode((m)))
#define digitalWrite(p,v) ((v) ? (p).high() : (p).low())

#endif /* GPIO_H_ */
