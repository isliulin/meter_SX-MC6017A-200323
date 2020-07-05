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



/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/

 

 

void LCD_Init()
{      
    
    /***** 以下代码用于配置液晶模块，以下代码不含GPIO配置 *****/  
    
    LCD_InitTypeDef  LCD_InitStructure;
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_LCDEN, ENABLE);            /*!< 使能LCD模块                */
    
    LCD_InitStructure.LCD_Bias = LCDBias_OneThird;                /*!< 1/3 bias 偏压驱动          */
    LCD_InitStructure.LCD_Com  = LCD_6COM;                        /*!< 6Com                       */
    LCD_InitStructure.LCD_CLK  = LCDCLK_Div1;                     /*!< 扫描频率最高               */
    LCD_InitStructure.LCD_ChargeMode  = LCDCharge_HighCurrent;    /*!< 大电流充电模式             */
    LCD_InitStructure.LCD_Vrsel = 0x08;                           /*!< 对比度配置                 */
    
    HT_LCD_Init(&LCD_InitStructure);      
}

void	UpdateDisp(char *sp)
	{
	
 
		HT_LCD->LCDBUF[0]=sp[0];
		HT_LCD->LCDBUF[1]=sp[1];
		HT_LCD->LCDBUF[2]=sp[2];
		HT_LCD->LCDBUF[3]=sp[3];
		HT_LCD->LCDBUF[4]=sp[4];
		HT_LCD->LCDBUF[5]=sp[5];
		HT_LCD->LCDBUF[14]=sp[6];
		HT_LCD->LCDBUF[18]=sp[7];
		HT_LCD->LCDBUF[19]=sp[8];
		HT_LCD->LCDBUF[26]=sp[9];
		HT_LCD->LCDBUF[27]=sp[10];
		HT_LCD->LCDBUF[33]=sp[11];
		HT_LCD->LCDBUF[34]=sp[12];
		HT_LCD->LCDBUF[35]=sp[13];
	 
	}		
		
	  
	