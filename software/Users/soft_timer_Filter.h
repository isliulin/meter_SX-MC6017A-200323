#ifndef  _soft_timer_filter_HPP
#define  _soft_timer_filter_HPP


 
#include "bit_edge.h"

#define  cSysTimer(n)   stbitp[n].sttimerstatu.timer	// ��ʱ��ֵ
#define  bSysTimein(n)  stbitp[n].sttimerstatu.bin

#define   Get_Timebit(iNo)  	bSysTimein(iNo)
#define   Get_TimerSV(iNo) 		cSysTimer(iNo)
#define		Set_TimerSV(iNo)		cSysTimer(iNo)

#define 	Close_SoftTime(iNo)   { Set_SoftTime ( iNo , 0 , 0 );    }

#define  KeepSelf(n) 	(Get_TimerSV(n) ||  cGetB(n))
void softtimer_run (unsigned short  iNo, unsigned char clen);	//      ��ʱ������
void Set_SoftTime (unsigned short  cNo, unsigned char cStatu, unsigned char cTimeout);	//      ���ö�ʱ��
void RSet_SoftTime (unsigned short  cNo, unsigned char cStatu, unsigned char cTimeout);	//      ���ö�ʱ��


 /*
    #define Set_SoftTime(iNo,cStatu,cTimeout)     { if (!cStatu) RamData.stsoftcon.bbitstatu[iNo].cbit = 0 ; else if ( 0 == bSysTimein(iNo) )           {               bSysTimein(iNo)  = 1;           cSysTimer(iNo)  = cTimeout;      }      }                       //      ���ö�ʱ��
  */

#define		mstimerun()			  	Do_Task(bInt_Ms	,softtimer_run (_cn_start_timer_ms, _cn_end_timer_ms - 	_cn_start_timer_ms))	//      ��ʱ������
#define		halfsectimerun()		Do_Task( bInt_HalfSec	 ,softtimer_run (_cn_start_timer_half , _cn_end_timer_half - _cn_start_timer_half ))	//      ��ʱ������
#define		sectimerun()			  Do_Task( bInt_SecBit,softtimer_run (_cn_start_timer_sec, _cn_end_timer_sec-_cn_start_timer_sec))	//      ��ʱ������
#define  rtcsectimerun      Do_Task(  bInt_SecRtc	,softtimer_run ( _cn_start_timer_rtcsec  , _cn_end_timer_rtcsec- _cn_start_timer_rtcsec ))	//      ��ʱ������
#define  mintimerun      Do_Task(  bInt_MinRtc	,softtimer_run ( 	_cn_start_timer_min   , _cn_end_timer_min- 	_cn_start_timer_min  ))	//      ��ʱ������


#endif
