#include "streamio.h"
#include "pubset.h"
#include "predef.h"
 
const char csprev[] = "\xfe\xfe";
const char cssucc[] = "\x68";
const char csendchar[] = "\x16";

#define			_iComResetTm_		1500
#define 		_iComSilence_		(_iComResetTm_/2)	//
#define 		_iComSendDelay	500	//
extern const unsigned char cSysTickTm;
const unsigned short iResetTmMax = _iComResetTm_ / _cSysTickTm_;
const unsigned short cSilenceMax = _iComSilence_ / _cSysTickTm_;
const unsigned short iSendTmMax = _iComSendDelay / _cSysTickTm_;
extern unsigned short iTurnOnTm;
extern unsigned short iTurnOffTm;

#define 	_cTRx_Num_ 			sizeof(FlashInfo.SafeInfo.TRx_Num)	//  通讯地址长度
static unsigned char sAddr[_cTRx_Num_];
#define 	sTRx_Num  			 sAddr

#if _cncSilence_Set_ >= 128
sdfasdfasfds
#endif
const struct STUARTUNIT stuartunit[_Com_End_] = {
  {HT_UART0, _R485_Chanel_, UART0_IRQn, NULL, NULL},
  {HT_UART3, _IR_Chanel_, UART3_IRQn, NULL, NULL},
};

enum ENUMCHANDEL
GetSCINUM (strSCIBuf * p)
{
  enum ENUMCHANDEL i;

  for (i = _CHANEL_Start_; i < _Com_End_; i++)
    if (p == (SCI + i))
      return i;
  return _Com_End_;
}

enum ENUMCHANDEL
Uart_Struct_Get (STUART * stp)
{
  enum ENUMCHANDEL i;

  for (i = _CHANEL_Start_; i < _Com_End_; i++)
    if (stp == stuartunit[i].unitp)
      return i;
  return _Com_End_;
}

unsigned char
stream_rece_fun_645 (struct STSCIBUF *p, unsigned char dr)	//unsigned char  clineno, unsigned char  sr, unsigned char  dr)
{
  Do_Task (((p->cStatu != _first_68_)
	    && (p->cHead > sizeof (p->DataBuf) - 1)), return SUCCESS);
a1:
  switch (p->cStatu)
    {
    case _first_68_:
      if (cssucc[0] == dr)
	{
	  p->cHead = 0;
	  p->bSendSum = 1;
	  p->cSum = 0;
	  p->bSendEny = 0;
	  p->cStatu++;
	}
      break;

    case _second_68_:
      Do_Task (7 == p->cHead, p->cStatu =
	       (cssucc[0] == dr) ? p->cStatu + 1 : _first_68_);
      break;

    case _frame_sum_:
      if (p->cHead > 9)
	{
	  p->bSendEny = 1;
	  if (p->cHead == p->DataBuf[9] + 10)
	    p->cStatu = (p->cSum == dr) ? p->cStatu + 1 : _first_68_;

	}
      break;

    case _rece_endchar_:
      Do_Task (csendchar[0] == dr, p->bEventRec645 = 1);
      p->cStatu = _first_68_;
      break;
    }
  Do_Task (p->bSendSum, p->cSum += dr);
  Do_Task (p->bSendEny, dr -= _ENYDATA_);
  Do_Task (p->cStatu != _first_68_, p->DataBuf[p->cHead++] = dr);
  Do_Flow (_first_68_ == p->cStatu, p->cSilence = 0, p->cSilence =
	   _cncSilence_Set_);
  //if ( _first_68_ == p->cStatu )
//        p->cSilence  = 0 ;
//        else
//                 p->cSilence = 120;

  Do_Task (p->bEventRec645, return SUCCESS);
  Do_Task ((_first_68_ == p->cStatu) && (cssucc[0] == dr), goto a1;
    );
}

