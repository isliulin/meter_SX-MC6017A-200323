/*
*********************************************************************************************************
*                                 			   HT6XXX
*                            			      HT6XXX³ÌÐò
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
#include "uartio.h"
#include "systickapp.h"

unsigned char	c5msno;

void SystemTick_Start (void)
{
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
}


void SystemTick_Stop (void)
{
    SysTick->CTRL  &= (~SysTick_CTRL_ENABLE_Msk);	
}

void SystemTick_Interrupt_On(void)
{
    SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;		/* Turn on SysTick INT  */
}

void SystemTick_Interrupt_Off(void)
{
    SysTick->CTRL  &= (~SysTick_CTRL_TICKINT_Msk);   /* Turn off SysTick INT  */
}
//---------------------------
// SysTick_Handler
//---------------------------
//
void SysTick_Handler(void)
{
	
	uint32_t clr;
	static uint32_t s = 0;
    
	clr = SysTick->CTRL; 	/* clear COUNTFLAG */
 SysTick_fun();
}

void SysTick_fun(void)
{
	static 		unsigned char   cHalfsectm = 0;
	static 	  unsigned char   cSectm =0; 
	
		c5msno++ ;
			NVIC_DisableIRQ(  PMU_IRQn     );   
 		  OnLine_Dete ();
			NVIC_EnableIRQ(  PMU_IRQn     );   
			bInt_bmagnetism_io =  ( HT_GPIOA->PTDAT & 0x40) ?1:0;
	
		if ( RamData.WDGTm  )
				RamData.WDGTm-- ;
			
		if(RamData.WDGTm){
			HT_FreeDog();
	    }else{
				debug_break (_debug_errno_wdgout_);
		    while(1) ;   
			}	

		stream_cSilence_fun();  
		bInt_Ms_io = Event_Ms = 1;
		
		if ( ++cSectm >= 200)
					cSectm = 0 ; 
		if ( ++cHalfsectm >=100)
					cHalfsectm = 0;
			bInt_HalfSec_io= !((_Bool)  cHalfsectm ) | bInt_HalfSec_io ; 
			bInt_SecBit_io = !((_Bool) cSectm ) | bInt_SecBit_io  ; 
	}