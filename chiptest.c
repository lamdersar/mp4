/******************************************************************
*
*   file:   chiptest.c
*   author: Ethan Bolker
*   date:   October, 1991
*   edited by Betty O'Neil, Apr, 1992, Apr, 1996, Nov, 1996
*
*   Chip testing program stub
*
*   Link with file containing the following global data structures 
*   describing the particular chip to be tested
*   b
*/
#include <stdio.h>

#include "chip.h"

#ifdef SAPC
#include <lp.h>
#include <cpu.h>
#endif

#define MAXBUF 200
/* final program should have INTERACTIVE defined, but
* you may want to comment this out while debugging
* to get rid of the user input.
*/
#define INTERACTIVE
/* non-interactive values-- */
#define DEFAULT_CHIPNUM 0
#define LP1_BASE 0x378
/* These arrays specify the logical order of LPT input and output pins */
static int PDR_pins[] = {2, 3, 4, 5, 6, 7, 8, 9};/* chip inputs, LPT output */
static int PSR_pins[] = {15, 13, 12, 10, 11}; /* chip outputs, LPT input */

int get_int_in_range(int, int);
void getline(char *buf, int maxbuf);
void chiptest(Chip *chip);

/* add to these-- */
extern Chip ls00;
extern Chip ls138;
Chip *chips[]={&ls00 , &ls138};
#define NCHIPS (sizeof(chips)/sizeof(chips[0]))

int main(void)
{
	int i, chipnum = DEFAULT_CHIPNUM;

	printf("Chip tester for chip(s):\n");
	for (i=0;i<NCHIPS;i++)
	  printf("%d--  %s\n",i,chips[i]->chipname);
#ifdef INTERACTIVE
	printf("\nEnter chip to test: ");
	chipnum = get_int_in_range(0,NCHIPS-1);
#endif
	chiptest(chips[chipnum]);
	return 0;
}

/* This needs filling out.  This code should work for any combinational
chip with properly setup Chip data (unless it has too many input or
output pins.)  Thus it should not do special-case code for LS138
vs. LS00, for example.  On the other hand, it does need to know 
about the LPT1 port, for ex., that outpins bit 0 needs to be read
from PSR bit 3, and one of these PSR bits needs to be inverted 
to get the chip's signal.
*/
/*****************************************/
/* This part is written to give the user */
/* pin connection instructions that are  */
/* derived from the Chip info only.      */
/* Note that some chips are already      */
/* connected up to mtip systems for you. */
/* See chiptest.txt for more information. */
/*****************************************/
void chiptest(Chip *chip)
{
	int i;
	int chip_input = 0, chip_output = 0, correct_output = 0;

	printf("Testing %s chip (%s)\n\n", chip->chipname,chip->chipdesc);
	printf("Connection requirements:\n\n");

	printf("1. Connect the following pins to VCC:\n");
	for (i=0; i<chip->n_highpins; i++) {
		printf("\tpin #%d\n", chip->highpins[i]);
	}
	printf("\n");

	printf("2. Connect the following pins to GND:\n");
	/* you fill in */
	for (i=0; i<chip->n_lowpins; i++) {
		printf("\tpin #%d\n", chip->lowpins[i]);
	}

	printf("3. Make the following LPT to %s connections:\n\n", chip->chipname);
	printf("\t###(Chip_Input pins)###\n");
	printf("\tLPT pin:\t%s pin:\n", chip->chipname);
	for (i=0; i<chip->n_inpins; i++)
		printf ("\t  %d\t\t  %d\n", PDR_pins[i], chip->inpins[i]);

	printf("\t###(Output pins)###\n");
	/* you fill in */
	for (i=0; i<chip->n_outpins; i++)
		printf ("\t  %d\t\t  %d\n", PSR_pins[i], chip->outpins[i]);

#ifdef INTERACTIVE  
	printf("\n<CR> when ready: ");
	while (getchar()!='\n')
		;
#endif

	/* generate all bit patterns by counting up to the right power of 2 */
	for (chip_input = 0; chip_input < (1 << chip->n_inpins); chip_input++) {
		/* you fill in */
		/* First use put chip_input value out LPT1, read back chip's result. */
		outpt(LP1_BASE,chip_input);
		/* Invert the LP_BUSY bit to compensate for its inversion in the */
		/* device: see pg 631 in S&S.  Shift the bits to the right by 3 */
		
		chip_output = inpt(LP1_BASE + 1);
		chip_output ^= 0x80;
		chip_output >>= 3;
		chip_output &= 0x0f;
		/* to get them in bits 0-4 to prepare for comparison. */
		/* Also make sure all bits at bit 5 or above are off. */
		/* Call "softchip" for chip to get correct result, also in bits 0-4. */
		/* Then compare the two results and report-- */
		chip->softchip(chip_input,&correct_output);
		if (chip_output != correct_output) {
			printf("Failure on input <%x>:\n", chip_input);
			printf("-- Expected output <%x> does not match chip output <%x>!\n",
				correct_output, chip_output);
			return;  /* give up on first failure */
		}
	}
	printf("*** Chip is OK. No failures encountered. ***\n");
}

/* portable function to get int value from user */
int get_int_in_range(int low, int high)
{
	char buf[MAXBUF];
	int num;

	getline(buf, MAXBUF);
	while ((sscanf(buf,"%d",&num)!=1) || (num<low) || (num>high)) {
		printf("Please enter a decimal number between %d and %d: ",low, high);
		getline(buf, MAXBUF);
	}
	return num;
}    

/* Turn platform-dependent library function calls into platform-independent
function to get one line from the user.
*/
void getline(char *buf, int maxbuf)
{
#ifdef SAPC
	fgets(buf, maxbuf, CONSOLE);
#else
	fgets(buf, maxbuf, stdin);
#endif
}

