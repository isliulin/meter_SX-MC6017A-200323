#ifndef  _soft_timer_filter_HPP
#define  _soft_timer_filter_HPP


 
#include "bit_edge.h"

#define  cSysTimer(n)   stbitp[n].sttimerstatu.timer	// 定时器值
#define  bSysTimein(n)  stbitp[n].sttimerstatu.bin

#define   Get_Timebit(iNo)  	bSysTimein(iNo)
#define   Get_TimerSV(iNo) 		cSysTimer(iNo)
#define		Set_TimerSV(iNo)		cSysTimer(iNo)

#define 	Close_SoftTime(iNo)   { Set_SoftTime ( iNo , 0 , 0 );    }

#define  KeepSelf(n) 	(Get_TimerSV(n) ||  cGetB(n))
void softtimer_run (unsigned short  iNo, unsigned char clen);	//      定时器运行
void Set_SoftTime (unsigned short  cNo, unsigned char cStatu, unsigned char cTimeout);	//      设置定时器
void RSet_SoftTime (unsigned short  cNo, unsigned char cStatu, unsigned char cTimeout);	//      设置定时器


 /*
    #define Set_SoftTime(iNo,cStatu,cTimeout)     { if (!cStatu) RamData.stsoftcon.bbitstatu[iNo].cbit = 0 ; else if ( 0 == bSysTimein(iNo) )           {               bSysTimein(iNo)  = 1;           cSysTimer(iNo)  = cTimeout;      }      }                       //      设置定时器
  */

#define		mstimerun()			  	Do_Task(bInt_Ms	,softtimer_run (_cn_start_timer_ms, _cn_end_timer_ms - 	_cn_start_timer_ms))	//      定时器运行
#define		halfsectimerun()		Do_Task( bInt_HalfSec	 ,softtimer_run (_cn_start_timer_half , _cn_end_timer_half - _cn_start_timer_half ))	//      定时器运行
#define		sectimerun()			  Do_Task( bInt_SecBit,softtimer_run (_cn_start_timer_sec, _cn_end_timer_sec-_cn_start_timer_sec))	//      定时器运行
#define  rtcsectimerun      Do_Task(  bInt_SecRtc	,softtimer_run ( _cn_start_timer_rtcsec  , _cn_end_timer_rtcsec- _cn_start_timer_rtcsec ))	//      定时器运行
#define  mintimerun      Do_Task(  bInt_MinRtc	,softtimer_run ( 	_cn_start_timer_min   , _cn_end_timer_min- 	_cn_start_timer_min  ))	//      定时器运行


#endif
