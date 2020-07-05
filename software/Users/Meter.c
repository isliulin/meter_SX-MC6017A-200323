
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
 
 
 #include "eeprom.h"
 #include "PubSet.h"
 
#define _MultiPort_Pluse_Set(a)  {}
extern  const  strFileSet FileSet;
u32 GetComUse(u8 FL) { //计算当前正向组合电量

	u32 Cnt;
	u8 ActMode;
	u32 Use[2];
	//strSectUse *pFL;
	CopyRam((u8*) &Use, (u8*) GetFLBase(FL), 8);
	Use[0] -= Use[1];
	ActMode = FlashInfo.SetInfo.ActUseMode;
	Cnt = 0;
	if (ActMode & 1) {
		Cnt += Use[0];
	}
	if (ActMode & 2) {
		Cnt -= Use[0];
	}
	if (ActMode & 4) {
		Cnt += Use[1];
	}
	if (ActMode & 8) {
		Cnt -= Use[1];
	}
#ifdef		_COM_MODE_
	return Cnt + FlashInfo.RunInfo.ComUseDiff[FL];
#else
	return Cnt;
#endif
}

void Up_Remain(void) { //计算剩余金额

	RamData.s_Remain = FlashInfo.SafeInfo.SumCharge - RamData.LVSave[0].SumMoney;
	if (RamData.s_Remain >= 0) {
		RamData.u_Remain = RamData.s_Remain;
		RamData.u_OverDraft = 0;
	} else {
		RamData.u_Remain = 0;
		RamData.u_OverDraft = -RamData.s_Remain;
	}
}

void GetAlmState(void) { //获取当前状态

	u8 Alm;
	if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		if (FlashInfo.RunInfo.EState & _ESTATE_FAR_ALM_) {
			Alm = ALM_ALM1;
		} else {
			Alm = ALM_NORM;
		}
		if (RamData.u_Remain > FlashInfo.SafeInfo.Alm1) {
		} else if (RamData.u_Remain > FlashInfo.SafeInfo.Alm2) {
			Alm = ALM_ALM1;
		} else if (RamData.u_Remain) {
			Alm = ALM_ALM2;
		} else {
			if (RamData.u_OverDraft < FlashInfo.SetInfo.OverDraftLimit) {
				Alm = ALM_OVERDRAFT;
			} else {
				Alm = ALM_OVEROFF;
			}
		}
		RamData.Alm_State = Alm;
		Protect_State = 0;
		if (FlashInfo.RunInfo.EState & _ESTATE_PROTECT_CMD_) {
			Protect_State = 1;
			RamData.Next_JD = 1;
		} else if (RamData.Alm_State < ALM_OVEROFF) {
			if (FlashInfo.RunInfo.EState & _ESTATE_JD_CMD_) {
				RamData.Next_JD = 0;
			} else if ((FlashInfo.RunInfo.EState & _ESTATE_OVER_OFF_) == 0) {
				RamData.Next_JD = 1;
			} else if ((RamData.Alm_State <= ALM_ALM2) && (RamData.s_Remain
					> ((s32) FlashInfo.SetInfo.JDOnMinPurse))) {
				RamData.Next_JD = 1;
			} else {
				RamData.Next_JD = 0;
			}
		} else {
			RamData.Next_JD = 0;
		}
		if (RamData.u_OverDraft) {
			RunState_OverDraft = 1;
		} else {
			RunState_OverDraft = 0;
		}
	} else {//#else
		if (FlashInfo.RunInfo.EState & _ESTATE_FAR_ALM_) {
			RamData.Alm_State = ALM_ALM1;
		} else {
			RamData.Alm_State = ALM_NORM;
		}
		Protect_State = 0;
		if (FlashInfo.RunInfo.EState & _ESTATE_PROTECT_CMD_) {
			Protect_State = 1;
			RamData.Next_JD = 1;
		} else if (FlashInfo.RunInfo.EState & _ESTATE_JD_CMD_) {
			RamData.Next_JD = 0;
		} else {
			RamData.Next_JD = 1;
		}
	}//#endif
	if (RamData.Next_JD) {
		JD_CMD_State = 0;
		if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
			if ((FlashInfo.RunInfo.EState & _ESTATE_ALM_OFF_) == 0) {
				ClrRcdTime(&FlashInfo.RunInfo.RcdTm.JDOverIphTime);
			}
		} else {//#else
			ClrRcdTime(&FlashInfo.RunInfo.RcdTm.JDOverIphTime);
		} //#endif
	} else {
		JD_CMD_State = 1;
	}
	if (RamData.Alm_State) {
		JD_OFF_AlmState = 1;
	} else {
		JD_OFF_AlmState = 0;
	}
	if (RamData.Alm_State <= ALM_ALM2) {
		ClrEBit(_BIT_OVERDAFT_);
	}
}

u8 ChkJDWaitAM(void) {

	if ((FlashInfo.SetInfo.AMOffDelayTm) && (FlashInfo.SetInfo.JDWaitAM)
			&& (labs(RamData.Iph.sVI) > FlashInfo.SetInfo.JDWaitAM)) {
		return SUCCESS;
	}
	return FAIL;
}

u8 ChkTimeGate(u32 *pTime) {

	if (*(pTime)) {
		if ((*pTime - RamData.RTCTime - 1) > (86400 * 2)) {
			return SUCCESS;
		}
	}
	return FAIL;
}

void Do_JDOff(void) {
#ifdef _DEL
	//u32 i;
	if ((_ISJDMODE_IN()) && (ChkJDWaitAM() == SUCCESS)) { //if((_JD_MODE_)&&(ChkJDWaitAM() == SUCCESS)){
		if (FlashInfo.RunInfo.RcdTm.JDOverIphTime == 0) {
			Up_u32Data(&FlashInfo.RunInfo.RcdTm.JDOverIphTime, (RamData.RTCTime
					+ ((u32) FlashInfo.SetInfo.AMOffDelayTm) * 60));
		} else if (ChkTimeGate(&FlashInfo.RunInfo.RcdTm.JDOverIphTime)
				== SUCCESS) {
			if (FlashInfo.RunInfo.FarJDPreTm) {
				Up_u32Data(&FlashInfo.RunInfo.RcdTm.FarJDOffPreTime,
						RamData.RTCTime + ((u32) FlashInfo.RunInfo.FarJDPreTm)
								* 300);
			}
			ClrRcdTime(&FlashInfo.RunInfo.RcdTm.JDOverIphTime);
			SetJD(_TURNOFF_);
		}
	} else {
		if (FlashInfo.RunInfo.FarJDPreTm) {
			Up_u32Data(&FlashInfo.RunInfo.RcdTm.FarJDOffPreTime,
					RamData.RTCTime + ((u32) FlashInfo.RunInfo.FarJDPreTm)
							* 300);
		}
		ClrRcdTime(&FlashInfo.RunInfo.RcdTm.JDOverIphTime);
		//	Up_u8Data(&FlashInfo.RunInfo.FarJDPreTm, 0);
		SetJD(_TURNOFF_);
	}
	#endif
}

