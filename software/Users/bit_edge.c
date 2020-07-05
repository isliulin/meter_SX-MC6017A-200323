#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"

#include <string.h>
#include "bit_edge.h"
#include "unit_instance.h"


_Bool bHalfSecStatu;

void
stBedge (unsigned short  iNo, unsigned short  clen)	//      
{
  unsigned short  i;
  struct STBITSTATU *p = (struct STBITSTATU *) stbitp + iNo;	// RamData.stsoftcon.bbitstatu[iNo].cbit);
  for (i = clen; i; i--, p++)
    p->bPrev = p->bCurr;

}




void
DTbitSave (void)
{
/*
  unsigned char i = sizeof (ldbbit) * 8 - 1;
  struct STBITSTATU *p = (struct STBITSTATU *) stbitp + _cn_start_s_bit;

  unsigned int ltemp = 0;

  do
    {
      ltemp *= 2;
      if ((*p).bCurr)
	ltemp++;
      p++;
    }
  while (i--);


  if (ltemp != ldbbit)
    lSave (ldbbit, ltemp);
*/
}


int
DTbitGet (void)
{
  unsigned char i;
  unsigned int j;
  unsigned int ltemp = (ldbbit);
  struct STBITSTATU *p = (struct STBITSTATU *) stbitp + _cn_start_s_bit;

  for (j = 1, i = 0; i < (8 * sizeof (ldbbit)); i++, j *= 2, p++)
    {
      if (ltemp & j)
	(*p).bPrev = (*p).bCurr = 1;
    }
}


void
timeclock_get (void)
{
  _disableInterrupts();
	memcpy ( ( char *)&stbitapp , (  char *)&stbitio, sizeof(stbitapp));				
	bInt_Ms = (_Bool) (bInt_Ms_io || IsUpB (Event_Firstrun));
  bInt_HalfSec = (_Bool) (bInt_HalfSec_io || IsUpB (Event_Firstrun));
  bInt_SecBit = (_Bool) (bInt_SecBit_io || IsUpB (Event_Firstrun));
	memset( ( char *)&stbitio, 0,sizeof(stbitio));//-(int)&(((struct  STBITIO  *)0)->ctemp));		
  _enableInterrupts ();
	
	if ( bInt_HalfSec  )
		bHalfSecStatu =~bHalfSecStatu;
	
}

void
timeclock_clear (void)
{
	
  bInt_Ms = 0;
  bInt_HalfSec = 0;
  bInt_SecBit = 0;
  bInt_SecRtc = 0;
  bInt_MinRtc = 0;
	bInt_bBeepStatu = 0;
	bInt_bBpschange =0;
	return ; 
}

void
bitstruct_init (void)
{
  unsigned short  i;

  for (i = 0; i < _cn_end_s_bit; i++)
    stbitp[i].cbit = 0;
  DTbitGet ();
  SetB (Event_Firstrun);	//     第一次次运

}
