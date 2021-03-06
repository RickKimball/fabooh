#
# board/chip specific options
#

TOOLCHAIN = msp430-
CORE = msp430
CORE_FLAGS = -mmcu=msp430g2553
MCLK_FREQ ?= 16000000UL
C_DEFS = -DF_CPU=$(MCLK_FREQ)
CXX_DEFS = $(C_DEFS) 
CXX_STD=gnu++0x
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
ifeq ($(TOOLCHAIN),msp430-elf-)
 C_DEFS += -DENERGIA -I$(HOME)/ti64/ccsv6/ccs_base/msp430/include_gcc
 LD_FLAGS = -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map
 LD_FLAGS += -L$(HOME)/ti64/ccsv6/ccs_base/msp430/include_gcc
 LD_FLAGS += -minrt
else
 LD_FLAGS = -mdisable-watchdog -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map
endif
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = rf2500 "prog $(OUT_DIR)/$(PROJECT).hex"

