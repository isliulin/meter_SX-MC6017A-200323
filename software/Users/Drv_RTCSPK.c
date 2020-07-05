#include "pubset.h"
 
#include "rtc_drv.h"
//#include "unit_instance.h"
#include "datastruct.h"

extern const strFileSet FileSet;
extern const strFData FData;
#define BUSY_WAIT_TIME 98300 // About 1/10s in cycles at 9.8MHz
#define WAKE_NUM 125    // 125/16 = 7.825 periods/sec
#define WAKE_DEN 16

 
#define		_RTC_HCLK_			0x00C80000L			//71315第二版
 void  Time_Restore(void);
unsigned char Check8025TData(char *TmBCD) {
	u8 a, b, i;

	const unsigned char s8025Mask[] = { 0x7f, 0x7f, 0x3f, 0x7f, 0x3f, 0x1f,
			0xff };	return 0;
	for (i = 0; i < 7; i++)
		TmBCD[i] &= s8025Mask[i];
	for (i = 0; i < 7; i++) {

		a = (TmBCD[i] & 0xf0) >> 4;
		b = TmBCD[i] & 0xf;

		if (_8025T_WEEK_ == i) {
			u8 j, k, l;
			for (l = TmBCD[_8025T_WEEK_], k = 0, j = 0; ((j < 7) && l); j++, l
					/= 2) {
				if (l & 1)
					k++;
				if (k > 1)
					return 1;
			}
			continue;
		}
		if ((a > 9) || (b > 9))
			return 2;
	}
	if ((TmBCD[_8025T_MONTH_] & 0x1f) > 0x12)
		return (0x80 | _8025T_MONTH_);
	if (TmBCD[_8025T_DAY_] > 0x31)
		return (0x80 | _8025T_DAY_);
	if (TmBCD[_8025T_HOUR_] > 0x23)
		return (0x80 | _8025T_HOUR_);
	if (TmBCD[_8025T_MIN_] > 0x59)
		return (0x80 | _8025T_MIN_);
	if (TmBCD[_8025T_SEC_] > 0x59)
		return (0x80 | _8025T_SEC_);

	return 0;
}
/*
void SetupRTC(void) {

	u8 S;
	u8 Cnt;
	u8 K1, K2;
	u8 TmBCD[_8025T_END_];
	const u8 rtc_cfg1[] = { 0, 0, 0xE0 };//0x60 };
	const u8 rtc_cfg2[] = { 0, 0, 0xC0 };//0x40 };
                 return;
	Cnt = 0;
	K1 = 0;
	K2 = 0;

	GetRTCTm(TmBCD);
	if (RamData.b8025Terr) {
		RunState_TmErr = 1;
		SetEvtState(_EVT_TIME_);
		RamData.RTCSec = 60;
	}

	//	if(RamData.Pwr_On){
	//		Write_I2C(rtc_cfg1, 0x640D, 3); //UIE, 500ms, 2s compensation interval, 秒脉冲
	//	} else {
	//		Write_I2C(rtc_cfg2, 0x640D, 3); //UIE, 500ms, 2s compensation interval, 秒脉冲
	//	}

}
*/
void RTC_TrimSet(void) {

	u32 TCSet[2];
	/*
	 CopyRam((u8*)&TCSet, (u8*)&FlashInfo.RTCSet[2], 8);
	 //    if (rtc_shadow_busy_wait(RTC_WR | RTC_BUSY)){
	 //       return;
	 //	}
	 //	DelayMS(100);
	 if(RTC_Busy_Wait(1)){
	 return;
	 }
	 WAKEFROM |= RTC_WR; // Freeze and unlock the shadow registers.
	 if(RamData.Pwr_On){
	 TEMPCNTL = 0x00050006;  // 2秒启动一次
	 } else {
	 TEMPCNTL = 0x00050009;  // 16秒启动一次
	 }
	 #ifdef	_RTC_CAL_OFF_
	 TCCD = 0;
	 TCAB = 0;
	 RTCCAL = _RTC_HCLK_+ ((s32)FlashInfo.RTCSet[2]);
	 #else
	 TCCD = TCSet[0];
	 TCAB = TCSet[1];
	 RTCCAL = _RTC_HCLK_;
	 #endif

	 //	RTCWAKE |= TEMP_PWR|TEMP_COMP|TEMP_BAT;
	 //	RTCWAKE |= TEMP_COMP;
	 WAKEFROM &= ~RTC_WR; // Unfreeze, write and relock the shadow registers.
	 //	DelayMS(100);

	 tmp_busy_wait();
	 RTCCONT = TEMP_START;
	 //	DelayMS(100);


	 */
}

