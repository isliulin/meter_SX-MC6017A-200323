 
#include "pubset.h"
#include "GetTimeCnt.h"
#include "metersystem.h"
#include "globlvar_debug.h"


#define 	u32 		unsigned long
#define 	u16  	unsigned short

static const char Const699[] = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99 };
const unsigned char MonthDates[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };	//大小月设置
static const unsigned char Tm_BCD_L[] = { 6, 5, 3 };

static u16
ChgMOD_1 (u32 * pData, u16 Num)
{

  u16 n;
  n = (u16) ((*pData) % Num);
  *pData /= Num;
  return n;
}

static const struct
{
  unsigned char Bias;
  unsigned char Lth;
} T_Time_Lth[] =
{
  {
  0, 6},			//T_YMDHMS
  {
  3, 3},			//T_HMS
  {
  0, 5},			//T_YMDHM
  {
  0, 4},			//T_YMDW
};
static long lGetDateTimeFun;

int
Time_Cnt_Exchange_Init (int GetDateTimeFun)
{

  lGetDateTimeFun = GetDateTimeFun;

  return _TimeChange_SUCCESS_;
}

void
GetCntTime (char *Addr, _tyTIMECNT SecCnt, u16 Mode)
{				//0            //由整数计算当前时间
  int iResult;
  unsigned char Cnt;
  unsigned char SP;
  unsigned char TP;
  u16 Days;
  // unsigned char      Week;                   //周次处理
  struct STTIME stTime;
  char *TmBCD = stTime.sTime;
  u32 ltemp = (u32) Addr;

  SP = Mode & S_FormatMask;
  TP = (Mode & 0x0C) >> 2;
  if (SP == S_RTC)
    {
      if (lGetDateTimeFun)
	iResult = ((int (*)(unsigned char *)) lGetDateTimeFun) (TmBCD);
      if (TP == (T_YMDW >> 2))
	{
	  TmBCD[3] = (TmBCD[1] >> 5);
	  if (TmBCD[3] == 7)
	    {
	      TmBCD[3] = 0;
	    }
	}
      TmBCD[1] &= 0x1F;
      if (Mode & _REV_)
	{
	  RevCopyRam (Addr, (char *) TmBCD + T_Time_Lth[TP].Bias,
		      T_Time_Lth[TP].Lth);
	}
      else
	{
	  memcpy (Addr, TmBCD + T_Time_Lth[TP].Bias, T_Time_Lth[TP].Lth);
	}
      return;
    }
  if (SecCnt == 0)
    {
      memset (Addr, 0, T_Time_Lth[TP].Lth);
      return;
    }
  else if (SecCnt == 0xFFFFFFFF)
    {
      memset (Addr, 0x99, T_Time_Lth[TP].Lth);
      return;
    }
  TmBCD[2] = 0;
  TmBCD[3] = 0;
  TmBCD[4] = 0;
  TmBCD[5] = 0;			//*(u32 *)(TmBCD+2) = 0;
  switch (SP)
    {
    case S_SEC:
      TmBCD[5] = ChgMOD_1 (&SecCnt, 60);
      // TmBCD[5] = SecCnt%60;
      // SecCnt = SecCnt/60;
    case S_MIN:
      TmBCD[4] = ChgMOD_1 (&SecCnt, 60);
      // TmBCD[4] = SecCnt%60;
      // SecCnt = SecCnt/60;
      TmBCD[3] = ChgMOD_1 (&SecCnt, 24);
      // TmBCD[3] = SecCnt%24;
      // SecCnt = SecCnt/24;
    }
  if (SecCnt)
    {
      SecCnt--;
      Days = ChgMOD_1 (&SecCnt, (365 * 4 + 1));
      TmBCD[0] = ((unsigned char) SecCnt) << 2;
      // SecCnt = Days;
      // TmBCD[0] = (SecCnt/(365*4+1))<<2;
      // SecCnt = GetMOD(&SecCnt, (364*4+1));
      //SecCnt = SecCnt%(365*4+1);
      if (Days == 0)
	{
	  TmBCD[1] = 1;
	  TmBCD[2] = 1;
	}
      else
	{
	  //TmBCD[0] += (SecCnt-1)/365;
	  //SecCnt = (SecCnt-1)%365;
	  SecCnt = Days - 1;
	  Days = ChgMOD_1 (&SecCnt, 365);
	  TmBCD[0] += (unsigned char) SecCnt;
	  if ((TmBCD[0] & 3) == 0)
	    {
	      Days++;
	    }
	  TmBCD[1] = 0;
	  while (1)
	    {
	      if ((TmBCD[1] == 1) && ((TmBCD[0] & 3) == 0))
		{
		  Cnt = 29;
		}
	      else
		{
		  Cnt = MonthDates[TmBCD[1]];
		}
	      if (Days < Cnt)
		{
		  break;
		}
	      Days -= Cnt;
	      TmBCD[1]++;
	    }
	  TmBCD[2] = Days + 1;
	  TmBCD[1]++;
	}
    }
  else
    {
      *(u16 *) TmBCD = 0;
      //TmBCD[0] = 0;
      //TmBCD[1] = 0;
      TmBCD[2] = 0;
    }
  Cnt = 0;
  do
    {
      TmBCD[Cnt] = BCD (TmBCD[Cnt]);
    }
  while (++Cnt < 6);
  if (Mode & _REV_)
    {
      RevCopyRam (Addr, TmBCD + T_Time_Lth[TP].Bias, T_Time_Lth[TP].Lth);
    }
  else
    {
      memcpy (Addr, TmBCD + T_Time_Lth[TP].Bias, T_Time_Lth[TP].Lth);
    }

}

