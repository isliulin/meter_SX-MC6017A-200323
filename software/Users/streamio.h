#ifndef		_STREAM_IO_HPP
#define		_STREAM_IO_HPP

#include "ht6xxx_lib.h"
#include "pubset.h"
#include <stdlib.h>
//#include <t16_ch3.h>
//#include <t16_ch1.h>
#include "uartio.h"
#include <string.h>
#include <stdio.h>
#define	 _ENYDATA_	0x33

struct STCOMLIST
{
  HT_UART_TypeDef *unitp;
  unsigned int ibps1;
  unsigned int itemp;
};

enum ENUMCHANDEL
{
  _CHANEL_Start_ = 0,
  _R485_Chanel_ = _CHANEL_Start_,
  _IR_Chanel_,
  _Com_End_,
  _CHANEL_END_ = _Com_End_,
  _ComChanel_End_ = _Com_End_,
  _Com_start_ = _IR_Chanel_,
  _Line_Chanel_
};
#define		_cncSilence_Set_    50

#define		Event_USART1		USARTCOM[_R485_Chanel_].bEventRec645
#define		Event_USART2		USARTCOM[_Line_Chanel_].bEventRec645
#define		Event_USART3		USARTCOM[_IR_Chanel_].bEventRec645
#define   	Event_USART			( Event_USART1 || Event_USART2 || Event_USART3 )
#define    _R485_config_    *(stcomchanel[_R485_Chanel_].sp)	// FlashInfo.SetInfo.Baud[1]
#define    _Line_config_   	*(stcomchanel[_Line_Chanel_].sp)	//FlashInfo.SetInfo.Baud[2]
#define    _IR_config_     	*(stcomchanel[_IR_Chanel_].sp)	//FlashInfo.SetInfo.Baud[0]

struct STUARTUNIT
{
  STUART *unitp;
  unsigned int iNo;
  IRQn_Type IRQn;
  void (*portinit) (void);
  void (*stopinit) (void);
};

extern const struct STUARTUNIT stuartunit[_ComChanel_End_];

enum RECESTATE
{
  _first_68_ = 0,
  _second_68_,
  _frame_sum_,
  _rece_endchar_,
  _rece_succ_
};

enum SENDSTATE
{
  _prev_char_state = 0,		// ǰ���ַ�fe
  _prev_68_state = 2,		// ǰ���ַ�68,�˴���ʼ�ۼӺ�
  _macaddr_start = 4,		//  MAC��ַ
  _succ_68_state = 6,		// ���ַ�68
  _explain_secondchar_start = 8,	//  ǰ�������ֽ�,������� ����
  _explain_text = 10,		//      ��ʽ����
  _end_sum = 12,		//      �ۼӺ�
  _end_char = 14,		//      ������
  _end_sendcom = 16
};
enum SENDSTATU
{
  _cnWaitSend_,
  _cnSendOver_
};
extern volatile struct STSCIBUF USARTCOM[_Com_End_];
//extern unsigned char SCI_Num;
extern unsigned char cIRSend;	//

struct STSCIBUF
{
  unsigned char cStatu:5;
  unsigned char bSendSum:1;	//  �ۼӺ�У��
  unsigned char bSendEny:1;	//  ����+33
 // unsigned char bEventRec:1;	//  645���ճɹ���־
  unsigned char bEventRec645:1;	//  645���ճɹ���־
  unsigned char bResetio:1;	//       RESET��־ 
  unsigned char cPSend_len;	//���ͳ���
  unsigned char *cPSend_start;	//����ָ��        
volatile unsigned char cSilence;	//��Ĭ�ڶ�ʱ��
//volatile unsigned short cSilence;	//��Ĭ�ڶ�ʱ��
  unsigned short iSendTm;	//             
  unsigned char cHead;		//��ǰ����ָ��
  unsigned int iBps;
  unsigned int iResetTm;	//           �����ʸ�λ��ʱ
  unsigned char DataBuf[255 + 8];
  unsigned char cSum;		// �ۼӺ�
  unsigned char cUnitno;
};

#define			_cnSCIBuf_			(int)&(((struct STSCIBUF *)0)->DataBuf)
#define		nSCI SCI[SCI_Num]
void stream_cSilence_fun (void);
unsigned char stream_rece_fun_645 (struct STSCIBUF *p, unsigned char dr);	//unsigned char  clineno, unsigned char  sr, unsigned char  dr)
unsigned char stream_send_fun_645 (struct STSCIBUF *p);
void SCI_init (strSCIBuf * p);
/*
@far @ interrupt void stream_changel1_rece_fun (void);
@far @ interrupt void stream_changel2_rece_fun (void);
@far @ interrupt void stream_changel3_rece_fun (void);

@far @ interrupt void stream_chanel1_send_fun (void);
@far @ interrupt void stream_chanel2_send_fun (void);
@far @ interrupt void stream_changl3_send_fun (void);
*/
void stream_refresh (enum ENUMCHANDEL i);
#define		 Rst_SCI(a) 		stream_refresh(a)	// { _asm("sim") ;SCI[SCI_Num].cStatu =_first_68_  ;_asm("rim");} // SetBaud( SCI_Num )
#define    stream_changel1_Reset()  		 		{ SCI[_R485_Chanel_].bResetio = 1 ; }
#define    stream_changel2_Reset()  		 		{ SCI[Line_SCI].bResetio = 1 ; }
#define    stream_changel3_Reset()  		 		{ SCI[IR_SCI].bResetio = 1 ; }

#define _iFLASHBLOCK_		2048	// Flash
#define  _s19format_clen_  	128	//s19�ļ���ʽ������������󳤶�
enum ENUMS19COMDLIST
{
  _S19CMD_RESET_ = 'r',
  _S19CMD_READ_ = 'g',
  _S19CMD_WRITE_ = 'w',
  _S19CMD_RETRURN_ = 'n'
};

enum S19FORMATRECESTATE
{
  _prev_first_ = 0,
  _prev_succ_,
  _clen_,
  _ReceData_,
  _ReceSum_
};

struct STBOOTLOADCMD
{
  unsigned char sprev[2];
  unsigned char cLen;
  unsigned char sAddr[3];
  unsigned char sData[_s19format_clen_];
  unsigned char cSum;
};

struct STBOOTLOADCOMBUF
{
  unsigned char cStatu;
  unsigned char cHead;
  unsigned char cSum;
  unsigned char cSilence;
  unsigned char cOK;
  struct STBOOTLOADCMD *stp;
};

struct STS19_S2
{
  unsigned char sprev[2];
  unsigned char cLen;
  unsigned char sAddr[3];
  unsigned char sData[_s19format_clen_];
  unsigned char cSum;
};

struct STFLASHWRITEBUF
{
  unsigned long lAddr;		//       ��ʼ��ַ
  unsigned char sData[_iFLASHBLOCK_];	//      ������
};

enum ENUMBOOTLOADERR
{
  _BootLoad_Success_ = 0,
  _ErrBootLoad_StartAddr_No2k_,	//   ��ʼ�����ַ��
  _ErrBootLoad_AddrOVer_,	//
  _ErrBootLoad_CurrBlockOVer_,	//

};
enum ENUMCHANDEL GetSCINUM (strSCIBuf * p);
enum ENUMCHANDEL Uart_Struct_Get (STUART * stp);
void stream_reset (enum ENUMCHANDEL i);
char stream_line_check (enum ENUMCHANDEL i);
#endif
