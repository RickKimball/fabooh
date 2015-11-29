/*
 * opossum.c - comparing good straight c code to fabooh
 *
 * See post here:
 * http://forum.43oh.com/topic/1643-g2553-hardware-uart-hello-world-example/?p=31310
 */
#include <msp430.h>
#include <stdint.h>

const unsigned RXD = BIT1;
const unsigned TXD = BIT2;
const unsigned SW2 = BIT3;

const unsigned long smclk_freq = 16000000;      // SMCLK frequency in hertz
const unsigned long bps = 9600;                 // Async serial bit rate


// Output char to UART
static inline void putc(const unsigned c) { while(UCA0STAT & UCBUSY); UCA0TXBUF = c; }

// Output string to UART
void puts(const char *s) { while(*s) putc(*s++); }

// CR LF
void crlf(void) { puts("\r\n"); }

// Output binary array to UART
void putb(const uint8_t *b, unsigned n) { do putc(*b++); while(--n); }

// Print unsigned int
void print_u(unsigned x)
{
    static const unsigned dd[] = { 10000, 1000, 100, 10, 1 };
    const unsigned *dp = dd; unsigned d;
    if(x) { while(x < *dp) ++dp;
        do { d = *dp++; char c = '0'; while(x >= d) ++c, x -= d; putc(c); } while(!(d & 1));
    } else putc('0');
}

// Print signed int
void print_i(const int x) { if(x < 0) putc('-'); print_u((x < 0) ? -x : x); }

// Print hex nibble
static inline void puth(const unsigned x) { putc("0123456789ABCDEF"[x & 15]); }

// Print hex byte
void print_hb(const unsigned x) { puth(x >> 4); puth(x); }

// Print hex word
void print_hw(const unsigned x) { print_hb(x >> 8); print_hb(x); }

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // No watchdog reset
                                                //
    DCOCTL = 0;                                 // Run at 16 MHz
    BCSCTL1 = CALBC1_16MHZ;                     //
    DCOCTL  = CALDCO_16MHZ;                     //
                                                //
    P1DIR = ~(RXD | SW2);                       // Setup I/O for UART and switch
    P1OUT = P1REN  = RXD | SW2;                 //
    P1SEL = P1SEL2 = RXD | TXD;                 //
                                                //
    const unsigned long brd = (smclk_freq + (bps >> 1)) / bps; // Bit rate divisor
    UCA0CTL1 = UCSWRST;                         // Hold USCI in reset to allow configuration
    UCA0CTL0 = 0;                               // No parity, LSB first, 8 bits, one stop bit, UART (async)
    UCA0BR1 = (brd >> 12) & 0xFF;               // High byte of whole divisor
    UCA0BR0 = (brd >> 4) & 0xFF;                // Low byte of whole divisor
    UCA0MCTL = ((brd << 4) & 0xF0) | UCOS16;    // Fractional divisor, oversampling mode
    UCA0CTL1 = UCSSEL_2;                        // Use SMCLK for bit rate generator, release reset

    for(;;) {                                   //
        int n = 1;                              //
        do (P1IN & BIT3) ? --n : (n = 5000); while(n);
        do (P1IN & BIT3) ? n = 5000 : --n; while(n);
        puts("Poke me with a Stick\r\n");       //
        for(n = -5; n <= 5;) print_i(n++), putc(' '); crlf();
        n = 0xDEAD; print_hw(n); putc(' ');     //
        n += 0xE042; print_hw(n); crlf();       //
        crlf();                                 //
    }                                           //
 }
