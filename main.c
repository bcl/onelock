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


short Rsel;			    /* DCO Resistor Selection 		*/
volatile unsigned char Status;	    /* Status flags			*/
volatile unsigned int VZC_2delta;   /* DCO count over ACLK/8            */

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
    short adj_timer;
    
    /* Watchdog disabled */
    WDTCTL = WDTPW|WDTHOLD;

    Status = 0;

    InitPorts();
    InitOSC();
    InitDCO();
    InitIRQ();

    adj_timer = DCO_ADJ_TIME;
    while (1) {                         //main loop, never ends...
      if( Status & TASK_OVR )
      {
        if( --adj_timer == 0 )
        {
          dco_step();
          adj_timer = DCO_ADJ_TIME;
          
          /* Diagnostic */
          P1OUT ^= 1;
        }
        Status &= ~TASK_OVR; 
      }
    }
}

