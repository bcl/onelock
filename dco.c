/* -----------------------------------------------------------------------
   DCO Functions
   Copyright 2005 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   
   
   The right to use this code for educational or non-commercial use is
   granted. Commercial users need to contact the author first.
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "dco.h"


extern volatile unsigned int VZC_2delta;


/* -----------------------------------------------------------------------
   Adjust the DCO resistor selection in streps until it is at rsel
   ----------------------------------------------------------------------- */
void Rsel_set( short rsel )
{
  while( (BCSCTL1 & 0x07) != rsel )
  {
    if( (BCSCTL1 & 0x07 ) > rsel )
    {
      BCSCTL1 -= 1;
    } else {
      BCSCTL1 += 1;
    }
  }
}


/* -----------------------------------------------------------------------
   Adjust the DCO using step method
   ----------------------------------------------------------------------- */
void dco_step( void )
{
    if( DCOCTL == DCO_MAX )
    {
      /* Increment Rsel */
      if( (BCSCTL1 & 0x07)  < 0x07 )
      {
        BCSCTL1 += 1;
        DCOCTL = 0x60;
      } 
    } else if( DCOCTL == DCO_MIN ) {
      /* Decrement Rsel */
      if( (BCSCTL1 & 0x07) > 0x00 )
      {
        BCSCTL1 -= 1;
        DCOCTL = 0x60;
      } 
    } else {
      if( VZC_2delta < FSET )
      {
        DCOCTL += 1;
      } else if( VZC_2delta > FSET ) {
        DCOCTL -= 1;
      }    
    }
}
