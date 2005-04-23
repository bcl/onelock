#ifndef ONEWIRE_H
#define ONEWIRE_H

#define D500US	100
#define D100US  16
#define D150US  26
#define D120US  22


/* Prototypes */
int ow_reset( void );
void ow_read_rom( unsigned char *sn );
void ow_delay( unsigned int d );

#endif  // ONEWIRE_H
