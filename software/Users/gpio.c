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
 

void
DEBUG_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitAFTypeDef GPIO_InitAFStructure;
return;
  //================================================================
  //DEBUG==========  //PA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;


  HT_GPIO_Init (HT_GPIOC, &GPIO_InitStructure);
  HT_GPIOC->PTUP |= 0xc;
  //=====================================================================
}

void
GPIO_Init ()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitAFTypeDef GPIO_InitAFStructure;

  // pA5 °´¼ü

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT ;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  ;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOA, &GPIO_InitStructure);
  //=========================================================

  //PC2£¬PC3  UART  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOC, &GPIO_InitStructure);
  //=========================================================

  //PE4£¬PE5  UART  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOE, &GPIO_InitStructure);
  //============================================

  //PE3£¬PE2  I2C  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOOUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOE, &GPIO_InitStructure);
	
	 

  //============================================

  //PB
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin =
    GPIO_Pin_14 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 |
    GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOB, &GPIO_InitStructure);
  //============================================

  //PD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOD, &GPIO_InitStructure);

  //============================================

  //PD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF2;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOD, &GPIO_InitStructure);

  //============================================

  //PD8-13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin =
    GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
    GPIO_Pin_13;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOD, &GPIO_InitStructure);
  //============================================

  //PA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOA, &GPIO_InitStructure);
  //============================================

  //PA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF2;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
  GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;

  HT_GPIO_Init (HT_GPIOA, &GPIO_InitStructure);


  DEBUG_Init ();

}