void SetBatTest(u8 Mode) {
	/*
	 if(((Mode)^(!(RTCWAKE&TEMP_BAT))) == 0){
	 RTC_Busy_Wait(1);
	 WAKEFROM |= RTC_WR;
	 if(Mode){
	 RTCWAKE |= TEMP_BAT;
	 } else {
	 RTCWAKE &= ~TEMP_BAT;
	 }
	 WAKEFROM &= ~RTC_WR;
	 }
	 */
}
/*
void Init_RTC(void) { //RTC初始化

	Rtcunint_init(1);//RTC_TrimSet();
	SetupRTC();//
	SetRTCOut();
	RamData.LastMin = 0;
	CheckTime();
}
*/
void SetRTCCnt(u32 RTC_Cnt) {
	/*
	 if (RTC_Busy_Wait(1))
	 return;
	 RamData.FrozeTime = WAKEFROM;


	 WAKEFROM |= RTC_WR;     // Freeze the shadow registers.
	 RTCSEC = Secs;       // Write new RTC value to the shadow registers.
	 RTCSUB = 0;             // Clear the fractional seconds.
	 */
	 struct 		STTIME  stTime;

	//u8 TmBCD[7],  Tm[16];                   
	unsigned char Cnt;
	GetCntTime(stTime.sTime, RTC_Cnt, (S_SEC | T_SEC));
	Cnt = GetWeek(&RTC_Cnt);  

	//RevCopyRam(TmBCD, Tm + 3, 3);
	//RevCopyRam(TmBCD + 4, Tm, 3);
	//TmBCD[3] = 1 << Cnt;
	//if (RamData.WDGTm < 10)
	//	RamData.WDGTm+= 10;
	Rtcunint_write(stTime.sTime);
//	RamData.b8025Terr = 0;

	//================
	RamData.RTCSec = HEX(stTime.sTime[_APPTIME_SEC_]);//  TmBCD[0]);
	//RamData.RTCMin =TmBCD[1];
	RamData.LastRTC = RTC_Cnt-1;
	RamData.xLastRTC = ~RamData.LastRTC;
	RamData.RTCTime = RTC_Cnt;
	RamData.LastMin = RTC_Cnt - RamData.RTCSec;

#ifndef		_CHGTIME_FROZE_
	SetRcdMin(&FlashInfo.RunInfo.LastMFrozeTm); //修改时间不做补充冻结
	SetRcdMin(&FlashInfo.RunInfo.LastYFrozeTm);
	SetRcdMin(&FlashInfo.RunInfo.LastDFrozeTm);
#endif
	RunState_TmErr = 0;
	_bEvent_Set(bRtcRefresh);
}
#ifdef _DEL
u32 GetRTCCnt(void) {

	unsigned char Cnt;

	return GetTimeCnt((u8*) &RamData.RTCTime, S_RTC | T_SEC);
	/*
	 u32	Secs;
	 u32	Retry;
	 Retry = BUSY_WAIT_TIME;
	 //	Secs = 1;
	 //	while(--Retry){
	 if(RTC_Busy_Wait(0)){
	 return 7;
	 }
	 WAKEFROM |= RTC_RD;
	 Secs = RTCSEC;
	 RamData.mSec = (u8)(RTCSUB>>24);
	 WAKEFROM &= ~RTC_RD;
	 if(Secs){
	 return Secs;
	 }
	 return 8;
	 //		Secs = 0x200;
	 //		while(--Secs);
	 //	}
	 //	return 2;

	 */
}
#endif

void SetRTCOut(void) { //设置秒脉冲输出
/*
 if(RamData.Pwr_On == 0){
 TMUXSEL = 0x00060000;
 } else if(RamData.SP_Mode == 0){
 TMUXSEL = 0x00030000;
 }else{
 TMUXSEL = 0x00060000;
 }
 */
}