void ChkAlmJD(void) {

	GetAlmState();
	if (RamData.Next_JD == 1) {
		SetJD(_TURNON_);
		ClrEBit(_BIT_OVER_OFF_);
	} else {
		Do_JDOff();
	}
}

static const __code strMvData mvMonthForze[] = { //获取正向/反向/组合有功电能
		SET_PCHAR( vComData+4, 0), READ_USEDATA( TYPE_ACT_USE, 4), READ_USEDATA( TYPE_REV_ACT_USE, 4), READ_USEDATA( TYPE_COM_ACT_USE, 4), READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_1, 4), READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_1, 4), READ_USEDATA( TYPE_COM_ACT_USE|TYPE_FEILV_1, 4), READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_2, 4), READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_2, 4), READ_USEDATA( TYPE_COM_ACT_USE|TYPE_FEILV_2, 4), READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_3, 4), READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_3, 4), READ_USEDATA( TYPE_COM_ACT_USE|TYPE_FEILV_3, 4), READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_4, 4), READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_4, 4), READ_USEDATA( TYPE_COM_ACT_USE|TYPE_FEILV_4, _RET_|4), };
const strMvData __code mvHFRcd_1[] = { NORM_COPY(((u8*)&vRamData.Vph.sVI), 2), NORM_COPY(((u8*)&vRamData.Iph.sVI), 3), NORM_COPY(((u8*)&vRamData.Freq), _RET_|2), };
const strMvData __code mvHFRcd_2[] = { NORM_COPY(((u8*)&vRamData.Pph.sVI), _RET_|3), };
const strMvData __code mvHFRcd_3[] = { NORM_COPY((u8*)&vRamData.Pfph, _RET_|2), };

typedef struct {
	u32* pLastTm; //上次冻结时间
	u8 FTmBias; //冻结时间偏移
	u8 FTmLth; //冻结时间长度
	u8 FTmNum; //冻结时间个数
	u8 FrozeLth; //追加冻结次数
} strMulFrozeSet;

static const __code strMulFrozeSet MulFrozeSet[] = { {
		&FlashInfo.RunInfo.LastMFrozeTm, 2, 2, 3, 12 }, {
		&FlashInfo.RunInfo.LastYFrozeTm, 1, 3, 4, 5 }, };

u8 GetFrozeLst(strMulFrozeSet const __code *pMulSet, u8 *pTm, u32* FrzLst) {

	u8 FTm[4][4];
	u8 Tm[6];
	u8 *p1, *p2;
	s32 k;
	u32 Cnt;
	u32 Bk;
	u32 MaxCnt;
	u32 FrzNext;
	u32 LNum;
	u32 Retry;
	u32 CmpCnt;
	ClrRam((u8*) &FTm, sizeof(FTm));
	p1 = (u8*) &FTm;
	p2 = pTm;
	Cnt = pMulSet->FTmNum;
	do {
		CopyRam(p1, p2, pMulSet->FTmLth);
		p1 += 4;
		p2 += pMulSet->FTmLth;
	} while (--Cnt);
	p1 = (u8*) &FTm;
	MaxCnt = pMulSet ->FTmNum;
	for (CmpCnt = 0; CmpCnt < (pMulSet ->FTmNum - 1); CmpCnt++) { //从大到小排序
		p2 = p1 + 4;
		for (Cnt = (CmpCnt + 1); Cnt < pMulSet ->FTmNum; Cnt++) {
			k = Cmp_4(p1, p2);
			if (k < 0) {
				Bk = *(u32*) p1;
				*(u32 *) p1 = *(u32*) p2;
				*(u32*) p2 = Bk;
			} else if (k == 0) {
				if ((*(u32*) p1) != 0) {
					*(u32*) p2 = 0;
					MaxCnt--;
				}
			}
			p2 += 4;
		}
		p1 += 4;
	}
	GetCntTime(Tm, RamData.RTCTime, S_SEC | T_YMDHM);
	LNum = 0;
	//	MaxCnt = 0;
	Retry = 0;
	Cnt = 0;
	while (LNum < pMulSet ->FrozeLth) {
		k = strCmp(Tm + pMulSet->FTmBias, FTm[Cnt], pMulSet->FTmLth);
		if (k >= 0) {
			ClrRam(Tm + pMulSet->FTmBias, 5 - pMulSet->FTmBias);
			CopyRam(Tm + pMulSet->FTmBias, FTm[Cnt], pMulSet->FTmLth);
			FrzNext = GetTimeCnt(Tm, S_MIN | T_MIN);
			if (FrzNext) {
				if (FrzNext < *pMulSet->pLastTm) {
					break;
				} else if ((FrzNext == (*pMulSet->pLastTm))
						&& ((RamData.FrozeTmChg == 0) || (RamData.FrozeTime
								!= FrzNext))) {
					break;
				}
				if (*(u16*) FTm[Cnt] == *(u16*) FlashInfo.SetInfo.FrozeDT[0]) {
					FrzNext |= 0x80000000;
				}
				FrzLst[LNum] = FrzNext;
				LNum++;
			}
		}
		if (++Cnt >= MaxCnt) {
			if (++Retry > 12) {
				break;
			}
			Cnt = pMulSet->FTmBias;
			do {
				if (Cnt == 1) {
					Tm[0] = BCD(HEX(Tm[0]) - 1);
					Tm[1] = 0x12;
					Tm[2] = 0x99;
					break;
				} else {
					if (Tm[1] > 1) {
						Tm[1] = BCD(HEX(Tm[1]) - 1);
						Tm[2] = 0x99;
						break;
					}
				}
			} while (--Cnt);
			Cnt = 0;
		}
	}
	if (LNum) {
		RevStr((u8*) FrzLst, LNum * 4);
		Rev4Lst((u8*) FrzLst, LNum);
	}
	return LNum;
}

