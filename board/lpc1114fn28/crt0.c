/*
 * crt0.c - handle chip reset
 */

#include "LPC11xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define ALIAS_DEFHANDLER __attribute__((weak,alias("IntDefaultHandler")))
#define WEAK_SYSISR __attribute__((weak))

extern int __stack;

void Reset_Handler(void) __attribute__((section(".init0"),naked));
void NMI_Handler(void) WEAK_SYSISR;
void HardFault_Handler(void) WEAK_SYSISR;
void SVCall_Handler(void) WEAK_SYSISR;
void PendSV_Handler(void) WEAK_SYSISR;
void SysTick_Handler(void) WEAK_SYSISR;

void IntDefaultHandler(void) __attribute__((weak));
void WAKEUP0_IRQHandler(void) ALIAS_DEFHANDLER;     /*!< All I/O pins can be used as wakeup source.       */
void WAKEUP1_IRQHandler(void) ALIAS_DEFHANDLER;     /*!< There are 13 pins in total for LPC11xx           */
void WAKEUP2_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP3_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP4_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP5_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP6_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP7_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP8_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP9_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP10_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP11_IRQHandler(void) ALIAS_DEFHANDLER;
void WAKEUP12_IRQHandler(void) ALIAS_DEFHANDLER;
//void CAN_IRQHandler(void) ALIAS_DEFHANDLER;       /*!< CAN Interrupt                                    */
void SSP1_IRQHandler(void) ALIAS_DEFHANDLER;        /*!< SSP1 Interrupt                                   */
void I2C_IRQHandler(void) ALIAS_DEFHANDLER;         /*!< I2C Interrupt                                    */
void TIMER16_0_IRQHandler(void) ALIAS_DEFHANDLER;  /*!< 16-bit Timer0 Interrupt                          */
void TIMER16_1_IRQHandler(void) ALIAS_DEFHANDLER;  /*!< 16-bit Timer1 Interrupt                          */
void TIMER32_0_IRQHandler(void) ALIAS_DEFHANDLER;  /*!< 32-bit Timer0 Interrupt                          */
void TIMER32_1_IRQHandler(void) ALIAS_DEFHANDLER;  /*!< 32-bit Timer1 Interrupt                          */
void SSP0_IRQHandler(void) ALIAS_DEFHANDLER;        /*!< SSP0 Interrupt                                   */
void UART_IRQHandler(void) ALIAS_DEFHANDLER;        /*!< UART Interrupt                                   */
//void Reserved0_IRQHandler(void) ALIAS_DEFHANDLER; /*!< Reserved Interrupt                               */
//void Reserved1_IRQHandler(void) ALIAS_DEFHANDLER;
void ADC_IRQHandler(void) ALIAS_DEFHANDLER;         /*!< A/D Converter Interrupt                          */
void WDT_IRQHandler(void) ALIAS_DEFHANDLER;         /*!< Watchdog timer Interrupt                         */
void BOD_IRQHandler(void) ALIAS_DEFHANDLER;         /*!< Brown Out Detect(BOD) Interrupt                  */
void FMC_IRQHandler(void) ALIAS_DEFHANDLER;         /*!< Flash Memory Controller Interrupt                */
void EINT3_IRQHandler(void) ALIAS_DEFHANDLER;       /*!< External Interrupt 3 Interrupt                   */
void EINT2_IRQHandler(void) ALIAS_DEFHANDLER;       /*!< External Interrupt 2 Interrupt                   */
void EINT1_IRQHandler(void) ALIAS_DEFHANDLER;       /*!< External Interrupt 1 Interrupt                   */
void EINT0_IRQHandler(void) ALIAS_DEFHANDLER;       /*!< External Interrupt 0 Interrupt                   */

int main( void );

// vector table entries are:
// declare __vector as const pointer to function (void) returning void
typedef void (* const __vector)(void);

