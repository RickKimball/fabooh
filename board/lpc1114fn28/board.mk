#
# board/chip specific options
#

TOOLCHAIN = arm-none-eabi-
CORE = cortex-m0
CORE_FLAGS = -mcpu=$(CORE) -mthumb
C_DEFS = -DCORE_M0
CXX_DEFS = -DCORE_M0
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/$(BOARD).ld
LD_FLAGS = -T$(LD_SCRIPT) -g -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch
LD_FLAGS += -Wl,--gc-sections
OPTIMIZATION += -ffunction-sections -fdata-sections
LD_FLAGS += -nostartfiles
LD=$(CXX)

BOOTLOADER = lpc21isp
BL_ARGS = -wipe -control -postreset $(OUT_DIR)/$(PROJECT).hex /dev/ttyUSB0 115200 12000

