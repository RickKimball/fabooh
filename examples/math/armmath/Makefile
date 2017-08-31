#
# fabooh example makefile
#

PROJECT ?= armmath_table
LIBS += -larm_cortexM3l_math

include ../../../common_board.mk

ifeq ($(BOARD),bluepill)
 include ../../../common.mk

else

.phony: all clean install

all:
	@echo "all target ignored bluepill only project"

clean:
	@echo "clean target ignored bluepill only project"

install:
	@echo "install target ignored bluepill only project"

endif
