/* -----------------------------------------------------------------------
   1-wire Lock
   Copyright 2005 by Brian C. Lane
   All Rights Reserved
   ==========================[ HISTORY ]==================================
   04/02/2005   Getting DCO adjustment working today.
   bcl

   03/20/2005   Storm has knocked out the internet, so I'll get a head 
   bcl          start on my LFNW presentation.
   
                First task is to see if I can tune the DCO with the 
                32767Hz watch crystal so I can do 1-wire operation.
   
   
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "interrupts.h"
#include "dco.h"


short Rsel;			/* DCO Resistor Selection 		*/
volatile unsigned char Status;	/* Status flags				*/

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
    /* Clear OSC Fault */
    IE1 = 0x00;
    IFG1 = 0x00;
    
    /* Select DCO/1 as MCLK */
    BCSCTL2 = 0x00;
}

void InitDCO( void )
{
    /* Setup the DCO frequency */
    Rsel = RSEL_NOM;
    Rsel_set( Rsel );
    
    /* Setup for ACLK/8 */
    BCSCTL1 |= DIVA_DIV8;

    /* Diagnostic */
    /* Output SMCLK on P1.4 pin 16 f149 */
    P1SEL |= 0x10;
    P1DIR |= 0x10;
    
    /* Output ACLK on P2.0 pin 20 f149 */
    P2SEL |= 0x01;
    P2DIR |= 0x01;
}



/* -----------------------------------------------------------------------
   Main function with a blinking LED
   ----------------------------------------------------------------------- */
int main(void)
{
    short i;
    short task_state;
    
    /* Watchdog disabled */
    WDTCTL = WDTPW|WDTHOLD;

    Status = 0;

    InitPorts();
    InitOSC();
    InitDCO();
    InitIRQ();

    /* Startup loop, 4 loops through */
    for( i=0; i<4; i++ )
    {
      while( !(Status & TASK_OVR) )
      {
         asm("   nop");
      }
      Status &= ~TASK_OVR;
    }
    dco_step;

    task_state = 0;
    while (1) {                         //main loop, never ends...
      if( Status & TASK_OVR )
      {
        /* Execute 10mS tasks */
        switch( task_state )
        {
          case 0:
              break;
              
          case 1:
              break;
              
          case 2:
              break;
              
          case 3:
              dco_step();
              
              /* Diagnostic */
              P1OUT ^= 1;
              break;
        }
        task_state = (task_state + 1) % 4;
        Status &= ~TASK_OVR; 
      }
    }
}

