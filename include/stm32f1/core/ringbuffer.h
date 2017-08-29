/*
   ringbuffer.h - yet another version of ringbuffer_t c++ template

   Desc: This template class creates a ringbuffer with arbitrary types. Provides push
         and pop methods for adding and removing items.  Access function for checking
         the number of items in the buffer, capacity and full or empty methods
         provide safe access to the info.

         This version has been optimized for the msp430-gcc and stm32. It doesn't disable
         or enable any interrupts. It is safe nonetheless for use when there is a single
         writer and single reader. This is common when using an interrupt and the main
         line thread working together to move data in and out of peripherals on demand.

   Version: 1.0.3
   Created: Jul-24-2012
    Author: rick@kimballsoftware.com
      Date: 02-28-2013

   =========================================================================
    Copyright © 2012-2016 Rick Kimball

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Jul-14-2016: rick@kimballsoftware.com removed extra stuff not needed for stm32
                 formatted using auto format in arduino ide

*/

#ifndef RINGBUFFER_T_H_
#define RINGBUFFER_T_H_
#include <stdint.h>

/**
   struct_is_power_of_two - power of 2 checker
     enforce at compile time that SIZE is power of 2 and >= 2
*/

template<unsigned N>
struct is_power_of_two {
  enum {val = (N >= 2) & !(N & (N - 1))};
  static const unsigned badSIZE[(val == 1) ? 1 : -1]; // N is not a power of 2 if you an error here.
};

/**
   uint16x2_t - a union containing 16 bit head and tail offsets into the ring buffer. The union
                allows the c code to grab both values with one assembler instruction access.

*/
union uint16x2_t {
  // access as 32 bit
  uint32_t both;
  // -- or as 2 16 bit values --
  struct {
    uint16_t head;
    uint16_t tail;
  };
};

/**
   ringbuffer_t - provide a circular_buffer without disabling interrupts
      expects a power of 2 container, and only one reader and one writer
      container capacity SIZE-1
*/

template <
  uint16_t SIZE,                  /* how many elements-1 must be power of 2 */
  typename T = uint8_t,           /* works with any type */
  typename POP_T = int,           /* return type of pop_front */
  POP_T EMPTY_ELEM = (POP_T)-1    /* default return value when empty */
  >
struct ringbuffer_t {
  // --- fake private data ---

  volatile uint16x2_t offsets;  // comes first so we can use 0 offset to variables
                                // for fastest access
  T elements[SIZE];

  enum { CAPACITY = SIZE - 1 }; // leave one slot open

  is_power_of_two<SIZE> check_buffer_size; // your SIZE is not a power of 2, if you get an error here

  // --- public methods ---

  // zeros out head and tail
  inline void clear(void) {
    offsets.both = 0;
  }

  // return the count of used slots
  size_t inline available() {
    register uint16x2_t temp = { offsets.both };

    temp.both = (temp.head - temp.tail) & CAPACITY;

    return temp.both;
  }

  // return maximum number of slots available
  size_t inline capacity() {
    return CAPACITY;
  }

  // returns true when there is no used slots
  bool inline isEmpty() {
    return !available();
  }

  // returns true when there is no used slots
  bool inline empty() {
    return isEmpty();
  }

  // returns true when all slots used
  bool inline isFull() {
    return available() == capacity();
  }

  // returns true when all slots used
  bool inline full() {
    return isFull();
  }

  /*
   * push_back() - adds an element to the end of the queue
   *
   * Note: affects head, reads tail, element ignored if overflow ~300 ns @72MHz
   */
  void inline push_back(const T element) {
    register uint16x2_t temp = { offsets.both };
    register uint16_t temp_head = temp.head;

    // if ( !isFull() ) {
    temp.both = (temp.head - temp.tail) & CAPACITY;
    if ( !(temp.both == capacity()) ) {
      elements[temp_head++] = element;
      offsets.head = temp_head & CAPACITY;
    }

    return;
  }

  // element inserter with bounds checking
  void inline push_back_nc(const T element) {
    register uint16_t temp_head = offsets.head;

    elements[temp_head++] = element;
    offsets.head = temp_head & CAPACITY;
    return;
  }

  // affects tail, reads head
  POP_T inline pop_front(void) {
    register uint16x2_t temp = { offsets.both };

    if ( (temp.head - temp.tail) & CAPACITY ) { // !empty
      POP_T elem = elements[temp.tail++];
      offsets.tail = temp.tail & CAPACITY;
      return elem;
    }

    return EMPTY_ELEM; // on underflow return default element
  }

  // no bounds check extractor, affects tail
  POP_T inline pop_front_nc(void) {
    register uint16_t temp_tail = offsets.tail;

    POP_T elem = elements[temp_tail++];
    offsets.tail = temp_tail & CAPACITY;
    return elem;
  }

  // no bounds check element inserter
  void put(const T element) {
    push_back_nc(element);
  }

  // no bounds check extractor, affects tail
  POP_T get(void) {
    return pop_front_nc();
  }
};

#endif /* RINGBUFFER_T_H_ */