void Do_JDOffPreOver(void) { //预跳闸结束处理
#ifdef _DEL
	ClrEBit(_BIT_JD_CMD_);
	Up_u8Data(&FlashInfo.RunInfo.FarJDPreTm, 0);
	if ((_ISJDMODE_OUT()) || (FlashInfo.RunInfo.EState & _ESTATE_OFF_PRE_) || (JD_State
			== 0)) { //if((_JD_MODE_== 0)||(FlashInfo.RunInfo.EState&_ESTATE_OFF_PRE_)||(JD_State == 0)){
		ChkAlmJD(); //立即合闸
	} else {
		GetAlmState(); //合闸允许
	}
	#endif
}

#ifdef		_USE_CARD_
void GetPrice(void) { //计算当前电价
	strLTbl stLTBl; //阶梯表(2*60B
	u8 Cnt;
	//u8 LStep;
	u32 MonthUse;
	u32 LUse[_MAX_LADDER_];
	struct {
		u32 StepPrice; //当前阶梯电价
		u32 FLPrice; //当前费率电价
		u32 ePrice; //当前总电价
	} tPrice;
	tPrice.ePrice = 0;
	tPrice.StepPrice = 0;
	//CopyRam((u8*)&tPrice.ePrice, (u8*)(GAddr.pPriceTbl[0]+(RamData.LVSave[0].FeilvNum*3)), 3);
	CopyExteeprom((u8*)&tPrice.ePrice, _Price_Get(0,RamData.LVSave[0].FeilvNum), 3);
	tPrice.FLPrice = tPrice.ePrice;
	CopyExteeprom((u8*)&stLTBl, _LTBl_Get(0),sizeof(stLTBl));
	if (CheckZero((u8*) (&stLTBl), sizeof(DftLTbl.LUse))) { //if(CheckZero((u8*)(GAddr.pLTbl[0]), sizeof(DftLTbl.LUse))){
		MonthUse = GetComUse(0);
		if ((FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) == 0) {
			MonthUse -= FlashInfo.RunInfo.LastUse;
		} else {
			MonthUse -= FlashInfo.RunInfo.LastYUse;
		}
		if (((s32) MonthUse) < 0) {
			MonthUse = 0;
		}
		CopyRam((u8*) &LUse, (u8*) &stLTBl, sizeof(LUse)); //CopyRam((u8*)&LUse, (u8*)GAddr.pLTbl[0], sizeof(LUse));
		for (Cnt = 0; Cnt < _MAX_LADDER_; Cnt++) {
			if (MonthUse < LUse[Cnt]) {
				break;
			}
			if ((Cnt) && (LUse[Cnt] <= LUse[Cnt - 1])) {
				break;
			}
		}
		Up_u8Data(&FlashInfo.RunInfo.LStep, Cnt);
		//CopyRam((u8*)&tPrice.StepPrice, (u8*)(GAddr.pLTbl[0]+(_MAX_LADDER_*4)+(Cnt*3)), 3);
		CopyExteeprom((u8*)&tPrice.StepPrice, _LTBl_LPrice_Get(0,Cnt), 3);
		tPrice.ePrice += tPrice.StepPrice;
		RamData.Use_Step = 1;
	} else {
		RamData.Use_Step = 0;
		Up_u8Data(&FlashInfo.RunInfo.LStep, 0);
	}
	Copy_FlashInfo((u8*) &FlashInfo.RunInfo.StepPrice, (u8*) &tPrice,
			sizeof(tPrice));

}

u32 Inc_Money(u32 Price, u8 Inc) { //用电金额处理(用电量转为用电金额)

	u32 M;
	u8 Cnt;
	M = Price * Inc;
	Cnt = 0;
	do {
		if ((*((&FlashInfo.SafeInfo.I_Mul) + Cnt)) > 1) {
			M *= (*((&FlashInfo.SafeInfo.I_Mul) + Cnt));
		}
	} while (++Cnt < 2);
	M += RamData.LVSave[0].CentDots;
	RamData.LVSave[0].CentDots = ChgMOD_1(&M, 10000);
	if (M) {
		RamData.LVSave[0].SumMoney += M;
		Up_Remain();
	}
	return M;
}

void ChkUseState(void) {

	u8 OldState;
	OldState = RamData.Alm_State;
	GetAlmState();
	if (RamData.Alm_State >= ALM_OVEROFF) {
		if ((FlashInfo.RunInfo.EState & _ESTATE_JD_STATE_)
				|| ((RamData.LVSave[0].SumUse.Use % 10) == 0)) {
			UpEState(FlashInfo.RunInfo.EState | _ESTATE_OVER_OFF_
					| _ESTATE_ALM_OFF_);
			if ((FlashInfo.RunInfo.EState & _ESTATE_PROTECT_CMD_) == 0) {
				RamData.Write = 0xFFFFFFFF;
				UpJDWriter();
				Do_JDOff();
				RamData.Next_JD = 0;
			}
		}
	} else if ((RamData.Alm_State != OldState) && (RamData.Alm_State
			>= ALM_ALM2)) {
		if ((FlashInfo.RunInfo.EState
				& (_ESTATE_PROTECT_CMD_ | _ESTATE_JD_CMD_)) == 0) {
			RamData.Write = 0xFFFFFFFF;
			UpJDWriter();
			Do_JDOff();
		}
		SetEBit(_BIT_ALM_OFF_);
	}
	if ((RunState_OverDraft) && ((FlashInfo.RunInfo.EState & _ESTATE_OVERDAFT_)
			== 0)) {
		SetEBit(_BIT_OVERDAFT_);
		SetEvtState(_EVT_OVERDRAFT_);
	}
}
#endif

void MStepFroze(void) {

	MoveDataLst((strMvData *)mvMonthForze);
	GetFrozeMinCnt((unsigned char *) ComData);
	Up_FrozeData(ComData, _FROZE_MONTH_);
	*(u32*) (ComData + 64) = (*(u32*) (ComData + 12))
			- FlashInfo.RunInfo.LastUse;
	Up_FrozeData(ComData + 64, _FROZE_MSTEP_);
	Up_u32Data(&FlashInfo.RunInfo.LastUse, *(u32*) (ComData + 12));
}

void ChangeFDT1Froze(void) {

	MStepFroze();
	if ((FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) == 0) {
#ifdef	_USE_CARD_
		GetPrice();
#endif
	}
}

u8 ChkYMode(void) {
#ifdef _OPT
	u8 Tm[5];
	u32 TmCnt;
	u8 Cnt;
	Tm[0] = 1;
	Tm[4] = 0;
	for (Cnt = 0; Cnt < 4; Cnt++) {
		//CopyRam(Tm+1, (u8*)(GAddr.pLTbl[0]+45+(Cnt*3)), 3);
		CopyExteeprom(Tm+1, (u8*)_LTBl_YStemp_Get(0,Cnt),3);
		TmCnt = GetTimeCnt(Tm, S_MIN | T_SEC);
		if (CheckDataAct((u8*) &TmCnt) == SUCCESS) {
			return SUCCESS;
		}
	}
#endif
	return FAIL;
}

