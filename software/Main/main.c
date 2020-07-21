/*
*********************************************************************************************************
*                                 			   HT6XXX
*                            			      HT6XXX程序
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : main.c
* By           : SocTeam
* Version      : CurrentVersion
* Description  : 
*********************************************************************************************************
*/

#define  __MAIN_C
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
//#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
//#include "emu.h"
#include "pubset.h"
 
#include "flash_filesystem.h"
#include "eeprom.h"
 
 

extern const			unsigned short 		iBpsSet[7];
extern  const  strFileSet FileSet;
extern const  strFlashInfo DftFlashInfo ;
extern unsigned char cWDGTm;
extern unsigned  char			cncDianLianSave;
extern enum  ENUMCHIPMODE  enChipMode  ;  
 
void  UpDisp1( unsigned long  N);
unsigned char	 toascii(unsigned char i);
void   OnLine_Dete ( void);
extern volatile struct STSCIBUF  USARTCOM[_CHANEL_END_];
void SysTick_fun(void);
extern 	struct 	STDATALIST  stdatalist[] ;
void PMU_Init() ;
void  Com_fun(void);
void DianLian_Restore(void);

	unsigned long  MeterData_Get( struct 	STDATALIST  *stp);
/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            本地变量
*********************************************************************************************************
*/


void Hex_BCD(s32 Data, u8* Dst) ;
/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/
extern unsigned char   c5msno ;
extern unsigned char cCWDTMAx ;
void   UART_init();
void CMU_Init();
void SystemTick_Start (void);
void SystemTick_Interrupt_On(void);
unsigned char		cSysTickNum;
void GPIO_Init();
void stream_cSilence_fun (void);
void get_emu_var_default(void);
extern unsigned short  iTurnOnTm ;
extern unsigned short iTurnOffTm;
unsigned char  printfdatano( unsigned short iData );
/*
*********************************************************************************************************
*                                              MAIN
*
*********************************************************************************************************
*/
 extern unsigned char	cResetSecTm ;
void DEBUG_Init(void);
void  UARR_Ir_Chanel_Init(unsigned  int ibps);
unsigned char	  IsOffLine(void);
 	 void  MeterData_Show(struct 	STDATALIST  *stp);
 unsigned char FileSystem_Init (struct STFLASHFILESYSTEM *stfilep);
 extern  struct STFLASHFILESYSTEM stDianLian;
void get_emu_var_default(void) ;
 extern const unsigned char cncOverVolMax ;	//       电量保存定时器延时设置
extern const unsigned char cnmagnetismc ;	//       电量保存定时器延时设置

 int
Rtc_Get_callback (char *TimeBase) ;
		  int main(void)
		  {}
