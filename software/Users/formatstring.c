
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "formatstring.h"

#define 	SUCCESS	 0
#define	 FAIL 		1

struct STFORMATOUT stformatout;
va_list argp;			/* 定义保存函数参数的结构 */

struct  STDATETIME
{
  unsigned char  Second;			//秒  
  unsigned char  Minute;			//分 
  unsigned char  Hour;				//时  
  unsigned char  Day;					//日  
  unsigned char  Month;				//月  
  unsigned char  Year;				//年   
} ;

void
FillRam (char *Dst, char Data, char Length)
{
  while (Length--)
    {
      *Dst++ = Data;
    }
}
 
 void
mem2mem (char *Dst, char *Src )
{
  unsigned int i = _ilen;
  while (i--)
    {
      if (_bRoper)
	*Dst++ = Src[i];
      else
	*Dst++ = *Src++;
    }

} 
char
HEX (char Data)
{				//0x11 ->0x0b
  return ((Data) - (((Data) / 16) * 6));
}

char
BCD (char Data)
{				//0x1f -> 0x31
  return ((Data) + (((Data) / 10) * 6));
}

 
long
Hex_BCD (long Data, char *Dst)
{				//01
  char a;
  char n = 4;

  while (n--)
    {
      a = Data % 100;
      Data = Data / 100;
      Dst[n] = BCD (a);
    }
  return *((long *) (Dst));
}

long
BCD_Hex (long Data)
{				//01
  char a;
  long x;

  a = Data;
  x = HEX (a);

  a = Data >> 8;
  x += HEX (a) * 100;

  a = Data >> 16;
  x += (long) 10000 *HEX (a);

  a = Data >> 24;
  x += (long) 1000000 *HEX (a);
  return x;
}

char
BCD_Inc (char d)
{
  char a;

  a = HEX (d);
  a++;
  return BCD (a);
}

long
abs32 (long x)
{
  if (x & 0x80000000)
    {
      x = 0 - x;
    }
  return x;
}

 
unsigned long 
GetTimeCnt (unsigned char  * stemp)
{
  struct STDATETIME DT ;
  unsigned char  T, K,i;
  unsigned long T32, TimeData = 0;

	for(i = 0; i < sizeof ( struct STDATETIME); i++)
		 ((unsigned char  *)&DT)[i] = HEX(stemp[i]);
		 
  	if((DT.Day==0)||(DT.Day>0x31)||(DT.Month > 12)){
		return 0;
	}
  for (T = 0, K = 0; T < DT.Year; T++)
    {				//累加年份        
      K = T & 0x03;
      TimeData += K ? 31536000 : 31622400;
    }
  for (T = 1; T < DT.Month; T++)
    {
      if (T != 2)
	{			//统计平月  
	  T32 = ((T + (T >> 3)) & 0x01) ? 2678400 : 2592000;
	}
      else
	T32 = K ? 2419200 : 2505600;	//统计闰月  
      TimeData += T32;
    }
  T32 = DT.Day - 1;
  TimeData += T32 * 86400;	//统计天  
  T32 = DT.Hour;
  TimeData += T32 * 3600;	//统计时  
  T32 = DT.Minute;
  TimeData += T32 * 60;		//统计分  
  TimeData += DT.Second;	//统计秒  
  return TimeData;
} 

const unsigned char  Const699[] ="\x99\x99\x99\x99\x99\x99";
  
void
GetCntTime (unsigned char  * DT1, unsigned long TimeData)
{
struct  STDATETIME  *DT  = (struct STDATETIME *) DT1;
  unsigned char  T, K,i;
  unsigned long T16, K16;
	if ( 0 == memcmp ( Const699 , ( unsigned char  *)&TimeData , sizeof(TimeData ) ))
	{
			FillRam  ( DT1 ,  0, sizeof (struct  STDATETIME));
			return ;
 }
  K16 = TimeData % 86400;	//分离时/分/秒  
  T16 = TimeData / 86400;	//分离年/月/日  
  DT->Second = K16 % 60;	//计算秒  
  K16 /= 60;
  DT->Minute = K16 % 60;	//计算分  
  K16 /= 60;
  DT->Hour = K16 % 24;		//计算时     
  for (K16 = 0, T = 0; T < 150; T++)
    {				//按天累计年份  
      K16 = (T & 0x03) ? 365 : 366;	//统计闰年  
      if (T16 < K16)
	{
	  K16 -= 337;
	  break;
	}			//计算当年二月份天数  
      else
	T16 -= K16;		//减掉年份   
    }
  DT->Year = T;			//计算得到相对年份  
  for (T = 1; T < 13; T++)
    {
      K = (T == 2) ? K16 : (((T + (T >> 3)) & 0x01) + 30);	//统计当月天数  
      if (T16 >= K)
	T16 -= K;		//减掉当前月  
      else
	break;
    }
  DT->Month = T;		//计算得到当年中的月份  
  DT->Day = T16 + 1;		//计算得到当年中的天  
	
		for(i = 0; i < sizeof ( struct STDATETIME); i++)
		 ((unsigned char  *)DT)[i] = BCD(((unsigned char  *)DT)[i]);
		 
		  
}


 
void
getnum (char *d, char *p)
{
  _bFirstzero = (*p) ? 0 : 1;
  while ((*p > '0') && (*p <= '9'))
    *d++ = *p++;
  *d = 0;
}