unsigned char
stream_send_fun_645 (struct STSCIBUF *p)
{
  unsigned char i;
  switch (p->cStatu)
    {
    case _prev_char_state:	// 前导字符
      p->bSendSum = p->bSendEny = 0;
      p->cSum = 0;
      p->cHead = sizeof (csprev) - 1;
      p->cPSend_start = (unsigned char *) csprev;
      p->cStatu++;
      break;

    case _prev_68_state:	// 前导字符68
    case _succ_68_state:	// 后导字符68
      p->bSendSum = 1;		//      发送累加和校验标志置位
      p->cHead = sizeof (cssucc) - 1;
      p->cPSend_start = (unsigned char *) cssucc;
      p->cStatu++;
      break;

    case _macaddr_start:	//  MAC地址
      for (i = 0; i < _cTRx_Num_; i++)
	sTRx_Num[i] = FlashInfo.SafeInfo.TRx_Num[_cTRx_Num_ - 1 - i];
      p->cHead = _cTRx_Num_;	//sizeofFlashInfo (FlashInfo.SetInfo.TRx_Num);
      p->cPSend_start = sTRx_Num;	//  FlashInfo.SetInfo.TRx_Num;
      p->cStatu++;
      break;

    case _explain_secondchar_start:	//  前导两个字节
      p->cHead = 2;
      p->cPSend_start = p->DataBuf;
      p->cStatu++;
      break;

    case _explain_text:	//      正式文字
      p->bSendEny = 1;
      p->cHead = p->cPSend_len - 2;
      p->cPSend_start = p->DataBuf + 2;
      p->cStatu++;
      Do_Flow (p->cHead, break, p->cStatu = _end_sum);

    case _end_sum:
      p->bSendSum = p->bSendEny = 0;
      p->cHead = 1;
      p->cPSend_start = &p->cSum;
      p->cStatu++;
      break;

    case _end_char:
      p->cHead = 1;
      p->cPSend_start = (unsigned char *) csendchar;
      p->cStatu++;
      break;
    }
  i = *(p->cPSend_start++);
  Do_Task (p->bSendEny, i += _ENYDATA_);
  Do_Task (p->bSendSum, p->cSum += i);
  return i;
}

/*
void
stream_cSilence_fun (void)
{
  enum ENUMCHANDEL i;
  for (i = 0; i < _Com_End_; i++)
    {
      if (USARTCOM[i].cSilence)
	{
	  USARTCOM[i].cSilence--;
	  Do_Task (0 == USARTCOM[i].cSilence, stream_refresh (i));
	}
    }
}
*/

void
stream_send_enable (unsigned char *p, unsigned int ilen)
{
  enum ENUMCHANDEL enChanel = GetSCINUM ((strSCIBuf *) p);

  USARTCOM[enChanel].cPSend_len =
    sizeof (USARTCOM[enChanel].DataBuf) >
    ilen ? ilen : sizeof (USARTCOM[enChanel].DataBuf);

  USARTCOM[enChanel].cPSend_start = USARTCOM[enChanel].DataBuf;
  USARTCOM[enChanel].cSilence = 0;
  USARTCOM[enChanel].cStatu = _prev_char_state;	// 前导字符
  UART_INTENABLE (stuartunit[enChanel].unitp, 1, 0);
		
	UART_SendIntEnable(stuartunit[enChanel].unitp);
		 USARTCOM[enChanel].iSendTm = iSendTmMax;
		
}

unsigned char
stream_send_over (unsigned short enChanel)
{
//-------       if ( IS_USART_SEND_INT(enChanel))
  return _cnWaitSend_;
  return _cnSendOver_;
}