#define _ATE_CAL_TEMP_D_    (220) // 22C is the ATE's Vref cal. temp.
void GetTempVBat(void) {
	/*
	 u32	Data;
	 s16	T;
	 Data = TEMP;
	 T = (Data >> STEMP_BIT) & STEMP_WIDTH_MASK;
	 //	T -= FlashInfo.RTCSet[0];
	 T = (s16)(((float)T)*((float)(13287+FlashInfo.RTCSet[1]))/16384);
	 T += _ATE_CAL_TEMP_D_;
	 RamData.Temperature = T;
	 T = (Data>>BSENSE_BIT)&BSENSE_WIDTH_MASK;
	 RamData.VBat[0] = T*1000/((u16)(10/BSENSE_LSB));
	 */
}

void ChkBatVolt(void) {
	/*
	 if(RamData.VBat[0]> (FlashInfo.SetInfo.BatLoLmt+10)){		//检查电池电压
	 if(FlashInfo.RunInfo.EState&_ESTATE_LOBAT_){
	 ClrEBit(_BIT_LOBAT_);
	 }
	 Bat1_State = 0;
	 Bat2_State = 0;
	 RamData.BatChkCnt = 0;
	 } else if(RamData.VBat[0] < FlashInfo.SetInfo.BatLoLmt){
	 if(RamData.VBat[0] < 150){
	 goto SetLoBat;
	 } else if(++RamData.BatChkCnt >= 10){
	 SetLoBat:
	 if((FlashInfo.RunInfo.EState&_ESTATE_LOBAT_) == 0){
	 SetEvtState(_EVT_BAT1_);
	 SetEBit(_BIT_LOBAT_);
	 }
	 Bat1_State = 1;
	 Bat2_State = 1;
	 RamData.BatChkCnt = 10;
	 }
	 }
	 */
}

void ChkRTCCal(void) { //获取当前温度
/*
 u32	Data;
 s16	T;
 if((RTCCONT & TEMP_ERR) == 0)
 {
 GetTempVBat();
 if(RTCWAKE&TEMP_BAT){
 ChkBatVolt();
 }
 if(((RTCWAKE&TEMP_BAT) == 0)&&(++RamData.TBatErrCnt == 5)){
 SetBatTest(1);
 RamData.TBatErrCnt = 0;
 }
 } else {
 if((RTCWAKE&TEMP_BAT)&&(++RamData.TBatErrCnt == 5)){
 SetBatTest(0);
 RamData.TBatErrCnt = -55;
 RamData.VBat[0] = 0;
 ChkBatVolt();
 }
 }
 RTCCONT = TEMP_START;
 */
}

u8 SetRTCTrim(s16 Diff, u8 Mode) {
#ifdef _DEL
	if (Mode == 1) { //温度校正(10~40度可以校正, 0为重置)
		//	if((Diff > 100)&&(Diff < 400)){
		Diff -= RamData.Temperature;
		Diff = (s16) ((float) Diff * (float) 16384 / (13287
				+ FlashInfo.RTCSet[1]));
		Diff = FlashInfo.RTCSet[0] - Diff;
		//	} else if(Diff){
		//		return FAIL;
		//	}
		Up_u16Data((u16*) &FlashInfo.RTCSet[0], (u16) Diff);
		//pxopt		SetEChipCal();
		return SUCCESS;
	} else if (Mode == 2) { //温度梯度校正


	} else if (Mode == 3) { //时钟误差校正(10~40度可以校正)
		if ((RamData.Temperature > 100) && (RamData.Temperature < 400)) {
			Diff = (s16) ((float) Diff * ((float) _RTC_HCLK_) / 10000000);
			Diff += FlashInfo.RTCSet[2];
			Up_u16Data((u16*) &FlashInfo.RTCSet[2], (u16) Diff);

			RTC_TrimSet();
			return SUCCESS;
		}
	}
	#endif
	return FAIL;
}

void Check_WakeMode(void) {
	/*
	 if(WAKEFROM&0x00100000){
	 RamData.exti_h = 1;
	 }
	 */
}

