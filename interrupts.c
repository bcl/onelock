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



void InitIRQ( void )
{
    /* Setup Timer A0 run off ACLK */
    
    
    /* Setup an interrupt on TA0 for 125mS (4096 count) */

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