#ifdef _DEL
int main(void)                                   
{     
	
    /*  SystemInit() has been called before enter main() */ 
		char cOffline; 
unsigned char c5msnobak;
	unsigned int i,j,Bit ;
 // volatile  struct   ST24256_DATACONFIG  *stexfrzoedata;
 //	stexfrzoedata = (struct   ST24256_DATACONFIG  *)0;
	// if ( stexfrzoedata->stFlashFrozeData_EEprom.FFHour[4][6]==9)
    /*  Add your application code here  */ 
    
    /*  Infinite loop  */
	
  HT_CMU->WPREG = 0xA55A;
  HT_CMU->CLKCTRL0 &=~CMU_CLKCTRL0_LFDETEN ;
	DEBUG_Init();
_OffLine_P()	;
	delay(0xafff);

	a3:
	LCD_Init();
	memset ( (char *)RamData.Disp.DispData,0xff,sizeof(RamData.Disp.DispData));
	UpdateDisp((char *)RamData.Disp.DispData);
 
	memcpy (( char *)&FlashInfo,( char *)&DftFlashInfo,sizeof(FlashInfo));
	
	PMU_Init();				
	GPIO_Init();
	__disable_irq() ;
	_setdog()	;
  delay(0xafff);delay(0xafff);delay(0xafff);delay(0xafff);delay(0xafff);
  _SaveEng_End();
	iTurnOnTm  = iTurnOffTm= 0;
	CMU_Init();
	  System_Init();
	bitstruct_init ();
	emu_init();
	

	
	 /*!< 使能RTC总中断        */
	 HT_RTC_ITConfig(RTC_RTCIE_MINIE|RTC_RTCIE_HRIE|RTC_RTCIE_SECIE , ENABLE);
   NVIC_EnableIRQ(RTC_IRQn);                                 
	 
	SysTick_Config(_systick_config_);	
	__enable_irq();
	
	//		 计量模块初始化
	 Time_Cnt_Exchange_Init ((long) Rtc_Get_callback);	
	 
	do{
		
		if(c5msno!=c5msnobak)
		{
			_setdog()	;
			c5msnobak = c5msno ;_Change_p();
			memset ( (char *)RamData.Disp.DispData,0xff,sizeof(RamData.Disp.DispData));
				UpdateDisp((char *)RamData.Disp.DispData);
				vol_read();
		}
		
	} while (  !(_OnLine()) && !(_OffLine()));
	 _OnLine_P();RamData.Pwr_On =1;
 cFlashinfo_Restore();
	DianLian_Restore(); 
	//--USARTCOM[_IR_Chanel_].iBps=_GetBpsSet(_defaultbps_	);
	//--USARTCOM[_IR_Chanel_].cSilence=1;
	
	 RamData.LVSave = &(RamData.stdianlian[0].LVSave);
	for (i = 0; i < _MAX_TARRIF_; i++)
		RamData.pFLUse[i] = (RamData.stdianlian[0].FLUse + i); //费率用电指针
	
		//  		485初始化， 	//	打开UART0中断
	USARTCOM[_R485_Chanel_].cSilence =1;
  //UART_INTENABLE(HT_UART0);
	
	 i = 0;
	 HT_CMU->WPREG = 0xA55A;
  HT_CMU->CLKCTRL0 |=CMU_CLKCTRL0_LFDETEN ;
	
	// 重刷RTC
	_bEvent_Set(bRtcRefresh);
	
	//		初始显示
		RamData.Disp.DispCode.Code =0x04000409;
		UpDisp();
	
    while (1)
    {	
			timeclock_get ();
			cOffline =  IsOffLine();
			 // 定时器状态更新
			_setdog()	;
      mstimerun();	
			halfsectimerun();
			sectimerun();
			Do_Task ( bInt_Ms , vol_read());
			
			//	 时间处理
			if (( stbitapp.bRtcRefresh) || ( bInt_MinRtc ))
			{
						GetCurrTimeCnt(&RamData.RTCTime); 
			}else  if  ( bInt_SecRtc)
			{
				RamData.RTCTime++;						
			}
			
		Hex_BCD(RamData.stdianlian[0].LVSave.SumUse.Use /10,(char *)&RamData.lDianL180)  ;
		Hex_BCD(RamData.stdianlian[0].LVSave.SumUse.RevUse /10,(char *)&RamData.lDianL280 ) ;
		
					// 厂内模式
			if   (bInt_SecBit)
			{
						if((FlashInfo.RunInfo.EState&_ESTATE_FACOFF_) == 0){			//??????
					RamData.FacProgTime = 1;
				} else if(RamData.FacProgTime){
					RamData.FacProgTime--;
					if ( 0 == 		RamData.FacProgTime) 
						CloseProgRecord();
				}
			}
			
			if   (bInt_SecBit &&  (RamData.DelayRst))
			{
					RamData.DelayRst--;
				if (  0 ==RamData.DelayRst)
					while(1);
			}
			Set_SoftTime (HscTime_FirstShow,1,5);
			Set_SoftTime (secTime_DianLianSave,RamData.NotChg, cncDianLianSave);	//      设置定时器
			Set_SoftTime (mstime_bmagnetism,!bInt_bmagnetism	, cnmagnetismc);		//      设置定时器
			Set_SoftTime (hsecTime_OverVol, _OverVolMax(RamData.Vph.sVI), cncOverVolMax) ;			//  过压定时器
			Do_Task ((( TRUE == cOffline) || cGetB(secTime_DianLianSave) || (RamData.NotChg >= 100))  ,Up_LVSave());
			if ( ( TRUE == cOffline	) )
			{
			_bEvent_Set(bMcuReset);				
			}
			
			Do_Task( IsUpB(HscTime_FirstShow )	,	RamData.Disp.DispCode.Code =0x0);
			Do_Task  ((cGetB( HscTime_FirstShow ) && bInt_HalfSec) ,Do_Disp());							
			Do_Task (  bInt_HalfSec ,		emu_proc());	
			Do_Task ( (bInt_HalfSec && !(_OffLine()))  ,   Do_Use());				//计算总用电量
			Do_Task  (bInt_MinRtc	 , CheckFroze());
			//Do_Task ( USARTCOM[_IR_Chanel_].bEventRec , Com_fun());
			Do_Task ( USARTCOM[_R485_Chanel_].bEventRec645 , Do_TRx());
			
			//Do_Task ( _bEvent_Get(bjb),Chip_Trim_IB());
			Do_Task ( _bEvent_Get(bjb),Chip_Trim_5l());
			Do_Task ( _bEvent_Get(bjbDataStore),Emu_Var_Write());
			// 校表数据复位
			Do_Task ( _bEvent_Get(bjbDataReset),_emu_data_reset());	
			// EMC模块复位
			Do_Task ( _bEvent_Get(bEmcModuleReset),_emu_module_reset());		
			//  MCU复位
			Do_Task ( _bEvent_Get(bMcuReset),NVIC_SystemReset());	
			stBitnewtoold_fun;
      timeclock_clear ();			
			
    } 
}


 /*
*********************************************************************************************************
*                                             ASSERT
*
*********************************************************************************************************
*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	
  while (1)
  {

  }
}
#endif

void CMU_Init()
{      
    
    /***** 以下代码用于将系统时钟配置为PLL时钟，CPU时钟为系统时钟2分频 *****/  
    
    CMU_InitTypeDef  CMU_InitStructure;
    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv2;

    HT_CMU_Init(&CMU_InitStructure);
	//	HT_CMU->WPREG = 0xA55A;
