#
# board/chip specific options
#

TOOLCHAIN ?= msp430-elf-
CORE = msp430
CORE_FLAGS = -mmcu=msp430fr5969
MCLK_FREQ ?= 8000000UL
MODE_FLAGS ?= -mlarge -mhwmult=f5series -mcode-region=lower -mdata-region=lower

AS_DEFS =
C_DEFS = -DFABOOH -DF_CPU=$(MCLK_FREQ)
C_DEFS += -DENERGIA -I$(HOME)/ti_7/ccsv7/ccs_base/msp430/include_gcc
C_DEFS += $(MODE_FLAGS)
CXX_DEFS = $(C_DEFS) -fno-threadsafe-statics
CXX_STD=gnu++11

LD_SCRIPT = $(FBD)$(BOARDDIR)/dummy.ld
LD_FLAGS = -Wl,--gc-sections,-Map=$(OUT_DIR_F)$(PROJECT).map
LD_FLAGS += -L$(HOME)/ti64/ccsv6/ccs_base/msp430/include_gcc
LD_FLAGS += -minrt
LD_FLAGS += $(MODE_FLAGS)
LD=$(CC)

BOOTLOADER = mspdebug
BL_ARGS = tilib "prog $(OUT_DIR)/$(PROJECT).hex"

