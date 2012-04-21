

#include "chip.h"

/* chip pin configuration -- arrays of pin numbers */
/* These arrays are only accessed via Chip, i.e., they are
   building-blocks of the bigger data structure for the chip */
static int high_pindata[] = {16}; /* VCC and any active high enable inputs */
static int low_pindata[] = {8};   /* GND and any active low enable inputs */
static int input_pindata[] = { 1,2,3,4,5,6 };
static int output_pindata[] = { 7,9,10,11,12 };

/* Similarly, softchip is called via Chip's softchip member-- */
static int ls138_softchip(int inpins,int *outpins);

/* The master data structure for an LS00/2 chip--everything the
 main program needs to know about this chip-- */

Chip ls138 = {"LS138","3 to 8 decoder",
	    sizeof(high_pindata)/sizeof(high_pindata[0]),high_pindata,
	    sizeof(low_pindata)/sizeof(low_pindata[0]),low_pindata,
	    sizeof(input_pindata)/sizeof(input_pindata[0]),input_pindata,
	    sizeof(output_pindata)/sizeof(output_pindata[0]),output_pindata,
	    ls138_softchip};

/* truth table:   in pin  A-2 A-1 A-0  out pin  O-3 0-2 O-1 O-0 */
static int TT[] = { 
             0x0e,     /*  0   0   0             1   1   1   0 */
	     0x0d,     /*  0   0   1             1   1   0   1 */
             0x0b,     /*  0   1   0             1   0   1   1 */
             0x07,     /*  0   1   1             0   1   1   1 */
             0x0f,     /*  1   0   0             1   1   1   1 */
             0x0f,     /*  1   0   1             1   1   1   1 */
             0x0f,     /*  1   1   0             1   1   1   1 */
             0x0f      /*  1   1   1             1   1   1   1 */
};

/* use old 2-gates TT for low 4 bits of input, then high 4 bits */
/* This is pretty ugly! */
#define LOW_GATES_INPUT_MASK 0x07
#define LLH 0x20
/* could use code instead of TT for this-- */
/* compute outputs for given inputs in bits of int input-- */

static int ls138_softchip( int input, int *output ) {
    int low_gates_expected_output = TT[input & LOW_GATES_INPUT_MASK];
    
    if((input & 0x38) != LLH)
      *output = 0x0f;
    else  
    {
      low_gates_expected_output = TT[input & LOW_GATES_INPUT_MASK]; 
      *output = low_gates_expected_output;
    }
    return 1;
}
