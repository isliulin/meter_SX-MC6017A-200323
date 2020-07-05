/*
 * public.c
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */
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

#include "pubdata.h" 
#include "uartio.h"


 
extern const strFileSet FileSet;
void RevStr(u8 *s, u16 Lth) { //反向数据
	signed char  Cnt;
	u8 Bk;
	Cnt = (Lth / 2) - 1;
	while (Cnt >= 0) {
		Bk = s[Cnt];
		s[Cnt] = s[Lth - 1 - Cnt];
		s[Lth - 1 - Cnt] = Bk;
		Cnt--;
	}
}

u16 ChgMOD_2(u32 *pData, u16 Num) {

	u16 n;
	n = (u16) ((*pData) % Num);
	*pData -= n;
	return n;
}

u16 ChgMOD_1(u32 *pData, u16 Num) {

	u16 n;
	n = (u16) ((*pData) % Num);
	*pData /= Num;
	return n;
}

void GetFrozeMinCnt(unsigned char  *pTm  ) {
	RamData.FrozeTime = RamData.RTCTime / 60;
	cCopyRam( (u8*)pTm , (u8*)&RamData.FrozeTime,sizeof( RamData.FrozeTime));//*pTm = RamData.FrozeTime;
}

u32 MulDivX(u32 Mul_1, u32 Mul_2, u32 Div) {

	u32 DivX;
	u32 MulX;  if (( 0 == Mul_1) || ( 0 == Div ))  return 0 ;
	if ( 0 == Mul_1)  return 0 ; 
	DivX = (u32) 0xFFFFFFFF / Mul_1;
	MulX = 1;
	while (DivX < Mul_2) {
		if ((Mul_1 & 1) == 0) {
			Mul_1 >>= 1;
			DivX = (u32) 0xFFFFFFFF / Mul_1;
		} else if (((Mul_2 & 1) == 0) || (Mul_1 < Mul_2)) {
			Mul_2 >>= 1;
		} else {
			Mul_1 >>= 1;
			DivX = (u32) 0xFFFFFFFF / Mul_1;
		}
		if ((Div & 1) == 0) {
			Div >>= 1;
		} else {
			MulX <<= 1;
		}
	}
	Mul_1 *= Mul_2;
	DivX = Mul_1 % Div;
	Mul_1 = Mul_1 / Div;
	return (Mul_1 * MulX) + ((DivX * MulX) / Div);
}

void tpCharInc(u8 Inc) {
	tpChar += Inc;
}

void NormCopy(uc8 *Addr, u8 Length) {
	CheckpCharLength(Length);
	CopyRam(tpChar, Addr, Length);
	tpCharInc(Length);
}

void RevCopy(uc8 *Addr, u8 Length) {
	CheckpCharLength(Length);
	RevCopyRam((char *)tpChar, (char *)Addr, Length);
	tpCharInc(Length);
}

void SetpCharInc(u8 Data) {
	*tpChar++ = Data;
}

