/*
 * rtc_drv.c
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

/****************************************************************************
*
* @file   boot.c
* @brief This is ADC12A driver program file for RM8025FA.
*
* @par                Copyright:
*                                      Copyright (C) SEIKO EPSON CORPORATION 2015-2015. All rights reserved.
*
* @warning -
*
* @par                History:
*                                      - 2015/05/29    1.00
*
***************************************************************************/

#include "system.h"
#include "pubset.h"
#include <string.h>
#include <stdio.h>
#include <libstdio.h>
#include "rtc_drv.h"

#include "c17m11_px_reg.h"
void   RSet_WDG(unsigned char ctemp);
//void cCopyRam(unsigned char  *Dst, unsigned char  const *Src, unsigned short  Length);


extern volatile unsigned char c8025TOperTm;			//	 定时，5MS减一
extern unsigned char	c8025Statu ; 				//	 为0 ，正常 ，非0，不正常
const  unsigned char 	c8025Init_Flag =0x55;	//		8025复位标志

#define 	DAT0			csRtcbuf[0]
#define 	DAT1			csRtcbuf[1]
#define 	DAT2			csRtcbuf[2]
#define 	DAT3			csRtcbuf[3]
#define 	DAT4			csRtcbuf[4]
#define 	DAT5			csRtcbuf[5]
#define 	DAT6			csRtcbuf[6]
#define 	DAT7			csRtcbuf[7]

#define 	cSec			csRtcbuf[0]
#define 	cMin			csRtcbuf[1]
#define 	cHour			csRtcbuf[2]
#define 	cWeek			csRtcbuf[3]
#define 	cDay			csRtcbuf[4]
#define 	cMon			csRtcbuf[5]
#define 	cYear			csRtcbuf[6]


enum {
	_bI2C1_TBEIF	= 0x1 ,	///< This bit indicate Transmit buffer empty interrupt cause occurrence status.
	_bI2C1_RBFIF	= 0x2 ,	///< This bit indicate Receive buffer full interrupt cause occurrence status.
	_bI2C1_ERRIF	= 0x4 ,	///< This bit indicate Error detection interrupt cause occurrence status.
	_bI2C1_STARTIF	= 0x8 ,	///< This bit indicate START condition interrupt cause occurrence status.
	_bI2C1_STOPIF	= 0x10 ,	///< This bit indicate STOP condition interrupt cause occurrence status.
	_bI2C1_NACKIF	= 0x20 ,	///< This bit indicate NACK reception interrupt cause occurrence status.
	_bI2C1_GCIF		= 0x40 ,	///< This bit indicate General call address reception interrupt cause occurrence status.
	_bI2C1_BYTEENDIF= 0x80 ,	///< This bit indicate End of transfer interrupt cause occurrence status.
	_bI2C1_reserved_1= 0x100 ,	///< Reserved.
	_bI2C1_TR		= 0x200 ,	///< This bit indicates whether the I2C is set in transmission mode or not.
	_bI2C1_BSY		= 0x400 ,	///< This bit indicates that the I2C bus is placed into busy status.
	_bI2C1_SCLLOW	= 0x800 ,	///< This bit indicates that SCL is set to low level.
	_bI2C1_SDALOW	= 0x1000 ,	///< This bit indicates that SDA is set to low level.
	_bI2C1_reserved_2	,	///< Reserved.
};


/* special definitions for vector table  */

#ifdef _DEL


void   Wait_I2c1_Flag ( unsigned short iFlag)
{
	unsigned short  itemp ;
	unsigned  long  lretry= 0;
	unsigned  short iBuf[16] ;
	unsigned  char	cbuflen = 0;
	if ( 0==c8025Statu)
		return ;

	c8025TOperTm = 2;RSet_WDG(c8025TOperTm);
	 cbuflen = 0;
	do {
		itemp =  I2C1INTF ;
		iBuf[cbuflen] =  itemp  ;
		if ( cbuflen )
		{
			if (iBuf[cbuflen]!=iBuf[cbuflen-1])
			{
				if (cbuflen < sizeof(iBuf))
					cbuflen++;
			}

		}
		else
			cbuflen++;
		if (0 ==c8025TOperTm)
				break;
	}while (0 ==( itemp &iFlag ) );
	c8025Statu = c8025TOperTm ;

}

void senddat(char dat) {

	I2C1INTF = 0xffff; // I2C Ch.1 Interrupt Flag: All Clear
	I2C1TXD = (int) dat; // transmit data
	 Wait_I2c1_Flag ((unsigned short )_bI2C1_TBEIF |( unsigned short ) _bI2C1_BYTEENDIF);     //asm("halt"); // halt (Wait Interrup(I2C Ch.1))
	return;
}

char readdat(void) {
	char rdata;
	Wait_I2c1_Flag (_bI2C1_RBFIF);     //asm("halt"); // halt (Wait Interrup(I2C Ch.1))
	rdata = I2C1RXD;
	return(rdata);
}


