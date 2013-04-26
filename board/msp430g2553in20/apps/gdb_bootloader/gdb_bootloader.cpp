/**
 * gdb_bootloader.cpp - implement chip resident GDB stub server
 *
 *  Created: Apr 4, 2013
 *   Author: rick@kimballsoftware.com
 *     Date: 04-04-2013
 *  Version: 1.0.0
 *
 * This code implements a small (1k) GDB stub server native on your msp430 chip.
 * It is used to reprogram your chip using msp430-gdb. This code acts as
 * a "bootloader" when used with the erase and load features of msp430-gdb.
 *
 * To load new code after you install this on your chip run:
 *
 * $ msp430-gdb -b 9600 -ex 'target remote /dev/ttyACM0' yourcode.elf
 *
 * $ msp430-size gdb_bootloader.elf
 *    text    data     bss     dec     hex filename
 *    1054       0       0    1054     41e gdb_bootloader.elf
 *
 * Check this thread:
 *  http://forum.43oh.com/topic/3661-1k-serial-gdb-bootloader/
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

#include <fabooh.h>
#define SMALL_INIT4
#include <main.h>
#include <stdint.h>
#include <cpu.h>
#include <serial.h>
#include <string.h>
#include "flashmgr.h"

#if 1 /* might need to comment this out to gain space for the software only UART */
__attribute__((section(".copyr"),used))
const char _copyright[] = {
    "Copyright \xC2\xA9 2013 Kimball Software"
    };
#endif

/*
 * provide union access to 4 unsigned bytes,
 * that can be also be accessed as 4 char bytes
 * data[3] will always be '\0 to terminate data[0..1]
 */
union hex_work_t {
  struct {
    char c_str[4];
  };
  // -- or as 4 8 bit usigned values --
  struct {
    uint8_t data[4]; // [0] low byte
                     // [1] high byte lil-endian order
                     // [2] always 0
                     // [3] checksum converted to uint8_t
                     //     so 'F','A' becomes a uint8_t value of 0xfa
  };
};

/*
 * These are chip specific values that indicate the start
 * of flash and start of our gdb_bootloader code
 *
 * Look at linker map, you may have to adjust based on the
 * size of this code. These values are for an msp430g2553
 */
const unsigned INFOMEM_ADDR = 0x1000;
const unsigned INFOMEM_END = 0x10c0-1;  // leave INFOA segment alone, just use INFOMEM D..B
const unsigned GDB_ROMADDR = 0xc000;    // rom as in the ldscript sense
const unsigned GDB_LOADER_ADDR = 0xfa00;
const unsigned GDB_BOOT_END = 0xfe00-1;
const unsigned VECTOR_TABLE_SEGMENT = 0xfe00;
const unsigned GDB_BOOT_RESET_VECTOR = 0xfffe;
const bool ERASE_INFO_MEM = true;

/*
 * hexstr2u16() - convert hex strings into uint16
 *
 * assumes caller is only passing '0'-'f' digits or null
 */
unsigned hexstr2u16(uint8_t *s) {
  uint16_t u16=0;

  do {
    u16 = u16 << 4;
    u16 |= (*s < 'a') ? *s-'0' : *s - 'a' + 10;
  } while (*++s);

  return u16;
}

/*
 * u16tohex() - converts unsigned value to hex character
 *
 * assumes caller is passing good data: 0x0-0x0F binary values
 */
char u16tohexc(const unsigned n) {
  return "0123456789abcdef"[n];
}

/*
 * checksum() - GDB RSP checksum - all data between '$' and '#'
 *
 */
ALWAYS_INLINE
uint16_t compute_checksum(char * data, unsigned count) {
  uint16_t chksum = 0;

  do {
    chksum = (chksum + *(data++)) % 256;
  } while(--count);

  return chksum;
}

/*
 * gdb_bootloader - this is the main GDB RSP server loop
 */
