#ifndef _STK500K_H_
#define _STK500K_H_

#include "command.h" /* defines for stk500k from atmel */

// STK Definitions
#define STK_OK      Resp_STK_OK      /* 0x10 */
#define STK_FAILED  Resp_STK_FAILED  /* 0x11 */
#define STK_UNKNOWN Resp_STK_UNKNOWN /* 0x12 */
#define STK_INSYNC  Resp_STK_INSYNC  /* 0x14 */
#define STK_NOSYNC  Resp_STK_NOSYNC  /* 0x15 */
#define CRC_EOP     Sync_CRC_EOP     /* 0x20 ok it is a space... */

#define PULSE_TIME 30

#define EECHUNK (32)

typedef struct param {
  uint8_t devicecode;
  uint8_t revision;
  uint8_t progtype;
  uint8_t parmode;
  uint8_t polling;
  uint8_t selftimed;
  uint8_t lockbytes;
  uint8_t fusebytes;
  uint8_t flashpoll;
  uint16_t eeprompoll;
  uint16_t pagesize;
  uint16_t eepromsize;
  uint32_t flashsize;
} parameter;

#endif