void
stream_refresh (enum ENUMCHANDEL enChanel)
{
  UART_InitTypeDef UART_InitStructure;

  UART_InitStructure.UART_Logic = UartLogicPositive;	/*!< UART逻辑为正               */
  UART_InitStructure.UART_StopBits = OneStopBits;	/*!< 1位停止位                  */
  UART_InitStructure.UART_WordLength = WordLength_8Bits;	/*!< 8位数据位                  */
  UART_InitStructure.UART_Parity = UartParity_EVEN;	/*!< 偶校验                     */
  UART_InitStructure.UART_BaudRate = 2400;	/*!< 波特率2400                 */
  UART_InitStructure.ReceiveEN = ENABLE;	/*!< 接收使能   注：非中断使能  */
  UART_InitStructure.SendEN = ENABLE;	/*!< 发送使能   注：非中断使能  */

  UART_init (stuartunit[enChanel].unitp, UART_InitStructure);
  ClrRam ((char *) (USARTCOM + enChanel), _cnSCIBuf_);
  USARTCOM[enChanel].cStatu = _first_68_;
  UART_INTENABLE (stuartunit[enChanel].unitp, 0, 1);
}

/*
void SCI_init(strSCIBuf *p  )
{
	p[_R485_Chanel_].DataBuf = USARTCOM[_R485_Chanel_].DataBuf;
	p[_Line_Chanel_].DataBuf = USARTCOM[_Line_Chanel_].DataBuf;
	p[IR_SCI].DataBuf = USARTCOM[IR_SCI].DataBuf;
}
*/

#ifdef _DEL
unsigned short
cGetbpsConfig (unsigned char ctemp, char cDiv)
{
#ifdef _DEL
  extern const struct STBSPCONFIG stbpsconfig[8];
  unsigned char i;

  for (i = 0; (ctemp && (i < 7)); i++, ctemp /= 2)
    {
      if (ctemp & 1)
	{
	  if (cDiv)
	    return stbpsconfig[i].cCLKDIV;
	  return stbpsconfig[i].iBPSCONFIG;
	}
    }
  return _bps2400_;
#endif
  return 0;
}
#endif
unsigned short
Get_Changel_BpsConfig (unsigned short cSCINUM, char cDiv)
{
  extern const struct STCOMCHANEL stcomchanel[_Com_End_];
  unsigned char ctemp;

  switch (cSCINUM)
    {
    case _R485_Chanel_:
      ctemp = _R485_config_;
      break;

    case _Line_Chanel_:
      ctemp = _Line_config_;
      break;

    default:
    case _IR_Chanel_:
      return _bps1200_;		//ctemp = _IR_config_ ;
      break;
    }

//--return  cGetbpsConfig(  ctemp,cDiv) ;
}

/*
void
stream_refresh (unsigned short i)
{
  HT_UART_TypeDef *unitp = stcomlist[i].unitp;
  UARR_Ir_Chanel_Init (USARTCOM[i].iBps);
  memset ((char *) &USARTCOM[i], 0,
	  (unsigned long) &USARTCOM[i].iBps - (unsigned long) &USARTCOM[i]);
}
*/
const struct STCOMLIST stcomlist[_CHANEL_END_] = {
  {HT_UART3, 300, 0},
  {HT_UART0, 300, 0},
};

void
stream_reset (enum ENUMCHANDEL i)
{
  UART_InitTypeDef UART_InitStructure;
  HT_UART_TypeDef *unitp = stuartunit[i].unitp;

  UART_InitStructure.UART_Logic = UartLogicPositive;	/*!< UART逻辑为正               */
  UART_InitStructure.UART_StopBits = OneStopBits;	/*!< 1位停止位                  */
  UART_InitStructure.UART_WordLength = WordLength_8Bits;	/*!< 8位数据位                  */
  UART_InitStructure.UART_Parity = UartParity_EVEN;	/*!< 偶校验                     */
  UART_InitStructure.UART_BaudRate = 2400;	/*!< 波特率2400                 */
  UART_InitStructure.ReceiveEN = ENABLE;	/*!< 接收使能   注：非中断使能  */
  UART_InitStructure.SendEN = ENABLE;	/*!< 发送使能   注：非中断使能  */

  UARR_Ir_Chanel_Init (USARTCOM[i].iBps);
  memset ((char *) &USARTCOM[i], 0,
	  (unsigned long) &USARTCOM[i].iBps - (unsigned long) &USARTCOM[i]);
//--  USARTCOM[i].iResetTm = iResetTmMax;
}

