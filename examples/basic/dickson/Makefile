#
# fabooh example makefile
#

PROJECT?=dickson

include ../../../common_board.mk

ifneq ($(filter $(BOARD),$(MSP430G_BOARDS)),)
include ../../../common.mk
FABOOH_FLAGS += -DSMALL_INIT4
else
include ../../../common_empty.mk
endif