void MvData(u8 *pData, u16 Opt) {

	s32 Data;
	u8 Buf[5];
	u8 L;
	u8 L2;
	u8 FL;
	if ((Opt & 0x3FE0) == 0) {
		L = ((Opt) & 0x1F);
		if (L == 0) {
			L = 32;
		}
		if (pData < ((u8*) _RAM_ZERO_END_)) {   // if (pData < ((u8*) 0x100)) {
			ClrpCharRam(L);
		} else if ((Opt & _REV_) == 0) { //	COPY_L(0x0 ~ 0x001F)
			NormCopy((u8*) pData, L);
		} else { //	REV_COPY_L(0x4000~0x401F)
			RevCopy((u8*) pData, L);
		}
	} else if ((Opt & 0x3FF0) == 0x20) {
		L = ((Opt) & 0x1F);
		if (L == 0) { //	SET_PCHAR(0x0020)
			tpChar = (u8*) pData;
		} else if (L == 1) { //	SET_PBASE(0x0021)
			tpBase = (u8*) pData;
		}
	} else if ((Opt & 0x3FF0) == 0x30) { //	READ_BCD_TIME(0x0030~0x003F)
		Data = GetTimeCnt(pData, Opt); //	结果(秒-4字节, 分-3字节, 4-2字节)
		L = ((Opt >> 2) & 3);
		NormCopy((u8*) &Data, (4 - L));
	} else if ((Opt & 0x3FC0) == 0x40) { //	OUT_TIME(0x0040~0x007F)
		Data = 0;
		L = (Opt & 3);
		if (L == S_MIN) {
			CopyRam((u8*) &Data, pData, 4);
			Data &= 0x3FFFFFF;
		} else {
			CopyRam((u8*) &Data, pData, 4 - L);
		}
		GetCntTime(tpChar, Data, Opt);
		L = (Opt >> 2) & 0x7;
		tpChar += FileSet.T_Time_Lth[L].Lth;
	} else if ((Opt & 0x3FE0) == 0x80) { //	REV_LST(0x0080~0x009F)
		L = (Opt & 3) + 1;
		FL = ((Opt >> 2) & 7) + 1;
		while (FL) {
			RevCopy(pData, L);
			pData += L;
			FL--;
		}
	} else if ((Opt & 0x3F00) == 0x0100) {
		L = ((Opt >> 4) & 3) + 1;
		L2 = ((Opt >> 2) & 3) + 1;
		FL = (Opt & 3);
	if (pData < ((u8*) _RAM_ZERO_END_)) {   //	if (pData < ((u8*) 0x100)) {
			ClrpCharRam(L2);
		} else if ((Opt & 0x80) == 0) { //BCD_OUT/HEX_OUT
			RamData.Use_Neg = 0;
			Buf[4] = 0;
			*(u32*) Buf = 0;
			CopyRam(Buf, pData, L);
			if ((Opt & 0x40) == 0) { //BCD_OUT
				if (Buf[L - 1] & 0x80) {
					RamData.Use_Neg = 1;
					if (L != 4) {
						FillRam(Buf+L, 0xFF, (4-L));
					}
				}
				Hex_BCD(*(u32*) Buf, Buf);
			}
			if (FL) {
				if (FL & 1) {
					*(u32*) Buf = ((*(u32*) Buf) >> 4) | (((u32) Buf[4]) << 28);
				}
				if (FL & 2) {
					Copy_4(Buf, Buf + 1);
				}
			}
			if ((RamData.Use_Neg) && (RamData.InDisp == 0)) {
				Buf[L2 - 1] |= 0x80;
			}
			if ((Opt & (0xC0 | _REV_)) == (_REV_)) { //	反向BCD_OUT/HEX_OUT
				NormCopy(Buf, L2);
			} else {
				RevCopy(Buf, L2);
			}
		} else { //BCD_IN/HEX_IN
			*(u32*) Buf = 0;
			if (Opt & _REV_) {
				CopyRam(Buf, pData, L);
			} else {
				RevCopyRam(Buf, pData, L);
			}
			if (FL) {
				*(u32*) Buf = ((*(u32*) Buf) << (FL * 4));
			}
			if ((Opt & 0x40) == 0) { //BCD_IN
				*(u32*) Buf = BCD_Hex(*(u32*) Buf);
			}
			NormCopy(Buf, L2);
		}
	}

}

