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
 
#include "flash_filesystem.h"
#include "cmdthread.h"
//#include "cmd_gctoper.h"
#include "string.h"
#include "pubset.h"

#include "cmd_flashoper.h"
#include "formatcmd.h"
//#include "cmd_otheroper.h"
//#include "cmd_eepromoper.h"
#include "cmd_ramoper.h"
//#include "cmd_adcoper.h"c
//#include "cmd_card7816.h"
//#include "cmd_cardi2c.h"
//#include "cmd_card4428.h"
//#include "cmd_card4442.h"
//#include "cmd_beep.h"
//#include "cmd_bpsset.h"
char
hex2char (char i) ;
extern volatile struct STSCIBUF USARTCOM[_Com_End_];
 
const char s_cardtypeerr[]="card type no reset !  please check card state";
const char	s_help[]="help";
const char  sMessage[]="[Message]";
const char 	sSuccess[]="[SUCCESS]";
const char	sError[]="[Error]";
const char stderror[] = "no command!";
const char stdprev[] = "stm8l>";
 
char *sresult = USARTCOM[_IR_Chanel_].DataBuf;
char
cmd_flashearse(char *s);
char
cmd_rXiaoBiaoZT (char *s);
 
const struct STCMDLIST stcmdlist[] = { {0,0}};
#ifdef _DEL
	const struct STCMDLIST stcmdlist[] = {
 // {"hello", cmd_hello,},	// hello 
	//{"dir", cmd_help,},
  //{"wrgct", cmd_wrgct,},
 // {"rdgct", cmd_rdgct},
  {"rflash", cmd_rflash,},
  {"wflash", cmd_wflash,},
	{"wjb",cmd_rXiaoBiaoZT,},
  //{"bset", cmd_bitoper,},		// 位置高 
  //{"bres", cmd_bitoper,},		// 位置底
 	//{"bget",  cmd_bget},
  {"rram",	cmd_rram  ,},
  {"wram", cmd_wflash,},
	{"fdel", cmd_flashearse,},
  /* {"reeprom", cmd_reeprom ,},
	{"weeprom", cmd_weeprom ,},
	{"adcin", cmd_adcin ,},
  {"cardreset", cmd_card7816reset,},
	{"cardwrite", cmd_card7816write,},
	{"cardclose", cmd_card7816close,},
	{"ci2creset", cmd_cardi2creset,},
	{"ci2cwrite", cmd_cardi2cwrite,},
	{"ci2cread", cmd_cardi2cread,},
	{"ci2close", cmd_cardi2close,},
	
	{"4428reset", cmd_card4428reset,},
	{"4428write", cmd_card4428write,},
	{"4428read", cmd_card4428read,},
	{"4428close", cmd_card4428close,},
	{"4428pword", cmd_card4428_password,},				//		设置认证密码
	{"4428psc" , cmd_card4428_psc,},							//	 认证
	{"4428wr16", cmd_card4428write16,},						//	  连续写入
	{"4428newpwd", cmd_card4428_updatapwd,},			
	{"4428despwd",cmd_card4428_resetpassword ,},		// 设置连 续写入密码
	
	{"4442reset", cmd_card4442reset,},
	{"4442write", cmd_card4442write,},
	{"4442read", cmd_card4442read,},
	{"4442close", cmd_card4442close,},
	{"4442pword", cmd_card4442_password,},		
	{"4442psc" , cmd_card4442_psc,},
	{"4442spread" , cmd_card4442_read_sp,},
	{"4442newpwd" , cmd_card4442_updatapwd,},
	{"4442wr16" , cmd_card4442write16,},
	{"setcard" , cmd_setcardtype,},								// 	  设置初始状态
	
	{"bpsset",	cmd_bpsset,},
	
{	"beep",  cmd_beep },
	{"help", cmd_help,},
*/
};
	#endif
 const char	 _stcmdlist_len = sizeof (stcmdlist) / sizeof (struct STCMDLIST) ;
#ifdef _DEL
void  prog_stop_dealy(unsigned char  cnum )
{
		unsigned char 	i,x ;
		
		for ( i = 0 ; i < cnum ; i++)
		{
		 	x =   	cmainnum ; 
			while ( x == cmainnum );
	  }
			return  ; 
}

char
hex2char (char i)
{
  i &= 0xf;

  if ((i >= 0) && (i < 10))
    return i + '0';
  i -= 10;
  return 'a' + i;
}

char
cmd_thread (void)
{
const   struct STCMDLIST *p = stcmdlist;
  unsigned char i;
  for (i = 0; i < sizeof (stcmdlist) / sizeof (struct STCMDLIST); i++, p++)
    {
      if (strcmp (p->scmd, _incmdid) == 0)
	return (p->DoProg (_incmdtext1));
    }
  return ERROR;

} 

