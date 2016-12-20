#
# fabooh example makefile
#

PROJECT?=hello_world
include ../../../common.mk

ifeq ($(TOOLCHAIN),msp430-)
    FABOOH_FLAGS += -DSMALL_INIT4
endif
