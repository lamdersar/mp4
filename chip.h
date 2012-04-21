/******************************************************************
*
*   file:   chip.h
*   author: Betty O'Neil
*   date:   April, 1992
*
* Header file for chip tester program--data structure for interface
*   between chip-specific code and mainline code 
*
*/

/* type for pointer to function that returns int, takes int and int ptr-- */
typedef int (*PFI_I_IPTR)(int, int *);

typedef struct {
  char *chipname;		/* "LS00" or whatever */
  char *chipdesc;		/* description string */
  int n_highpins;		/* no. of logic-1 pins */
  int *highpins;		/* array of logic-1 pin nos. */
  int n_lowpins;		/* no. of logic-0 pins */
  int *lowpins;			/* array of logic-0 pin nos. */
  int n_inpins;			/* no. of input pins */
  int *inpins;			/* array of input pin nos. */
  int n_outpins;		/* no. of output pins */
  int *outpins;			/* array of output pin nos. */
  PFI_I_IPTR softchip;		/* does chip logic in software */
} Chip;
