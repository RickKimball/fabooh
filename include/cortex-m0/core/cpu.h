/*
 * cpu.h - abstract CPU clock initialization, timing and sleeping
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 04-16-2013
 * Version: 1.0.3
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CPU_H_
#define CPU_H_

#include <LPC11xx.h>

template<uint32_t MCLK_FREQ>
struct cpu_base_t {
    static const uint32_t frequency = MCLK_FREQ;
    static const uint32_t msec_cycles = frequency/1000;
    static const uint32_t usec_cycles = frequency/1000000;
};

#include "cpu_lpc11xx.h"


#endif /* CPU_H_ */
