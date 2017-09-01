#
# fabooh example makefile
#

PROJECT?=dickson

include ../../../common_board.mk

ifeq ($(BOARD),msp430g2553in20)
include ../../../common.mk
FABOOH_FLAGS += -DSMALL_INIT4
else
include ../../../common_empty.mk
endif
