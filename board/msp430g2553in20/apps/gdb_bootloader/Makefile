#
# fabooh example makefile
#

#override BOARD = msp430g2553in20
BOARD = msp430g2553in20
PROJECT ?= gdb_bootloader

include ../../../../common.mk

LD_FLAGS += -L ./$(BOARD)/ -T _msp430.x
