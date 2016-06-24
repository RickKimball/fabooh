all:
	make -C ascii_table all
	make -C ascii_table_iostream all
	make -C echo all
ifneq ($(BOARD),lpc1114fn28)
	make -C feed_the_dog all
endif
ifeq ($(BOARD),msp430g2553in20)
	make -C compare_c_cpp all
	make -C gdb_bootloader all
endif

clean:
	make -C ascii_table clean
	make -C ascii_table_iostream clean
	make -C echo clean
ifneq ($(BOARD),lpc1114fn28)
	make -C feed_the_dog clean
endif
ifeq ($(BOARD),msp430g2553in20)
	make -C compare_c_cpp clean
	make -C gdb_bootloader clean
endif
