MEMORY {
  sfr              : ORIGIN = 0x0000, LENGTH = 0x0010 /* END=0x0010, size 16 */
  peripheral_8bit  : ORIGIN = 0x0010, LENGTH = 0x00f0 /* END=0x0100, size 240 */
  peripheral_16bit : ORIGIN = 0x0100, LENGTH = 0x0100 /* END=0x0200, size 256 */
  ram (wx)         : ORIGIN = 0x0200, LENGTH = 0x0200 /* END=0x0400, size 512 */
  infomem          : ORIGIN = 0x1000, LENGTH = 0x00c0 /* END=0x10c0, size 192 as 3 64-byte segments */
  infod            : ORIGIN = 0x1000, LENGTH = 0x0040 /* END=0x1040, size 64 */
  infoc            : ORIGIN = 0x1040, LENGTH = 0x0040 /* END=0x1080, size 64 */
  infob            : ORIGIN = 0x1080, LENGTH = 0x0040 /* END=0x10c0, size 64 */
  infoa            : ORIGIN = 0x10c0, LENGTH = 0x0040 /* END=0x1100, size 64 */
  urom (rx)        : ORIGIN = 0xc000, LENGTH = 0x3800 /* END=0xf200, size 14336 */
  rom (rx)         : ORIGIN = 0xfa00, LENGTH = 0x0400 /* END=0xfdff, size 1024 sadly we have 480 byte gap */
  vectorseg (rx)   : ORIGIN = 0xfe00, LENGTH = 0x01e0 /* END=0xffdf, size 480 */
  vectors          : ORIGIN = 0xffe0, LENGTH = 0x0020 /* END=0x10000, size 32 as 16 2-byte segments */
  /* Remaining banks are absent */
  bsl              : ORIGIN = 0x0000, LENGTH = 0x0000
  ram2 (wx)        : ORIGIN = 0x0000, LENGTH = 0x0000
  ram_mirror (wx)  : ORIGIN = 0x0000, LENGTH = 0x0000
  usbram (wx)      : ORIGIN = 0x0000, LENGTH = 0x0000
  far_rom          : ORIGIN = 0x00000000, LENGTH = 0x00000000
}
/*  rom (rx)         : ORIGIN = 0xfa00, LENGTH = 0x0400 /* END=0xfe00, size 1024 sadly we have 480 byte gap */

REGION_ALIAS("REGION_USER", urom);
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_INFOMEM", infomem);
REGION_ALIAS("REGION_FAR_ROM", far_rom);
PROVIDE (__info_segment_size = 0x40);
PROVIDE (__infod = 0x1000);
PROVIDE (__infoc = 0x1040);
PROVIDE (__infob = 0x1080);
PROVIDE (__infoa = 0x10c0);
PROVIDE (__urom  = 0xc000);
PROVIDE (__tos   = 0x0400);
PROVIDE (__user_reset_vector = 0xffbe);

/*
PROVIDE (__bootaddr = 0xf800);
PROVIDE (__bootvect = 0xffe0);
PROVIDE (__vector_segment = 0xfe00);
*/
