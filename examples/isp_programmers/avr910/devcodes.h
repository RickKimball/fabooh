/*
 * 
 * File   : devcodes.h
 * Author : Minifloat 
 * Project: Launchprog
 * Version: 1.2 on 10 May 2012
 *
 */


//These Device codes are directly taken from Serasidis' AVR910
//i know i shouldn't put this in a header file only
//but now its more like using a asm .inc file

//Device codes and program mode table (0(zero)=byte mode, <>0=page mode)
//i.e. 'P' can be used for page mode for easier reading.

#ifndef DEVCODES_H
#define DEVCODES_H

#include <inttypes.h>

const static uint8_t device_codes[][2] = {
   
   //AT90S1200 rev. A	//From 0x00-0x0f unused yet
   {0x10, 0},
   //AT90S1200 rev. B	
   {0x11, 0},
   //AT90S1200 rev. C
   {0x12, 0},
   //AT90S1200
   {0x13, 0},		//From 0x14-0x1f unused yet
   //AT90S2313
   {0x20, 0},		//From 0x21-0x27 unused yet
   //AT90S4414
   {0x28, 0},		//From 0x29-0x2f unused yet
   //AT90S4433
   {0x30, 0},		//From 0x31-0x33 unused yet
   //AT90S2333
   {0x34, 0},		//From 0x35-0x37 unused yet
   //AT90S8515
   {0x38, 0},		//0x39 unused yet
   //ATmega8515
   {0x3A, (uint8_t)'P'},
   //ATmega8515 BOOT
   {0x3B, (uint8_t)'P'},	//From 0x3c-0x40 unused yet
   //ATmega103
   {0x41, (uint8_t)'P'},
   //ATmega603
   {0x42, (uint8_t)'P'},
   //ATmega128
   {0x43, (uint8_t)'P'},
   //ATmega128 BOOT
   {0x44, (uint8_t)'P'},
   //ATmega64
   {0x45, (uint8_t)'P'},	// v1.40
   //ATmega64 BOOT
   {0x46, (uint8_t)'P'},	// v1.40  0x47 unused yet
   
   //AT90S2323
   {0x48, 0},		//From 0x49-0x4b unused yet		
   //AT90S2343
   {0x4C, 0},		//From 0x4d-0x4f unused yet
   
   //0x50,0x51 used. From 0x52-0x54 unused yet
   //
   //ATtiny12
   {0x55, 0},
   //ATtiny15
   {0x56, 0},		//0x57 unused yet
   //ATtiny19
   {0x58, 0},		//From 0x59-0x5b unused yet	
   //ATtiny28
   {0x5C, 0},		//0x5d unused yet
   //ATtiny26
   {0x5E, (uint8_t)'P'},	//0x5f unused yet
   //
   //ATmega161
   {0x60, (uint8_t)'P'},
   //ATmega161 BOOT
   {0x61, (uint8_t)'P'},	//0x62-0x63 unused yet
   //ATmega163
   {0x64, (uint8_t)'P'},
   //ATmega83
   {0x65, (uint8_t)'P'},
   //ATmega163 BOOT
   {0x66, (uint8_t)'P'},
   //ATmega83 BOOT
   {0x67, (uint8_t)'P'},
   //AT90S8535
   {0x68, 0},
   //ATmega8535
   {0x69, (uint8_t)'P'},	// v1.40 From 0x6a-0x6b unused yet	
   //ATmega8535 BOOT??
   //	{0x6a, (uint8_t)'P'},
   //
   //AT90S4434
   {0x6C, 0},		//From 0x6d-0x6f unused yet	
   //AT90C8534
   {0x70, 0},
   //AT90C8544
   {0x71, 0},
   //ATmega32
   {0x72, (uint8_t)'P'},
   //ATmega32 BOOT
   {0x73, (uint8_t)'P'},
   //ATmega16
   {0x74, (uint8_t)'P'},
   //ATmega16 BOOT
   {0x75, (uint8_t)'P'},
   //ATmega8
   {0x76, (uint8_t)'P'},
   //ATmega8 BOOT
   {0x77, (uint8_t)'P'},
   //ATmega169
   {0x78, (uint8_t)'P'},	// v1.40
   //ATmega169 BOOT
   {0x79, (uint8_t)'P'},	// v1.40 From 0x7a-0x7f unused yet
   
   //test
   //	{0x08, (uint8_t)'P'},
   //	{0x09, (uint8_t)'P'},
   //	{0x0a, (uint8_t)'P'},
   //	{0x0b, (uint8_t)'P'},
   
   //These devices are not supported by this hardware
   //ATtiny10
   //	{0x51, 0},
   //ATtiny11
   //	{0x50, 0},
   //AT89C1051
   //	{0x80, 0},
   //AT89C2051
   //	{0x81, 0},		//From 0x82-0x85 unused yet
   //AT89S8252
   //	{0x86, 0},
   //AT89S53
   //	{0x87, 0},		//From 0x88-0x9f unused yet
   
   //end_of_device_codes:
   //.dw	0xffff
   {0xFF, 0xFF}
   //@@
};


#endif //DEVCODES_H
