#include <stm32f1xx.h>
#include <core_cm3.h>
#include <inttypes.h>

/*
 * Configure SYSCLOCK for 72MHz using an 8MHz xtal and the PLL
 */
static inline void SystemClock_Configure(void) {

	/* set flash wait states */
#if F_CPU > 48000000UL
  /* 2 wait states */
  FLASH->ACR |= FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE;
#elif F_CPU > 24000000UL
  /* 1 wait states */
  FLASH->ACR |= FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE;
#else
  /* 0 wait states */
  FLASH->ACR |= FLASH_ACR_LATENCY_0 | FLASH_ACR_PRFTBE;
#endif

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

		/* configure PLL */
#if F_CPU == 4000000UL
    RCC_CFGR_PLLMULL2   | /* 2MHz xtal * 2 pllmul = 4MHz */
#elif F_CPU == 8000000UL 
    RCC_CFGR_PLLMULL2   | /* 4MHz xtal * 2 pllmul = 8MHz */
#elif F_CPU == 12000000UL
    RCC_CFGR_PLLMULL3   | /* 4MHz xtal * 3 pllmul = 12MHz */
#elif F_CPU == 24000000UL
    RCC_CFGR_PLLMULL3   | /* 8MHz xtal * 3 pllmul = 24MHz */
#elif F_CPU == 32000000UL
    RCC_CFGR_PLLMULL4   | /* 8MHz xtal * 4 pllmul = 32MHz */
#elif F_CPU == 40000000UL
    RCC_CFGR_PLLMULL5   | /* 8MHz xtal * 5 pllmul = 40MHz */
#elif F_CPU == 48000000UL
    RCC_CFGR_PLLMULL6   | /* 8MHz xtal * 6 pllmul = 48MHz */
#elif F_CPU == 56000000UL
    RCC_CFGR_PLLMULL7   | /* 8MHz xtal * 7 pllmul = 56MHz */
#elif F_CPU == 64000000UL
    RCC_CFGR_PLLMULL8   | /* 8MHz xtal * 8 pllmul = 64MHz */
#elif F_CPU == 72000000UL
    RCC_CFGR_PLLMULL9   | /* 8MHz xtal * 9 pllmul = 72MHz */
#elif F_CPU == 80000000UL /* warning slightly overclocked */
    RCC_CFGR_PLLMULL10  | /* 8MHz xtal * 10 pllmul = 80MHz */
#elif F_CPU == 88000000UL /* warning overclocked */
    RCC_CFGR_PLLMULL11  | /* 8MHz xtal * 11 pllmul = 88MHz */
#elif F_CPU == 96000000UL /* warning super overclocked */
    RCC_CFGR_PLLMULL12  | /* 8MHz xtal * 12 pllmul = 96MHz */
#elif F_CPU == 120000000UL /* warning insane overclocked */
    RCC_CFGR_PLLMULL15  | /* 8MHz xtal * 15 pllmul = 120MHz */
#elif F_CPU == 128000000UL /* warning super insane overclocked */
    RCC_CFGR_PLLMULL16  | /* 8MHz xtal * 16 pllmul = 128MHz */
#else
	#error unsupported F_CPU frequency
#endif

#if F_CPU == 72000000UL
    /* default(0) RCC_CFGR_USB_PRE is 1.5 */
#elif F_CPU == 48000000UL
		RCC_CFGR_USBPRE     | /* 1 */
#else
		#warning F_CPU will not produce a valid USB frequency
#endif

#if F_CPU > 12000000UL
    RCC_CFGR_HPRE_DIV1  | /* SYSCLK/1 8MHz */
#elif F_CPU > 4000000UL
    RCC_CFGR_HPRE_DIV2  | /* SYSCLK/2 4MHz */
#else
    RCC_CFGR_HPRE_DIV4  | /* SYSCLK/4 2MHz */
#endif

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
         RCC_APB2ENR_AFIOEN
       | RCC_APB2ENR_IOPAEN
       | RCC_APB2ENR_IOPBEN
       | RCC_APB2ENR_IOPCEN
       | RCC_APB2ENR_ADC1EN
       | RCC_APB2ENR_ADC2EN
       | RCC_APB2ENR_TIM1EN
       | RCC_APB2ENR_SPI1EN
       | RCC_APB2ENR_USART1EN
       );

  RCC->APB1ENR |= (
         RCC_APB1ENR_USART2EN
       | RCC_APB1ENR_USART3EN
       );
}

