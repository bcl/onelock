/* -----------------------------------------------------------------------
   1-wire Lock
   Interrupt handler code
   Copyright 2005 by Brian C. Lane
   All Rights Reserved   
   ==========================[ HISTORY ]==================================
   03/20/2005   Define default interrupt vectors for the F149
   bcl          
   
   
   ----------------------------------------------------------------------- */
#include "hardware.h"

#define T125_MS	4096
#define MIN_DCO_TIME 29687
#define MAX_DCO_TIME 32812


short last_tbr;

void InitIRQ( void )
{
  /* Setup Timer A0 run off ACLK /1, continuous up count, start */
  TACTL = TASSEL_ACLK | TACLR;  
  TACTL |= MC_CONT;
  
  /* Setup Timer B0 to run off MCLK (DCO) /4 */
  TBCTL = TBSSEL_SMCLK | TBCLR | ID_DIV4;
  TBCTL |= MC_CONT;
    
  /* Setup an interrupt on TA0 for 125mS (4096 count) */
  last_tbr = TBR;
  CCR0 = TAR + T125_MS;
  CCTL0 = CCIE;  

  /* Enable Interrupts */
  _EINT();
}




interrupt (PORT2_VECTOR) INT_port2( void )
{
}

interrupt (UART1TX_VECTOR) INT_uart1tx( void )
{
}

interrupt (UART1RX_VECTOR) INT_uart1rx( void )
{
}

interrupt (PORT1_VECTOR) INT_port1( void )
{
} 

interrupt (TIMERA1_VECTOR) INT_timera1( void )
{
}

interrupt (TIMERA0_VECTOR) INT_timera0( void )
{
  unsigned short t;
  
  /* Stop Timer B */
  TBCTL = TBSSEL_SMCLK | ID_DIV4;
  
  t = TBR;
  CCR0 += T125_MS;

  /* Figure out how fast the DCO is running */
  if( t < MIN_DCO_TIME )
  {
    /* The DCO is running too slow */
    if( DCOCTL < 0xFF )
    {
      DCOCTL += 1;
    }
  } else if( t > MAX_DCO_TIME ) {
    /* The DCO is running too fast */
    if( DCOCTL > 0 )
    {
      DCOCTL -= 1;
    }
  } else {
    asm("  nop");
  }

  /* Clear and restart Timer B */
  TBCTL |= TBCLR;
  TBCTL |= MC_CONT;
}

interrupt (ADC_VECTOR) INT_adc( void )
{
}

interrupt (UART0TX_VECTOR) INT_uart0tx( void )
{
}

interrupt (UART0RX_VECTOR) INT_uart0rx( void )
{
}

interrupt (WDT_VECTOR) INT_wdt( void )
{
}

interrupt (COMPARATORA_VECTOR) INT_compa( void )
{
}

interrupt (TIMERB1_VECTOR) INT_timerb1( void )
{
}

interrupt (TIMERB0_VECTOR) INT_timerb0( void )
{
}

interrupt (NMI_VECTOR) INT_nmi( void )
{
}