unsigned char			CheckAddType ( unsigned long  laddr )
{
	unsigned long ltemp1 = (unsigned long)(&HT_EMUECA->SRSTREG);
	
				unsigned char	 ctemp = checkaddr(laddr);
			ltemp1+=sizeof(HT_EMUECA->SRSTREG) ;
			if ( SUCCESS != ctemp )
						return _addr_revser_ ;
			
			 if (( laddr >=0 ) && (laddr < 0x20000 ))
				 return 	_addr_flash_  ;
			 else
				  //	 if (( laddr >=HT_EMU_ECA_BASE ) && (laddr < ((unsigned long)(&HT_EMUECA->SRSTREG)+sizeof(HT_EMUECA->SRSTREG)) ))
					if (( laddr >=HT_EMU_ECA_BASE ) && (laddr < ltemp1 ))
								return _addr_emu_;
						else
								return 		_addr_memory_ ;
		 }

unsigned char checkaddr( unsigned long laddr )
{
		if ( laddr > 0x40080000)
				return ERROR;
		else
			   if ( (laddr >= 0x20002000 ) && ( laddr <0x40000000))
						return ERROR;
				 else
						if ( (laddr >= 0x00040200 ) && ( laddr <0x20000000))
							return ERROR;
						else if ( (laddr >= 0x00020000 ) && ( laddr <0x00040000))
							return ERROR;
		return SUCCESS ;
}

unsigned char   getladdr (  char *s  , char  clen  , unsigned long  *laddr )   //
{
	unsigned long ltemp , ltemp1 ; 
	char 	stemp[32];
	char *w ; 
	unsigned char i ; 
	char 	*p ;
	
		p =  strpbrk( s , "]=");
		
  if (( s[0] != '[') || p == NULL )   
	{
      return ERROR;
  }

  memset (stemp, 0, sizeof (stemp));
  
	*laddr = strtol( s + 1, &w, 0);
	for ( ltemp1 = 0xffffffff , i = 4 ; i >=clen ; i-- , ltemp1 /=256)
		*laddr  &=ltemp1 ; 
			return SUCCESS ;
}


unsigned char   getstr (  char *s  , char  clen  , unsigned char  *sp )   //
{
	unsigned long ltemp , ltemp1 ; 
	char 	stemp[32];
	char **w ; 
	unsigned char i ; 
	char 	*p ;
	
		p =  strpbrk( s , "]=");
		
  if (( s[0] != '[') || p == NULL )   
	{
      return ERROR;
  }

 
	
	return SUCCESS ;
}


unsigned char  get_value (  char *s , unsigned char *cvalue)
{
	char **w;
	unsigned long ltemp ; 
	char 	*p  ;
	
	p =  strpbrk( s , "]=");
	
	if ( p == NULL ) 	return ERROR;
	
	if ( p >= ( s + strlen(s)) ) return ERROR; 
	
  ltemp = strtol (p+2, w, 0);
  *cvalue = ltemp & 0xff;
	return SUCCESS ;
} 


 char hex_str ( unsigned long lvalue , char *s  )
{
		char		 x , y ; 
		char 	stemp[20];unsigned char i ; 
		char *p = ( unsigned char *)&lvalue ; 
	
		memset ( stemp ,0 , sizeof(stemp));
		for (  i = 0 ; i <  2 * sizeof(lvalue ) ; i+=2 ,p++ )
		{
				stemp[i ] = *p >> 4 ;
				stemp[i +1]   = *p &0xf ; 
	  }
			for ( i = 0  ; i <2 *  sizeof ( lvalue );i++)	
				if ( stemp[i] ) break;
				
		if ( i >= 2 * sizeof( lvalue )) 
		{
			i = 0 ;
		  x = 2; 
		}
		else	
			x = sizeof(lvalue) *2 -i  ;
			memcpy ( s , stemp +i  , x ) ; 
 
			
			for ( i = 0 ; i< x   ; i++)
				s[i] =  hex2char (s[i]);
				return SUCCESS  ; 
}		
	 #endif
void
com_shell (void)
{
	#ifdef _DEL
  unsigned char clen;
  unsigned char i, j;
  for (i = 0; i < _CHANEL_END_; i++)
    {
      if (USARTCOM[i].bEventRec) 
	{
	  clen = USARTCOM[i].cHead;USARTCOM[i].cHead=0;USARTCOM[i].bEventRec=0;
	  format_cmd (USARTCOM[i].DataBuf, clen);
	  if (cargc)
		{
	      if (SUCCESS == cmd_thread ())
				{} //USARTCOM[i].cHead = 0;
			else
					fprintf(((FILE *)_Uart_Handle_), "%s%s\n",sMessage,stderror)  ; 
	  
	   }

	 //fprintf(((FILE *)_Uart_Handle_), "\n%s%s",sMessage,stdprev)  ;  
	USARTCOM[_IR_Chanel_].cPSend_len = USARTCOM[_IR_Chanel_].cHead;
	  
	}
    }
		#endif
		
}
 
					 
