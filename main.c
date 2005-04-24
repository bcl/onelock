/* -----------------------------------------------------------------------
   1-wire Lock
   Copyright 2005 by Brian C. Lane
   All Rights Reserved
   ==========================[ HISTORY ]==================================
   04/23/2005   Moving over to new hardware, F1101
   bcl          Trying to optimize the 'c' code first, debugging ASM is
                going to be a bear with gdb.

   04/02/2005   Getting DCO adjustment working today. done.
   bcl          Adding 1-wire on P1.1

   03/20/2005   Storm has knocked out the internet, so I'll get a head 
   bcl          start on my LFNW presentation.
   
                First task is to see if I can tune the DCO with the 
                32767Hz watch crystal so I can do 1-wire operation.

   ===========================[ PIN MAP ]=================================
   P1.0   LED
   P1.1   1-Wire data
   P1.2   1-Wire pullup power
   P1.3   Motor control
   P1.4   JTAG
   P1.5   JTAG
   P1.6   JTAG
   P1.7   JTAG
   
   P2.0   MAX3221 ForceOff
   P2.1   N/C
   P2.2   RS232 input? (From PC)
   P2.3   RS232 output? (To PC)
   P2.4   N/C
   
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "interrupts.h"
#include "dco.h"
#include "onewire.h"


short Rsel;			    /* DCO Resistor Selection 		*/
volatile unsigned char Status;	    /* Status flags			*/
volatile unsigned int VZC_2delta;   /* DCO count over ACLK/8            */




/* -----------------------------------------------------------------------
   Main function with a blinking LED
   ----------------------------------------------------------------------- */
int main(void)
{
    short i;
    unsigned int success;
    short adj_timer;
    unsigned char sn[8];		/* 1-wire serial number 	*/
    
    /* Watchdog disabled */
    WDTCTL = WDTPW|WDTHOLD;

    Status = 0;

    /* Initialize the ports */
    P1OUT  = P1OUT_INIT;                //Init output data of port1
    P2OUT  = P2OUT_INIT;                //Init output data of port2

    P1SEL  = P1SEL_INIT;                //Select port or module -function on port1
    P2SEL  = P2SEL_INIT;                //Select port or module -function on port2

    P1DIR  = P1DIR_INIT;                //Init port direction register of port1
    P2DIR  = P2DIR_INIT;                //Init port direction register of port2

    /* Initialize the Oscillator */
    /* Clear OSC Fault */
    IE1 = 0x00;
    IFG1 = 0x00;
    
    /* Select DCO/1 as MCLK */
    BCSCTL2 = 0x00;

    /* Initialize the DCO */
    /* Setup the DCO frequency */
    Rsel = RSEL_NOM;
    Rsel_set( Rsel );
    
    /* Setup for ACLK/8 */
    BCSCTL1 |= DIVA_DIV8;

    InitIRQ();

    /* Turn on power to 1-wire pullup on P1.2 */
    P1OUT |= 0x04;

    /* Turn off the LED on P1.0 */
    P1OUT &= ~0x01;
    adj_timer = DCO_ADJ_TIME;
    while (1) {                         //main loop, never ends...
      if( Status )
      {
        if( --adj_timer == 0 )
        {
          dco_step();
          adj_timer = DCO_ADJ_TIME;

          /* Turn off the LED */
          P1OUT &= ~0x01;
        }
        Status = 0; 
      }


      /* Poll the 1-wire on P1.1 (should be an interrupt) */
      if( !(P1IN & 0x02) )
      {
        success = 0;

        /* Erase the serial number  */
//        for(i=0;i<8;i++)
//        {
//          sn[i] = 0;
//        }
        sn[0] = 0;

        /* Delay about 10mS to let the line settle */
        ow_delay( 2000 );

        /* Try up to 5 times */
        for(i=0;(i<5) && !success;i++)
        {
          /* Check to see if there is a device present */
          if( ow_reset() )
          {
            /* Go Read the 1-wire serial number */
            ow_read_rom( sn );
 
            /* If it is a thermochron, turn on the LED and motor */ 
            if( sn[0] == 0x21 )
            {
              /* Turn on the LED on P1.0 */
              P1OUT |= 0x01;
 
              /* Check checksum */
   
              /* Check against the access list */
                
              /* Unlock the lock on P1.3 */
              P1OUT |= 0x08;
              ow_delay( 0x6000 );
              P1OUT &= ~0x08;
              
              success = 1;
            }
          } /* presence */
        } /* for loop */
      }
    }
}

