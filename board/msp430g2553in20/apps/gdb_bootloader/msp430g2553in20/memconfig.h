/*
 * memconfig.h - 
 *
 *  Created: Apr 11, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: 04-11-2013
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

#ifndef MEMCONFIG_H_
#define MEMCONFIG_H_

/*
 * These are chip specific values that indicate the start
 * of flash and start of our gdb_bootloader code
 *
 * Look at linker map, you may have to adjust based on the
 * size of this code. These values are for an msp430g2553
 */

extern unsigned __urom, __user_reset_vector;
const unsigned INFOMEM_ADDR = 0x1000;
const unsigned INFOMEM_END = 0x10c0-1;      // leave INFOA segment alone, just use INFOMEM D..B
const unsigned GDB_LOADER_ADDR = 0xfa00;
const unsigned GDB_BOOT_END = 0xfe00-1;
const unsigned VECTOR_TABLE_SEGMENT = 0xfe00;
const unsigned GDB_BOOT_RESET_VECTOR = 0xfffe;
const unsigned USER_RESET_VECTOR = 0xffbe;
const bool ERASE_INFO_MEM = true;

#endif /* MEMCONFIG_H_ */
