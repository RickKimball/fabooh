#include <msp430.h>
#include <stdint.h>

typedef uint16_t *u16_ptr;
typedef u16_ptr *u16_handle;

class tweak_clk {
  unsigned plus_;
public:
  tweak_clk(unsigned plus) : plus_(plus) {
    DCOCTL += plus_;
  }

  ~tweak_clk(void) {
    DCOCTL -= plus_;
  }
};

static void inline send_pixel( u16_ptr pixel_pulse_bits  ) {
  //__delay_cycles(28);
  while(!(USICTL1 & USIIFG));
  USISR = *pixel_pulse_bits;
  //USICNT = USI16B | 10;
  USICNT = 8;
}

static inline void sendRGB(unsigned r, unsigned g, unsigned b) {
  unsigned bit_value[8*3];
  register uint16_t *p = &bit_value[0];
  register int x;

  {
    tweak_clk clk_plus(0x0);
    x = 7;
    do {
      *(p+16) = (b & (1<<x)) ? 0b1111111 : 0b00000011;
      *(p+8 ) = (g & (1<<x)) ? 0b1111111 : 0b00000011;
      *(p++ ) = (r & (1<<x)) ? 0b1111111 : 0b00000011;
    } while(x--);
    
    p = &bit_value[0];
  }

  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);

  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);

  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);
  send_pixel(p++);

}

int main( void )
{
  WDTCTL = WDTPW | WDTHOLD;
  
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;
  
  USICTL0  = USISWRST;                   // put in reset mode to make changes
  USICTL0 |= USIPE5 | USIPE6 | USIMST | USIOE | USILSB;
  USICTL1 = 0;
  USICKCTL = USISSEL_3 | USIDIV_2;      // SMCLK/4 (0b10 << 2) | (0b10 << 5)
  USICTL0 &= ~USISWRST;                 // enable it no
  
  while(1) {
    sendRGB(0xFF,0x00,0x00);
    sendRGB(0x00,0xFF,0x00);
    sendRGB(0x00,0x00,0xFF);
    sendRGB(0x00,0x00,0x00);
    sendRGB(0xFF,0xFF,0xFF);
    __delay_cycles(1600000);
  }
	    
} 
