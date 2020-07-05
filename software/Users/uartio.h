/*
 * lcd_1.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef _UART_IO_H_
#define _UART_IO_H_

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"

struct STUARTCON
{
  uint16_t b1TXEN:1;	// 发送使能位1：使能0：禁止
  uint16_t b1RXEN:1;	//UART接收使能位1：使能0：禁止
  uint16_t b1TXIE:1;	//UART发送中断使能位1：使能0：禁止
  uint16_t b1RXIE:1;	//UART接收中断使能位1：使能0：禁止

  uint16_t b1PARITYEN:1;	//UART奇偶校验使能位1：使能0：禁止
  uint16_t b2PARITYSEL:2;	//UART奇偶校验选择位 11：固定为1 00：固定为0 01：奇校验10：偶校验
  uint16_t b1LENSEL:1;	//UART通讯数据长度选择位1：7bit0：8bit
  uint16_t b1STOPSEL:1;	//UART通讯停止位长度选择位1：2bit0：1bit
  uint16_t b1UNEG:1;	//UART通讯中的正逻辑或者是负逻辑0：正逻辑（默认）1：负逻辑
  uint16_t b1NEGFIX:1;	//UART通讯中的逻辑锁定：0：由用户软件控制UART通讯是正逻辑还是负逻辑，通过UNEG控制位1：由硬件自动配置UART通讯是正逻辑还是负逻辑，UNEG控制位无效
  uint16_t b4temp;
};


struct 	STUARTSTA
{
	uint16_t	b1TXIF:1;
	uint16_t	b1RXIF:1;
	uint16_t	b1PARITY:1; 
	uint16_t	b13temp:13;
};
	

void UARR_Ir_Chanel_Init (unsigned int ibps);

void UART_init (HT_UART_TypeDef * unitp, UART_InitTypeDef UART_InitStructure);


void UART_Close (HT_UART_TypeDef * unitp);


void stream_cSilence_fun (void);



void stream_fun_send (HT_UART_TypeDef * unitp);


void stream_fun_rece (HT_UART_TypeDef * unitp);


//=========================================================
void Uart_ReceIntFun (HT_UART_TypeDef * unitp);

void Uart_SendIntFun (HT_UART_TypeDef * unitp);


#define STUART  HT_UART_TypeDef
#define STUARTREG UART_REG_TypeDef

typedef struct
{
  unsigned char *buf;
  unsigned short rd;
  unsigned short wr;
  unsigned short sz;
} RBUF_TypeDef;



extern int initUart (unsigned char portnum);


int Uart_sendData (STUART * unitp, unsigned char *data, unsigned char size);
#define sendUartData(a,b,c) Uart_sendData(stuartunit[a].unitp,b,c) ;


//void Uart_start(struct STUART *  unitp) ;
//#define startUart(a)          Uart_start(stuartunit[a].unitp)
//void Uart_start(STUART *  unitp) ;
//#define startUart(a)          Uart_start(stuartunit[a].unitp)
//void UART_stop(struct STUART *  unitp);
void UART_stop (STUART * unitp);
#define stopUart(a) 		{}	//UART_stop(stuartunit[a].unitp)

extern int receiveUartData (int portnum, unsigned char *data,
			    unsigned short size);



struct STBSPCONFIG
{
  unsigned char cCLKDIV;
  unsigned short iBPSCONFIG;
};
#define 	_UARTCLKDIV_1_		0
#define		_UARTCLKDIV_2_   	1
#define		_UARTCLKDIV_4_   	2

#define		_bps1200_		1200	// 1200 ,0.01           0x2cf           ?? 0.04%
#define 	_bps2400_		2400	//      2400  0.0.1             0x4cf           ?? 0.04%
#define 	_bps4800_		4800	//      4800,           ?? 0.04%
#define 	_bps9600_		9600	//      ?? 0.042%//0xF33                //  9600                ?? 0.2%
int init_Uart_2_GPIO (unsigned char portnum);

void
UART_SendIntEnable (HT_UART_TypeDef * unitp);

#endif /* LCD_1_H_ */
