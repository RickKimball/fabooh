readme.txt - compare straight c code (opossum) with fabooh c++ (notopossum) sizes.
             At least with msp430-gcc, it turns out the c++ is smaller

$ msp430-size opossum.elf 
 text    data     bss     dec     hex filename
  616       0       0     616     268 opossum.elf

$ msp430-size notopossum.elf 
 text    data     bss     dec     hex filename
  490       0       0     490     1ea notopossum.elf

