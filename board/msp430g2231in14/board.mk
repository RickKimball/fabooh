#
# board/chip specific options
#

MCLK_FREQ ?= 16000000UL
TOOLCHAIN = msp430-
CORE = msp430
CORE_FLAGS = -mmcu=msp430g2231
C_DEFS = -DFABOOH -DF_CPU=$(MCLK_FREQ)
CXX_DEFS = -DFABOOH -DF_CPU=$(MCLK_FREQ)
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
LD_FLAGS = $(CORE_FLAGS) -mdisable-watchdog -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map,-umain
OPTIMIZATION += -ffunction-sections -fdata-sections
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = rf2500 "prog $(OUT_DIR)/$(PROJECT).hex"
