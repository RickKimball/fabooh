/*
 *  print.h - all dancing, all singing print_t class template
 *
 *  CRTP (Curiously Recurring Template Pattern ) style print class template
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 04-16-2013
 * Version: 1.0.4
 *
 */

#ifndef PRINT_T_H_
#define PRINT_T_H_

enum _endl_enum { endl };

/*
 * print_t - full function print class template
 *
 * Desc: The print_t template provide a common binary to ASCII print implementation.
 *       Think of it as itoa() with templates.  The user is expected to provide
 *       a Write class that exposes a function called write_impl().
 */

template<typename Writer>
class print_t {

  //------------------------------------------------------------
  // private
  //------------------------------------------------------------
private:
  //--------------------------------------------------------------------------------
  // these _functions() are private internal functions not fit for human consumption

  void _puts(const unsigned char *s) {
    while (*s) {
      write(*s++);
    }
  }

  void _puts(const char *s) {
    _puts(reinterpret_cast<const unsigned char *>(s));
  }

  //------------------------------------------------------------
  // public
  //------------------------------------------------------------
public:

  /*
   * _print_base() - binary to ASCII conversion using arbitrary base
   *
   * _print_base() is implemented as a template to allow for the
   * smallest possible code. User code that only prints 16 bit values
   * can produce the smallest code with the a settings of
   * T=uint16_t.  Code that prints a mix of 16 bit and 32 bit values
   * will produce smaller code if uses the a single _print_base()
   * implementation. In that case, T=uint32_t. When used in this
   * fashion, all conversions are done by one function, _print_base<uint32_t>()
   * so that all print results are completed using 32 bit integers.
   * This results in smaller final code as there will only be one
   * set of _udiv[xx]3/_umod[xx]3 functions included from libc.a
   *
   * Note: Other implementations of binary to base X conversion routines
   *       have gone to great lengths to avoid using division. However,
   *       in this case it seems this is a reasonable approach with
   *       using msp430-gcc's efficient implementation of _udiv3 and _umod3.
   *
   * Con:  we use the stack to assemble the print string, it might use
   *       up to 64+ bytes of stack depending on the T used.
   *
   * Alternate approaches tried:
   *
   * o itoa() - 126 bytes larger, 2x stack usage, doesn't deal with 16/32 bit
   *
   * o looping with no division, ok for decimal but not for all base routine
   *   coverage, tables takes up more room than division/modulo. Also, this
   *   method can't take advantage of compiler's power of 2 bases optimizations
   *   that use 'and' and 'shift' operators to implement div and modulo.
   *
   * o div_t - 64 bytes larger, doesn't optimize for 16/32 bit
   *
   * Typical conversion times:
   *   TODO: measure cortex-m3 results
   */

  template<typename T=uint32_t, const int zerofillwidth=0, const int ascii_a='a' >
  void _print_base(T n, const base_e base) {
    static const int max_width = 8 * sizeof(T);
    char buf[max_width + 1];     // (8 * N bytes) 8 because base 2 is the worst case '11111111'
                                 // add 1 for null
    char *str = &buf[max_width]; // start at last byte and null terminate the string
    *str = '\0';

    // work from least significant digit of n to most
    do {
      int digit = n % base;
      digit = digit < 10 ? digit + '0' : digit + ascii_a - 10;
      *(--str) = digit;
    } while( (n /= base) );

    if (zerofillwidth > 0) {
      char * const min_str_ptr = (max_width - zerofillwidth < 0) ? buf : &buf[max_width - zerofillwidth];
      while(str > min_str_ptr ) {
        *(--str) = '0';
      }
    }

    _puts(str);
  }


  void print(char c) {
    write(c);
  }

  void print(const char s[]) {
    _puts(s);
  }

  void print(unsigned char b, const base_e base=DEC) {
    print((unsigned long) b, base);
  }

  void print(int n, const base_e base=DEC) {
    print((long) n, base);
  }

