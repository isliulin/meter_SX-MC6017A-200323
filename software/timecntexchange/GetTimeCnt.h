#ifndef		_gettimecnt_hpp_
#define		_gettimecnt_hpp_

enum
{
  _TimeChange_SUCCESS_ = 0,
  _TimeChange_ERROR_ = 1,
};

typedef unsigned long _tyTIMECNT;

enum
{
  S_SEC = 0,
  S_MIN = 1,
  S_DAY = 2,
  S_RTC = 3,
  S_FormatMask = 3,
  T_SEC = 0x0,
  T_MIN = 0x4,
  T_DAY = 0x8,

  T_YMDHMS = 0x0,		//
  T_HMS = 0x4,			//
  T_YMDHM = 0x8,		//
  T_YMDW = 0xC,			//
	_RET_	=			0x8000,	//Return Code
	_REV_	=			0x4000,	//Reverse Code
};


enum ENUMAPPTIME
{
  _APPTIME_YEAR_ = 0,
  _APPTIME_MON_,
  _APPTIME_DAY_,
  _APPTIME_HOUR_,
  _APPTIME_MIN_,
  _APPTIME_SEC_,
#if _USE_WEEK_
  _APPTIME_WEEK_,
#endif
  _APPTIME_END_,
};

struct STTIME
{
  char sTime[_APPTIME_END_];
};

int Time_Cnt_Exchange_Init (int GetDateTimeFun);
_tyTIMECNT GetTimeCnt_inputbin (char *Time, unsigned short Mode);

void GetCntTime (char *Addr, _tyTIMECNT SecCnt, unsigned short Mode);
_tyTIMECNT GetTimeCnt1 (char *sTime, unsigned short Mode);

#endif
