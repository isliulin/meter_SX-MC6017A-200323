
#include "unit_instance.h"

union unSTBITSTATU bbitstatu[_cn_end_total_bit];
unsigned long lESTATE;
unsigned char  	cmainnum ; 
#if _cn_end_total_bit >= (1<<8)
error ("bit struct  too len");
#endif

const unsigned char  cBitlen = _cn_end_total_bit;
const unsigned long  csBitstruct = (unsigned long )(&bbitstatu);
struct  STBITIO   stbitio;					//	 ÖÐ¶ÏIO
struct  STBITIO   stbitapp;					//		Ó¦ÓÃIO

