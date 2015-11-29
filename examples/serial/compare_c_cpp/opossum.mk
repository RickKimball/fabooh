#
# fabooh blink makefile
#

TARGET = opossum
CC=msp430-gcc
OUT_DIR_F := msp430g2553in20_release
ELF       :=$(OUT_DIR_F)/$(TARGET).elf

.phony: all clean

all: make_output_dir $(ELF) print_size

$(OUT_DIR_F)/opossum.elf: opossum.c_src
	msp430-gcc -mmcu=msp430g2553 -mdisable-watchdog -Os -x c opossum.c_src -o $@
	msp430-size $@

clean:
	rm -f msp430g2553in20_release/opossum.elf

make_output_dir :
	$(shell mkdir $(OUT_DIR_F) 2>/dev/null)

print_size :
	msp430-size $(ELF)
