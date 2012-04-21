/******************************************************************
*
*   file:   ls00.chip
*   author: Ethan Bolker
*   date:   October, 1991
*
*   Data file specifying (part of) an LS00 quad nand gate chip.
*   By limiting the testing to two of the four gates I need only
*   a 16 line rather than a 256 line truth table! But I could 
*   probably have gotten all the logic in fewer than 16 lines by
*   programming the nand logic in c.
*
*   Link with chiptest.o/opc to produce executable testing code.
*/

#include "chip.h"

/* chip pin configuration -- arrays of pin numbers */
/* These arrays are only accessed via Chip, i.e., they are
   building-blocks of the bigger data structure for the chip */
static int high_pindata[] = {14}; /* VCC and any active high enable inputs */
static int low_pindata[] = {7};   /* GND and any active low enable inputs */
static int input_pindata[] = { 1, 2, 4, 5, 9, 10, 12, 13 };
static int output_pindata[] = { 3, 6, 8, 11 };

/* Similarly, softchip is called via Chip's softchip member-- */
static int ls00_softchip(int inpins,int *outpins);

/* The master data structure for an LS00/2 chip--everything the
 main program needs to know about this chip-- */

Chip ls00 = {"LS00","quad NAND gate",
	    sizeof(high_pindata)/sizeof(high_pindata[0]),high_pindata,
	    sizeof(low_pindata)/sizeof(low_pindata[0]),low_pindata,
	    sizeof(input_pindata)/sizeof(input_pindata[0]),input_pindata,
	    sizeof(output_pindata)/sizeof(output_pindata[0]),output_pindata,
	    ls00_softchip};

/* truth table:   in pin  5 4 2 1  out pin  6 3 */
static int TT[] = { 
             0x03,     /* 0 0 0 0           1 1 */
             0x03,     /* 0 0 0 1           1 1 */
             0x03,     /* 0 0 1 0           1 1 */
             0x02,     /* 0 0 1 1           1 0 */
             0x03,     /* 0 1 0 0           1 1 */
             0x03,     /* 0 1 0 1           1 1 */
             0x03,     /* 0 1 1 0           1 1 */
             0x02,     /* 0 1 1 1           1 0 */
             0x03,     /* 1 0 0 0           1 1 */
             0x03,     /* 1 0 0 1           1 1 */
             0x03,     /* 1 0 1 0           1 1 */
             0x02,     /* 1 0 1 1           1 0 */
             0x01,     /* 1 1 0 0           0 1 */
             0x01,     /* 1 1 0 1           0 1 */
             0x01,     /* 1 1 1 0           0 1 */
             0x00,     /* 1 1 1 1           0 0 */
};

/* use old 2-gates TT for low 4 bits of input, then high 4 bits */
/* This is pretty ugly! */
#define LOW_GATES_INPUT_MASK 0x0f
#define HIGH_GATES_INPUT_MASK 0xf0

/* could use code instead of TT for this-- */
/* compute outputs for given inputs in bits of int input-- */

static int ls00_softchip( int input, int *output ) {
    int low_gates_expected_output = TT[input & LOW_GATES_INPUT_MASK];
    /* do same for high 2 gates, shifted over by 4 bits on input, 
       shifted back 2 on output, next to the 2 bits from first 2 gates */
    int high_gates_expected_output = 
      (TT[(input & HIGH_GATES_INPUT_MASK)>>4])<<2;
    *output = high_gates_expected_output|low_gates_expected_output;
    return 1;
}
