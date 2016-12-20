# fabooh

fabooh is an optimized C++ template based peripheral framework.  The current implementation focus is on the STMicroelectronics STM32F1 cortex-m3 bluepill, the NXP cortex-m0 lpc1114fn28 and TI’s smaller msp430 microcontrollers.  Firmware written using fabooh creates very small and efficient code while still providing a flexible framework similar to what you might expect to find with the Arduino API.  The template code makes liberal use of inline functions and, when appropriate, gcc assembler to produce size optimized binaries that can sometimes be smaller than generic ‘C’ code.

# hello world (fabooh style)

```
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

# etymology

fabooh – [\’fab-‘ü\ fab](http://fabooh.com/wp-content/uploads/2013/04/fabooh.mp3?_=1)
*fab* (construct, manufacture; specifically : to construct from diverse and usually standardized parts)
*+*
*ooh* (to exclaim in amazement)

# blog 
[fabooh blog](http://fabooh.com)

