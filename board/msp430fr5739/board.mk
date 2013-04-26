#
# board/chip specific options
#

TOOLCHAIN = msp430-
CORE = msp430
CORE_FLAGS = -mmcu=msp430fr5739
C_DEFS = -DFABOOH -DF_CPU=8000000
CXX_DEFS = -DFABOOH -DF_CPU=8000000
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
LD_FLAGS = $(CORE_FLAGS) -mdisable-watchdog -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map,-umain
OPTIMIZATION += -ffunction-sections -fdata-sections
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = rf2500 "prog $(OUT_DIR)/$(PROJECT).hex"
