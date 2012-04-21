/******************************************************************************
*
*  FILE: scope.c
*
*
*  PURPOSE: mp4, main program. A digital oscilloscope, generates a serial byte
*           by writing to COM1's UART TX register and then monitors the resulting
*           signal through LPT1, storing successive values in an array in
*           memory for playback after character transmission has completed.
*
******************************************************************************/

#include <stdio.h>      /* standard input/output header */
#ifdef SAPC
#include <serial.h>		/* serial communications header */
#include <lp.h>			/* LPT header */
#include <cpu.h>		/* for inpt/outpt prototypes */
#endif

#define MAXLINE 200
/* final program should have INTERACTIVE defined, but
 * you may want to comment this out while debugging
 * to get rid of the user input.
 */
#define INTERACTIVE
/* non-interactive values-- */
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_CHAR 'A'

extern void collect(char *data, int maxdata);    /* collect loop */

/* FUNCTION PROTOTYPES */
void getline(char *buf, int maxbuf);
void get_user_info(char *ch, int *baudrate);
void set_baudrate(int dev, int baudrate);
void display_one_signal(int signal);
void print_data_display(int consec_ctr, int measurement);

int main(void)
{

  int baudrate = DEFAULT_BAUDRATE;
  unsigned char ch = DEFAULT_CHAR;

#ifdef INTERACTIVE
  get_user_info(&ch, &baudrate);
#endif
#ifdef SAPC
  set_baudrate(COM1, baudrate);
#endif

  /* do experiment--you fill in */
  return 0;
}

/****************************************************************************
*
* FUNCTION: set_baudrate(int dev, int baudrate)
*
* PURPOSE: Sets the specific baudrate for the specified COM line dev, using
*          baudrate specified by user.  The formula for the baud rate divisor
*          is given in the assignment writeup.  The reason for the divisor
*          is that the UART's get a 1.8432-MHz input clock frequency 
*          (UART_BAUD_CLOCKHZ) and need a 16X baud rate input to sample 
*          the input in the middle of the baud period.
*
*****************************************************************************/
#ifdef SAPC
void set_baudrate(int dev, int baudrate)
{
  /* you fill in */
}
#endif

/****************************************************************************
*
*  FUNCTION:  get_user_info
*
*  PURPOSE:  Prompt the user to type a character, echo this character back to
*            the user as a character, and in hexadecimal.  Also prompt the user
*            for a baudrate, to be used by set_baudrate(int dev, int baudrate)
*
******************************************************************************/

void get_user_info(char *ch, int *baudrate)
{
  char line[MAXLINE];

  /* Prompt the user for info */
  printf("\n***************  DIGITAL OSCILLOSCOPE ************************\n");
  printf("\nThis program acts as a digital oscilloscope.  It allows you to\n");
  printf("\nsee a serial byte in flight.\n");
  printf("\n**************************************************************\n");
  printf("\n\n\nPlease type a character:     ");
  getline(line,MAXLINE);
  *ch = line[0];  			/* Store char */
  printf("\nThe character is a:     %c", *ch);
  printf("\nThe character has a hexadecimal value of:     %x\n", *ch);
  do {
    printf("\nPlease specify a baudrate:     ");
    getline(line,MAXLINE);
  } while (sscanf(line,"%d", baudrate)!=1);
  printf("\nCharacter %c will be sent to COM1 at baudrate %d\n",
	 *ch, *baudrate);
}


/******************************************************************************
*
*  FUNCTION:  display_one_signal
*  ARGUMENT:  signal, equal to 0 or 1 for run of data points
*             or equal to -1 to force out last few points at the very end
*
*  PURPOSE:  Accumulates the data points and calls print_data_display
*            to print out the results for one run of equal signals.
*
******************************************************************************/
void display_one_signal(int signal)
{
  static int i = 0;		/* signal number (increments on each call) */
  static int consec_ctr = 0;	/* Counter for # of consecutive highs or lows */
  static int last_sig = -1;	/* start with impossible signal to force init. */

  /* if this measurement is the same as the last */
  if (signal == last_sig) {
    consec_ctr++;		/* increment the # of consecutive occurrences */
  } else {			/* changed--display last batch */
    if (i>0)			/* --unless just starting */
      print_data_display(consec_ctr, last_sig);  /* print previous results */
    consec_ctr = 1;		/* reset consecutive counter */
    last_sig = signal;		/* remember new signal */
  }
  i++;
}

/******************************************************************************
*
*  FUNCTION:  print_data_display
*  PARAMETERS: consec_ctr: number of consecutive signals, all the same value
*              signal: 0 or 1
*
*  PURPOSE:  Displays the collected results, using "_" for a low point and "|"
*            for a high point.  For more than 4 consecutive data points all
*	     high or all low, it will be represented by __#__ or ||#||, where #
*            is the number of consecutive lows or highs.  As on practice final.
*
******************************************************************************/

void print_data_display(int consec_ctr, int signal)
{
  int count = 0;
  char mark = signal?'|':'_';

  if(consec_ctr < 4) {
     for (count=0; count<consec_ctr; count++)
       printf("%c", mark);
  } else {  /* more than 4, print __#__ or ||#|| */
     printf("%c%c%d%c%c",mark,mark,consec_ctr,mark,mark);
  }   /* end of else */
}
/*****************************************************************************/


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
