#ifndef CPU_H_
#define CPU_H_
#include <stm32f1xx.h>
#include <core_cm3.h>
#include <inttypes.h>

template<uint32_t MCLK_FREQ >
struct cpu_base_t {
  static const uint32_t frequency = MCLK_FREQ;
  static const uint32_t cycles_per_msec = frequency / 1000;
  static const uint32_t cycles_per_usec = frequency / 1000000;

  static void enable_clkout() {
  }

  static void reset(void) {
    NVIC_SystemReset();
  }
};

typedef cpu_base_t<F_CPU> CPU;

static inline void SystemClock_Configure(void) {
  /* 2 wait states */
  FLASH->ACR |= FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE;

  /* turn on HSE xtal and wait for it */
  RCC->CR |= RCC_CR_HSEON;
  while((RCC->CR & RCC_CR_HSERDY) == 0);

  /*
   * PLL  Source: HSE
   * PLL  Multiply Factor: 9
   * USB  Prescaler: 1.5
   * AHB  Prescaler: 1
   * APB1 Prescaler: 2
   * APB2 Prescaler: 1
   * ADC  Prescaler: 6
   */
  RCC->CFGR =
    RCC_CFGR_PLLSRC     |   /* HSE PLL */
    RCC_CFGR_PLLMULL9   | /* 8MHz xtal * 9 pllmul = 72MHz */
    /* default(0) RCC_CFGR_USB_PRE is 1.5 */
    RCC_CFGR_HPRE_DIV1  | /* SYSCLK/1 */
    RCC_CFGR_PPRE1_DIV2 | /* ABP1/2 Low Speed */
    RCC_CFGR_PPRE2_DIV1 | /* ABP2/2 High Speed */
    RCC_CFGR_ADCPRE_DIV6;

  /* turn on PLL using HSE and wait for it */
  RCC->CR |= RCC_CR_PLLON;
  while((RCC->CR & RCC_CR_PLLRDY) == 0);

  /*
   * Switch the System clock to the PLL
   */
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_PLL) | RCC_CFGR_SW_PLL;

  /*
   * Updates the SystemCoreClock variable
   * Now the CPU clock is 9*HSE (72MHz for
   * an 8MHz oscillator)
   */
  SystemCoreClockUpdate();
}

extern "C" {
  volatile uint32_t tickcount;

  uint32_t millis(void) {
    return tickcount;
  }

  /*
   * vector table systick handler must have c name bindings
   */
  void SysTick_Handler(void) {
    ++tickcount;
  }

  /*
   * _init() - system level routine called from __libc_init_array
   *
   * Reset_Handler establishes the stack, initializes .data and .bss
   * sections then calls any c++ global constructors. This routine
   * is called from __libc_init_array before those ctors are called.
   *
   * This is the place to configure any peripheral clocks or configure
   * any special devices before those ctors run.
   */
  void _init(void) {
    // establish HSE + PLL as the system clock and configure it for 72MHz
    SystemClock_Configure();

    // perform board level initialization here
    // enable RCC Clock for GPIO A,B,C,D,ALT IO, USART1
    RCC->APB2ENR |= (
           RCC_APB2ENR_IOPAEN
         | RCC_APB2ENR_IOPBEN
         | RCC_APB2ENR_IOPCEN
         | RCC_APB2ENR_IOPDEN
         | RCC_APB2ENR_AFIOEN
         | RCC_APB2ENR_USART1EN
         );

    RCC->APB1ENR |= (
           RCC_APB1ENR_USART2EN
         | RCC_APB1ENR_USART3EN
         );
  }
}

/*
 * delay - delay milliseconds
 */
void __noinline delay(const unsigned msecs) {
  if ( msecs ) {
    unsigned t0 = tickcount;

    while((tickcount - t0) < msecs ) {
      //__WFI();
    }
  }
}

#endif /* CPU_H_ */