void MvDataAddr(u16 pIn, u16 Opt) {

	u8 L;
	u8 SCode;
	u8 FL;
	strDispCode Code645;
	s32 Data[2];
	u8 UpOpt;
	u8 *pData;
	strSectUse *pFL;

	Opt &= 0x7FFF;
	SCode = (u8) ((pIn) >> 12);
	UpOpt = 0;
	if (SCode == (EX_ADDR >> 12)) {
		if (pIn == _FILL_RAM_ADDR_) { //FILL_RAM
			L = ((Opt >> 8) & 0x7F) + 1;
			CheckpCharLength(L);
			FillRam( tpChar, Opt&0xFF, L);
			tpChar += L;
		} else if (pIn == (_FILL_RAM_ADDR_ + 1)) { //FILL_RAM_PBASE
			L = ((Opt >> 8) & 0x7F) + 1;
			CheckpCharLength(L);
		if (tpBase < ((u8*) _RAM_ZERO_END_)) {   // 	if (tpBase < (u8*) 0x100) {
				ClrpCharRam(L);
			} else {
				FillpCharRam( Opt&0xFF, L);
			}
		} else if ((pIn & 0xFFE) == 0x002) { //SET_RAM_2
			Data[0] = Opt | ((pIn & 1) << 15);
			RevCopy(((u8*) Data), 2);
		} else if ((pIn & 0xE00) == 0x200) { //SET_RAM_3
			Data[0] = Opt | (((u32) (pIn & 0x1FF)) << 15);
			RevCopy(((u8*) Data), 3);
		} else if ((pIn & 0xF00) == 0x400) { //READ_USEDATA
			SCode = pIn & 0xFF;
			if ((SCode & 0x0C) <= 0x04) { //全相或A相
				FL = (SCode >> 4);
				if (FL <= _MAX_TARRIF_) {
					pFL = GetFLBase(FL);
					CopyRam((u8*) Data, (u8*) pFL, 8);
					SCode &= 3;
					if (SCode == 0) { //TYPE_COM_ACT_USE
						Data[0] = GetComUse(FL);
					} else if (SCode == 1) { //TYPE_ACT_USE
						Data[0] -= Data[1];
					} else if (SCode == 2) { //TYPE_REV_ACT_USE
						Data[0] = Data[1];
					} //TYPE_ADD_USE
					UpOpt = 1;
				} else {
					Data[0] = 0;
				}
				pData = (u8 *) Data;
			}
		} else if ((pIn & 0x800) == 0x800) {
			pData = tpBase + (pIn & 0x7FF);
			UpOpt = 1;
		}
#ifdef		_USE_V_ADDR_										//使用虚拟地址
	} else if ((pIn & 0xF000) == _V_RAMDATA_ADDR_) { //	RamData
		pData = ((u8*) &RamData) + (pIn & 0xFFF);
		UpOpt = 1;
	} else if ((pIn & 0xF000) == _V_EEDATA_ADDR_) { //	FlashInfo
		pData = ((u8*) &FlashInfo) + (pIn & 0xFFF);
		UpOpt = 1;
	} else if ((pIn & 0xF000) == _V_COMDATA_ADDR_) { //	ComData
		pData = ComData + (pIn & 0xFFF);
		UpOpt = 1;
	} else if (pIn == 0) {
		pData = NULL;
		UpOpt = 1;
#else
	} else {
		UpOpt = 1;
#endif
	}
	if (UpOpt) {
		MvData(pData, Opt);
	}

}

void MoveDataLst(const strMvData *pMv) {

	u16 Opt;
	do {
		Opt = pMv ->Opt;
		MvDataAddr(pMv ->pData, Opt);
		pMv++;
	} while ((Opt & _RET_) == 0);

}

void N_MvData(void *pIn, u16 Opt, u8 Mode) {

	u8 Cnt;
	u8 Step;
	Cnt = Mode & 0x3F;
	Step = (Mode >> 6) + 3;
	while (Cnt) {
		MvData(pIn, Opt);
		pIn = (void*) (((u8*) pIn) + Step);
		Cnt--;
	}
}

void ClrRam(u8* buf, u16 Cnt) {
	FillRam( buf, 0, Cnt);
}

void SetRam(u8* buf, u16 Cnt) {
	FillRam( buf, 0xFF, Cnt);
}

void FpCharRam(u16 FillCode) {
	FillRam( tpChar, (u8)(FillCode>>8), (u8)FillCode);
	tpCharInc((u8) FillCode);
}

void ClrpCharRam(u8 Cnt) {//0
	FillpCharRam( 0, Cnt);
}

void SetpCharRam(u8 Cnt) {
	FillpCharRam( 0xFF, Cnt);
}

void RevStrLst(u8 *s, u8 Lth, u8 Repeat) {

	while (Repeat) {
		RevStr(s, Lth);
		s += Lth;
		Repeat--;
	}
}

void Rev_4(u8 *s) {
	RevStr(s, 4);
}

void Rev4Lst(u8 *s, u8 Repeat) {

	RevStrLst(s, 4, Repeat);
}

ucode *Get_Code_Lst(ucode *Addr, u16 Code, u8 Bytes, u8 Length) {
	u16 N;
	u16 Step;
	u16 Code_2;

	if (Code < (*(const __code u16*) Addr)) {
		return 0;
	}
	N = 0;
	Step = 0x8000;
	do {
		if (((N + Step) < Length) && (Code >= (*(const __code u16*) (Addr + ((N
				+ Step) * Bytes))))) {
			N += Step;
		}
		Step >>= 1;
	} while (Step);
	Addr = (Addr + ((N + Step) * Bytes));
	if (((const __code strChkCode *) Addr) ->DI0_Max) {
		Code_2 = (((((const __code strChkCode *) Addr)->Code) & 0xFF00))
				| (((const __code strChkCode *) Addr) ->DI0_Max);
		if (Code <= Code_2) {
			return Addr;
		}
	} else if (((const __code strChkCode *) Addr)->Code == Code) {
		return Addr;
	}
	return NULL;

}

