#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ht6xxx_lib.h"
//#include "stm32f1xx_hal.h"
//#include "globlvar_debug.h"
#include "metersystem.h"
#include "rtc_app.h"
//#include "modbusdata.h"
//#include "mainapp.h"
#include "GetTimeCnt.h"

//extern RTC_HandleTypeDef hrtc;
const char _ReadTime_Retrynum_ = 2;
char cUpRtcDateTime = SUCCESS;	//       RTCģ�����ģ־

char
UpDateRtcDateTime_Get (void)
{
  return (cUpRtcDateTime) ? SUCCESS : ERROR;
}

void
UpDateRtcDateTime_Set (char cState)
{
  cUpRtcDateTime = (SUCCESS == cState) ? SUCCESS : ERROR;
}

int 
Rtc_Get_SafeIo (char * sTime)
{
		char	i;
	  char 	stemp[8];
		 char 	stemp1[8];
	
		for ( i =0 ;i < _ReadTime_Retrynum_;i++)
	{
						HT_RTC_Read(stemp);
					HT_RTC_Read(stemp1);
		if ( 0 == memcmp ( stemp ,stemp1 ,sizeof(stemp)))
		{			  
    sTime[_APPTIME_YEAR_] = HT_RTC->YEARR & RTC_YEARR;
    sTime[_APPTIME_MON_] = HT_RTC->MONTHR& RTC_MONTHR;
    sTime[_APPTIME_DAY_] = HT_RTC->DAYR  & RTC_DAYR;
    sTime[_APPTIME_HOUR_] = HT_RTC->HOURR & RTC_HOURR;
    sTime[_APPTIME_MIN_] = HT_RTC->MINR  & RTC_MINR;
    sTime[_APPTIME_SEC_] = HT_RTC->SECR  & RTC_SECR;		
				for ( i = 0; i< _APPTIME_END_;i++)
					sTime[i]= BCD(sTime[i]);
				return SUCCESS;
	}
}
 return ERROR;
}


int
Rtc_Mod_SafeIo (char *sInDate)
{
		 return ERROR;
}	

#ifdef _DEL

#endif

int
Rtc_Get_Charp (char *TimeBase, unsigned long  tMode)
{
  char 	stemp[_APPTIME_END_];
  if (SUCCESS == Rtc_Get_SafeIo (TimeBase ))
    {
   //   *(TimeBase + _APPTIME_YEAR_) = sDate.Year;
    //  *(TimeBase + _APPTIME_MON_) = sDate.Month;
     // *(TimeBase + _APPTIME_DAY_) = sDate.Date;
     // *(TimeBase + _APPTIME_HOUR_) = sTime.Hours;
     // *(TimeBase + _APPTIME_MIN_) = sTime.Minutes;
      //*(TimeBase + _APPTIME_SEC_) = sTime.Seconds;
      return SUCCESS;
    }
  return ERROR;
}


