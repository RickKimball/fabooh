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

#define ALWAYS_INLINE __attribute__((always_inline))
#define STATIC_INLINE static inline

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

#include <LPC11xx.h>
#include <core_cm0.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <libfixmath/fix16.hpp>
#ifdef __cplusplus
}
#endif

#include "cpu.h"
#include "gpio.h"
#include "print.h"
#include <pins.h>
#include "serial.h"

#endif /* FABOOH_H_ */
