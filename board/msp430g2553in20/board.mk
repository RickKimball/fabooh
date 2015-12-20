#
# board/chip specific options
#

TOOLCHAIN = msp430-
CORE = msp430
CORE_FLAGS = -mmcu=msp430g2553
MCLK_FREQ ?= 16000000UL
C_DEFS = -DF_CPU=$(MCLK_FREQ)
CXX_DEFS = $(C_DEFS) 
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
ifeq ($(TOOLCHAIN),msp430-elf-)
 C_DEFS += -DENERGIA -I/mnt/vbox/shared/ti/ccsv6/ccs_base/msp430/include_gcc
 LD_FLAGS = -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map
 LD_FLAGS += -L/mnt/vbox/shared/ti/ccsv6/ccs_base/msp430/include_gcc
 LD_FLAGS += -minrt
else
 LD_FLAGS = -mdisable-watchdog -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map
endif
OPTIMIZATION += -ffunction-sections -fdata-sections
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = rf2500 "prog $(OUT_DIR)/$(PROJECT).hex"