//		HT_CMU->CLKCTRL0 |=	CMU_CLKCTRL0_EMUEN ;
	 
	  
	 delay(0x5fff);
	
} 




void open_pll(void)
{	
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 |= 0x0010;            //enable PLL
	delay(0x5fff);
//	delay(0x5fff);
//	delay(0x5fff);
	HT_CMU->SYSCLKCFG = 0x83;              //PLL as Fsys 
//	HT_CMU->SYSCLKDIV = 0x00;              //PLL:19.66MHz, Fsys/1 as Fcpu		
	HT_CMU->SYSCLKDIV = 0x01;              //PLL:19.66MHz, Fsys/2 as Fcpu	
}


void open_pll_tmp(void)
{
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 |= 0x0010;            //enable PLL
	delay(0x5f);
	HT_CMU->SYSCLKCFG = 0x83;              //PLL as Fsys 	
//	HT_CMU->SYSCLKDIV = 0x04;             //PLL:19.66MHz, Fsys/16 as Fcpu	
	HT_CMU->SYSCLKDIV = 0x05;              //PLL:19.66MHz, Fsys/32 as Fcpu
//	HT_CMU->SYSCLKDIV = 0x06;              //PLL:19.66MHz, Fsys/64 as Fcpu
}


void close_pll(void)
{
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 &= 0xFFEF;            //close PLL
}


		
void Up_BkLVSave() 
	{

}
	void Up_LVSave() 
{

struct STFLASHFILESYSTEM *stfilep  = &stDianLian ;
	 _SaveEng_Start();
	Record_Put (stfilep,(char *)&(RamData.stdianlian[0].LVSave));
	_SaveEng_End();
		RamData.NotChg = 0;
			RamData.Use_Chg = 1;
						
		
}
void	Do_Use(void)
	{

	u8	Imp;
	u16	sImp;
	u32	AInc;
	u32	Inc;
	u32	SInc;
	u8 cImpbuf;
					
	RamData.ImpBfr = Read_EPADR(EPR_EnergyPC );
	if ( 0 == RamData.ImpBfr ) return ; 
	cparToB(Event_Rev , _bREVP  )	;
	RamData.Use_Dir =  (cGetBOld(Event_Rev)	&& cGetB(Event_Rev)	)?1:0;
	if(RamData.Use_Dir)
	{
		if(AUse_Dir == 0){
			AUse_Dir = 1;
			AUse_Dir_A = 1;
			RamData.RevTmCnt = 0;
			return;
		}
	} else {
		if(AUse_Dir){
			AUse_Dir = 0;
			AUse_Dir_A = 0;
			RamData.RevTmCnt = 0;
			return;
		}
	}
	Imp =RamData.ImpBfr;
	if(RamData.RevTmCnt < _REV_DELAY_){								//反向处理
		if((++RamData.RevTmCnt < _REV_DELAY_)&&(Imp < _IMP_DELAY_)){
			return;
		}
	}

	if(RamData.Use_Dir){
		sImp = 0x8000|Imp;
	} else {
		sImp = Imp;
	}
	
	//Inc = Inc_Use( &RamData.LVSave[0].FLUse, sImp);					//计算费率用电量
	SInc = Inc_Use( &RamData.LVSave[0].SumUse, sImp);				//计算总用电量
/*if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
	AInc = 0;
	if(Inc){
		AInc = Inc_Money(FlashInfo.RunInfo.FLPrice, Inc);			//增加费率用电金额
	}
	if(SInc){
		AInc += Inc_Money(FlashInfo.RunInfo.StepPrice, SInc);		//增加阶梯用电金额
	}
} //#endif
	*/
	Up_BkLVSave();													//建立电量数据备份
	if(SInc){
		RamData.NotChg += SInc;
//if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
//		GetPrice();
//} //#endif
	//	if(((RamData.NotChg >= 100)||(cGetB(secTime_DianLianSave)))){
	//		Up_LVSave();
	//		RamData.NotChg = 0;
	//		RamData.Use_Chg = 1;
	//	}
	}
//if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
//	if(AInc){														//有功电量变化处理
//		ChkUseState();
//	}
	
}//#endif		//_USE_CARD_
	
	
  
 
void DianLian_Restore(void)
{
		 struct STFLASHFILESYSTEM *stfilep  = &stDianLian ;
	  unsigned char ctemp;
		struct STRECORD stRecordBuf[2];
		struct STRECORD *stCurr = &(stRecordBuf[0]);
		struct STRECORD *stPrev = &(stRecordBuf[1]);
		unsigned long lAddr,lAddr1, lBlockEnd;
		unsigned short i, j;
		ctemp = Record_Get_First (stfilep, stCurr);
	
	if ((  _Record_Null_ == stCurr->cFlag )  &&  (  _Record_Ok_ == stPrev->cFlag ))
	{
					memcpy ( (char *)&RamData.stdianlian[0].LVSave ,  stPrev->sData ,sizeof(RamData.stdianlian[0].LVSave));							
	}
	else
	{
			memset( (char *)&RamData.stdianlian[0].LVSave , 0 ,sizeof(RamData.stdianlian[0].LVSave));							
				
	}
}
 
 #ifdef _DEL