void RTC_SetWakeup(u8 Secs) {
	/*
	 int32_t rtcwake, tempcntl, tempcntlOld;
	 u32	alm;
	 u8	TCnt;
	 //	rtcwake = (RTCWAKE & 0xFFFF8000)|EW_TMR |1;						//设置定时唤醒
	 rtcwake = TEMP_COMP|EW_TMR|1;

	 alm = 0;
	 if(RamData.Key_State){
	 TCnt = 2;													//0.25秒唤醒
	 } else {
	 RamData.Last_Key = 0;
	 TCnt = 5;													//最小唤醒时间1秒
	 while(Secs > 1){
	 Secs >>= 1;
	 TCnt++;
	 }
	 rtcwake |= BIT20|EW_VSYS;									//设置按键(BIT20)/电源唤醒
	 if((RamData.mSec > 192)&&(TCnt <= 5)){						//设置760毫秒唤醒(其他1秒唤醒实际时间为1.07秒
	 TCnt = 2;
	 rtcwake |= 4;
	 rtcwake &= ~1;
	 } else if(RamData.EOpen_PwrOff == 0){						//停电合盖后不再检测开盖
	 if(RamData.EOpen_State){
	 if(TCnt > 6){
	 TCnt = 6;
	 }
	 } else {
	 rtcwake |= BIT19;
	 }
	 }

	 }
	 tempcntlOld = TEMPCNTL;
	 tempcntl = (tempcntlOld & 0xFFFFFFE0)|TCnt;						//温度校正时间设定

	 if((rtcwake != RTCWAKE)||(tempcntl != TEMPCNTL)){
	 RTC_Busy_Wait(1);
	 WAKEFROM = RTC_WR;
	 RTCWAKE = rtcwake;
	 TEMPCNTL = tempcntl;    // Set the register.
	 WAKEFROM = 0;
	 RTC_Busy_Wait(0);
	 }
	 */
} // static bool batmode_set_rtcwake_tempcntl(void)

void EnterLowPwrMode(u8 WakeTm) { //进入休眠状态 WakeTm - 唤醒时间
/*
 u32	Cnt;
 RESET_WD();
 tmp_busy_wait();
 RTC_SetWakeup(WakeTm);
 if(LCDMODE&LCD_8_0P3){
 RTCCONT = SLEEP|LCD_ONLY;
 } else {
 RTCCONT = SLEEP;
 }
 while(1);
 */
}

void RTC_ISR(void) {

	//	RTCI &= ~RTC_M_AL;
}
extern const unsigned char Const699[];
//const unsigned char Const6991[6]={0x99,0x99,0x99,0x99,0x99,0x99};
//const u8 MonthDates[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //大小月设置
static const u8 Tm_BCD_L[] = { 6, 5, 3 };
#ifdef _DEL
u32 GetTimeCnt(u8* Time, u16 Mode) {}
#endif
static const struct {
	u8 Bias;
	u8 Lth;
} T_Time_Lth[] = { { 0, 6 }, //T_YMDHMS
		{ 3, 3 }, //T_HMS
		{ 0, 5 }, //T_YMDHM
		{ 0, 4 }, //T_YMDW
		};
#ifdef _DEL
void GetCntTime(u8* Addr, u32 SecCnt, u16 Mode) {}

#endif
void ChangeTime(u32 NewCnt) {

	u32 Diff;
	u32 Cnt;
	u32 *pTm;
	Diff = NewCnt - GetRTCCnt();
	SetRTCCnt(NewCnt);
	if (NewCnt < (86400 * 367)) {
		SetEBit(_BIT_TIME_0_);
	} else {
		ClrEBit(_BIT_TIME_0_);
	}
	pTm = (u32*) &FlashInfo.RunInfo.RcdTm;
	for (Cnt = 0; Cnt < (sizeof(FlashInfo.RunInfo.RcdTm) / 4); Cnt++) {
		if ((*pTm) && (*pTm > 86400)) {
			Up_u32Data(pTm, (*pTm + Diff));
		}
		pTm++;
	}
}

