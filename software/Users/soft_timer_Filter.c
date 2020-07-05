#include "soft_timer_filter.h"
#include "bit_edge.h"
#include "unit_instance.h"

void
softtimer_run (unsigned short  iNo, unsigned char clen)	//      定时器运行
{
  unsigned char i, j, n;



  struct STTIMERSATU *p = (struct STTIMERSATU *) &(stbitp[iNo].cbit);
  for (i = 0; i < clen; i++, p++, iNo++)
    {
      if ((p->bin) && (p->timer))
	{
	  (p->timer)--;
	  if (0 == p->timer)
	    p->bCurr = 1;
	}
      if (!(p->bin))
	p->timer = 0;
    }
}



void
RSet_SoftTime (unsigned short  iNo, unsigned char cStatu, unsigned char cTimeout)	//      设置定时器
{
  struct STTIMERSATU *p = (struct STTIMERSATU *) &(stbitp[iNo].cbit);
  if (!cStatu)
    return;
  p->timer = cTimeout;
  p->bCurr = 0;
  p->bPrev = 0;
}


void
Set_SoftTime (unsigned short  iNo, unsigned char cStatu, unsigned char cTimeout)	//      设置定时器
{
  struct STTIMERSATU *p = (struct STTIMERSATU *) &(stbitp[iNo].cbit);

  if (0 == cStatu)
    {
      p->bCurr=p->bin =0;// *((unsigned char *) p) = 0;
    }
  else if (0 == p->bin)
    {
      p->timer = cTimeout;
      p->bin = 1;
    }
}