void ChkYFroze(void) { //检查年阶梯切换

	u32 Lst[3];
	//u32 Data;
	//u8 Cnt;
	GetFrozeMinCnt((unsigned char *)&Lst[0]);
	Lst[1] = GetComUse(0);
	if (ChkYMode()) {
		if (FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) {
			Lst[2] = Lst[1];
			Lst[1] -= FlashInfo.RunInfo.LastYUse;
			Up_FrozeData((u8*) &Lst[0], _FROZE_YSTEP_);
			ClrEBit(_BIT_YSTEP_); //月结算
			MStepFroze();
			Up_u32Data(&FlashInfo.RunInfo.LastYFrozeTm, Lst[0]);
			Up_u32Data(&FlashInfo.RunInfo.LastYUse, Lst[2]);
		}
	} else if ((FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) == 0) {
		RamData.MToYFroze = 1;
		SetEBit(_BIT_YSTEP_); //年结算
		MStepFroze();
		Up_u32Data(&FlashInfo.RunInfo.LastYFrozeTm, Lst[0]);
		Up_u32Data(&FlashInfo.RunInfo.LastYUse, Lst[1]);
	} else if (RamData.YTm_Chg) {
		Lst[2] = Lst[1];
		Lst[1] -= FlashInfo.RunInfo.LastYUse;
		Up_FrozeData((u8*) &Lst[0], _FROZE_YSTEP_);
		Up_u32Data(&FlashInfo.RunInfo.LastYUse, Lst[2]);
	}
	RamData.YTm_Chg = 0;
}

u8 ChkActFL(u8 FL) { //检查当前费率

	u8 Max;
	if (FlashInfo.SetInfo.FLMode[3] > _MAX_TARRIF_) {
		Max = _MAX_TARRIF_;
	} else {
		Max = FlashInfo.SetInfo.FLMode[3];
	}
	if ((FL == 0) || (FL > Max)) {
		if (Max >= _DFT_FL_) { //检查费率号合法性
			return (_DFT_FL_ - 1);
		} else if (Max) {
			return Max - 1;
		} else {
			return 0;
		}
	}
	return (FL - 1);
}

void Up_FLState(void) {

	if (FlashInfo.RunInfo.EState & _ESTATA_YTB_STATE) {
		YTBL_State = 1;
	} else {
		YTBL_State = 0;
	}
	if (FlashInfo.RunInfo.EState & _ESTATE_TTB_STATE) {
		TMTBL_State = 1;
	} else {
		TMTBL_State = 0;
	}
}

u8 GetFeilv(void) { //获得当前费率

	u8 TmBCD[5];
	u8 Cnt;
	u8 Act;
	u8 MaxFL;
	u8 YTbl_Num;
	u16 Tm;
	u16 Tm1, Tm2;
	u32 *pTm;

	strHolidayTbl stholidaytbl;
	//	unsigned char ctemp ;
	//   u16 itemp ,itemp1;

	pTm = &FlashInfo.SafeInfo.StartTm_1;
	GetFrozeMinCnt((unsigned char *)&RamData.FrozeTime);
	for (Cnt = 0; Cnt < 4; Cnt++) { //由起始时间做费率表切换
		if ((*pTm) && (*pTm <= RamData.RTCTime)) {
			if (Cnt < 2) {
				if (FlashInfo.RunInfo.EState & (_ESTATA_YTB_STATE << Cnt)) {
					ClrEBit(_BIT_YTB_STATE+Cnt);
				} else {
					SetEBit(_BIT_YTB_STATE + Cnt);
				}
				if (RamData.SP_Mode) {
					_MultiPort_Pluse_Set(RamData.PulseTm );
				}
			} else {
				if (Cnt == 2) { //费率电价表变化
					//Up_TTbl((u8*)GAddr.pPriceTbl[1], PTBL_START);
					unsigned char stemp[_Price_Eeprom_First_Len];
					CopyExteeprom((unsigned char *)stemp, _Price_Get(1,0) , _Price_Eeprom_First_Len);
					Write_EEData((unsigned char *) stemp, _Price_Get(0,0), _Price_Eeprom_First_Len);

				} else
				//{											//阶梯电价表变化
				//	#define		STEP_TM_BIAS		((_MAX_LADDER_*4)+((_MAX_LADDER_+1)*3))
				//		if(strCmp((u8*)(GAddr.pLTbl[0]+STEP_TM_BIAS), (u8*)(GAddr.pLTbl[1]+STEP_TM_BIAS), 12))
				//	{
				//			RamData.YTm_Chg = 1;
				//	}
				{
					{ //阶梯电价表变化
						u8 YStepTm[4][3]; //年结算日
						u8 YStepTm1[4][3]; //年结算日
						CopyExteeprom( (unsigned char *)YStepTm ,_LTBl_YStemp_Get(0,0),12);
						CopyExteeprom( (unsigned char *)YStepTm1 ,_LTBl_YStemp_Get(1,0),12);
						if (strCmp((u8*) YStepTm, (u8*) YStepTm1, 12)) {
							RamData.YTm_Chg = 1;
							ChkYFroze();
						}
						Write_EEData((unsigned char *) &YStepTm1,
								_LTBl_YStemp_Get(0,0), 12);

					}
					{
						// Up_TTbl((u8*)GAddr.pLTbl[1], LTBL_START);
						strLTbl stLTbl;
						CopyExteeprom( (unsigned char *)&stLTbl ,_LTBl_Get(1) ,sizeof(strLTbl) );
						Write_EEData((unsigned char *) &stLTbl, _LTBl_Get(0), sizeof(strLTbl));
					}

					SetEBit(_BIT_YTB_STATE + Cnt);
					if (Cnt == 3) {
						ChkYFroze();
					}
				}
			}
			ClrRcdTime(pTm);
			Up_NormFroze(_FROZE_YTBL_CHG_ + Cnt);
		}
		pTm++;
	}
	Up_FLState();
	if (FlashInfo.SetInfo.FLMode[3] == 0) { //费率数为零走默认费率
		Up_u8Data(&FlashInfo.RunInfo.ActTblNum, 0);
		return ChkActFL(_DFT_FL_);
	}
	Tm = GetTimeCnt(0, S_RTC | T_DAY);
	YTbl_Num = FlashInfo.RunInfo.YTbl_Num;
	if ((Tm != RamData.LastDay) || (RamData.Event_Feilv)) {
		RamData.LastDay = Tm;
		RamData.LastDayYTbl = 0;
		Cnt = 0;
		while (Cnt < FlashInfo.SetInfo.FLMode[4]) { //判断假日表
			CopyExteeprom((unsigned char *)&stholidaytbl, _Hld_Get(Cnt),_cpHld_Unit_Len);
			if (strCmp((u8*) stholidaytbl.HDay, (u8*) &Tm, 2) == 0) {
				Tm1 = stholidaytbl.TblNum;
				if ((Tm1 <= FlashInfo.SetInfo.FLMode[1]) && (Tm1 != 0)) {
					RamData.LastDayYTbl = 1;
					YTbl_Num = Tm1;
				}
				break;
			}

			Cnt++;
		}
		if (RamData.LastDayYTbl == 0) {
			GetCntTime(TmBCD, 0, S_RTC | T_YMDW); //判断周休表
			if ((FlashInfo.SetInfo.WeekMode & (1 << (TmBCD[3]))) == 0) {
				Tm1 = FlashInfo.SetInfo.WeekTblNum;
				if ((Tm1 <= FlashInfo.SetInfo.FLMode[1]) && (Tm1 != 0)) {
					RamData.LastDayYTbl = 1;
					YTbl_Num = Tm1;
				}
			}
		}
	}
	GetCntTime(TmBCD, 0, S_RTC | T_YMDHM);
	if (FlashInfo.RunInfo.EState & _ESTATA_YTB_STATE) {
		//CopyRam(RamData.Buf645, (u8*)(GAddr.pYTbl[1]), 28);
		CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->YearTbl[1])), 28);
	} else {
		//CopyRam(RamData.Buf645, (u8*)(GAddr.pYTbl[0]), 28);
		CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->YearTbl[0])), 28);

	}
