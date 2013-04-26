/*
 * cordic_wrapper.h -
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

#ifndef CORDIC_WRAPPER_H_
#define CORDIC_WRAPPER_H_

 #include "cordic.h"

/*
 * d15 - scale a q15 binary value to decimal scale
 */
template<int digits>
struct d15 {
  int r;    // 0 - .9999 remainder
  int sign; // -1 negative, 0 - positive
  static const int scale = digits <= 1 ? 1
                  : (digits == 2) ? 100
                  : (digits == 3) ? 1000
                  : (digits == 4) ? 10000 : 3;

  d15(int qvalue) {
    sign = qvalue < 0 ? -1 : 0;
    long l = ((long)qvalue*scale)/32768; // convert to decimal scale
    r = l % scale;
    if ( l < 0 ) {
      r = -r;
    }
  }
};

// Function to find the sine and cosine of an angle using CORDIC
int sin_q15(int theta);
int cos_q15(int theta);

#endif /* CORDIC_WRAPPER_H_ */
