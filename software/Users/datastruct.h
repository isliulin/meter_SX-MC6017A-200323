#ifndef  _MAIN_DATASTRUCT_HPP
#define  _MAIN_DATASTRUCT_HPP

 

#include "appbit.h"

/*
struct stINSTATE		// 中断位定义
{
  u16 iIntstate;
  u16 itempstate;		// 临时状态

};*/

struct  STBITIO
{
	unsigned char  	bMs:1;		//毫秒事件(每5毫秒置1)
	unsigned char 	bHalfSec:1;   	//500MS状态
	unsigned char 	bSecBit:1;   	//1000MS状态
	unsigned char 	bSecRtc:1;	//RTC秒变化
	unsigned char 	bMinRtc:1;	//RTC秒变化
	unsigned char 	bDayZero:1;//0点标志
	unsigned char  	bBeep:1;//循显键
	unsigned char  	bbBpschange:1;//BPS修改
	unsigned char  	bmagnetism:1;		//      IC卡插入，1插入。0未插入
	unsigned char		ctemp; //		 此标志上部是每次重刷，下部标志是异步置位
	unsigned char 	bMcuReset:1;		//RTC重刷标志
	unsigned char 	bRtcRefresh:1;		//RTC重刷标志
	unsigned char   bjb:1;		// 校表函数
	unsigned char   bjbDataStore:1;		// 校表数据存储
	unsigned char   bjbDataReset:1;		// 校表数据复位
	unsigned char   bEmcModuleReset:1;		// EMC模块复位
	unsigned char 	bEmcDataRefresh:1;		//将校表数据重刷 
	unsigned char 	bEmcDataRestoredef:1;  // 	 数据
	
	//unsigned char   bjbDataStore:1;		// 校表数据破除计
	//unsigned char   bjbDataStore:1;		// 校表数据破除计
	
};

//      中断状态定义，必需和主程序状态严格对应
#define	  bInt_Ms_io						stbitio.bMs	//毫秒事件(每5毫秒置1)
#define	  bInt_HalfSec_io				stbitio.bHalfSec	//500MS状态
#define	  bInt_SecBit_io				stbitio.bSecBit	//1000MS状态
#define	  bInt_SecRtc_io		 		stbitio.bSecRtc	//RTC秒变化
#define	  bInt_MinRtc_io				stbitio.bMinRtc	//RTC秒变化
 
#define		bInt_Eopen_Exti_io  	stbitio.bEopen_Exti	//按键唤醒标志，该位仅用于低功耗状态下的唤醒
#define		bInt_IRKey_io					stbitio.bIRKey	//红外模拟按键
#define		bInt_DayZero_io 			stbitio.bDayZero	//0点标志
#define   bInt_KeyProg_io				stbitio.bKeyProg	//编程键
#define   bInt_KeyPress_io			stbitio.bKeyPress	//循显键
#define 	bInt_bBeepStatu_io		stbitio.bBeep	//循显键
#define 	bInt_bBpschange_io		stbitio.bbBpschange //BPS修改
#define		bInt_bmagnetism_io				stbitio.bmagnetism	//      IC卡插入，1插入。0未插入
#define		bInt_HalfSecStatu_io	stbitio.bHalfSecStatu	//      IC卡插入，1插入。0未插入

//      正常状态定义，必需和主程序状态严格对应
#define	  bInt_Ms						stbitapp.bMs	//毫秒事件(每5毫秒置1)
#define	  bInt_HalfSec			stbitapp.bHalfSec	//500MS状态
#define	  bInt_SecBit				stbitapp.bSecBit	//1000MS状态
#define	  bInt_SecRtc		 		stbitapp.bSecRtc	//RTC秒变化
#define	  bInt_MinRtc				stbitapp.bMinRtc	//RTC秒变化
 
#define		bInt_Eopen_Exti  	stbitapp.bEopen_Exti	//按键唤醒标志，该位仅用于低功耗状态下的唤醒
#define		bInt_IRKey				stbitapp.bIRKey	//红外模拟按键
#define		bInt_DayZero 			stbitapp.bDayZero	//0点标志
#define   bInt_KeyProg			stbitapp.bKeyProg	//编程键
#define   bInt_KeyPress			stbitapp.bKeyPress	//循显键
#define 	bInt_bBeepStatu		stbitapp.bBeep	//循显键
#define 	bInt_bBpschange		stbitapp.bbBpschange //BPS修改
#define		bInt_bmagnetism		stbitapp.bmagnetism	//      IC卡插入，1插入。0未插入
#define		bInt_HalfSecStatu 	stbitapp.bHalfSecStatu	//      IC卡插入，1插入。0未插入
 
 #define  _bEvent_Set(a)			{stbitio.a =1;}
 #define  _bEvent_Get(a)			( stbitapp.a)

#define			_cnRESET_EN    0
#define    _cnRESET_DIS		0xd0


extern struct  STBITIO   stbitio;					//	 中断IO
extern struct  STBITIO   stbitapp;					//		应用IO


#endif
