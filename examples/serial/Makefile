all:
	make -C ascii_table all
	make -C ascii_table_iostream all
	make -C echo all
	make -C helloworld all
ifeq ($(BOARD),msp430g2553in20)
	make -C feed_the_dog all
	make -C compare_c_cpp all
	make -C gdb_bootloader all
endif

clean:
	make -C ascii_table clean
	make -C ascii_table_iostream clean
	make -C echo clean
	make -C helloworld clean
ifeq ($(BOARD),msp430g2553in20)
	make -C feed_the_dog clean
	make -C compare_c_cpp clean
	make -C gdb_bootloader clean
endif
