/* -----------------------------------------------------------------------
   1-wire Lock
   Interrupt handler code
   Copyright 2005 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved   
   
   The right to use this code for educational or non-commercial use is
   granted. Commercial users need to contact the author first.
   ==========================[ HISTORY ]==================================
   09/05/2005   Added interrupt on 1-wire data line low
   bcl

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


extern volatile char presence;
extern volatile char Status;
volatile unsigned int VZC_2delta;
unsigned int VZC_LastCap;



void p1ie_on( void )
{
  /* Enable interrupt on low edge of P1.1 */
  P1IES |= 0x02;
  P1IE |= 0x02;
  P1IFG = 0;
}


void p1ie_off( void )
{
  /* Disable the interrupt */
  P1IE &= ~0x20;
}


/* ------------------------------------------------------------------------
   Setup the interrupts
   ------------------------------------------------------------------------ */
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

  p1ie_on();

  _EINT();
}


/* ------------------------------------------------------------------------
   Set a flag when a low is detected on the 1-wire data line
   Only active when waiting for a iButton to be connected
   ------------------------------------------------------------------------ */
interrupt (PORT1_VECTOR) INT_port1( void )
{
    if( P1IFG & 0x02 )
    {
        presence = 1;
    }
    P1IFG = 0;

    _BIC_SR_IRQ(LPM0_bits);
} 


interrupt (PORT2_VECTOR) INT_port2( void )
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

    _BIC_SR_IRQ(LPM0_bits);
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