void
stream_cSilence_fun (void)
{
  enum ENUMCHANDEL i;
  for (i = _CHANEL_Start_; i < _Com_End_; i++)
    {
      if ((_OffLine ()))
	UART_Close (stuartunit[i].unitp);
      if (USARTCOM[i].iResetTm)
	{
	  USARTCOM[i].iResetTm--;
	  Do_Task ((0 == USARTCOM[i].iResetTm), stream_reset (i));
	}

      if (USARTCOM[i].cSilence)
	{
	  USARTCOM[i].cSilence--;
	  Do_Task (0 == USARTCOM[i].cSilence, stream_refresh (i));
	}
      if (USARTCOM[i].iSendTm)
	{
	  USARTCOM[i].iSendTm--;
	  if (0 == USARTCOM[i].iSendTm)
	    {
	      if (USARTCOM[i].cPSend_len)
		{
		  USARTCOM[i].cHead = 0;			
		  //HT_UART3->UARTCON |= UART_UARTCON_TXIE;
		  //HT_UART3->SBUF = USARTCOM[i].DataBuf[USARTCOM[i].cHead++];
			stuartunit[i].unitp->SBUF = csprev[0];//USARTCOM[i].DataBuf[USARTCOM[i].cHead++];
		}
	      else
		{
		  stream_refresh (i);
		}
	    }
	}
    }
}

void
stream_fun_send (HT_UART_TypeDef * unitp)
{
  unsigned short iChanel = 0;

  USARTCOM[iChanel].iResetTm = iResetTmMax;
  if (USARTCOM[iChanel].cHead < USARTCOM[iChanel].cPSend_len)
    {
      unitp->SBUF = USARTCOM[iChanel].DataBuf[USARTCOM[iChanel].cHead++];
    }
  else
    {
      stream_refresh (iChanel);	//  USARTCOM[iChanel].cSilence =        1 ; 
      //HT_UART3->UARTCON  &= ~UART_UARTCON_TXIE  ; 
    }
}
#ifdef _DEL
int
fputc (int ch, FILE * fp)
{
  extern unsigned char cXorCheck;
  extern unsigned char cXorEnable;
  if (((unsigned int) fp == _Uart_Handle_))
    {
      USARTCOM[_IR_Chanel_].DataBuf[USARTCOM[_IR_Chanel_].cHead++] =
	(char) ch;
      if (cXorEnable)
	cXorCheck ^= ch;
      if (2 == ch)
	{
	  cXorEnable = 1;
	  cXorCheck = 0;
	}
      else if (3 == ch)
	{
	  cXorEnable = 0;
	}
      return 0;
    }
}

char
stream_line_check (enum ENUMCHANDEL i)
{
  unsigned char j, k;
  if (USARTCOM[i].cHead > 1)
    {
      j = USARTCOM[i].DataBuf[USARTCOM[i].cHead - 2] - '\r';
      k = USARTCOM[i].DataBuf[USARTCOM[i].cHead - 1] - '\n';
      j |= k;
      if (0 == j)
	return TRUE;
    }
  return FALSE;
}


void
stream_fun_rece (HT_UART_TypeDef * unitp)
{
  unsigned short iChanel = 0;

  SystemTick_Interrupt_Off ();
  USARTCOM[iChanel].iResetTm = iResetTmMax;
  USARTCOM[iChanel].cSilence = cSilenceMax;
  if (USARTCOM[iChanel].cHead < sizeof (USARTCOM[iChanel].DataBuf))
    {
      USARTCOM[iChanel].DataBuf[USARTCOM[iChanel].cHead++] = unitp->SBUF;
      USARTCOM[iChanel].DataBuf[USARTCOM[iChanel].cHead] = 0;
    }

  if (TRUE == stream_line_check (iChanel))
    {
      USARTCOM[iChanel].bEventRec = 1;
      USARTCOM[iChanel].cSilence = 0;
      USARTCOM[iChanel].iSendTm = iSendTmMax;
      HT_UART3->UARTCON &= ~UART_UARTCON_RXIE;
    }
  SystemTick_Interrupt_On ();
}
#endif