void SetRamData(u8 *Dst, u8 Data, u16 Length) {
	while (Length--) {
		if (Data == _ADD_33_) {
			*Dst += 0x33;
		} else if (Data == _SUB_33_) {
			*Dst -= 0x33;
		} else {
			*Dst = Data;
		}
		Dst++;
	}
}

void RevCopyRam(char * Dst, char *Src, u16 Length) {

	while (Length--) {
		*Dst++ = Src[Length];
	}
}

void Copy_4(u8 *Dst, uc8 *Src) {

	CopyRam(Dst, Src, 4);
}

void RevCopy_4(u8* Dst, uc8 *Src) {

	RevCopyRam((char *)Dst, (char *)Src, 4);

}
void Copy_2(u8 *Dst, uc8 *Src) {
	CopyRam(Dst, Src, 2);
}

s32 Cmp_4(uc8 *s1, uc8 *s2) {
	return strCmp((u8*) s1, (u8*) s2, 4);
}

s8 Is_DispCode(u32 DCode) {
	//	RevStr((u8*)&DCode, 4);
	if (RamData.Disp.DispCode.Code == DCode) {
		return SUCCESS;
	}
	return FAIL;
}

u8 HEX(u8 Data) { //0x11 ->0x0b
	return ((Data) - (((Data) / 16) * 6));
}

u8 BCD(u8 Data) { //0x1f -> 0x31
	return ((Data) + (((Data) / 10) * 6));
}

void HEXLst(u8 *Data, u8 Lth) {

	u8 Cnt;
	for (Cnt = 0; Cnt < Lth; Cnt++) {
		Data[Cnt] = HEX(Data[Cnt]);
	}
}

void BCDLst(u8 *Data, u8 Lth) {

	u8 Cnt;
	for (Cnt = 0; Cnt < Lth; Cnt++) {
		Data[Cnt] = BCD(Data[Cnt]);
	}
}

u8 CheckDataAct(u8 *pData) {
	u32 Data;
	Copy_4((u8*) &Data, pData);
	if ((Data == 0) || (Data == 0xFFFFFFFF)) {
		return FAIL;
	}
	return SUCCESS;
}
#ifdef _DEL
u8 CheckKeyChg(u32 *pData) {
	u32 KData;
	KData = ((*pData) & ((((u32) 1) << FlashInfo.SetInfo.KeyNum) - 1));
	if (KData == 0) {
		return 2;
	} else if (KData < ((((u32) 1) << FlashInfo.SetInfo.KeyNum) - 1)) {
		return 1;
	}
	return 0;
}
#endif

void Hex_BCD(s32 Data, u8* Dst) {
	u8 a;
	u8 n;
	RamData.Use_Neg = 0;
	if (Data < 0) {
		Data = -Data;
		RamData.Use_Neg = 1;
	}
	for (n = 0; n < 5; n++) {
		a = Data % 100;
		Data = Data / 100;
		Dst[n] = BCD(a);
	}
}

u32 BCD_Hex(u32 Data) {
	u32 x;
	s32 Cnt;
	x = 0;
	for (Cnt = 3; Cnt >= 0; Cnt--) {
		x = ((u32) x * 100) + HEX(((u8*) &Data)[Cnt]);
	}
	return x;
}

char 		IsMemZero(char *sp , unsigned short ilen)
{
		unsigned short i;
	
		for ( i =0 ;i < ilen ; i++)
		 if (sp[i])
			 return 1;
		 return 0;
}
				

void Up_u32Data(u32 *pDst, u32 Data) {

	Copy_FlashInfo((u8*) pDst, (u8*) &Data, 4);
}

void Up_u16Data(u16* pData, u16 Data) {

	Copy_FlashInfo((u8*) pData, (u8*) &Data, 2);
}

void Up_u8Data(uc8* pData, u8 Data) {

	Copy_FlashInfo((u8*) pData, &Data, 1);
}

void ClrRcdTime(u32 *pTm) {

	Up_u32Data(pTm, 0);
}

void SetRcdTime(u32 *pTm) {

	Up_u32Data(pTm, RamData.RTCTime);
}

void SetRcdMin(u32 *pTm) {

	Up_u32Data(pTm, RamData.RTCTime / 60);
}

void UpJDWriter(void) {

	Up_u32Data(&FlashInfo.RunInfo.LastJDWriter,  RamData.Write);
}

