/* -----------------------------------------------------------------------
   1-wire interface code
   Copyright 2005 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   =======================================================================
   04/02/2005   Started this module.
   bcl
   
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "onewire.h"


/* -----------------------------------------------------------------------
   delay loop for 1-wire
   ----------------------------------------------------------------------- */
void ow_delay( unsigned int d )
{
  unsigned int i;

  for( i=0; i<d; i++ )
  {
    asm("  nop");
  }
}


/* -----------------------------------------------------------------------
   1-wire reset and presence detection

   return 0 no presence detected
   return 1 presence detected
   ----------------------------------------------------------------------- */
int ow_reset( void )
{
  int status;
  
  /* Pull the 1-wire bus low for 500uS */
  P1OUT &= ~0x02;
  P1DIR |= 0x02;
  
  ow_delay( D500US );
  
  /* Allow resistor to pull it high */
  P1DIR &= ~0x02;
  
  /* Delay 100uS before testing for presence */
  ow_delay( D100US );

  /* Grab the status of the bus */  
  status =  !(P1IN & 0x02) ;
  
  /* Finish out the presence pulse time */
  ow_delay( D150US );
  
  return status;
}



