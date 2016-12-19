#
# board/chip specific options
#

TOOLCHAIN = arm-none-eabi-
FIXMATH_FLAGS = -DFIXMATH_NO_CACHE -DFIXMATH_NO_ROUNDING
F_CPU ?= 48000000
CORE = cortex-m0
CORE_FLAGS = -mcpu=$(CORE) -mthumb
C_DEFS = -DCORE_M0 -DFABOOH -DF_CPU=$(F_CPU)
CXX_DEFS = -DCORE_M0 -DFABOOH -DF_CPU=$(F_CPU)
CXX_STD = gnu++14
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/$(BOARD).ld
LD_FLAGS = -T$(LD_SCRIPT) -g -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch
LD_FLAGS += -Wl,--gc-sections --specs=nano.specs -specs=nosys.specs
LD_FLAGS += -nostartfiles 
LD=$(CXX)

#BOOTLOADER = lpc21isp
#BL_ARGS = -wipe -control -postreset $(OUT_DIR)/$(PROJECT).hex /dev/ttyUSB0 115200 12000

BOOTLOADER = arm-none-eabi-gdb
BL_ARGS = -batch -ex 'target extended-remote /dev/ttyACM0' -ex 'mon swdp_scan' -ex 'attach 1' -ex 'load' $(OUT_DIR)/$(PROJECT).elf

