#
# board/chip specific options
#

TOOLCHAIN = arm-none-eabi-
FIXMATH_FLAGS = -DFIXMATH_NO_CACHE -DFIXMATH_NO_ROUNDING -DARM_MATH_CM3
F_CPU ?= 72000000
MCPU = cortex-m3
CORE = stm32f1
CORE_FLAGS = -mcpu=$(MCPU) -mthumb -mfloat-abi=soft
C_STD = c11
CXX_STD = gnu++11
C_DEFS = -DFABOOH -DF_CPU=$(F_CPU)
C_DEFS += -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DSTM32F103xB
C_DEFS += -I$(FBD)$(BOARDDIR)/CMSIS/core
C_DEFS += -I$(FBD)$(BOARDDIR)/CMSIS/device
# flash offset
C_DEFS += -DVECT_TAB_OFFSET=0x08000000
# ram offset
#C_DEFS += -DVECT_TAB_OFFSET=0x20000000
CXX_DEFS = $(C_DEFS) 
AS_DEFS =
LD_SCRIPT = $(FBD)$(BOARDDIR)/stm32f103c8.ld
LD_FLAGS = -T$(LD_SCRIPT) -g -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch
LD_FLAGS += -Wl,--gc-sections -specs=nosys.specs -specs=nano.specs
LIBS ?=
LD_FLAGS += -nostartfiles
LD=$(CXX)

OPTIMIZATION_FLAGS = -mslow-flash-data -fsingle-precision-constant
UPLOAD_VIA ?= stlink

ifeq ($(UPLOAD_VIA),bmp)
 BOOTLOADER = arm-none-eabi-gdb
 BL_COM ?= /dev/ttyACM0
 BL_ARGS = -ex 'target extended-remote $(BL_COM)'
 BL_ARGS += -ex 'mon swdp_scan' -ex 'attach 1' -ex 'load' -batch
 BL_ARGS += $(OUT_DIR)/$(PROJECT).elf
else ifeq ($(UPLOAD_VIA),nucleo)
 BOOTLOADER = openocd
 BL_ARGS = -f interface/stlink-v2-1.cfg
 BL_ARGS += -f target/stm32f1x.cfg
 BL_ARGS += -c "program $(OUT_DIR)/$(PROJECT).elf verify reset exit"
else ifeq ($(UPLOAD_VIA),stlink)
 BOOTLOADER = openocd
 BL_ARGS = -f interface/stlink-v2.cfg
 BL_ARGS += -f target/stm32f1x.cfg
 BL_ARGS += -c "program $(OUT_DIR)/$(PROJECT).elf verify reset exit"
else
 BOOTLOADER = stm32flash
 BL_COM ?= /dev/ttyUSB0
 BL_ARGS = -w $(OUT_DIR)/$(PROJECT).hex $(BL_COM)
endif

