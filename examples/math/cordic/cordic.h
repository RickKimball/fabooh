//******************************************************************************
//
//   Description: Header file for CORDIC sine/cosine function, see .c file for
//                more detailed description.
//
//   T. Brower
//   Version    1.00
//   Feb 2011
//     IAR Embedded Workbench Kickstart (Version: 5.20.1)
//******************************************************************************


#ifndef CORDIC_H
#define CORDIC_H

// Function to find the sine and cosine of an angle
// using CORDIC
void cordic_sincos(int,
                   char,
                   int *,
                   int *);

#endif /* CORDIC_H */
