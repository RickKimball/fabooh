all:
	make -C ascii_table all
	make -C ascii_table_iostream all
	make -C echo all
ifeq ($(BOARD),msp430g2553in20)
	make -C gdb_bootloader all
endif

clean:
	make -C ascii_table clean
	make -C ascii_table_iostream clean
	make -C echo clean
ifeq ($(BOARD),msp430g2553in20)
	make -C gdb_bootloader clean
endif