void  Com_fun(void)
{
	  long  ltemp;unsigned char	 clen ;
		extern unsigned char			cBpsset;
	extern const	char	sAsk[];
extern const	 char	 sAnswer[];
extern const		char	sAsk1[];
	extern unsigned char		cXorCheck;
	extern unsigned char	 	cXorEnable;

	char *p ;
		char	*sp = USARTCOM[_IR_Chanel_].DataBuf ; struct 	STDATALIST  *stp =  stdatalist;
		unsigned short iChanel = 0;
		USARTCOM[iChanel].bEventRec =0;  clen = USARTCOM[_IR_Chanel_].cHead ;
		USARTCOM[_IR_Chanel_].cHead=0;
		if ( 0 == strcmp ( sAsk , sp ) )
		{
				   ltemp = fprintf(((FILE *)_Uart_Handle_), "%s",sAnswer);
				if ( ltemp > 0)
					 USARTCOM[_IR_Chanel_].cPSend_len = (unsigned char )ltemp;
				else
					 USARTCOM[_IR_Chanel_].cPSend_len = 0;
					return ; 
		}
		  
		if ( 0 == strcmp(	sAsk1,sp))
		{

 
		USARTCOM[_IR_Chanel_].iBps = ((sp[2]-0x30)>(sizeof(iBpsSet)/sizeof(iBpsSet[0]))?300:iBpsSet[sp[2]-0x30]) ;//_GetBpsSet(sp[2]) ; 
						SystemTick_Interrupt_Off();
						UARR_Ir_Chanel_Init(USARTCOM[_IR_Chanel_].iBps);
						SystemTick_Interrupt_On();
							
			 	 
							fprintf(((FILE *)_Uart_Handle_), "%c",0x2);
						 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
						p = (unsigned char *)stp->lData; 
						ltemp=fprintf(((FILE *)_Uart_Handle_), "(%c%c%c%c%c%c%c%c)\r\n" , toascii((p[0]>>4)&0xf),toascii((p[0])&0xf),toascii((p[1]>>4)&0xf),toascii((p[1])&0xf),\
									toascii((p[2]>>4)&0xf),toascii((p[2])&0xf),toascii((p[3]>>4)&0xf),toascii((p[3])&0xf));
				

				
						stp++;
						 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
						ltemp =  MeterData_Get(stp);
						ltemp =fprintf(((FILE *)_Uart_Handle_), "(%07d.%03d*kWh)\r\n" , ltemp/1000,ltemp%1000);
					
			
					stp++;
						 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
						ltemp =  MeterData_Get(stp);
				ltemp =fprintf(((FILE *)_Uart_Handle_), "(%07d.%03d*kWh)\r\n" , ltemp/1000,ltemp%1000);
				
			
				stp++;
			 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
			ltemp =fprintf(((FILE *)_Uart_Handle_), "(16)\r\n" , ltemp/1000,ltemp%1000);
				
				
			stp++;
						 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
						ltemp =  MeterData_Get(stp);
				ltemp =fprintf(((FILE *)_Uart_Handle_), "(%03d.%d*A)\r\n" , (ltemp%10000)/1000,(ltemp/100)%10);
				
				
				stp++;
						 USARTCOM[_IR_Chanel_].cPSend_len +=printfdatano( stp->iDataNo );
						ltemp =  MeterData_Get(stp);
				ltemp =fprintf(((FILE *)_Uart_Handle_), "(%03d.%d*V)\r\n" , (ltemp%10000)/10,ltemp%10);
				
				
					fprintf(((FILE *)_Uart_Handle_), "%c",(char)3);
					fprintf(((FILE *)_Uart_Handle_), "%c",cXorCheck );
					 USARTCOM[_IR_Chanel_].cPSend_len = USARTCOM[_IR_Chanel_].cHead;
					 USARTCOM[_IR_Chanel_].cHead=0;
					 	return ; 
		}
			
				USARTCOM[iChanel].bEventRec =1;
				USARTCOM[_IR_Chanel_].cHead=clen;
				com_shell ();	USARTCOM[iChanel].bEventRec =0;
		return ;
					
		}	

	#endif


