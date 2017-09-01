#
# fabooh example makefile
#
PROJECT ?= gdb_bootloader

include ../../../common_board.mk

ifeq ($(BOARD),msp430g2553in20)
include ../../../common.mk
LD_FLAGS += -L ./$(BOARD)/ -T _msp430.x
else
include ../../../common_empty.mk
endif
