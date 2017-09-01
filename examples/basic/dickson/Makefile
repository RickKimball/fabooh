#
# fabooh example makefile
#

PROJECT?=dickson

include ../../../common_board.mk

ifneq ($(filter $(BOARD),msp430g2553in20 msp430g2452in20 msp43g2231in14),)
include ../../../common.mk
FABOOH_FLAGS += -DSMALL_INIT4
else
include ../../../common_empty.mk
endif
