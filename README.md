# fabooh

fabooh is an optimized C++ template based peripheral framework.  The current implementation focus is on the STMicroelectronics STM32F1 cortex-m3 bluepill, the NXP cortex-m0 lpc1114fn28 and TI’s smaller msp430 microcontrollers.  Firmware written using fabooh creates very small and efficient code while still providing a flexible framework similar to what you might expect to find with the Arduino API.  The template code makes liberal use of inline functions and, when appropriate, gcc assembler to produce size optimized binaries that can sometimes be smaller than generic ‘C’ code.

# hello world (fabooh style)

```C++
#include <fabooh.h>
#include <main.h>

serial_default_t<9600, CPU::frequency, TX_PIN, NO_PIN> Serial; // xmit only serial

void setup(void) {
  Serial.begin();
}

void loop() {
  Serial.print("Hello world!\n");
  while(1);
}
```

```
**compiled binary sizes for selected targets:**
arm-none-eabi-size bluepill_release/hello_world.elf
   text	   data	    bss	    dec	    hex	filename
    892	      4	      4	    900	    384	bluepill_release/hello_world.elf

arm-none-eabi-size lpc1114fn28_release/hello_world.elf
   text	   data	    bss	    dec	    hex	filename
    812	      0	      8	    820	    334	lpc1114fn28_release/hello_world.elf

msp430-size msp430g2553in20_release/hello_world.elf
   text	   data	    bss	    dec	    hex	filename
    146	      0	      0	    146	     92	msp430g2553in20_release/hello_world.elf
```

# etymology

fabooh – [\’fab-‘ü](http://fabooh.com/wp-content/uploads/2013/04/fabooh.mp3?_=1)
*fab* (construct, manufacture; specifically : to construct from diverse and usually standardized parts)
*+*
*ooh* (to exclaim in amazement)

# blog 
[fabooh blog](http://fabooh.com)
