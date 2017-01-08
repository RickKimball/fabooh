/*
 * fabooh.h
 *
 *  Created on: Apr 15, 2013
 *      Author: kimballr
 */

#ifndef FABOOH_H_
#define FABOOH_H_

#define FABOOH_VER 0x0100
#define FABOOH_BLD 0x0004

#define __always_inline __attribute__((always_inline))
#define __no_inline __attribute__((noinline))

#include <inttypes.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stm32f1xx.h>
#include <arm_math.h>

#include "cpu.h"
#include "gpio.h"
#include "print.h"
#include "serial.h"

#include <pins.h>   /* from board directory */

#endif /* FABOOH_H_ */