#define		pTbl		((u16*)(RamData.Buf645))
	if (RamData.LastDayYTbl == 0) { //判断年时区表
		Tm = (HEX(TmBCD[1]) << 5) + HEX(TmBCD[2]);
		for (Cnt = 0; Cnt < (FlashInfo.SetInfo.FLMode[0] - 1); Cnt++) {
			Tm1 = pTbl[Cnt] >> 4;
			Tm2 = pTbl[Cnt + 1] >> 4;
			if (Tm1 < Tm2) {
				if ((Tm >= Tm1) && (Tm < Tm2)) {
					break;
				}
			} else if (Tm1 > Tm2) {
				if ((Tm >= Tm1) || (Tm < Tm2)) {
					break;
				}
			}
		}
		YTbl_Num = pTbl[Cnt] & 0x0F;
	}
	if ((YTbl_Num > FlashInfo.SetInfo.FLMode[1]) || (YTbl_Num == 0)) {
		YTbl_Num = 1;
	}
	Up_u8Data(&FlashInfo.RunInfo.YTbl_Num, YTbl_Num);
	if (FlashInfo.RunInfo.EState & _ESTATE_TTB_STATE) {
		//CopyRam(RamData.Buf645, (u8*)(GAddr.pTmTbl[1][YTbl_Num-1]), 28);
		CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->TmTbl[1][YTbl_Num-1])), 28);
	} else {
		//CopyRam(RamData.Buf645, (u8*)(GAddr.pTmTbl[0][YTbl_Num-1]), 28);
		CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->TmTbl[0][YTbl_Num-1])), 28);
	}
	if (FlashInfo.SetInfo.FLMode[3] > _MAX_TARRIF_) {
		MaxFL = _MAX_TARRIF_;
	} else {
		MaxFL = FlashInfo.SetInfo.FLMode[3];
	}
	Tm = (HEX(TmBCD[3]) * 60) + HEX(TmBCD[4]);
	Act = 0xFF;
	for (Cnt = 0; Cnt < (FlashInfo.SetInfo.FLMode[2] - 1); Cnt++) { //判断日时段表
		Tm1 = pTbl[Cnt] >> 5;
		Tm2 = pTbl[Cnt + 1] >> 5;
		if ((pTbl[Cnt + 1] & 0x01F) == 0) {
			break;
		} else if (Tm1 < Tm2) {
			if ((Tm >= Tm1) && (Tm < Tm2)) {
				if (Act == 0xFF) {
					Act = Cnt;
				}
			}
		} else if (Tm1 > Tm2) {
			if ((Tm >= Tm1) || (Tm < Tm2)) {
				if (Act == 0xFF) {
					Act = Cnt;
				}
			}
		} else {
			break;
		}
	}
	Up_u8Data(&FlashInfo.RunInfo.ActTblNum, Cnt + 1); //记录日时段数
	if (Act == 0xFF) {
		Act = Cnt;
	}


	Cnt = ChkActFL(pTbl[Act]&0x1F);
	Act = (Act<<4)|Cnt;
	if(Act != RamData.LastTblAct){
		RamData.LastTblAct = Act;
		if(RamData.SP_Mode){
			_MultiPort_Pluse_Set(RamData.PulseTm );
		}
	}
	return Cnt;
}



void	Do_FeilChg(void){

	u8 NewFeilv;

	NewFeilv = GetFeilv();
	if (NewFeilv != RamData.LVSave[0].FeilvNum) {
		Chg_Feilv(NewFeilv);
	}
	if (_ISMETER_LOCAL()) { //
		#ifdef		_USE_CARD_
		GetPrice();
		#endif
	} 
	
	RamData.Event_Feilv = 0;
}
#ifdef DEL
u8 Inc_Us1e(strSectUse *pSectUse, u16 Imp) { //用电量处理(脉冲数转为电量/Imp最高为为1时表示反向电量)

	u32 k;
	u32 t;
	u32 Dir;
	u32 *pUse;
	u16 *pImp;
	if (Imp & 0x8000) {
		pUse = &pSectUse ->RevUse;
		pImp = &pSectUse ->RevImp;
		Dir = 1;
	} else {
		pUse = &pSectUse ->Use;
		pImp = &pSectUse ->Imp;
		Dir = 0;
	}
	Imp &= 0xFF;
	Imp += *pImp;
	k = (FData.DftSet.kWhImp / 100);
	if (Imp >= k) {
		t = Imp / k;
		*pUse += t;
		*pImp = Imp % k;
		if (Dir) {
			pSectUse ->Use += t;
		}
		return (u8) t;
	}
	*pImp = Imp;
	return 0;
}