void Up_NormFroze(u8 Type) { //按冻结描述冻结数据

	const __code u8* Addr;
	strFrozeSet const __code *pSet;
	Type = Chk_FrozeType(Type);
	if (Type >= 0x80) {
		return;
	}
	pSet = &FileSet.FrozeSet[Type];
	if (pSet->pFrozeData == 0) { //无冻结描述返回
		return;
	}
	Addr = (const __code u8*) (&FileSet) + (pSet->pFrozeData);
	tpChar = RamData.FrozeBuf;
	MoveDataLst((strMvData *) Addr); //MoveDataLst((strMvData const __code*) Addr);
	Up_FrozeData(RamData.FrozeBuf, Type);

}

u32 GetTimeDiff(u32 *pTime) {

	return ((RamData.RTCTime - (*pTime)) / 60);
}

void Up_FrozeOverTime(u8 FType, u32* pTime, u32* pLastTime) { //记录总时间冻结

	u8 Type;
	u32 Cnt;
	Type = FType & 0x3F;
	if (FlashInfo.FrozeCnt[Type] & 1) {
		if (*pLastTime) {
			Cnt = *pTime;
			if (RamData.RTCTime > (*pLastTime)) {
				Cnt += GetTimeDiff(pLastTime);//((RamData.RTCTime - (*pLastTime))/60);
				if (FType == _FROZE_PWR_ON_) {
					Up_FrozeData((u8*) &RamData.PwrOnTime, _FROZE_PWR_ON_);
				} else {
					Up_NormFroze(FType); //记录结束时间
				}
			} else if (FType == _FROZE_PWR_ON_) {
				Up_FrozeData((u8*) pLastTime, _FROZE_PWR_ON_);
			} else {
				Up_NormFroze(FType); //记录结束时间
			}
			Up_u32Data(pTime, Cnt); //记录总时间
			ClrRcdTime(pLastTime);
		} else {
			Up_NormFroze(FType); //记录结束时间
		}
	}

}

void Up_ProgFroze(u8 Type, u8 Mode, u8* Src) { //参数修改记录Mode-修改序号

	u16 Addr;
	strFrozeSet const __code *pSet;
	if (Type >= _FROZE_NUM_)
		return;
	pSet = &FileSet.FrozeSet[Type];
	if (pSet ->Length < 9)
		return;
	*(u32*) RamData.FrozeBuf = RamData.RTCTime;
	*(u32*) (RamData.FrozeBuf + 4) = RamData.Write;
	RamData.FrozeBuf[8] = Mode;
	CopyRam(RamData.FrozeBuf + 9, Src, pSet ->Length - 9);
	Up_FrozeData(RamData.FrozeBuf, Type);

}

void AddProgRecord(u8 *Code) { //增加编程记录Code:数据标识

	u32 Data[3];
	Copy_4((u8*) &Data[2], Code);
	if ((FlashInfo.RunInfo.EState & _ESTATE_IN_PROG_) == 0) {
		Data[0] = RamData.RTCTime;
		Data[1] = RamData.Write;
		Copy_FlashInfo((u8*) &FlashInfo.RunInfo.ProgTime, (u8*) &Data, 12);
		Up_u16Data(&FlashInfo.RunInfo.ProgCodeCnt, 1);
		SetEBit(_BIT_IN_PROG_);
		SetEvtState(_EVT_PROG_);
	} else {
		Up_u32Data(&FlashInfo.RunInfo.ProgCode[FlashInfo.RunInfo.ProgCodeCnt
				% 10], Data[2]);
		Up_u16Data(&FlashInfo.RunInfo.ProgCodeCnt,
				(FlashInfo.RunInfo.ProgCodeCnt + 1));
	}
}

void GetNewProgRcd(u8 *Buf) {

	u16 CodeCnt;
	u8 Cnt;
	CopyRam(Buf, (u8*) &FlashInfo.RunInfo.ProgTime, 8);
	CodeCnt = FlashInfo.RunInfo.ProgCodeCnt;

	for (Cnt = 0; Cnt < 10; Cnt++) {
		if (CodeCnt) {
			CodeCnt--;
			Copy_4(Buf + 8 + (Cnt * 4),
					(u8*) &FlashInfo.RunInfo.ProgCode[CodeCnt % 10]);
		} else {
			SetRam(Buf + 8 + (Cnt * 4), 4);
		}
	}
}

void CloseProgRecord(void) { //结束编程记录

	u32 Data[12];
	u8 Cnt;
	u16 CodeCnt;
	if ((FlashInfo.RunInfo.EState & _ESTATE_IN_PROG_) == 0) {
		return;
	}
	ClrEBit(_BIT_IN_PROG_);
	GetNewProgRcd((u8*) Data);

	Up_FrozeData((u8*) Data, _FROZE_PROG_);

}