__attribute__ ((section(".vectors")))
__vector __isr_vectors[16 + 32] = {
    /******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
    (__vector ) &__stack    // -16 set the top of stack
    , Reset_Handler         // -15 reset handler
    , NMI_Handler           // -14 The NMI handler
    , HardFault_Handler     // -13 The hard fault handler
    , 0                     // -12 Reserved
    , 0                     // -11 Reserved
    , 0                     // -10 Reserved
    , 0                     // -9 Reserved
    , 0                     // -8 Reserved
    , 0                     // -7 Reserved
    , 0                     // -6 Reserved
    , SVCall_Handler        // -5 SVCall handler
    , 0                     // -4 Reserved
    , 0                     // -3 Reserved
    , PendSV_Handler        // -2 The PendSV handler
    , SysTick_Handler       // -1 The SysWAKEUP_IRQHandlerTick handler

    /******  LPC1114FN28 Specific Interrupt Numbers *******************************************************/
    , WAKEUP0_IRQHandler    /*!< All I/O pins can be used as wakeup source.       */
    , WAKEUP1_IRQHandler    // PIO0_1  Wakeup
    , WAKEUP2_IRQHandler    // PIO0_2  Wakeup
    , WAKEUP3_IRQHandler    // PIO0_3  Wakeup
    , WAKEUP4_IRQHandler    // PIO0_4  Wakeup
    , WAKEUP5_IRQHandler    // PIO0_5  Wakeup
    , WAKEUP6_IRQHandler    // PIO0_6  Wakeup
    , WAKEUP7_IRQHandler    // PIO0_7  Wakeup
    , WAKEUP8_IRQHandler    // PIO0_8  Wakeup
    , WAKEUP9_IRQHandler    // PIO0_9  Wakeup
    , WAKEUP10_IRQHandler   // PIO0_10 Wakeup
    , WAKEUP11_IRQHandler   // PIO0_11 Wakeup
    , WAKEUP12_IRQHandler   // PIO1_12 Wakeup
    , 0                     // CAN
    , SSP1_IRQHandler       // SSP1
    , I2C_IRQHandler        // I2C0
    , TIMER16_0_IRQHandler  // CT16B0 (16-bit Timer 0)
    , TIMER16_1_IRQHandler  // CT16B1 (16-bit Timer 1)
    , TIMER32_0_IRQHandler  // CT32B0 (32-bit Timer 0)
    , TIMER32_1_IRQHandler  // CT32B1 (32-bit Timer 1)
    , SSP0_IRQHandler       // SSP0
    , UART_IRQHandler       // UART0
    , 0                     // USB IRQ
    , 0                     // USB FIQ
    , ADC_IRQHandler        // ADC (A/D Converter)
    , WDT_IRQHandler        // WDT (Watchdog Timer)
    , BOD_IRQHandler        // BOD (Brownout Detect)
    , FMC_IRQHandler        // Flash (IP2111 Flash Memory Controller)
    , EINT3_IRQHandler      // PIO INT3
    , EINT2_IRQHandler      // PIO INT2
    , EINT1_IRQHandler      // PIO INT1
    , EINT0_IRQHandler      // PIO INT0
};

/*
 * Reset_Handler - initialize .bss section, .data section, c++ static globals, and call main
 */
void Reset_Handler(void) {
    extern int __data_init_start;
    extern int __data_start;
    extern int __data_end;
    extern int __bss_start;
    extern int __bss_end;

    int *s, *d, *e;

    asm("mov sp,%0" :: "r" (&__stack));
    // clear .bss section
    d = &__bss_start;
    e = &__bss_end;
    while (d != e) {
        *d++ = 0;
    }

    // copy .data section from flash to ram
    s = &__data_init_start;
    d = &__data_start;
    e = &__data_end;
    while (d != e) {
        *d++ = *s++;
    }

    extern void __libc_init_array(void);

    __libc_init_array();

    // call main
    (void)main();

#if 0
    // call the c++ global objects constructors
    s = &__fini_array_start;
    e = &__fini_array_end;
    while (s != e) {
        (*((void (**)()) s++))();
    }
#endif

    // trap exit
    while(1);
}

//*****************************************************************************
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code. These all have __attribute__((weak))
//*****************************************************************************
void NMI_Handler(void)
{
    while(1);
}

void HardFault_Handler(void)
{
    while(1);
}

void SVCall_Handler(void)
{
    while(1);
}

void PendSV_Handler(void)
{
    while(1);
}

void SysTick_Handler(void)
{
    while(1);
}

//*****************************************************************************
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//*****************************************************************************
void IntDefaultHandler(void)
{
    while(1);
}

// called when a pure virtual method is called
void __cxa_pure_virtual( void )
{
    while(1); // stop attempts to make pure virtual calls
}

#ifdef __cplusplus
}
#endif