  void print(unsigned int n, const base_e base=DEC) {
    print((unsigned long) n, base);
  }

  void print(unsigned long n, const base_e base=DEC) {
    print((long) n, base);
  }

  void print(long n, const base_e base=DEC) {
    if ( base == 0) {
      write(n);
    }
    else if (base == 10 || base == 1) {
      if ( n < 0 ) {
        write('-');
        n = -n;
      }
      _print_base(n, DEC);
    }
    else {
      _print_base(n,base);
    }
  }

  //----------------- specials --------------------------
  //

  void print(_endl_enum) {
    println();
  }

  void println(void) {
    print("\r\n");
  }

#if 0
  /*
   * print() - fix16_t fixed point values
   */
  void print(const fix16_t value, const unsigned digits) {
    _print_fix16(value, digits > 5 ? 5 : digits);
  }
#endif

  /*
   * print(float) - probably should be avoided if you care about small size code
   *
   */
  void print(float f, unsigned decimal_places) {
    return _print_float(f, decimal_places);
  }

#if 0
  /*
   * printf() - you probably don't want to use this either if you care about code size
   *
   * minimal printf() implementation based on implementation from Kevin Timmermans's
   * tiny printf see: http://forum.43oh.com/topic/1289-tiny-printf-c-version/
   */

  void printf(const char *format, ...) {
    long n;
    int i;
    char c;
    va_list a;

    va_start(a, format);

    while ((c = *format++)) {
      if (c == '%') {
        switch (c = *format++) {
          case 's': // String
            print(va_arg(a, char*));
            break;

          case 'c': // Char
            write((uint8_t)va_arg(a, int));
            break;

          case 'i': // 32 bit Integer
          case 'u': // 32 bit Unsigned
            i = va_arg(a, int);
            if(c == 'i' && i < 0) i = -i, write('-');
            print((uint32_t)i);
            break;

#if 0
          case 'l': // 64 bit Long
          case 'n': // 64 bit uNsigned loNg
            n = va_arg(a, long);
            if(c == 'l' && n < 0) n = -n, write('-');
            print((uint64_t)n);
            break;
#endif

          case 'x': // 32 bit heXadecimal
            i = va_arg(a, unsigned);
            print((uint32_t)i,HEX);
            break;

          case 0:
            return;

  #if 0 // you could enable these if you have lots of flash space
          case 'b':
            print((uint16_t)va_arg(a, uint16_t),BIN);
            break;

          case 'o': // 16 bit Octal
            print((uint16_t)va_arg(a, uint16_t),OCT);
            break;

          case 'O': // 32 bit Octal
            print((uint32_t)va_arg(a, uint32_t),OCT);
            break;

          case 'X':// 32 bit heXadecimal
            n = va_arg(a, long);
            print((unsigned long)n,HEX);
            break;
  #endif

          default:
            goto bad_fmt;
        }
      }
      else {
   bad_fmt:
        write(c);
      }
    }
    va_end(a);
  }
#endif

  /*
   * write - write 1 byte to output, uses Writer's implementation
   *
   * Note: this code expects a public write_impl function in the Writer class
   */

  int write(uint8_t c) {
    return static_cast< Writer* >(this)->write_impl(c);
  }

  /* TODO: explain why I avoided println(n) functions to discourage usage, as they causes bloat */

private:

#if 0
  /*
   * _print_fix16() - similar to fix16_to_str routines
   *
   *  optimized to use write routines so less stack is used
   */
  void _print_fix16(const fix16_t value, const unsigned digits) {
    /* 5 decimals is enough for full fix16_t precision */
    static const uint32_t scales[8] = { 1, 10, 100, 1000, 10000, 100000, 100000, 100000 };

    uint32_t uvalue = (value >= 0) ? value : -value;
    /* print integer results */
    if (value < 0) {
      write((uint8_t) '-');
    }

    /* Separate the integer and decimal parts of the value */
    unsigned int_part = uvalue >> 16;
    uint32_t remainder = uvalue & 0xFFFF;
    uint32_t scale = scales[digits & 7];
    remainder = fix16_mul(remainder, scale);

    if (remainder >= scale) {
      /* Handle carry from decimal part */
      int_part++;
      remainder -= scale;
    }

    print(int_part);

    /* print decimal results with zero fill */
    if (digits > 0) {
      write((uint8_t)'.');
      bool skip = false;

      while (scale/=10) {
        unsigned digit = (remainder / scale);

        if (!skip || digit || scale == 1) {
          skip = false;
          write((uint8_t)('0' + digit));
          remainder %= scale;
        }
      }
    }
  }
#endif

