/* -----------------------------------------------------------------------
   1-wire interface code
   Copyright 2005 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   =======================================================================
   04/02/2005   Started this module.
   bcl          Interrupts need to be disabled while running 1-wire ops.
   
   ----------------------------------------------------------------------- */
#include "hardware.h"
#include "onewire.h"


/* -----------------------------------------------------------------------
   delay loop for 1-wire
   ----------------------------------------------------------------------- */
void ow_delay( unsigned int d )
{
  while( d-- )
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
  

  _DINT();
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
  
  _EINT();
  return status;
}



/* -----------------------------------------------------------------------
   Write a 1-wire byte
   ----------------------------------------------------------------------- */
void ow_write( unsigned char d )
{
  unsigned int i;
  
  /* Output a 1 */
  _DINT();
  P1OUT |= 0x02;
  P1DIR |= 0x02;
  
  for(i=0;i<8;i++)
  {
    if( d & 0x01 )
    {
      /* Write a 1 -- pull low for ~8uS (must be 1-15uS) */
      P1OUT &= ~0x02;
      asm("  nop");
      asm("  nop");
      asm("  nop");			/* About 8uS       	*/
      P1OUT |= 0x02;                    /* Keep it high		*/
      ow_delay( D100US );
    } else {
      /* Write a 0 */
      P1OUT &= ~0x02;
      ow_delay( D100US );
      P1OUT |= 0x02;      
      asm("  nop");
      asm("  nop");
    }
    d = d >> 1;
  }
  
  /* Let pullup pull data line high */
  P1DIR &= ~0x02;

  _EINT();
}


/* -----------------------------------------------------------------------
   Read a 1-wire byte
   ----------------------------------------------------------------------- */
unsigned char ow_read()
{
  unsigned char d;
  unsigned int i;
  
  _DINT();
  P1OUT |= 0x02;
  P1DIR |= 0x02;
  
  d = 0;
  for(i=0;i<8;i++)
  {
    P1OUT &= ~0x02;			/* Pull ow line low	*/
    P1DIR |= 0x02;
    asm("  nop");
    asm("  nop");
    asm("  nop");			/* About 8uS       	*/
    P1DIR &= ~0x02;			/* Let pullup pull	*/      
  
    /* Delay for 20uS before sampling */
    ow_delay( 1 );
    
    d = d >> 1;
    if( P1IN & 0x02 )
    {
      d = d | 0x80;
    } else {
      d = d & 0x7F;
    }
    
    /* Delay to allow release */
    ow_delay( 5 );
  }
    
  _EINT();
  return d;
}


/* -----------------------------------------------------------------------
   Read the serial number from a device
   ----------------------------------------------------------------------- */
void ow_read_rom( unsigned char *sn )
{
  unsigned int i;

  if( ow_reset() == 1 )
  {
    /* Send the read ROM command */
    ow_write( 0x33 );
    
    for(i=0;i<8;i++)
    {
      /* Read the serial number */
      sn[i] = ow_read();
    }
    
    ow_reset();
  }
}