//时间转换为整数
_tyTIMECNT
GetTimeCnt (char *sTime, u16 Mode)
{				//0                                
	_tyTIMECNT TimeCnt;
  struct STTIME stTime[2];
  char *TmBCD = stTime[0].sTime;
  char *Tm_2 = stTime[1].sTime;
  unsigned char Cnt,cResult,ctemp,cHexMonth;  

  ClrRam ((char *) stTime, sizeof (stTime));
  ctemp = (Mode & S_FormatMask);
  if (S_FormatMask == ctemp)
    {
      if (lGetDateTimeFun)
	{
	  cResult = ((int (*)(char *)) lGetDateTimeFun) (TmBCD);
	  if (SUCCESS != cResult)
	    return 0xFFFFFFFF;
	}
    }
  else
    {
      if (NULL == sTime)
		  return 0;
	    if (ERROR == CheckBCD (TmBCD, Tm_BCD_L[ctemp]))
		  return 0;

      if (Mode & _REV_)
	{
	  RevCopyRam (TmBCD, (char *) (sTime), Tm_BCD_L[ctemp]);
	}
      else
	{
	  memcpy (TmBCD, (char *) (sTime), Tm_BCD_L[ctemp]);
	}
    }
  if (0 == memcmp ((char *) TmBCD, (char *) Const699, _APPTIME_SEC_))
    return 0xFFFFFFFF;

  for (Cnt = 0; Cnt < _APPTIME_END_; Cnt++)
    Tm_2[Cnt] = HEX (TmBCD[Cnt]);

  cHexMonth = Tm_2[_APPTIME_MON_] - 1;

  TimeCnt =
    (Tm_2[_APPTIME_YEAR_] * ((u16) 365) + ((Tm_2[_APPTIME_YEAR_] + 3) >> 2));
  Cnt = 0;
  while (Cnt < cHexMonth)
    {
      TimeCnt += MonthDates[Cnt];
      if (((Tm_2[_APPTIME_YEAR_] & 3) == 0) && (Cnt == 1))
	{
	  (TimeCnt)++;
	}
      Cnt++;
    }
  (TimeCnt) += Tm_2[2];
  (TimeCnt) = (TimeCnt) * 24 + Tm_2[_APPTIME_HOUR_];
  (TimeCnt) = (TimeCnt) * 60 + Tm_2[_APPTIME_MIN_];
  (TimeCnt) = (TimeCnt) * 60 + Tm_2[_APPTIME_SEC_];
  GetCntTime (Tm_2, (TimeCnt), (u16) (S_SEC | T_SEC));
  if (memcmp (Tm_2, TmBCD, _APPTIME_END_))
    {
      debug_break (_debug_errno_timezero_);
      return 0;
    }
  ctemp = (Mode & 0x0C);
  if (ctemp == T_SEC)
    {
      return (TimeCnt);
    }
  else if (ctemp == T_MIN)
    {
      ChgMOD_1 (&TimeCnt, 60);
      return (TimeCnt);
    }
  else if (ctemp == T_DAY)
    {
      return (TimeCnt / 86400);
    }
  else
    {
      debug_break (_debug_errno_timezero_);
      return 0;
    }
}

_tyTIMECNT
GetCurrTimeCnt (_tyTIMECNT * lTime)
{				//0                         //时间转换为整数
  struct STTIME stTime;
  *lTime = GetTimeCnt (stTime.sTime, S_RTC | T_SEC);	//时间转换为整数
  return (*lTime);
}

		//0                         //时间转换为整数
_tyTIMECNT
GetTimeCnt_inputbin (char *Time, u16 Mode)
{
  return GetTimeCnt (Time, Mode);	//时间转换为整数        
}

char
GetWeek (_tyTIMECNT SecCnt)
{
  return ((((SecCnt) / 86400) + 5) % 7);
}