void usart_file_device_Init(void)
{
// 	  _cpGlobal = (*( struct STSCIBUF *)csDevice).DataBuf;
}	
	 
	 

char
format_msg (void)
{
  char stemp[8];
 
  char ctemp;
	char	*operp;
	long		ltemp;
	int			itemp;


  _ilen = 0;
  _bRoper = 0;
  _bFirstzero = 0;
	_bRight	 = 0;
	
  while ((*_cpMsgp != '%') && (*_cpMsgp != 0))
    {
      if ((*_cpMsgp >= '0') && (*_cpMsgp <= '9'))
	{
	  getnum (stemp, _cpMsgp);
	  _ilen = atoi (stemp);
	  _cpMsgp += strlen (stemp);

	}
      switch (*_cpMsgp)
	{
		
	case 'b':		//      将指定字符填充
		ctemp =va_arg(argp,char );
	  FillRam (_cpGlobal,  ctemp , _ilen);
	  return SUCCESS;

	case 's':		// 字符串
			operp = va_arg (argp,char *);
	  if (!_ilen)
	    _ilen = strlen (operp);
	  if (_ilen > strlen (operp))
	    _ilen = strlen (operp);
	  mem2mem (_cpGlobal, operp );
	  return SUCCESS;
		
	case 'l':		//	 长整型 
		ltemp =  va_arg (argp,long);
		_ilen = sizeof( long);
		mem2mem(  _cpGlobal, (char *)&ltemp   );
		 return SUCCESS;

	case 'd':		// 整型数  
		itemp =  va_arg (argp,int);
		_ilen = sizeof( int );
		mem2mem(  _cpGlobal, (char *)&itemp   );
		 return SUCCESS;

	case 'x':		// 输出格式为16进制数
	case 'X':

	case 'p':		// 指针，指定长度
	 	if ( 0 == _ilen ) return ; 
		operp =  va_arg (argp,char *);
	  mem2mem (_cpGlobal, operp  );
	  return SUCCESS;

	case 'r':		// 反向
	  _bRoper = 1;
	  break;
		
	case 'q':  // 右对齐
		_bRight	 = 1 ; 
		break;
	 		
	 case 't':		// Time2cnt
	  	if ( 0 == _ilen ) _ilen = sizeof ( struct STDATETIME) /sizeof(char);
	  	operp = va_arg (argp,char *);
	  	ltemp = GetTimeCnt ( operp) ;
		mem2mem ( _cpGlobal, (char *)&ltemp  );
  return SUCCESS;

	case 'm':		//     cnt2Time
		if ( 0 == _ilen ) _ilen = sizeof ( struct STDATETIME)/sizeof(char) ;
		ltemp = va_arg (argp,unsigned long );
		GetCntTime ( stemp , ltemp);
		mem2mem(   _cpGlobal,stemp  );
  return SUCCESS;

	
	case 'o':		//	
	
	case '\n':   // 带缓冲
	   
						
	  break;
	}
	_cpMsgp++;
    }
}


/* ANSI标准形式的声明方式，括号内的省略号表示可选参数 */
char
vformatstring(char *msg, ...)
{
  
  char i;
  char *_cpMsgend;
 
  va_start (argp, msg);
  _cpMsgp = msg;
  _cpMsgend = msg + strlen (msg);

  while (_cpMsgp < _cpMsgend)
    {
      if ('%' == *_cpMsgp)
	{
		_cpMsgp++;
		i = format_msg ();
		 _cpGlobal += _ilen;
	  _cpMsgp++;
	 
	  if (FAIL == i)
	    return FAIL;
	  continue;
 	}
      else
	*_cpGlobal++ = *_cpMsgp++;
    }

  va_end (argp);		/* 将argp置为NULL */
  return 0;
}

 

