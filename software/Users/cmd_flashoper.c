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
#include "cmd_flashoper.h"
#include "formatcmd.h"
#include "emu.h"
 extern  char 	sSuccess[];
 extern unsigned char	cResetSecTm ;
unsigned char			CheckAddType ( unsigned char  laddr );
unsigned char
BlockEarse (struct STFLASHFILESYSTEM *stfilep, unsigned long lAddr);

const unsigned char _clen = 16;		// 一次读出的内存单元的个数 







unsigned char  getladdr (  char *s  , char  clen  , unsigned long   *laddr )  ;
char
cmd_rflash (char *s)
{
  unsigned char i;
  unsigned long  laddr;
	char **w ;
 
  const char prev[] =
    "The get Flash(or ram ) read result [0xaddr]=(0xvalue)\r\nThis addr is Hex\r\n";
 
  if ( ERROR == getladdr( _incmdtext1,4, &laddr))
 {
		fprintf(((FILE *)_Uart_Handle_), "%s\r\n",  prev );
      return SUCCESS;
		}
   
	if (SUCCESS != checkaddr(laddr ))
		   return ERROR;
	
	fprintf(((FILE *)_Uart_Handle_), "addr[0x%08x]= ", laddr );
	
	for ( i = 0 ;i < _clen; i++,laddr++)
	{
			if (SUCCESS == checkaddr(  laddr ))
				fprintf(((FILE *)_Uart_Handle_), "%02x ",(unsigned int )(*((  uint8_t*)laddr) ));
		
	}
	fprintf(((FILE *)_Uart_Handle_), "%s","\r\n");
		
		

 
  return SUCCESS;
}


 unsigned char   flash_wrio (  unsigned long   laddr ,  unsigned char cvalue);

char
cmd_flashearse(char *s)
{
  unsigned char i;
  unsigned long  laddr,ltemp;
	char **w ;
 struct STFLASHFILESYSTEM stDianLian1={
  0,
  _iBlcokLen_	,
  0,
  0,
  0,
};
 
  const char prev[] =
    "The get Flash(or ram ) read result [0xaddr]=(0xvalue)\r\nThis addr is Hex\r\n";
 
  if ( ERROR == getladdr( _incmdtext1,4, &laddr))
 {
		fprintf(((FILE *)_Uart_Handle_), "%s\r\n",  prev );
	  return ERROR;
      
		}
   
	if (SUCCESS != checkaddr(laddr ))
		   return ERROR;
	
	
	i = CheckAddType (laddr);
	if ( _addr_flash_  != i )
	{
				return ERROR;
	}
	ltemp =  laddr /_iBlcokLen_	;
	if ( laddr != (ltemp *_iBlcokLen_	))
				return ERROR;
	
	
	i = BlockEarse (&stDianLian1, laddr);
	if ( TRUE == i ) 
	{
			
				fprintf(((FILE *)_Uart_Handle_), "ok\r\n");
		return SUCCESS;
	}	
	else
				return ERROR;
	
	
		
		

 
  return SUCCESS;
}
void  System_Init_app(void);
void Chip_Trim_5l(void);
 void Chip_Trim_5(void);
char
cmd_rXiaoBiaoZT (char *s)
{
  unsigned char i;
  unsigned long  laddr;
	char **w ;
 extern enum  ENUMCHIPMODE  enChipMode ;//= _cChipRun_Mode_  ; 
	extern const struct STCHIPSET stChipdef ;
  const char prev[] =
    "The get Flash(or ram ) read result [0xaddr]=(0xvalue)\r\nThis addr is Hex\r\n";
 
  if ( ERROR == getladdr( _incmdtext1,4, &laddr))
 {
		fprintf(((FILE *)_Uart_Handle_), "%s\r\n",  prev );
      return SUCCESS;
	}
 
	if  ( _Set_NoralMode_ ==laddr)
	{			
		enChipMode =   _cChipRun_Mode_  ;
		fprintf(((FILE *)_Uart_Handle_), "%snormalrun\r\n",sSuccess );
	}
	else 	if (_Set_XiaobMode_ == laddr )
	{
		
		enChipMode =   _cChipXiaoBiao_Mode_ ;
		emu_refresh( &stChipdef, sizeof(stChipdef.stChipreg)/sizeof(stChipdef.stChipreg[0]));
		fprintf(((FILE *)_Uart_Handle_), "%sxiao biaomode\r\n",sSuccess );
	}
	else if (_Set_XiaoSave_ == laddr )
	{		
	Emu_Var_Write();
	fprintf(((FILE *)_Uart_Handle_), "%sxianbiadatawrite\r\n",sSuccess );
 	
 
	} else if ((_Set_XiaobIB_ == laddr ) && (  _IsXBMode(enChipMode)))
	{
		
		Chip_Trim_IB();
		fprintf(((FILE *)_Uart_Handle_), "%sxianbiaoIB\r\n",sSuccess );
 	

	} else if ((_Set_Xiaob5l_== laddr ) && (  _IsXBMode(enChipMode)))
	{
		
		  Chip_Trim_5l( );
	fprintf(((FILE *)_Uart_Handle_), "%sxiao  baiao 0.5L\r\n",sSuccess );
 	

	} else if ((_Set_Xiaob5persent_ == laddr ) && (  _IsXBMode(enChipMode)))
	{
		 Chip_Trim_5();
	fprintf(((FILE *)_Uart_Handle_), "%sxianbiadatawrite\r\n",sSuccess );
 	

	} else if ((_Get_Electr_ == laddr ) )
	{
		fprintf(((FILE *)_Uart_Handle_), "%svol=%d,curr=%d,power=%d\r\n",sSuccess ,	RamData.Vph.sVI,RamData.Iph.sVI,RamData.Pph.sVI );
	}  else if ( ( 	_Meter_Clear_ == laddr ) )			//	 电表清0 
	{
			System_Init_app() ;//System_Init();
			fprintf(((FILE *)_Uart_Handle_), "%smemter clear ok\r\n",sSuccess );
			cResetSecTm = 3;//while(1);
	}
	 else if ( ( 	_Read_DianLian_ == laddr ) )			//	 电表清0 
	{
			
			fprintf(((FILE *)_Uart_Handle_), "%zhz_dianliang=%d,imp=%d,fxz_dianliang=%d,imp=%d\r\n",RamData.LVSave[0].SumUse.Use,RamData.LVSave[0].SumUse.Imp,RamData.LVSave[0].SumUse.RevUse,RamData.LVSave[0].SumUse.RevImp);
			//cResetSecTm = 3;//while(1);
	}
  return SUCCESS;
}