void Do_Use1(void) {

	u8 Imp;
	u16 sImp;
	u32 AInc =0;
	u32 Inc;
	u32 SInc;
	u8 cImpbuf;

	main_sequential_off(); //asm("di");
	cImpbuf = RamData.ImpBfr;
	main_sequential_on(); //asm("ei");

	if (RamData.OldImpBfr == cImpbuf) {
		return;
	}
	if (RamData.Use_Dir) {
		if (AUse_Dir == 0) {
			AUse_Dir = 1;
			AUse_Dir_A = 1;
			RamData.RevTmCnt = 0;
			return;
		}
	} else {
		if (AUse_Dir) {
			AUse_Dir = 0;
			AUse_Dir_A = 0;
			RamData.RevTmCnt = 0;
			return;
		}
	}
	Imp = cImpbuf - RamData.OldImpBfr;
	if (RamData.RevTmCnt < _REV_DELAY_) { //反向处理
		if ((++RamData.RevTmCnt < _REV_DELAY_) && (Imp < _IMP_DELAY_)) {
			return;
		}
	}
	RamData.OldImpBfr += Imp;
	if (RamData.Use_Dir) {
		sImp = 0x8000 | Imp;
	} else {
		sImp = Imp;
	}
	Inc = Inc_Use(&RamData.LVSave[0].FLUse, sImp); //计算费率用电量
	SInc = Inc_Use(&RamData.LVSave[0].SumUse, sImp); //计算总用电量
	if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		AInc = 0;
		if (Inc) {
			AInc = Inc_Money(FlashInfo.RunInfo.FLPrice, Inc); //增加费率用电金额
		}
		if (SInc) {
			AInc += Inc_Money(FlashInfo.RunInfo.StepPrice, SInc); //增加阶梯用电金额
		}
	} //#endif
	Up_BkLVSave(); //建立电量数据备份
	if (SInc) {
		RamData.NotChg += SInc;
		#ifdef		_USE_CARD_
			GetPrice();
		if (_ISMETER_LOCAL()) { //
		} //
		#endif
		if ((RamData.NotChg >= 100) && (RamData.UseChgTm >= (15 * 6))) {
			Up_LVSave();
			RamData.NotChg = 0;
			RamData.Use_Chg = 1;
		}
	}
	if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		if (AInc) { //有功电量变化处理
			ChkUseState();
		}
	}//#endif		//_USE_CARD_
}
#endif

void ClrJDOffTime(void) {

	ClrRcdTime(&FlashInfo.RunInfo.RcdTm.FarJDOffTime);
	ClrRcdTime(&FlashInfo.RunInfo.RcdTm.FarJDOffPreTime);
	Up_u8Data(&FlashInfo.RunInfo.FarJDPreTm, 0);
}

void WaitTimeOver(u32* pTm) { //设置等待时间结束
	Up_u32Data(pTm, 1);
}
 