  /*
   * _printFloat() - .. hmmm do you really want to use this?
   *
   * hint: .. probably not!
   */


  void _print_float(float number, unsigned digits) {
    /* constants */
    const float divf_10=1.0f / 10.0f;
    const float mulf_10=10.f;

    // Handle negative numbers
     if (number < 0.0f) {
        write('-');
        number = -number;
     }

     // Round correctly so that print(1.999, 2) prints as "2.00"
     float rounding = 0.5f;

     for (unsigned i=0; i < digits; ++i) {
       rounding *= divf_10; // rounding /= 10.f
     }
     number += rounding;

     // Extract the integer part of the number and print it
     unsigned int_part = (unsigned)number;
     print(int_part);

     // Print the decimal point, but only if there are digits beyond
     if (digits > 0) {
       write('.');
     }

     // Extract digits from the remainder one at a time
     float remainder = number - int_part;

     while (digits-- > 0) {
       remainder *= mulf_10;
       unsigned digit = unsigned(remainder);
       print(digit);
       remainder = remainder-digit;
     }
   }

};

/*
 * print_t insertion operator support
 *
 * similar and mostly based on ideas in:
 *  http://arduiniana.org/libraries/streaming/
 */

// Serial << 1234; // print ascii decimal version of 1234

template<typename PRINT_T, typename T>
inline PRINT_T &operator << (PRINT_T &obj, const T arg) {
  obj.print(arg); return obj;
}

//   Serial << _HEX(1234); // print ascii hex version of decimal 1234

template<typename T>
struct _BASE {
  const T n;
  const base_e base;

  _BASE(const T _n, const base_e _base): n(_n), base(_base) {}
};

#define _RAW(n) _BASE<typeof((n))>(n,RAW)
#define _BIN(n) _BASE<typeof((n))>(n,BIN)
#define _OCT(n) _BASE<typeof((n))>(n,OCT)
#define _DEC(n) _BASE<typeof((n))>(n,DEC)
#define _HEX(n) _BASE<typeof((n))>(n,HEX)

template<typename PRINT_T, typename T>
inline PRINT_T &operator << (PRINT_T &obj, const _BASE<T> &arg) {
  obj.print(arg.n, arg.base); return obj;
}

//   Serial << _FLOAT(gps_latitude, 6); // 6 digits of precision

template<typename T>
struct _FBASE
{
  const T val;
  const unsigned digits;

  _FBASE(const T v, const unsigned d): val(v), digits(d) {}
};

#define _FLOAT(f,d) _FBASE<float>((f),(d))

template<typename PRINT_T, typename T>
inline PRINT_T &operator <<(PRINT_T &obj, const _FBASE<T> &arg) {
  obj.print(arg.val, arg.digits); return obj;
}

//   Serial << _FIX16(temp_celsius, 2); // 2 digits of precision

template<typename T>
struct _FIX16BASE
{
  const T val;
  const unsigned digits;

  _FIX16BASE(const T v, const unsigned d): val(v), digits(d) {}
};

#define _FIX16(f16,d) _FIX16BASE<fix16_t>((f16),(d))

template<typename PRINT_T, typename T>
inline PRINT_T &operator <<(PRINT_T &obj, const _FIX16BASE<T> &arg) {
  obj.print(arg.val, arg.digits); return obj;
}

#endif /* PRINT_T_H_ */
