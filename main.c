/* -----------------------------------------------------------------------
   1-wire Lock
   Copyright 2005 by Brian C. Lane
   All Rights Reserved
   ==========================[ HISTORY ]==================================
   03/20/2005   Storm has knocked out the internet, so I'll get a head 
   bcl          start on my LFNW presentation.
   
                First task is to see if I can tune the DCO with the 
                32767Hz watch crystal so I can do 1-wire operation.
   
   
   ----------------------------------------------------------------------- */
#include "hardware.h"



/* -----------------------------------------------------------------------
Delay function.
   ----------------------------------------------------------------------- */
void delay(unsigned int d) {
   int i;
   for (i = 0; i<d; i++) {
      nop();
      nop();
   }
}


void InitPorts( void )
{
  /* Initialize the ports */
  P1OUT  = P1OUT_INIT;                //Init output data of port1
  P2OUT  = P2OUT_INIT;                //Init output data of port2

  P1SEL  = P1SEL_INIT;                //Select port or module -function on port1
  P2SEL  = P2SEL_INIT;                //Select port or module -function on port2

  P1DIR  = P1DIR_INIT;                //Init port direction register of port1
  P2DIR  = P2DIR_INIT;                //Init port direction register of port2
}

void InitOSC( void )
{
    /* Setup DCO for about 1MHz*/
    /* ~1MHz Rsel=5 DCO=3 MOD=0 DCOR=0 */    
    BCSCTL1 = 0x03;
    
}


/* -----------------------------------------------------------------------
   Main function with a blinking LED
   ----------------------------------------------------------------------- */
int main(void) {

    /* Watchdog disabled */
    WDTCTL = WDTPW|WDTHOLD;

  InitPorts();
  InitOSC();
  InitIRQ();
    
    
    /* Initialize the interrupts */
    P1IES  = P1IES_INIT;                //init port interrupts
    P2IES  = P2IES_INIT;
    P1IE   = P1IE_INIT;
    P2IE   = P2IE_INIT;

    while (1) {                         //main loop, never ends...
       P1OUT ^= 1;
       delay(0x4fff);
    }
}