void CheckFroze(void) { //冻结数据

	u8 TmBk[5];
	u8 Tm[5];
	u16 Addr;
	//u32 CntPre;
	u32 CntNext;
	u32 CntNow;
	//u8 Retry;
	u8 Cnt;
	//	u32	Tmp[3];
	u8 LNum;
	u32 FrzLst[12];

	GetCntTime(TmBk, RamData.RTCTime, S_SEC | T_YMDHM);
	CopyRam(Tm, TmBk, 5);
	GetFrozeMinCnt((unsigned char *)&CntNow);
	if (CntNow == 0) {
		CntNow = 1;
	}
	if (CntNow != RamData.LastFrozeTime) {
		RamData.FrozeTmChg = 1;
	} else {
		RamData.FrozeTmChg = 0;
	}
	if(FlashInfo.RunInfo.LastMFrozeTm > CntNow){
		Up_u32Data(&FlashInfo.RunInfo.LastMFrozeTm, CntNow);
	} else {
	//	LNum = GetMthFrozeTime(&FlashInfo.RunInfo.LastMFrozeTm, FrzLst);
		LNum = GetFrozeLst(&MulFrozeSet[0], (u8*)&FlashInfo.SetInfo.FrozeDT, FrzLst);
		if(LNum){
			Up_LVSave();
			MoveDataLst((strMvData *)mvMonthForze);
			for(Cnt = 0; Cnt < LNum; Cnt++){
				*(u32*)(ComData) = FrzLst[Cnt];
				ComData[3] &= (~0x80);
				Up_FrozeData( ComData, _FROZE_MONTH_);
				if(FrzLst[Cnt]&0x80000000){
					*(u32*)(ComData+100) = (*(u32*)(ComData+12)) - FlashInfo.RunInfo.LastUse;
					if(RamData.MToYFroze == 0){
						Up_FrozeData(ComData+100, _FROZE_MSTEP_);
					}
					Up_u32Data(&FlashInfo.RunInfo.LastUse, *(u32*)(ComData+12));
					if((FlashInfo.RunInfo.EState&_ESTATE_YSTEP_) == 0){
						#ifdef	_USE_CARD_
						if ((_ISMETER_LOCAL())) //
							GetPrice();
						//
						#endif
					}
				}
				RamData.MToYFroze = 0;
			}
			Up_u32Data(&FlashInfo.RunInfo.LastMFrozeTm, *(u32*) ComData);
		}
	}
	RamData.MToYFroze = 0; //月冻结结束
	#ifdef		_USE_CARD_
	if (_ISMETER_LOCAL()) { //
		if (FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) { //年结算冻结
			if (FlashInfo.RunInfo.LastYFrozeTm > CntNow) {
				Up_u32Data(&FlashInfo.RunInfo.LastYFrozeTm, CntNow);
		} else {
			strLTbl  stlTbl;
			CopyExteeprom( (u8 *)&stlTbl ,_LTBl_Get(0)	 ,sizeof(strLTbl)	 ) ;
	//		LNum = GetFrozeLst(&MulFrozeSet[1], (u8*)(GAddr.pLTbl[0]+45), FrzLst);
		 LNum = GetFrozeLst(&MulFrozeSet[1], (u8*)(&(stlTbl.YStepTm[0])), FrzLst);
			if(LNum){
				Up_LVSave();
				for(Cnt = 0; Cnt < LNum; Cnt++){
					*(u32*)(ComData) = FrzLst[Cnt];
					*(u32*)(ComData+8) = GetComUse(0);
					*(u32*)(ComData+4) = (*(u32*)(ComData+8)) - FlashInfo.RunInfo.LastYUse;
					Up_u32Data(&FlashInfo.RunInfo.LastYUse, *(u32*)(ComData+8));
					Up_u32Data(&FlashInfo.RunInfo.LastYFrozeTm, *(u32*)ComData);
					Up_FrozeData( ComData, _FROZE_YSTEP_);
				}				
				GetPrice();				
				}
			}
		}
	}//
	#endif
	if (RamData.FrozeTmChg) {
	//	if (*((u32*) FlashInfo.SetInfo.FrozeCode)) { //	定时冻结
			if ( IslongZero(*((u32*) FlashInfo.SetInfo.FrozeCode))){
			for (Cnt = 0; Cnt < 4; Cnt++) {
				if (FlashInfo.SetInfo.FrozeCode[Cnt] == 0x99) {
					Tm[1 + Cnt] = 0x99;
				}
			}
			if (Cmp_4(FlashInfo.SetInfo.FrozeCode, (Tm + 1)) == 0) {
				GetFrozeMinCnt((unsigned char *)&RamData.FrozeTime);
				Up_NormFroze(_FROZE_TM_);
			}
		}
	} //	定时冻结结束
	if (FlashInfo.RunInfo.LastDFrozeTm > CntNow) { //	日冻结
		Up_u32Data(&FlashInfo.RunInfo.LastDFrozeTm, CntNow);
	} else {
		//static const u32 DayMin = (60 * 24);
		CopyRam(Tm, TmBk, 5);
		RevCopyRam(Tm + 3, (u8*) &FlashInfo.SetInfo.FrozeHM, 2);
		CntNext = GetTimeCnt(Tm, S_MIN | T_MIN);
		LNum = 0;
		if (((RamData.FrozeTmChg)) && (CntNext == CntNow)) {
			LNum = 1;
			CntNext -= (60 * 24);
		} else if (CntNext > CntNow) {
			CntNext -= (60 * 24);
		}
		while (CntNext > FlashInfo.RunInfo.LastDFrozeTm) {
			CntNext -= (60 * 24);
			if (++LNum >= 7) {
				break;
			}
		}
		if (LNum) {
			while (LNum--) {
				CntNext += 60 * 24;
				RamData.FrozeTime = CntNext; //FrzLst[LNum];
				Up_NormFroze(_FROZE_DAY_);
			}
			Up_u32Data(&FlashInfo.RunInfo.LastDFrozeTm, RamData.FrozeTime);
		}
	}																//	日冻结结束
	CopyRam(Tm, TmBk, 5);
	if(((RamData.FrozeTmChg))&&(FlashInfo.SetInfo.HourFrozeStartTm)&&(RamData.RTCTime >= FlashInfo.SetInfo.HourFrozeStartTm)){ //整点冻结
		CntNext = FlashInfo.SetInfo.HourFrozeStartTm;
		ChgMOD_1(&CntNext, 60);
		LNum = ChgMOD_1(&CntNext, 60);
		if((((HEX(Tm[4])+60) - LNum)%FlashInfo.SetInfo.HourFrozeStep) == 0){
			Up_NormFroze(_FROZE_HOUR_);
		}
	}																//整点冻结结束
	if((CntNow != RamData.FHSet.LastFH[0].FHTime) && ( _CODE_DEL_)){					// 负荷记录
		static const   strMvData *const pMvHFRcd[4] = {
			mvHFRcd_1,												//电压/电流/频率
			mvHFRcd_2,												//功率
			mvHFRcd_3,												//功率因数
			&FileSet.mvHourFroze[1],								//正向/反向有功
		};
		u8	FHSet;
		if((RamData.PwrUpFrozeOver == 0)&&(FlashInfo.RunInfo.EState&_ESTATE_FUHE_)&&((CntNow -RamData.FHSet.LastFH[4].FHTime) >= FlashInfo.SetInfo.FHTmStep[3])){
			*(u32*)ComData = RamData.FHSet.LastFH[4].FHTime + FlashInfo.SetInfo.FHTmStep[3];
			*(u32*)ComData |= (((u32)1)<<(26+3));
			tpChar = ComData+4;
			MoveDataLst((strMvData *)pMvHFRcd[3]);
			Up_FHFroze(ComData);
		}
		if((RamData.Use_Act)&&((RamData.FrozeTmChg)||((FlashInfo.RunInfo.EState&_ESTATE_FUHE_) == 0))){
			FHSet = 0;CntNext = CntNow%1440;
			for(Cnt = 0; Cnt < 4; Cnt++){
				if(FlashInfo.SetInfo.FHMode&(1<<Cnt)){

					if((CntNext%FlashInfo.SetInfo.FHTmStep[Cnt]) == 0){
						FHSet |= (1 << Cnt);
					}
				}
			}
			if ((FlashInfo.RunInfo.EState & _ESTATE_FUHE_) == 0) {
				FHSet = FlashInfo.SetInfo.FHMode;
			}
			if (FHSet) {
				*(u32*) ComData = CntNow | (((u32) FHSet) << 26);
				tpChar = ComData + 4;
				for (Cnt = 0; Cnt < 4; Cnt++) {
					if (FHSet & 1) {
						MoveDataLst((strMvData *)pMvHFRcd[Cnt]);
					}
					FHSet >>= 1;
				}
				Up_FHFroze(ComData);
				SetEBit(_BIT_FUHE_);
			}
		}
	} //负荷记录结束
	Addr = (CntNow / (60 * 24)); //零点事件处理
	if (Addr != FlashInfo.RunInfo.LastTmDay) {
		Up_u16Data(&FlashInfo.RunInfo.LastTmDay, Addr);
		ClrRcdTime((u32*) &FlashInfo.RunInfo.KeyErrCnt); //清零KeyErrCnt, MacErrCnt
		ClrEBit(_BIT_DAY_CHG_);
	} //零点事件处理结束
	if(RamData.FarSubTm){											//远程扣款延时
		RamData.FarSubTm --;
	}

	if(((CntNow%60) == 0)&&(RamData.PwrUpCnt > 40)&&(RamData.Temperature > (-200))&&(RamData.Temperature < 600)){
		CheckBatVolt();
	}

	RamData.LastFrozeTime = CntNow;
	RamData.PwrUpFrozeOver = 1;
}
 

