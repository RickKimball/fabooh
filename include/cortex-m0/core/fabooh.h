/*
 * fabooh.h
 *
 *  Created on: Apr 15, 2013
 *      Author: kimballr
 */

#ifndef FABOOH_H_
#define FABOOH_H_

#define FABOOH_VER 0x0100
#define FABOOH_BLD 0x0003

#ifndef F_CPU
#define F_CPU 12000000
#endif

#define ALWAYS_INLINE __attribute__((always_inline))
#define STATIC_INLINE static inline

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <lpc1100l.h>

#include <libfixmath/fix16.hpp>

#include "cpu.h"
#include "gpio.h"
#include "print.h"
#include <pins.h>
#include "serial.h"

#endif /* FABOOH_H_ */