#endif


void Rtcunint_init(unsigned short cPoweron)
{
	if (0 == cPoweron)
	{
		RTC->IE &=0xfffe;
		return ; 
	}
	
	RTC->IF = 0;
	RTC->IE |=1;
	NVIC_ClearPendingIRQ( RTC_IRQn );
  NVIC_EnableIRQ( RTC_IRQn );

 c8025Statu = 0;
	 
}

 
unsigned char  Rtcunint_write (unsigned char 	*sin)
{ 
	unsigned char	  csRtcbuf[_8025T_END_];
	unsigned char 	i;
	
	
	cCopyRam( csRtcbuf , sin  , sizeof(csRtcbuf)  );
	
	for ( i = 0 ; i <7 ;i++)
		if ( cWeek & ( 1<<i))
			break;
	
	
	// latch time
  RTC_REG->LATCH.BITS.LATCH_EN = 1;
  while (RTC_REG->LATCH.BITS.LATCH_EN);
	
	
	RTC_REG->CLOCK.BITS.SECOND = cSec;	// transmit data(Setting data: bank 1, address 0 (SEC))
	RTC_REG->CLOCK.BITS.MINUTE = cMin;	// transmit data(Setting data: bank 1, address 1 (MIN))
	RTC_REG->CLOCK.BITS.HOUR = cHour; 	// transmit data(Setting data: bank 1, address 2 (HOUR))
	RTC_REG->CALENDAR.BITS.WEEK = i;//cWeek;// transmit data(Setting data: bank 1, address 3 (WEEK))
	RTC_REG->CALENDAR.BITS.DAY = cDay; 	// transmit data(Setting data: bank 1, address 4 (DAY))
	RTC_REG->CALENDAR.BITS.MONTH = cMon;// transmit data(Setting data: bank 1, address 5 (MONTH))
	RTC_REG->CALENDAR.BITS.YEAR = cYear;// transmit data(Setting data: bank 1, address 6 (YEAR))
  
  RTC_REG->LOAD.VAL = 0x1;
	
	while( ! ( RTC_REG->STATUS.VAL & 0x1 ) );
	
	RTC->RAM[0]=_lRTC_RUNMODE_;

	return 2;

}



unsigned char Rtcunint_read (unsigned char 	*sin)
{
	unsigned char	csRtcbuf[_8025T_END_];
  //cCopyRam( sin ,csRtcbuf , sizeof(csRtcbuf));
	
	RTC_REG->LATCH.BITS.LATCH_EN = 1;
  while (RTC_REG->LATCH.BITS.LATCH_EN);
  
	DAT0 = RTC_REG->CLOCK.BITS.SECOND; // receive data(bank 1, address 0 (SEC))
	DAT1 = RTC_REG->CLOCK.BITS.MINUTE; // receive data(bank 1, address 1 (MIN))
	DAT2 = RTC_REG->CLOCK.BITS.HOUR; 	 // receive data(bank 1, address 2 (HOUR))
	DAT3 = RTC_REG->CALENDAR.BITS.WEEK; // receive data(bank 1, address 3 (WEEK))
	DAT4 = RTC_REG->CALENDAR.BITS.DAY; // receive data(bank 1, address 4 (DAY))
	DAT5 = RTC_REG->CALENDAR.BITS.MONTH; // receive data(bank 1, address 5 (MONTH))
	DAT6 = RTC_REG->CALENDAR.BITS.YEAR; // receive data(bank 1, address 6 (YEAR))
	
	cCopyRam( sin ,csRtcbuf , sizeof(csRtcbuf));
	
	 return  2;//c8025Statu ;
	
	 
}


void   SetF32K_out(void)
{
    return;
	//CLGOSC_OSC1EN = 1;
	//CLGFOUT_FOUTSRC = 1;
	//CLGFOUT_FOUTDIV= 7;
	//CLGFOUT_FOUTEN =1;
}



void RTC_Handler(void)
{
  uint8_t tmp;
	extern unsigned short	iRtcintno;
	iRtcintno++;
		RTC->IF = 0;
  RTC_REG->CTL.BITS.ALMEN = 0;

  RTC_REG->IE.BITS.ALM = 0;
  RTC_REG->IF.BITS.ALM = 0;  
	
 if (_SYS_OFF_LINE())
 { 
	NVIC_ClearPendingIRQ(RTC_IRQn  );
  NVIC_DisableIRQ(RTC_IRQn  );
 }
	
 /*return ;  
	#ifdef _DEL
  // next alarm
  tmp = bcd_add(RTC_REG->ALARM.BITS.SEC, ALARM_INTERVAL);
  if (tmp >= 0x60) {
    // subs with 9's complememt 
    // tmp - 60 ==> tmp + (99 - 60) + 1 
    tmp = bcd_add(tmp, 0x40);  
  }
  RTC_REG->ALARM.BITS.SEC = tmp;
	#endif*/
}