unsigned long  MeterData_Get( struct 	STDATALIST  *stp)
{
	
	unsigned  short iData  =  stp->iDataNo;
	unsigned long		ltemp ;
	memcpy ( (char *)&ltemp , (unsigned char *)(stp->lData),sizeof(ltemp));
	return ltemp;
}

unsigned char	 toascii(unsigned char i)
{
		if ( i < 0xa)
				return i+0x30;
		else
				return i+87-0x20;
	}

unsigned char printfdatano( unsigned short iData )
{
	long ltemp=		fprintf(((FILE *)_Uart_Handle_), "%c.%c.%c" , toascii((iData>>8)&0xf),toascii((iData>>4 )&0xf),toascii((iData )&0xf));
		if ( ltemp > 0)
			return (unsigned char)ltemp;
		return 0;
}


int
Rtc_Get_callback (char *TimeBase)
{
	Rtc_Get_SafeIo (TimeBase);
	//HT_RTC_Read(TimeBase);
  return SUCCESS;
}
   #endif 
/*
void	Do_MinTask(void){

	RamData.Event_Min = 0;CheckBatVolt();
	 Do_FeilChg();
	CheckFroze();
	Rtcunint_ReadTemp(&RamData.Temperature);
	Freq_Trim ( RamData.Temperature,_Maybe_);

}
 */