/* -----------------------------------------------------------------------
   DCO related definitions
   ----------------------------------------------------------------------- */
#ifndef DCO_H
#define DCO_H

#define T125_MS	4096
#define MIN_DCO_TIME 29687
#define MAX_DCO_TIME 32812


#define DCO_SET		1000

// rsel 0    1    2    3   4   5   6   7
// f@3V 0.12 0.19 0.31 0.5 0.8 1.2 2.0 3.1
#define RSEL_NOM	4
// 32768/4
#define F_REF		8192
#define TCLK_DIV	1
// (DCO_SET*1000)/(F_REF*TCLK_DIV)
#define FSET		244
#define DCO_MAX		0xE2
#define DCO_MIN		0x05



/* Status Flags */
#define TASK_OVR	0x0001


/* Prototypes */
void dco_step( void );
void Rsel_set( short rsel );


#endif  // DCO_H