void CheckTime(void) {

	u32 TCnt;
	u8* Addr;

	if ((RamData.RTCSec < 60) && (RamData.LastMin)) {
		RamData.RTCTime = RamData.LastMin + RamData.RTCSec;
		return;
	}
	RamData.Event_Min = 1;

	TCnt = GetRTCCnt();
	if (TCnt < 86400) {
		if (++RamData.TmErrCnt < 3) {
			TCnt = RamData.RTCTime;
		}
		RamData.Tm_Err = 1;
	} else {
		RamData.TmErrCnt = 0;

	}
	if (TCnt < 86400) {
		if (RamData.LastRTC == ~RamData.xLastRTC) {
			TCnt = RamData.LastMin + RamData.RTCSec;
		} else {
			TCnt = _DFT_TIME_; //缺省起始时间
		}
		if ((RamData.Pwr_On) && (RamData.TmErrCnt >= 3)) {
			Addr = GetFrozeStart(_FROZE_PWR_OFF_, 0);
			if ((Addr) && (TCnt < (*(u32*) Addr))) {
				TCnt = *(u32*) Addr;
			}
			SetRTCCnt(TCnt);
			SetEvtState(_EVT_TIME_);
			ClrEBit(_BIT_DAY_CHG_);
		}
		RunState_TmErr = 1;
	} else if ((TCnt == RamData.LastRTC) && (RamData.Pwr_On)) {
		if (++RamData.RTCErrTm > 3) {
			RunState_TmErr = 1;
			SetEvtState(_EVT_TIME_);
			ClrEBit(_BIT_DAY_CHG_);
		}
	} else {
		RamData.RTCErrTm = 0;
	}
	RamData.RTCTime = TCnt;
	RamData.LastMin = TCnt;
	RamData.RTCSec = ChgMOD_2(&RamData.LastMin, 60);
	if (RamData.Tm_Err == 0) {
		RamData.LastRTC = TCnt;
		RamData.xLastRTC = ~TCnt;
	} else {
		RamData.LastMin -= 60;
		RamData.RTCSec = 58;
	}
	RamData.LastRTC = TCnt;
	RamData.xLastRTC = ~TCnt;
	return;
}

void Do_BeepTask(void) {
#ifdef _DEL
	u8 T;
	if (--RamData.BeepTm) { //每5毫秒减1
		if (RamData.BeepMode == CARD_OK_BEEP) {
			if (RamData.BeepTm == 125) {
#ifdef		_METER_LOC_
				Pin_Spk_start();
			//	runSndaNormal();//runSndaOneShot ();//Init_TB0(1);
#endif
			}
		} else if (RamData.BeepMode == CARD_ERR_BEEP) {
			if ((RamData.BeepTm % 25) == 0) {
				T = RamData.BeepTm / 25;
				if (T & 1) {
#ifdef		_METER_LOC_
					Pin_Spk_start();
					//runSndaNormal(); //runSndaOneShot();//	Init_TB0(1);
#endif
				} else {
#ifdef		_METER_LOC_
					//stopSndaSound();//Init_TB0(0);// runSndaOneShot();//Init_TB0(0);
					Pin_Spk_end();
#endif
				}
			}
		}
	} else {
#ifdef		_METER_LOC_
		//stopSndaSound();//Init_TB0(0);
		Pin_Spk_end();
#endif
	}
	#endif
}

void StartBeep(u8 Mode) {
	#ifdef _DEL
	RamData.BeepMode = Mode;
	RamData.BeepTm = 150;
#ifdef		_METER_LOC_
//	Pin_Spk_start()		;startSnda();
#endif
	#endif
}
 

 void  Time_Restore(void)
 {
	 unsigned long TCnt,Addr;
	 if(_IsReset())
	 {
			Addr = (u32)GetFrozeStart(_FROZE_PWR_OFF_, 0);
			if ((Addr) && (TCnt < (*(u32*) Addr))) 
				{
				TCnt = *(u32*) Addr;
			}
				if (0 == TCnt)
						TCnt = _DFT_TIME_;
			SetRTCCnt(TCnt);
			SetEvtState(_EVT_TIME_);
			ClrEBit(_BIT_DAY_CHG_);
		}
}
	
	