u8 Do_FarCtrl(u8* Ctrl) { //远程控制处理
#ifdef del
	if (Ctrl[0] == 0x1A) { //跳闸
		if (FlashInfo.RunInfo.EState & _ESTATE_PROTECT_CMD_) {
			return FarCtrlRet(_FCS_SAFE_ | _FCS_JDOFFERR_);
		}
		UpJDWriter();
		if (FlashInfo.SetInfo.JDOffDelayTm == 0) { //延时为零立即跳闸
			RamData.FarCtrlState = _FCS_JD_OFF_;
			ClrJDOffTime();
			goto _Far_JDOff_;
		}
		RamData.FarCtrlState =_FCS_JD_OFF_DELAY_;					//延时跳闸
		ClrRcdTime(&FlashInfo.RunInfo.RcdTm.JDOverIphTime);
		ClrJDOffTime();
		Up_u32Data(&FlashInfo.RunInfo.RcdTm.FarJDOffTime, RamData.RTCTime + FlashInfo.SetInfo.JDOffDelayTm*60);
		goto _Far_JDOff_2_;
	} else if((Ctrl[0] == 0x1D)||(Ctrl[0] == 0x1E)){				//预跳闸-合闸/跳闸-合闸允许
		if(FlashInfo.RunInfo.EState&_ESTATE_PROTECT_CMD_){			//保电状态返回
			return FarCtrlRet(_FCS_SAFE_|_FCS_JDOFFPREERR_);
		}
		if(Ctrl[1] == 0){											//预跳闸延时不能为零
			return FarCtrlRet(_FCS_SAFE_|_FCS_JDOFFPRETMERR_);
		}
//		if((FlashInfo.RunInfo.RcdTm.FarJDOffTime)					//延时跳闸/延时跳闸过流期间错误返回
//				||(FlashInfo.RunInfo.EState&_ESTATE_JD_CMD_)&&(FlashInfo.RunInfo.FarJDPreTm == 0)&&(FlashInfo.RunInfo.RcdTm.JDOverIphTime)){
		if((FlashInfo.RunInfo.EState&_ESTATE_JD_CMD_)&&(FlashInfo.RunInfo.FarJDPreTm == 0)){	//跳闸指令执行期间错误返回
			return FarCtrlRet(_FCS_SAFE_|_FCS_JDOFFPREOFF_);
		}
		if((FlashInfo.RunInfo.FarJDPreTm == 0)&&((FlashInfo.RunInfo.EState&_ESTATE_JD_STATE_) == 0)){	//非延时已跳闸错误返回
			return FarCtrlRet(_FCS_SAFE_|_FCS_JDOFFPREOFF_);
		}
		UpJDWriter();
		if ((_ISJDMODE_OUT()) || (Ctrl[0] == 0x1D)) { //设置延时合闸方式  //		if((_JD_MODE_ == 0)||(Ctrl[0] == 0x1D)){					//设置延时合闸方式
			SetEBit(_BIT_OFF_PRE_);
		} else {
			ClrEBit(_BIT_OFF_PRE_);
		}
		RamData.FarCtrlState = _FCS_JD_OFF_PRE_;
		ClrJDOffTime();
		Up_u8Data(&FlashInfo.RunInfo.FarJDPreTm, Ctrl[1]);
_Far_JDOff_:
		Do_JDOff();
		if((_ISJDMODE_IN())&&FlashInfo.RunInfo.RcdTm.JDOverIphTime){		//内置继电器过流检测  //if((_JD_MODE_)&&FlashInfo.RunInfo.RcdTm.JDOverIphTime){		//内置继电器过流检测
			RamData.FarCtrlState = _FCS_JD_OFF_IPH_;
			if(FlashInfo.RunInfo.RcdTm.FarJDOffPreTime){
				Up_u8Data(&FlashInfo.RunInfo.FarJDPreTm, Ctrl[1]);
				ClrRcdTime(&FlashInfo.RunInfo.RcdTm.FarJDOffPreTime);
			}
		}
_Far_JDOff_2_:
		SetEBit(_BIT_JD_CMD_);
		GetAlmState();
	} else if((Ctrl[0] == 0x1B)||(Ctrl[0] == 0x1C)){				//合闸允许/直接合闸
		UpJDWriter();
		ClrJDOffTime();
		UpEState(FlashInfo.RunInfo.EState&(~(_ESTATE_JD_CMD_|_ESTATE_ALM_OFF_)));
		//if(Ctrl[0] == 0x1C){
			RamData.FarCtrlState = _FCS_JD_ON_;
			ChkAlmJD();
		/*} else {
			RamData.FarCtrlState = _FCS_JD_ON_ALLOW_;
			if (_ISJDMODE_OUT()) { //if(_JD_MODE_ == 0){
				ChkAlmJD();
			} else {
				GetAlmState();
			}
		}*/
	} else if (Ctrl[0] == 0x2A) { //报警
		SetEBit(_BIT_FAR_ALM_);
		JD_OFF_AlmState = 1;
		RamData.FarCtrlState = _FCS_ALM_;
		GetAlmState();
	} else if (Ctrl[0] == 0x2B) { //报警解除
		ClrEBit(_BIT_FAR_ALM_);
		JD_OFF_AlmState = 0;
		RamData.FarCtrlState = _FCS_ALM_OFF_;
		GetAlmState();
	} else if(Ctrl[0] == 0x3A){										//保电
		UpJDWriter();
		ClrJDOffTime();
		UpEState((FlashInfo.RunInfo.EState&(~(_ESTATE_ALM_OFF_|_ESTATE_JD_CMD_)))|_ESTATE_PROTECT_CMD_);
		if ( _ISJDMODE_OUT()){//if(_JD_MODE_ == 0){
			ChkAlmJD();
		} else {
			GetAlmState();
		}
		RamData.FarCtrlState = _FCS_PRT_;
	} else if(Ctrl[0] == 0x3B){										//保电解除
		if((FlashInfo.RunInfo.EState&_ESTATE_PROTECT_CMD_) == 0){
			RamData.FarCtrlState = _FCS_PRT_OFF_;
			return SUCCESS;
		} else if((JD_State == 0)&&((RamData.Alm_State >= ALM_OVEROFF)||((FlashInfo.RunInfo.EState&_ESTATE_JD_CMD_)&&(FlashInfo.RunInfo.RcdTm.FarJDOffTime==0)))){
			UpJDWriter();
			RamData.FarCtrlState = _FCS_PRT_OFF_;
			Do_JDOff();
			UpEState((FlashInfo.RunInfo.EState|_ESTATE_OVER_OFF_|_ESTATE_ALM_OFF_)&(~_ESTATE_PROTECT_CMD_));
		} else {
			RamData.FarCtrlState = _FCS_PRT_OFF_;
			UpEState(FlashInfo.RunInfo.EState&(~(_ESTATE_PROTECT_CMD_|_ESTATE_ALM_OFF_)));
		}
		GetAlmState();
	} else {
		return FarCtrlRet(_ERR_NODATA_ | _FCS_KEYERR_);
	}
	#endif
	return SUCCESS;
}
 