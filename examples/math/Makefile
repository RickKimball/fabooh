all:
	make -C sin_table all
	make -C cordic all
	make -C fixmath all
ifeq ($(BOARD),bluepill)
	make -C armmath all
endif

clean:
	make -C sin_table clean
	make -C cordic clean
	make -C fixmath clean
	make -C armmath clean

install: all
	make -C sin_table install
	make -C cordic install
	make -C fixmath install
ifeq ($(BOARD),bluepill)
	make -C armmath install
endif