void gdb_bootloader() {
  const uint32_t BAUD_RATE = 9600;
  serial_default_t<BAUD_RATE, CPU::frequency, TX_PIN, RX_PIN> Serial; // TX=varies, RX=varies

  PUSH2::setmode_inputpullup();

  if ( !PUSH2::is_low() ) {
    RED_LED::setmode_input();
    // check to see if there is code at 0xc000, if not then just go into gdb server mode
    __asm__ (
        " mov.w #__usercode, r15 ; varies based on chip\n"
        " cmp.w #0xffff,0(r15)   ; is there code there? or just erased bytes\n"
        " jeq   run_gdb_boot     ; don't launch just start bootloader\n"
        " mov.w r15, r0          ; jump to usercode, never return r0==PC\n"
        "run_gdb_boot:           ;\n"
        );
  }

  RED_LED::setmode_output();
  RED_LED::high();

  Serial.begin(BAUD_RATE);      // configure serial device pins and uart clock

  const unsigned bufsize=63;    // match packet size we advertise see query packet below
  static uint8_t rsp_data[bufsize+1];  // space to store received gdb rsp packet without '$' or '#xx' checksum
  unsigned indx, c;

  hex_work_t chksum;
  chksum.c_str[2]=0; // reserve this byte for null termination

  while(1) {
    indx = 0;

    // parse GDB packet +$....#xx
    // Note: GDB Remote Serial Protocol is not a line oriented protocol. There aren't any
    // '\n' in  the packet. End of packet is just the '#XX' where XX is the checksum

    //----------------------------------------------------------------------------------------
    // gather a GDB serial request packet

    // find sync character '$'
    do {
      c = Serial.read();
    } while(c != '$');

    // squirrel away the RSP data bytes. We don't parse on the fly.
    // We want to use this with software only UART solutions that
    // might be slow.
    do {
      c=Serial.read();

      if ( c == '#') {                      // end of packet data
        chksum.data[0] = Serial.read();     // read low byte
        chksum.data[1] = Serial.read();     // read high byte
        chksum.data[3] = hexstr2u16(chksum.data);
        break;                              // OK, now go handle it
      }
      rsp_data[indx++] = c;
    } while( indx < bufsize );
    rsp_data[indx]=0;                     // null terminate

    // verify checksum, ACK '+' if good, NAK '-' if bad
    // note: GDB will re-send the request again if we NAK
    unsigned _chkdigits = compute_checksum((char *)rsp_data,indx);
    if ( _chkdigits != chksum.data[3] ) {
      Serial << "-";
      continue;
    }
    else {
      Serial << "+";
    }

    //------------------------------------------------------------------------------------------
    // handle the commands in the packet, send a null response for commands we don't
    //
    // Note: The if logic order here is based on most frequent to least frequent
    //       command patterns.

    if ( rsp_data[0] == 'm' || rsp_data[0] == 'M') {
      uint8_t *begin, *end;

      // get the staring address for flash memory writes
      begin = end = &rsp_data[1];
      while(*++end != ','); *end = 0;
      uint16_t addr = hexstr2u16(begin);
      begin = end+1;

      // get the count of how many bytes to read or write
      while(*++end != ':'); *end = 0;
      unsigned cnt = hexstr2u16(begin);
      begin = end+1;

      /*
       * read memory - $maddr,length#checksum
       *
       * $mc000,1#bd
       */

      if ( rsp_data[0] == 'm' ) {
        unsigned _chkdig = 0;

        Serial << "$";
        // reuse checksum structure as a work buffer
        char *hex_value=chksum.c_str;
        do {
          int16_t value_at_addr = *(uint8_t *)addr++; // use signed to avoid clrc

          // output the value at the address
          hex_value[0] = u16tohexc(value_at_addr >> 4);
          _chkdig = (_chkdig + hex_value[0]) % 256;
          hex_value[1] = u16tohexc(value_at_addr & 0x0f);
          _chkdig = (_chkdig + hex_value[1]) % 256;
          Serial << hex_value;

        } while(--cnt);

        // output checksum of the hex version of the value at address
        //value_at_addr = compute_checksum(hex_value,2);
        hex_value[1] = u16tohexc(_chkdig & 0x0f);
        hex_value[0] = u16tohexc((int)_chkdig >> 4); // use signed to avoid clrc
        Serial  << "#" << hex_value;
        continue;
      }

      /*
       * write memory - $Maddr,length:hexb#checksum
       *
       * $Mc000,c:b240805a2001314000043f40#4b
       */
      if ( rsp_data[0] == 'M' ) {

        // iterate over the data bytes and write each hex pair to flash memory
        FlashMgr::enable_write();
        do {
          // check for the _reset_vector and our code range,
          // we don't modify flash in those ranges
          if ( addr < GDB_LOADER_ADDR ||  addr > GDB_BOOT_END ) {
            if ( addr != 0xfffe && addr != 0xffff ) { // check for writes to GDB_BOOT_RESET_VECTOR 0xfffe, 0xffff
              // reuse chcksum as a temporary buffer
              chksum.data[0] = begin[0];
              chksum.data[1] = begin[1];
              *(uint8_t *)addr = hexstr2u16(chksum.data); // FlashMgr::write_u8()...
            }
          }
          addr++;
          begin += 2;
        } while(--cnt);
        FlashMgr::disable_write();

        // all good
        Serial << "$OK#9a";
        continue;
      }
    }

    /*
     * queries and custom remote commands
     *
     * we just check the first letter of q commands and use the checksum
     * to see if it is one we handle, probably not safe but small
     *
     * TODO: look at other random bytes in the packet to verify cmd, only
     *       if this becomes a problem
     */

    if ( rsp_data[0] == 'q' ) {

      /*
       * eraseflash - host says to erase flash memory
       *
       * $qRcmd,657261736520616c6c#2e
       */
      if ( chksum.data[3] == 0x2e) {
        unsigned addr = GDB_ROMADDR;

        // erase main flash memory one 512 byte segment at a time
        // only erase main memory, preserve the boot loader code
        do {
          FlashMgr::erase_segment((unsigned char *)addr);
          addr += 512;
        } while(addr < GDB_LOADER_ADDR);

        // erase the vector table segment
        FlashMgr::erase_segment((unsigned char *)VECTOR_TABLE_SEGMENT);

        // place our start address back in the _reset_vector_
        // this allows the user to provide their own interrupt vectors
        // however, the gdb boot code still runs first to check the button state
        FlashMgr::write_u16(static_cast<unsigned *>((unsigned *)GDB_BOOT_RESET_VECTOR), GDB_LOADER_ADDR);

        if ( ERASE_INFO_MEM ) {
          addr = INFOMEM_ADDR;
          do {
            FlashMgr::erase_segment((unsigned char *)addr);
            addr += 64;
          } while(addr < INFOMEM_END);
        }

        Serial << "$6f6b0a#c5"; // ok
        continue;
      }

      /*
       * query capabilities -
       *
       *   msp430-gdb wants to know what we can do. which is not much :)
       *   we just tell them our max packet size. msp430-gdb will limit
       *   the data packet requests to the length we provide
       *
       * $qSupported:qRelocInsn+#9a
       */
      if ( chksum.data[3] == 0x9a ) {
        Serial << "$PacketSize=3f#c9"; // set maximum packet size gdb host will send
                                       // must match or be less than sizeof(rsp_data)-1
        continue;
      }

#if 0 /* doesn't seem to add any value, just adds to code size */
      /*
       * $qOffsets#4b
       * $Text=0;Data=0;Bss=0#04
       */
      if ( chksum.data[3] == 0x4b ) {
        Serial << "$Text=0;Data=0;Bss=0#04";
        continue;
      }
#endif

    }

    /*
     * kill request, we reset on quit
     *
     * you will have to hold the P1.3 button down during restart
     * to get gdb_bootloader to run again once code is loaded at 0xc000
     *
     * $k#6b
     */
    if ( rsp_data[0] == 'k') {
      Serial << "$#00";
      Serial.flush();
      WDTCTL = 0; // force a reset by using invalid WDT password
    }

    /*
     * get current register values --
     *
     * $g#67
     */

    if ( rsp_data[0] == 'g' ) {
      unsigned cnt=(16-1)*4; // send value of 16 registers 0..15
      Serial << "$00c0";  // though it does seem to want a real PC register value
      do {
        Serial << "0";    // send '0'  we aren't going to debug anyways
      } while(--cnt);
      Serial << "#33";
      continue;
    }

    /*
     * status request - why did we stop?
     *
     * Note: we get this one time at start up
     *
     * $?#3f
     */
    if ( rsp_data[0] == '?' ) {
      // for our purposes of loading code, we can get away with just providing
      // the PC register (R0) and STACK register (R1)

      // T05 is SIGTRAP  5 Trace trap (POSIX) ..
      //Serial << "$T0500:00c0;01:fe03;#85";
      Serial << "$S05#b8";
      continue;
    }

    /*
     * all other unhandled request get a NULL GDB RSP response
     */
    Serial << "$#00";

  } // end while
}

void setup() {
  gdb_bootloader();
}

inline void loop() {}

/*
 * TODO: think about using a jump table so we don't have to lose 480 bytes in 0xfe00 segment
 * TODO: expose common routines, serial, hex convert, checksum so the user can use them as "rom routines"
 * TODO: think about implementing $X, however right now we are 7 bit clean so ignore for now
 */
