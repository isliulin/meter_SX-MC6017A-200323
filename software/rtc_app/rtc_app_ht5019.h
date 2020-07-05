#ifndef _rtc_app_ht5019_hpp_
#define _rtc_app_ht5019_hpp_


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ht6xxx_lib.h"
#include "GetTimeCnt.h"

#define 		Rtc_Modify_Io_BIN(sDatap ) 	Rtc_Mod_SafeIo(sDatap,RTC_FORMAT_BIN)
#define 		Rtc_Modify_Io_BCD(sDatap ) 	Rtc_Mod_SafeIo(sDatap,RTC_FORMAT_BCD)

int Rtc_Mod_SafeIo (char *sInDate);
//int Rtc_Get_SafeIo (RTC_DateTypeDef * sDate, RTC_TimeTypeDef * sTime,
//                  uint32_t tMode);
int Rtc_Get_SafeIo (char *sTime);
int Rtc_Get_ModbusReg (unsigned short *TimeBase, _tyTIMECNT * lRtcTime);
//int Rtc_Get_Charp (char *TimeBase, uint32_t tMode);

char UpDateRtcDateTime_Get (void);
void UpDateRtcDateTime_Set (char cState);
enum ENUMHT5019RTC
{
  _ht_5019rtc_WEEK_ = 0,
  _ht_5019rtc_YEARR_,		//  = pRTCWrite[1] & RTC_YEARR;
  _ht_5019rtc_MONTHR_,		// = pRTCWrite[2] & RTC_MONTHR;
  _ht_5019rtc_DAYR_,		//   = pRTCWrite[3] & RTC_DAYR;
  _ht_5019rtc_HOURR_,		//  = pRTCWrite[4] & RTC_HOURR;
  _ht_5019rtc_MINR_,		//   = pRTCWrite[5] & RTC_MINR;
  _ht_5019rtc_SECR_,		//   = pRTCWrite[6] & RTC_SECR;
  _ht_5019rtc_END_,		//   = pRTCWrite[6] & RTC_SECR;
  _ht_5019Bcd_,
  _ht_5019Hex_,
};

struct STHT5019RTC
{
  char sTime[_ht_5019rtc_END_];
};

#endif
