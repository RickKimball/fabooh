#
# Makefile - top level make for fabooh examples
#
# common.mk is used to select MCU and BOARD variants
#


.PHONY: clean sizes

all:
	@make -C examples/basic all
	@make -C examples/led all
	@make -C examples/lcd all
	@make -C examples/math all
	@make -C examples/serial all
	@make -C examples/spi all
	@make -C examples/adc all
	@make -C apps all

clean:
	@make -C examples/basic clean
	@make -C examples/led clean
	@make -C examples/lcd clean
	@make -C examples/math clean
	@make -C examples/serial clean
	@make -C examples/spi clean
	@make -C examples/adc clean
	@make -C apps clean
