#ifndef _rtc_app_hpp_
#define _rtc_app_hpp_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "main.h"
//#include "stm32f1xx_hal.h"
 #include "GetTimeCnt.h"

#define 		Rtc_Modify_Io_BIN(sDatap ) 	Rtc_Mod_SafeIo(sDatap,RTC_FORMAT_BIN)
#define 		Rtc_Modify_Io_BCD(sDatap ) 	Rtc_Mod_SafeIo(sDatap,RTC_FORMAT_BCD)

int Rtc_Mod_SafeIo (char *sInDate );
//int Rtc_Get_SafeIo (RTC_DateTypeDef * sDate, RTC_TimeTypeDef * sTime,
//		    uint32_t tMode);
int 
Rtc_Get_SafeIo (char * sTime) ;
int Rtc_Get_ModbusReg (unsigned short *TimeBase, _tyTIMECNT * lRtcTime);
//int Rtc_Get_Charp (char *TimeBase, uint32_t tMode);

char UpDateRtcDateTime_Get (void);
void UpDateRtcDateTime_Set (char cState);
#endif
