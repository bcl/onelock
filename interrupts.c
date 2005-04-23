/* -----------------------------------------------------------------------
   1-wire Lock
   Interrupt handler code
   Copyright 2005 by Brian C. Lane
   All Rights Reserved   
   ==========================[ HISTORY ]==================================
   04/02/2005   Changing to use the DCO adjustment method from slaa074.pdf
   bcl          Run Timer A from the DCO
                Connect 32.768Khz clock to CCI2B

   03/20/2005   Define default interrupt vectors for the F149
   bcl          
   
   
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "interrupts.h"
#include "dco.h"
#include "onewire.h"


extern volatile char Status;
volatile unsigned int VZC_2delta;
unsigned int VZC_LastCap;


void InitIRQ( void )
{
  /* Setup Timer A0 run off MCLK /1, continuous up count, start */
  TACTL = TASSEL_SMCLK | TACLR;
  TACTL |= MC_CONT;
  
  /* Setup CCR2 */
  CCTL2 = CM_POS|CCIS_1|SCS|CAP|OUTMOD_SET;

  /* Delay a bit for ACLK to settle */
  ow_delay( 0x1000 );
  
  /* Enable CCTL2 interrupt */
  CCTL2 |= CCIE;

  _EINT();
}




interrupt (PORT2_VECTOR) INT_port2( void )
{
}

interrupt (PORT1_VECTOR) INT_port1( void )
{
} 

/* -----------------------------------------------------------------------
   Measure the 1/2 period of the 32.768KHz xtal
   ----------------------------------------------------------------------- */
interrupt (TIMERA1_VECTOR) INT_timera1( void )
{
  if( TAIV == 0x04 )
  {
    /* CCR2 Interrupt */
    
    /* Measure the 32.768KHz period */
    VZC_2delta = CCR2 - VZC_LastCap;
    VZC_LastCap = CCR2;
    Status = TASK_OVR;
  }
}

interrupt (TIMERA0_VECTOR) INT_timera0( void )
{
}

interrupt (WDT_VECTOR) INT_wdt( void )
{
}

interrupt (COMPARATORA_VECTOR) INT_compa( void )
{
}

interrupt (NMI_VECTOR) INT_nmi( void )
{
}

