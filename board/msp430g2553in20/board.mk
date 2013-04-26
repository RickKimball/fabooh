#
# board/chip specific options
#

TOOLCHAIN = msp430-
CORE = msp430
CORE_FLAGS = -mmcu=msp430g2553
C_DEFS = -DFABOOH -DF_CPU=16000000
CXX_DEFS = -DFABOOH -DF_CPU=16000000
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
LD_FLAGS = -mmcu=$(CORE)g2553 -mdisable-watchdog -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map,-umain
OPTIMIZATION += -ffunction-sections -fdata-sections
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = rf2500 "prog $(OUT_DIR)/$(PROJECT).hex"

