/*
 * cordic_wrapper.cpp - provide utility functions for cordic routines
 *
 *  Created: Mar 12, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: Mar 12, 2013
 *  Version: 1.0.0
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
 */

/*
 * c++ wrapper for the cordic functions from https://github.com/the0b/MSP430-CORDIC-sine-cosine
 *
 */

#include <fabooh.h>
#include "cordic_wrapper.h"

template <typename T>
int _abs(T x) {
  return (x < 0)  ?  -x  :  x;
}

int sin_q15(int theta) {
  if ( theta == 0 || _abs(theta) == 180 || _abs(theta) == 360)
    return 0;

  int sin_result,dummy;

  cordic_sincos(theta,14,&sin_result,&dummy);

  return sin_result;
}

int cos_q15(int theta) {
  int cos_result,dummy;;

  cordic_sincos(theta,14,&dummy,&cos_result);

  return cos_result;
}
