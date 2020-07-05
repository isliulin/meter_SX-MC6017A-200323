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
 
 extern unsigned short  iTurnOnTm ;
extern unsigned short iTurnOffTm;
		 
void PMU_Init()
{      
    
    /***** 以下代码用于配置芯片PMU模块 *****/  
    
    PMU_InitTypeDef  PMU_InitStructure;
    
    PMU_InitStructure.Lvdin1State = LVDINEnable ;                  /*!< LVDIN功能使能                */
    
    PMU_InitStructure.BorLvlSet = BORLVL_2V4;                   /*!< BOR阈值2.4v                  */
    PMU_InitStructure.BorState = BORReset;                      /*!< BOR产生复位                  */
 
    PMU_InitStructure.VccLvlSet = VccLVL_2V8;                   /*!< Vcc阈值设为2.8v              */
    
    PMU_InitStructure.VccBorDetPeriod = DETPRD_1072mS;          /*!< Vcc和Bor分时检测周期=1072ms  */
    PMU_InitStructure.VccBorDetTime = DETTIM_300uS;             /*!< Vcc和Bor分时检测时间=300uS   */
    
    HT_PMU_Init(&PMU_InitStructure);
		HT_PMU_ITConfig(PMU_PMUIE_VCCIE, ENABLE);
	
	  NVIC_SetPriority(PMU_IRQn ,3);
		NVIC_EnableIRQ(  PMU_IRQn     );   
		
} 

unsigned char	  IsOffLine(void)
{
		if ( _OffLine())
			return TRUE;
		else
			return FALSE;
	}

	
void   OnLine_Dete ( void)
{
		extern unsigned short iVolDete;
unsigned short iPMUSTA =HT_PMU->PMUSTA &1;

	unsigned short itemp =((iPMUSTA)  ||  ( iVolDete>800)) ?0xffff:0;
	if ( itemp)
	{
		if ( 	!_OnLine())
					iTurnOnTm++;
		
		if(iTurnOffTm)iTurnOffTm--;
	}
	else
	{
	
	
	if ( !_OffLine())
				iTurnOffTm++;
		
		if(iTurnOnTm)iTurnOnTm--;
	}
}
	
	