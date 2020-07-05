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
#include "uartio.h"


extern const unsigned short iBpsSet[7];
extern volatile struct STSCIBUF USARTCOM[_Com_End_];
extern const struct STCOMLIST stcomlist[_CHANEL_END_];




void
UARR_Ir_Chanel_Init (unsigned int ibps)
{
  UART_InitTypeDef UART_InitStructure;
  // HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN  , DISABLE);        /*!< 使能UART0模块 
  //NVIC_DiableIRQ(  UART3_IRQn      );                                  /*!< 使能RTC总中断        */

  //HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN  , ENABLE);        /*!< 使能UART0模块     */
  UART_InitStructure.UART_Logic = UartLogicPositive;	/*!< UART逻辑为正               */
  UART_InitStructure.UART_StopBits = OneStopBits;	/*!< 1位停止位                  */
  UART_InitStructure.UART_WordLength = WordLength_7Bits;	/*!< 8位数据位                  */
  UART_InitStructure.UART_Parity = UartParity_EVEN;	/*!< 偶校验                     */
  UART_InitStructure.UART_BaudRate = ibps;	/*!< 波特率2400                 */
  UART_InitStructure.ReceiveEN = ENABLE;	/*!< 接收使能   注：非中断使能  */
  UART_InitStructure.SendEN = ENABLE;	/*!< 发送使能   注：非中断使能  */

  HT_UART_Init (HT_UART3, &UART_InitStructure);
  HT_UART3->UARTCON |= UART_UARTCON_RXIE;
  HT_UART3->UARTCON &= ~UART_UARTCON_TXIE;
  //NVIC_EnableIRQ(  UART3_IRQn      );    
}

void
UART_ini1t ()
{

    /***** 以下代码用于配置芯片UART0模块，以下代码不含GPIO配置 *****/

  UART_InitTypeDef UART_InitStructure;

  HT_CMU_ClkCtrl1Config (CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);	/*!< 使能UART0模块              */



  NVIC_SetPriority (UART3_IRQn, 3);
  NVIC_EnableIRQ (UART3_IRQn);	/*!< 使能RTC总中断        */

  HT_CMU_ClkCtrl1Config (CMU_CLKCTRL1_UART0EN, ENABLE);	/*!< 使能UART0模块              */

  UART_InitStructure.UART_Logic = UartLogicPositive;	/*!< UART逻辑为正               */
  UART_InitStructure.UART_StopBits = OneStopBits;	/*!< 1位停止位                  */
  UART_InitStructure.UART_WordLength = WordLength_8Bits;	/*!< 8位数据位                  */
  UART_InitStructure.UART_Parity = UartParity_EVEN;	/*!< 偶校验                     */
  UART_InitStructure.UART_BaudRate = 2400;	/*!< 波特率2400                 */
  UART_InitStructure.ReceiveEN = ENABLE;	/*!< 接收使能   注：非中断使能  */
  UART_InitStructure.SendEN = ENABLE;	/*!< 发送使能   注：非中断使能  */

  HT_UART_Init (HT_UART0, &UART_InitStructure);
}


void
UART_init (HT_UART_TypeDef * unitp, UART_InitTypeDef UART_InitStructure)
{

  if (HT_UART0 == unitp)
    HT_CMU_ClkCtrl1Config (CMU_CLKCTRL1_UART0EN, ENABLE);	/*!< 使能UART0模块              */
  else if (HT_UART3 == unitp)
    HT_CMU_ClkCtrl1Config (CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);	/*!< 使能UART0模块              */
#ifdef _DEL
  UART_InitStructure.UART_Logic = UartLogicPositive;	/*!< UART逻辑为正               */
  UART_InitStructure.UART_StopBits = OneStopBits;	/*!< 1位停止位                  */
  UART_InitStructure.UART_WordLength = WordLength_8Bits;	/*!< 8位数据位                  */
  UART_InitStructure.UART_Parity = UartParity_EVEN;	/*!< 偶校验                     */
  UART_InitStructure.UART_BaudRate = 2400;	/*!< 波特率2400                 */
  UART_InitStructure.ReceiveEN = ENABLE;	/*!< 接收使能   注：非中断使能  */
  UART_InitStructure.SendEN = ENABLE;	/*!< 发送使能   注：非中断使能  */
#endif
  HT_UART_Init (unitp, &UART_InitStructure);
}

void
UART_INTENABLE (HT_UART_TypeDef * unitp, char enSendEn, char enReceEn)
{

  struct STUARTCON *stp = (struct STUARTCON *) &(unitp->UARTCON);

  stp->b1TXIE = (enSendEn) ? 1 : 0;
  stp->b1RXIE = (enReceEn) ? 1 : 0;
  if (HT_UART0 == unitp)
    NVIC_EnableIRQ (UART0_IRQn);
  else if (HT_UART3 == unitp)
    NVIC_EnableIRQ (UART3_IRQn);
}

void
UART_Close (HT_UART_TypeDef * unitp)
{
  if (HT_UART0 == unitp)
    NVIC_DisableIRQ (UART0_IRQn);
  else if (HT_UART3 == unitp)
    NVIC_DisableIRQ (UART3_IRQn);
}

void
UART_SendIntEnable (HT_UART_TypeDef * unitp)
{
	  struct 	STUARTSTA *stp =(struct 	STUARTSTA *)&(unitp->UARTSTA);
	
  if (HT_UART0 == unitp)
	{
		 stp->b1TXIF =1;
			 NVIC_SetPendingIRQ( UART0_IRQn  );
	} 
  else if (HT_UART3 == unitp)
	{
		 stp->b1TXIF =1;
     NVIC_SetPendingIRQ( UART3_IRQn  );
	}
}

void
Uart_ReceIntFun (HT_UART_TypeDef * unitp)
{

  struct STSCIBUF *usartcomp = &(USARTCOM[Uart_Struct_Get (unitp)]);
  unsigned char cbuf;
  unsigned int flag;

  unsigned char stream_send_fun_645 (struct STSCIBUF *p);

  cbuf = unitp->SBUF;
  stream_rece_fun_645 (usartcomp, cbuf);
  resetWdt ();
  if (usartcomp->bEventRec645)
    {
      UART_INTENABLE (unitp, 0, 0);
    }
  else
    {
     

    }
}

void
Uart_SendIntFun (HT_UART_TypeDef * unitp)
{
  struct STSCIBUF *usartcomp = &(USARTCOM[Uart_Struct_Get (unitp)]);
  unsigned char cbuf;
  if (usartcomp->cStatu < _end_sendcom)
    {
      cbuf = stream_send_fun_645 (usartcomp);
      resetWdt ();
      unitp->SBUF = cbuf;
      if (0 == --(usartcomp->cHead))
	usartcomp->cStatu++;
    }
  else
    {
      UART_INTENABLE (unitp, 0, 0);
      do
	{
	  usartcomp->cSilence = 1;
	}
      while (1 != usartcomp->cSilence);
    }
}