#ifdef _DEL
int
Rtc_Get_SafeIo (RTC_DateTypeDef * sDate, RTC_TimeTypeDef * sTime,
		uint32_t tMode)
{
  RTC_DateTypeDef sDate1;
  RTC_TimeTypeDef sTime1;
  int i;
  HAL_StatusTypeDef iResult, iResult1, iResult2, iResult3;
  for (i = 0; i < _ReadTime_Retrynum_; i++)
    {
      iResult = HAL_RTC_GetTime (&hrtc, sTime, tMode);	//RTC_FORMAT_BIN);
      iResult1 = HAL_RTC_GetDate (&hrtc, sDate, tMode);	//RTC_FORMAT_BIN);
      iResult2 = HAL_RTC_GetTime (&hrtc, &sTime1, tMode);
      iResult3 = HAL_RTC_GetDate (&hrtc, &sDate1, tMode);
      if ((HAL_OK == iResult) && (HAL_OK == iResult1) && (HAL_OK == iResult2)
	  && (HAL_OK == iResult3))
	{
	  if (0 == _IsCmpDataUnit (*sTime, sTime1))
	    if (0 == _IsCmpDataUnit (*sDate, sDate1))
	      {
		return SUCCESS;
	      }
	}
    }
  return ERROR;
}
int
Rtc_Mod_SafeIo (char *sInDate, int DataForMat)
{
  RTC_DateTypeDef sDate;
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDatebak;
  RTC_TimeTypeDef sTimebak;
  HAL_StatusTypeDef iResult, iResult1;
  int i;
  sDate.Year = sInDate[_APPTIME_YEAR_];
  sDate.Month = sInDate[_APPTIME_MON_];
  sDate.Date = sInDate[_APPTIME_DAY_];
  sTime.Hours = sInDate[_APPTIME_HOUR_];
  sTime.Minutes = sInDate[_APPTIME_MIN_];
  sTime.Seconds = sInDate[_APPTIME_SEC_];
  debug_break (_debug_app_ModifyTime_);
  for (i = 0; i < _ReadTime_Retrynum_; i++)
    {
      iResult = HAL_RTC_SetTime (&hrtc, &sTime, DataForMat);
      iResult1 = HAL_RTC_SetDate (&hrtc, &sDate, DataForMat);
      if ((HAL_OK == iResult) && (HAL_OK == iResult1))
	{
	  if (SUCCESS == Rtc_Get_SafeIo (&sDatebak, &sTimebak, DataForMat))
	    {
	      if (0 == _IsCmpDataUnit (sDate, sDatebak))
		if (0 == _IsCmpDataUnit (sTime, sTimebak))
		  {
		    UpDateRtcDateTime_Set (SUCCESS);
		    return SUCCESS;
		  }
	    }
	}
    }
  return ERROR;
}

int
Rtc_Get_ModbusReg (USHORT * TimeBase, _tyTIMECNT * lRtcTime)
{
  char stemp[_APPTIME_END_];
  int iResult = Rtc_Get_Charp (stemp, RTC_FORMAT_BIN);
  if (SUCCESS == iResult)
    {
      *lRtcTime = GetTimeCnt_inputbin ((char *) stemp, S_SEC);
      if (TimeBase)
	{
	  *(TimeBase + _APPTIME_YEAR_) = stemp[_APPTIME_YEAR_];
	  *(TimeBase + _APPTIME_MON_) = stemp[_APPTIME_MON_];
	  *(TimeBase + _APPTIME_DAY_) = stemp[_APPTIME_DAY_];
	  *(TimeBase + _APPTIME_HOUR_) = stemp[_APPTIME_HOUR_];
	  *(TimeBase + _APPTIME_MIN_) = stemp[_APPTIME_MIN_];
	  *(TimeBase + _APPTIME_SEC_) = stemp[_APPTIME_SEC_];
	}
    }
  return iResult;
}
HT_RTC_Write
#endif

enum 		ENUMHT5019RTC
{
			_ht_5019rtc_WEEK_ = 0,
		_ht_5019rtc_YEARR_, //  = pRTCWrite[1] & RTC_YEARR;
    _ht_5019rtc_MONTHR_, // = pRTCWrite[2] & RTC_MONTHR;
    _ht_5019rtc_DAYR_, //   = pRTCWrite[3] & RTC_DAYR;
    _ht_5019rtc_HOURR_, //  = pRTCWrite[4] & RTC_HOURR;
    _ht_5019rtc_MINR_, //   = pRTCWrite[5] & RTC_MINR;
    _ht_5019rtc_SECR_, //   = pRTCWrite[6] & RTC_SECR;
	  _ht_5019rtc_END_, //   = pRTCWrite[6] & RTC_SECR;
};

struct   STHT5019RTC
{
	  char 		sTime[_ht_5019rtc_END_];
};



void   RTC_2_HT5019RTC( struct 		STTIME *stp , struct   STHT5019RTC  *st5019 )
{
			
}

void  Rtcunint_write( struct 		STTIME *stp)
{
}
	