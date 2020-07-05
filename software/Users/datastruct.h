#ifndef  _MAIN_DATASTRUCT_HPP
#define  _MAIN_DATASTRUCT_HPP

 

#include "appbit.h"

/*
struct stINSTATE		// �ж�λ����
{
  u16 iIntstate;
  u16 itempstate;		// ��ʱ״̬

};*/

struct  STBITIO
{
	unsigned char  	bMs:1;		//�����¼�(ÿ5������1)
	unsigned char 	bHalfSec:1;   	//500MS״̬
	unsigned char 	bSecBit:1;   	//1000MS״̬
	unsigned char 	bSecRtc:1;	//RTC��仯
	unsigned char 	bMinRtc:1;	//RTC��仯
	unsigned char 	bDayZero:1;//0���־
	unsigned char  	bBeep:1;//ѭ�Լ�
	unsigned char  	bbBpschange:1;//BPS�޸�
	unsigned char  	bmagnetism:1;		//      IC�����룬1���롣0δ����
	unsigned char		ctemp; //		 �˱�־�ϲ���ÿ����ˢ���²���־���첽��λ
	unsigned char 	bMcuReset:1;		//RTC��ˢ��־
	unsigned char 	bRtcRefresh:1;		//RTC��ˢ��־
	unsigned char   bjb:1;		// У����
	unsigned char   bjbDataStore:1;		// У�����ݴ洢
	unsigned char   bjbDataReset:1;		// У�����ݸ�λ
	unsigned char   bEmcModuleReset:1;		// EMCģ�鸴λ
	unsigned char 	bEmcDataRefresh:1;		//��У��������ˢ 
	unsigned char 	bEmcDataRestoredef:1;  // 	 ����
	
	//unsigned char   bjbDataStore:1;		// У�������Ƴ���
	//unsigned char   bjbDataStore:1;		// У�������Ƴ���
	
};

//      �ж�״̬���壬�����������״̬�ϸ��Ӧ
#define	  bInt_Ms_io						stbitio.bMs	//�����¼�(ÿ5������1)
#define	  bInt_HalfSec_io				stbitio.bHalfSec	//500MS״̬
#define	  bInt_SecBit_io				stbitio.bSecBit	//1000MS״̬
#define	  bInt_SecRtc_io		 		stbitio.bSecRtc	//RTC��仯
#define	  bInt_MinRtc_io				stbitio.bMinRtc	//RTC��仯
 
#define		bInt_Eopen_Exti_io  	stbitio.bEopen_Exti	//�������ѱ�־����λ�����ڵ͹���״̬�µĻ���
#define		bInt_IRKey_io					stbitio.bIRKey	//����ģ�ⰴ��
#define		bInt_DayZero_io 			stbitio.bDayZero	//0���־
#define   bInt_KeyProg_io				stbitio.bKeyProg	//��̼�
#define   bInt_KeyPress_io			stbitio.bKeyPress	//ѭ�Լ�
#define 	bInt_bBeepStatu_io		stbitio.bBeep	//ѭ�Լ�
#define 	bInt_bBpschange_io		stbitio.bbBpschange //BPS�޸�
#define		bInt_bmagnetism_io				stbitio.bmagnetism	//      IC�����룬1���롣0δ����
#define		bInt_HalfSecStatu_io	stbitio.bHalfSecStatu	//      IC�����룬1���롣0δ����

//      ����״̬���壬�����������״̬�ϸ��Ӧ
#define	  bInt_Ms						stbitapp.bMs	//�����¼�(ÿ5������1)
#define	  bInt_HalfSec			stbitapp.bHalfSec	//500MS״̬
#define	  bInt_SecBit				stbitapp.bSecBit	//1000MS״̬
#define	  bInt_SecRtc		 		stbitapp.bSecRtc	//RTC��仯
#define	  bInt_MinRtc				stbitapp.bMinRtc	//RTC��仯
 
#define		bInt_Eopen_Exti  	stbitapp.bEopen_Exti	//�������ѱ�־����λ�����ڵ͹���״̬�µĻ���
#define		bInt_IRKey				stbitapp.bIRKey	//����ģ�ⰴ��
#define		bInt_DayZero 			stbitapp.bDayZero	//0���־
#define   bInt_KeyProg			stbitapp.bKeyProg	//��̼�
#define   bInt_KeyPress			stbitapp.bKeyPress	//ѭ�Լ�
#define 	bInt_bBeepStatu		stbitapp.bBeep	//ѭ�Լ�
#define 	bInt_bBpschange		stbitapp.bbBpschange //BPS�޸�
#define		bInt_bmagnetism		stbitapp.bmagnetism	//      IC�����룬1���롣0δ����
#define		bInt_HalfSecStatu 	stbitapp.bHalfSecStatu	//      IC�����룬1���롣0δ����
 
 #define  _bEvent_Set(a)			{stbitio.a =1;}
 #define  _bEvent_Get(a)			( stbitapp.a)

#define			_cnRESET_EN    0
#define    _cnRESET_DIS		0xd0


extern struct  STBITIO   stbitio;					//	 �ж�IO
extern struct  STBITIO   stbitapp;					//		Ӧ��IO


#endif
