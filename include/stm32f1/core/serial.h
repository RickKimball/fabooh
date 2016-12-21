/*
 * serial.h - common serial include
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-02-2013
 * Version: 1.0.1
 *
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "usart_basic.h"

template <const uint32_t BAUD, const uint32_t MCLK_HZ, typename TXPIN, typename RXPIN>
struct serial_default_t:
    public usart_basic_t<BAUD, MCLK_HZ, TXPIN, RXPIN>,
    public print_t< serial_default_t<BAUD, MCLK_HZ, TXPIN, RXPIN> >
{
};

// TODO: implement a Timer based software version
// TODO: enhanced software version to support port pin triggering
// TODO: add timeout option
// TODO: add ring buffer implementations

#endif
