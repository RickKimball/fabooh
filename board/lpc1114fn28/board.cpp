#include <fabooh.h>

extern "C" {

/*
 * _init called from __libc_array_init() before main after bss, data initialization
 */
void _init(void)
{
    CPU::init_clock();
    CPU::enable_clkout();
    SysTick_Config(F_CPU/1000);
}

}
