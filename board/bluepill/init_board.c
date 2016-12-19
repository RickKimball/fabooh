#include <stm32f1xx.h>
#include <core_cm3.h>
#include <inttypes.h>

/*
 * Configure SYSCLOCK for 72MHz using an 8MHz xtal and the PLL
 */
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
   * APB1 Prescaler: 2 - slow bus
   * APB2 Prescaler: 1 - fast bus
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

/*
 * _init() - system level routine called from __libc_init_array
 *
 * Reset_Handler establishes the stack, initializes .data and .bss
 * sections then calls any c++ global constructors. This routine
 * is called from __libc_init_array before those ctors are called.
 *
 * This is the place to configure any peripheral clocks or configure
 * any special devices before those ctors run.
 *
 * We could wait to see if specific peripherals are used and then
 * enable their clock at init time. However, that would add up to a
 * bunch of calls that just toggle a bit.  So let's do it once here
 * and be done with it.
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

