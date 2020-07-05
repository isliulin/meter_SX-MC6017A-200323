#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
#include "lcdio.h"
#include "flash_filesystem.h"
#include "formatcmd.h"
extern  char 	sSuccess[];
 
unsigned char			CheckAddType ( unsigned long  laddr );


void bit16copy( unsigned short *des,  unsigned short *src, unsigned short ilen)
{
		unsigned short i ;
	
		for ( i = 0;i <ilen;i++)
			*(des+i)= *(src+i);
}
	
char
cmd_wflash (char *s)
{

  u8 i, x;
  u16 j;
  const char prev[] =
    "The Cardw_4428 context [addr]=value\r\nexample:   addr[0x0000]=0x56\r\nThis addr len must 4\r\n";
  const char shelp[] =
    "4428wr16 [0x98]=12,34,56,78,90,12,34,56,78,90,12,13,13,14,15,16\r\n[SUCCESS]addr[0x98]= 0c 22 38 4e 5a 0c 22 38 4e 5a 0c 0d 0d 0e 0f 10\r\n";

  u32 laddr;
  char cvalue,ctemp;
  char stemp[16];
  char *w;
  

  unsigned char cLen = (sizeof (stemp) / sizeof (stemp[0]));	// ¼Ä´æÆ÷¸öÊý 

  unsigned long ltemp;
  char *p;


  if (ERROR == getladdr (_incmdtext1,4, &laddr))
	{
    fprintf(((FILE *)_Uart_Handle_), "%s\r\n",  prev );
		 return ERROR;
	}

 

  p = strpbrk (s, "]=");
  if (NULL == p)
    return ERROR;
  if (p >= (s + strlen (s)))
    return ERROR;

  p += 2;

  for (i = 0; i < cLen; i++)
    {
      w = s + strlen (s);
      if (p > w)
	{
	  if (i < cLen)
	    cLen = i;
	  break;
	}
      ltemp = strtol (p, &w, 0);
      if (NULL == w)
	{
    fprintf(((FILE *)_Uart_Handle_), "%s\r\n",  prev );
		 return ERROR;
	}
      else
	p = w + 1;
      stemp[i] = (unsigned char) (ltemp & 0xff);
   }

		ctemp =  	CheckAddType(    laddr ) ;
	 	if (  _addr_flash_  ==  ctemp) 
					 HT_Flash_ByteWrite (stemp, laddr, cLen);
		if ( 		_addr_memory_  == ctemp) 
				memcpy ( ( char *)laddr , stemp ,cLen);
		if (( 		_addr_emu_  == ctemp) && ( sizeof(short) == cLen))
				 bit16copy ( ( unsigned short  *)laddr , ( unsigned short  *)stemp ,cLen/sizeof(short));

  for (i = 0; i < cLen; i++,laddr++)
    {
       if ( *((char *)laddr )  != stemp[i] )
				return ERROR ;
    }


  fprintf(((FILE *)_Uart_Handle_),"%s\r\n", sSuccess);

    
  return SUCCESS;

}
/*
unsigned char
BlockEarse (struct STFLASHFILESYSTEM *stfilep, unsigned long lAddr)
	 
  HT_Flash_ByteWrite (stCurr->sData, lAddr, stCurr->cLen-_Receord_iCheckData_Len_	-_Receord_iNo_Len_);
char
cmd_wram (char *s)
{
   const char prev[] =
    "The mcu chip write  context (addr)=value\nexample:   addr[0x0000]=(0x56)\r\nThis addr len must 4\r\n";
  char succ[] = "The wram(0xxxxx) value =0xxx\r\n";
   u32 laddr;
  char cvalue;
  

if ( ERROR == getladdr( _incmdtext1,4, &laddr))
	{
		 printf ( "%s\r\n",  prev );
      return SUCCESS;
		}
   
	 if ( ERROR ==get_value ( _incmdtext1   ,&cvalue))
	 {
		   printf ( "%s\r\n",  prev );
      return SUCCESS;
		}
   
	*( ( u8 *)laddr  ) = cvalue ; 
		cvalue = *( ( u8 *)laddr  ) ;
	
printf ( "The wram(0x%02x%04x) value =0x%02x\n",(u16) (laddr /0xffff),*((unsigned int *)( &laddr)+1) ,(u16)cvalue ) ; 

   return SUCCESS;
}

*/