void SetEBit(u8 SetBit) {

	u8 Cnt;
	u8 Bit;
	u8 EBit;
	u8 Data;
	Bit = (SetBit & 7);
	Cnt = ((SetBit >> 3) & 3);
	Data = ((u8*) &FlashInfo.RunInfo.EState)[Cnt];
	if (SetBit & 0x80) {
		Data &= ~(1 << Bit);
	} else {
		Data |= (1 << Bit);
	}
	Up_u8Data(((u8*) &FlashInfo.RunInfo.EState) + Cnt, Data);
}

void UpEState(u32 NewState) {

	Up_u32Data(&FlashInfo.RunInfo.EState, NewState);
}
 /*
void ChkEvtState(void) { //检查主动上报

	u8 Cnt;
	for (Cnt = 0; Cnt < NUMOF(FlashInfo.RunInfo.EvtCnt); Cnt++) {
		if (FlashInfo.RunInfo.EvtCnt[Cnt]) {
			SetEBit(_BIT_EVT_OUT_);
if (_ISCOMMODE_ZB()){	 //#ifndef		_METER_485_
			Set_EVTOut();
}//#endif
			return;
		}
	}
	ClrEBit(_BIT_EVT_OUT_);
if (_ISCOMMODE_ZB()){	 //#ifndef		_METER_485_
	Clr_EVTOut();
}//#endif

}

void SetEvtState(u8 BitCnt) {

	u8 Cnt;
	u8 Mask;
	if (BitCnt >= NUMOF(FlashInfo.RunInfo.EvtCnt)) {
		return;
	}
	Cnt = BitCnt >> 3;
	Mask = 1 << (BitCnt & 7);
	if ((FlashInfo.SetInfo.EvtMode[Cnt] & Mask) == 0) {
		return;
	}
	if (((FileSet.EvtCntMask[Cnt] & Mask) == 0)
			&& (FlashInfo.RunInfo.EvtCnt[BitCnt] < 0xFF)) {
		Up_u8Data(&FlashInfo.RunInfo.EvtCnt[BitCnt],
				FlashInfo.RunInfo.EvtCnt[BitCnt] + 1);
	} else {
		Up_u8Data(&FlashInfo.RunInfo.EvtCnt[BitCnt], 0xFF);
	}
	ChkEvtState();

}
*/
void WDGTm_Refresh(unsigned char clen)
{
	unsigned short itemp =((unsigned short )clen+1)*12 + RamData.WDGTm;
	unsigned char ctemp = ( itemp >0xff)?0xff:((unsigned char)itemp);
	do {
		RamData.WDGTm = ctemp;
	} while ( ctemp!=RamData.WDGTm);

}

void RSet_WDG(unsigned char ctemp) {
	unsigned short itemp = RamData.WDGTm + ctemp;
	if (itemp > 0xff)
		itemp = 0xff;
	do {
	RamData.WDGTm = itemp;
	} while (RamData.WDGTm != itemp);
}

void Set_VDDLO(void) {
	RamData.bVDD_Down = 1;
	stopUart(_R485_Chanel_); //	 485通道
	stopUart(_Line_Chanel_); //	载波通道
	stopUart(_IR_Chanel_);

}



uint8_t bcd_add(uint8_t bcd_a, uint8_t bcd_b)
{
  uint16_t ret;

  ret = (bcd_a & 0xf) + (bcd_b & 0xf);
  
  if (ret > 0x9) {
    ret += 0x6;
  }

  ret += (bcd_a & 0xf0) + (bcd_b & 0xf0);  
  
  if ((ret & 0x1f0) > 0x90) {
    ret += 0x60;
  }
  return (uint8_t)ret;
}

//---------------------------------------------------------
// modulo-100 bcd increament
uint8_t bcd_inc(uint8_t bcd)
{
  if ((bcd & 0xf) >= 0x9) {
    if (bcd >= 0x99) {
      bcd = 0x0;
    }
    else {
      bcd += 0x7;
    }
  }
  else {
    bcd++;
  }
  return bcd;
}

//---------------------------------------------------------
// modulo-100 bcd decreament
uint8_t bcd_dec(uint8_t bcd)
{
  bcd--;
    if ((bcd & 0x0f) >= 0x09) {
        if (bcd > 0x99) {
            bcd = 0x99;
        }
        else {
            bcd -= 0x06;
        }
    }
    return bcd;
}