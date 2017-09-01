#
# fabooh example makefile
#

PROJECT ?= armmath_table
LIBS += -larm_cortexM3l_math

include ../../../common_board.mk

ifeq ($(BOARD),bluepill)
 include ../../../common.mk
else
 include ../../../common_empty.mk
endif
