/*
 * rtc_drv.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef RTC_DRV_H_
#define RTC_DRV_H_



#include <string.h>
#include <stdio.h>
//#include <libstdio.h>
//#include "rm8025fa_reg.h" //#include "c17m11_reg.h"

extern volatile unsigned char c8025TOperTm;			//	 定时，5MS减一
extern unsigned 	char	c8025Statu ; 				//	 为0 ，正常 ，非0，不正常
extern const  unsigned char 	c8025Init_Flag;

void senddat(char dat);
char readdat(void);
//void Rtcunint_init(unsigned short cPoweron);
//unsigned char  Rtcunint_write(unsigned char 	*sin);
//unsigned char  Rtcunint_read(unsigned char 	*sin);
//#define Rtcunint_read(a)  (0)//unsigned char 	*sin);

//void SetF32K_out(void);

enum ENUM8025TDATA
{
		_8025T_SEC_ = 0,
		_8025T_MIN_ = 1,
		_8025T_HOUR_ = 2,
		_8025T_WEEK_ = 3,
		_8025T_DAY_ = 4,
		_8025T_MONTH_ = 5,
		_8025T_YEAR_ = 6,
		_8025T_RAM_ = 7,
		_8025T_END_
};
extern const  unsigned char 	c8025Init_Flag ;
#define _lRTC_RUNMODE_			0x52707000  
#define _IsRun()				(0)  // (RTC->RAM[0] == _lRTC_RUNMODE_)
#define _IsReset()			(!_IsRun())

#endif /* RTC_DRV_H_ */
