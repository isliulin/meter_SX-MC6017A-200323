//#include "system.h"
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "emu.h"
#include "bit_edge.h"
#include "pubset.h"
#include "flash_filesystem.h"
#include "math.h"
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
#include "predef.h" 
 
 
#include <stdlib.h>
#include "emu.h"
#include "streamio.h"
#include "streamio.h"
#include "rtc_drv.h"

#include "gc_5_60_1200.h"
extern volatile struct STSCIBUF USARTCOM[_Com_End_];
 
//#include "ce.h"
#define _USE_24_CODE_
u8	ChkKey( u8* Key);
void ChgKey(strDispCode const *pCode);
 void  OSC16_Rfresh(void);
 
u8 Jlregread(strDispCode const *pCode, u8* pAddr);
u8 Jbregread(strDispCode const *pCode, u8* pAddr);
u8 effective_value(strDispCode const *pCode, u8* pAddr);
 
u8 Jlrestore(strDispCode const *pCode, u8* pAddr);
 u8 RTCOFFSETREAD(strDispCode const *pCode, u8* pAddr);
 u8 RTCOFFSEINIT(strDispCode const *pCode, u8* pAddr);
u8 ReadFlashData(strDispCode const *pCode, u8* pAddr);
u8 ReadExtEEprom(strDispCode const *pCode, u8* pAddr);
u8 Get645_LVD(strDispCode const *pCode, u8* pAddr);
u8 RTC_DIV_READ(strDispCode const *pCode, u8* pAddr);
u8 RTC_Trim_Enable(strDispCode const *pCode, u8* pAddr);
u8 Write_meterconfig(strDispCode const *pCode, u8 * pAddr);
u8 ReadEEPROMFData(strDispCode const *pCode, u8* pAddr);
u8 WriteVolEffect(strDispCode const *pCode, u8* pAddr);
u8 RTCOFFSETREAD(strDispCode const *pCode, u8* pAddr);
u8 WriteRTCSet(strDispCode const *pCode, u8* pAddr) ;
u8 ReadWriteTRxNum(strDispCode const *pCode, u8* pAddr);
extern const strFileSet FileSet;
extern const strFData FData;
extern const struct STTblSetLst TblSetLst[];
#define		_SAFE_D_1_		(_SAFE99_|_SF_FAC_)						//第一类数据
#define		_SAFE_D_2_		(_SAFE98_|_SF_FAC_)						//第二类数据
#define		_SF_FAC_0_		(_SF_FAC_)								//工厂模式数据
#define		_SF_FAC_1_		(_SF_FAC_|_SFFOFF_)						//工厂模式
#define		_SF99NCHG_		(_SAFE99_|_SFNCHG_)						//99级修改密钥前可写
#define  	_IsIRChangel()	( Ex645.SCIx == &SCI[_IR_Chanel_] ) 			    //当前使用红外通道为真



const  struct STCOMCHANEL stcomchanel[_Com_End_ ] =
{
		{ 	3,	&(FlashInfo.SetInfo.Baud[1]) },	//  #define    _R485_config_    FlashInfo.SetInfo.Baud[1]
	//	{	5,	&(FlashInfo.SetInfo.Baud[2]) },	//	#define    _Line_config_   	FlashInfo.SetInfo.Baud[2]
		{	1 ,	&(FlashInfo.SetInfo.Baud[0]) },	//	#define    _IR_config_     	FlashInfo.SetInfo.Baud[0]
};

#define   _Hu_Datano_			0x040e
u8	WriteFHStartTm(strDispCode const *pCode, u8* pAddr);
void errintfun(unsigned short n);

void Send_SCI(strSCIBuf *SCIx) {
} //发送下一数据

void GetProgData(u8* Dst, u32 pCode, u32 Lth) {
	unsigned long i;
unsigned long lAddr;
	for (i = 0; i < Lth; i++)
	{
		lAddr = pCode +i;
		if ( lAddr <FData.DftSet.CodeLth)			
		 Dst[i] = *((u8 *) (pCode + i));
		else
		Dst[i] =0xff;
		
	}
}

void SCIProg(void) {
}

u8 WriteEXTEEPROM(strDispCode const *pCode, u8* pAddr);

//void	Rx_Enable(strSCIBuf *SCIx)
//{
//			 if ( SCIx == SCI )
//			 {
//					 _ENABLE_USART1_RECE_INT_;
//					 return ;
//			 }
//			 else if ( SCIx == (SCI+1))
//					{
//						 _ENABLE_USART0_RECE_INT_;
//						 return;
//					}
//			 else if ( SCIx== (SCI+2))
//			 {
//					 _ENABLE_USART2_RECE_INT_ ;
//					 return;
//			 }
//}								//接收使能
void Tx_Enable(strSCIBuf *SCIx) {
} //发送使能
void SubSCIBuf(u8 Cnt) {
} //清除通讯数据


void Up_BCDData(u32 Data, u8 L) { //数据输出(输出时低字节在前)

	u8 B_Data[5];
	if (L) {
		Hex_BCD(Data, B_Data);
		if ((RamData.Use_Neg) && (RamData.InDisp == 0)) {
			B_Data[L - 1] |= 0x80;
		}
		NormCopy(B_Data, L);
	}
}

u8 GetCompData(u8 Mode) {return SUCCESS;} //获取程序比对数据
#ifdef _DEL
u8 GetCompData(u8 Mode) { //获取程序比对数据

	u8* pData;
	u32 pCode;
	u16 MaxCnt;
	u16 Cnt;
	u16 Bias;
	if (Mode) {
		MaxCnt = 256; //比对数据
		//pCode = *(u32*)(Ex645.SCIx->DataBuf+16);
		CopyRam((char*) &pCode, (Ex645.SCIx->DataBuf + 16), 4);
	} else {
		MaxCnt = 16; //比对因子
		//pCode = *(u32*)(Ex645.SCIx->DataBuf+12);
		CopyRam((char*) &pCode, (Ex645.SCIx->DataBuf + 12), 4);
	}
	if (pCode >= _FLASH_CODE_SIZE_) { //地址超限处理
		return FAIL;
	}
	if (pCode + MaxCnt <= _FLASH_CODE_SIZE_) {
		GetProgData(ComData, pCode, MaxCnt);
	} else {
		Cnt = (_FLASH_CODE_SIZE_ - pCode);
		GetProgData(ComData, pCode, Cnt);
		ClrRam(ComData + Cnt, MaxCnt - Cnt);
		ComData[Cnt] = 0x80;
	}
	return SUCCESS;
}
#endif

u8 SetErrReturn(u8 Err) { //设置错误返回(Err-错误

	if (RamData.InDisp) {
		return FAIL;
	}
	if (Ex645.SCIx->AddrMode > 1) {
		Ex645.SCIx->NoRet = 1;
	}
	Ex645.SCIx->DataBuf[0] |= 0xC0;
	Ex645.SCIx->DataBuf[1] = 0x01;
	Ex645.SCIx->DataBuf[2] = Err;
	return FAIL;
}

u8 GetIRSafe(void) { //返回 0 - 非红外或者红外已认证  1 - 只允许红外可读指令
	if ((RamData.InDisp == 0) && (_IsIRChangel()) && (RunState_IR == 0) && (RamData.FacProgTime == 0)) {
		return ERROR;
	}
	return SUCCESS;
}

void AddTRxWRecord(void) { //增加编程记录

	AddProgRecord(Ex645.SCIx->DataBuf + 2);
	Ex645.SCIx->DataBuf[1] = 0;
}

u8 GetSafeState(u8 Safe, u8 Length) { //检查安全状态,密码/MAC/长度等信息

	u32 Cnt;
	u32 KeyNum;
		if (Safe && (!_localAddr)) {//Ex645.SCIx->AddrMode)){
		return SetErrReturn(_ERR_SAFE_);
	}
	if ((Safe & _SFNCHG_) && (CheckKeyChg(&FlashInfo.SetInfo.EKeyState)
			== SUCCESS)) {
		return SetErrReturn(_ERR_SAFE_);
	}
	if((Safe)&&(Ex645.SCIx->DataBuf[6] == 0x99)){		 //第一/二类数据权限
		if ((Safe & _SAFE99_) == 0) {
			return SetErrReturn(_ERR_SAFE_);
		}
		//		RamData.Write = *(u32*)(SCIx->DataBuf+10);
		return GetSafeData(Ex645.SCIx->DataBuf, Length);
} else if((Safe)&&(Ex645.SCIx->DataBuf[6] == 0x98)){	//第一/二类数据权限
		if (Safe & _SAFE99_) {
			return SetErrReturn(_ERR_SAFE_);
		}
		//		RamData.Write = *(u32*)(Ex645.SCIx->DataBuf+10);
		return GetSafeData(Ex645.SCIx->DataBuf, Length);
	} else if (Ex645.SCIx->DataBuf[1] == (Length + 12)) { //其他数据
		Copy_4((u8*) &RamData.Write, Ex645.SCIx->DataBuf + 10);
		Cnt = Safe & 3;
		//if (Cnt == _SF_FAC_) {
			if ( _ISFACMODE() || ((!_ISFACMODE())&& (Ex645.Code645.Code ==_INTOFACMODE_645_) && ( 0x14==Ex645.SCIx->DataBuf[0]) && ( 0==Ex645.SCIx->DataBuf[14]))) 
				{ 			
			if (strCmp(Ex645.SCIx->DataBuf + 6, (u8*) (FileSet.FacKey), 8)) {
				return SetErrReturn(_ERR_SAFE_);
			}
			if (((Safe & _SFFOFF_) == 0) && (RamData.FacProgTime == 0)) {
				return SetErrReturn(_ERR_SAFE_); //非工厂模式返回
			}
		
#ifdef		_USE_24_CODE_
		} else if(Cnt) {
			KeyNum = ChkKey(Ex645.SCIx->DataBuf+6);
			if(KeyNum == 0) {
				return SetErrReturn(_ERR_SAFE_);
			}
		//	if((Safe&3) == _SAFE_2_) {
		//		if(KeyNum> _SAFE_2_) {
		//			return SetErrReturn(_ERR_SAFE_);
		//		}
		//	} else if((KeyNum < _SAFE_2_)||(KeyNum> (Safe&3))) {
		//		return SetErrReturn(_ERR_SAFE_);
		//	}
#endif
		} else if (Safe) { //仅(Safe == 0)时无须验证
			return SetErrReturn(_ERR_SAFE_);
		}
		CopyRam(Ex645.SCIx->DataBuf + 6, Ex645.SCIx->DataBuf + 14, Length); //去除密码/操作者代码信息
		/*		if(Ex645.ExLth){												//处理分组数据反向(日时段表等信息)
		 RevStrLst(Ex645.SCIx->DataBuf+6, Ex645.ExLth, Length/Ex645.ExLth);
		 //			Ex645.ExLth = 0;
		 } else {
		 RevStr(Ex645.SCIx->DataBuf+6, Length);					//处理数据反向
		 }	*/
		return SUCCESS;
	}
	return SetErrReturn(_ERR_OTHER_);
}

u8 WBaud(u8 D0, u8 Baud) { //修改通讯速率

	static const u8 aBaud[] = { 0x04, 0x08, 0x10, 0x20 };
	u32 Cnt;
	if (D0 == 1) { //红外波特率
		if (Baud == 4) {
			Up_u8Data(&FlashInfo.SetInfo.Baud[0], Baud);
			return SUCCESS;
		} else {
			return SetErrReturn(_ERR_BAND_);
		}
	} else if ((D0 == 3) || (D0 == 5)) { //485/载波速率
		for (Cnt = 0; Cnt < sizeof(aBaud); Cnt++) {
			if (Baud == aBaud[Cnt]) {
				Up_u8Data(&FlashInfo.SetInfo.Baud[D0 >> 1], Baud);
#ifdef		_USE_SET_BAUD_
				D0 = (D0 / 2) - 1;
				if (SCI[D0].TRxTm == 0) {
					//Rst_SCI(D0);
				}
				if (D0 == 1) {
					RamData.TRxNumChgTm = 5;
				}
#endif
				return SUCCESS;
			}
		}
		return SetErrReturn(_ERR_BAND_);
	} else {
		return SetErrReturn(_ERR_NODATA_);
	}
}

u8 WriteBaud(strDispCode const *pCode, u8* pAddr) { //修改通讯速率(2007)

	if (WBaud((pCode->Code & 0xFF), (u8) Ex645.FastData) == SUCCESS) {
		return SUCCESS;
	}
	return FAIL;
}

void RxChgTRXBd(void) { //修改通讯速率(97)

	u32 D0;
	static const u8 D0Lst[] = { 3, 5, 1 };
	D0 = Ex645.SCIx - &SCI[0];
	D0 = D0Lst[D0];
	if (WBaud(D0, Ex645.SCIx->DataBuf[2]) == SUCCESS) {
		Ex645.SCIx->DataBuf[1] = 1;
	}
}

u8 RxWriteKey(strDispCode const *pCode, u8* pAddr) { //修改密码

	u8* pData;
	pData = Ex645.SCIx->DataBuf;
	if (Ex645.WBit) {
		if ((u8) (pCode ->Code) != (pData[6] + 1)) { //
			return SetErrReturn(_ERR_OTHER_);
		}
		Copy_FlashInfo(FlashInfo.SetInfo.TRxKey[pData[6] >> 1], pData + 6, 4);
		return SUCCESS;
	} else {
		return SetErrReturn(_ERR_SAFE_);
	}
}

const u8 Const5FF[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const u8 Const699[] = { 0x99, 0x99, 0x99, 0x99, 0x99, 0x99 };
const u8 Const6665[] = { 0x61, 0x62, 0x63, 0x64, 0x65, 0x66 };

#define		_PUB_LTH_		2
u8 Chk_PubAddr(u8 *pAddr) { //检查通用地址

	if (strCmp((Const699), pAddr, 6) == 0) {
		return 0;
	}
	if (strCmp((Const6665), pAddr, 6) == 0) {
		return 1;
	}
	return 2;
}

u8 CheckPubMode(void) { //设置广播地址无返回

	if (_localAddr) {
		return SUCCESS;
	} else if (_broadcast99Addr) {
		Ex645.SCIx->NoRet = 1;
		return SUCCESS;
	}
	Ex645.SCIx->NoRet = 1;
	return FAIL;
}

void SetTrxDisp(void) { //显示指令处理
#if			_TRX_TYPE_ == 1
	if (Ex645.SCIx == &SCI[1]) {
		SetDispSubItem(WTRX_OK | 5);
#ifdef		_ZB_RISECOMM_
		RamData.ZBTime = 0;
#endif
	} else {
		SetDispSubItem(TRX_OK | 5);
	}
#else
	SetDispSubItem(TRX_OK|5);
#endif
}

void WriteSafeData(const strWriteCode *pWt) { //写数据处理

	u32 Safe;
	u32 Lth;
	u32 Lth2;
	u32 Ret;
	u8* Addr;
	u32 Min;
	u32 Max;
	u32 MinMax;
	u32 Code16;
	u8 *pData;

	if (pWt ->Safe & _SF_ROL_) {
		SetErrReturn(_ERR_SAFE_);
		return;
	}
	pData = Ex645.SCIx->DataBuf + 6;
	Code16 = (u16) (Ex645.Code645.Code);
	Code16 -= pWt ->Code;
	Addr = (pWt ->pAddr) + (Code16 << (pWt ->BitSet.Rol));
	if (pWt ->BitSet.HexBCD >= 2) {
		Lth = pWt ->BitSet.Lth;
	} else {
		Lth = (pWt ->BitSet.Lth & 7);
	}
	if (GetSafeState(pWt ->Safe, Lth)) {
		return;
	}
	if (SUCCESS != Check_Datatype_Mod_Enable(pWt->cMod))
	{
		SetErrReturn(_ERR_SAFE_);
				return ;
	}
	if (pWt ->BitSet.HexBCD & 1) {
		if (CheckBCD(pData, Lth)) {
			SetErrReturn(_ERR_OTHER_);
			return;
		}
	}
	Ex645.FastData = 0;
	MinMax = (u32) (pWt ->MinMax);
	if (pWt ->BitSet.WriteProg) {
		if ((MinMax > 0x20000) && (_CODE_DEL_)) {
			SetErrReturn(_ERR_NODATA_);
			return;
		}
		if ((pWt ->BitSet.HexBCD & 2) && (pWt ->BitSet.Dir)) {
			RevStr(pData, Lth);
		}
		if (Lth <= 4) {
			CopyRam((u8*) &Ex645.FastData, pData, Lth);
		}
		//if (((pWt ->MinMax))(&Ex645.Code645, Addr)) {
		if (((u8(*)(strDispCode const *, uc8*)) (pWt ->MinMax))(&Ex645.Code645,
				Addr)) {
			return;
		}
	} else if (pWt ->BitSet.HexBCD < 2) {
		if (Lth > 4) {
			SetErrReturn(_ERR_OTHER_);
			return;
		}
		Ex645.FastData = 0;
		if (pWt ->BitSet.Dir) {
			RevCopyRam((char *) &Ex645.FastData, pData, Lth);
		} else {
			CopyRam((u8*) &Ex645.FastData, pData, Lth);
		}
		if (pWt ->BitSet.HexBCD == 1) {
			Ex645.FastData = BCD_Hex(Ex645.FastData);
		}
		Lth2 = (pWt ->BitSet.Lth >> 3);
		if (Lth2 == 0) {
			Lth2 = Lth;
		}
		Ret = ((MinMax >> 24) & 0xF8);
		if (Ret == 0x80) { //_MAX_0_
			Min = 0;
			Max = MinMax & 0x7FFFFFF;
		} else if (Ret == 0x88) { //_MAX_1_
			Min = 1;
			Max = MinMax & 0x7FFFFFF;
		} else if (Ret >= 0xC0) { //_MIN_MAX_
			Min = (MinMax >> 16) & 0x3FFF;
			Max = MinMax & 0xFFFF;
		} else {
			Min = 0;
			Max = 0xFFFFFFFF;
		}
		if ((Ex645.FastData < Min) || (Ex645.FastData > Max)) {
			SetErrReturn(_ERR_OTHER_);
			return;
		}
		if (Chk_Mac_99()) {
			return;
		}
		Copy_FlashInfo(Addr, (u8*) &Ex645.FastData, Lth2);
	} else {
		if (pWt ->BitSet.Dir) {
			RevStr(pData, Lth);
		}
		Copy_FlashInfo(Addr, pData, Lth);
	}
	if (pWt ->BitSet.ProgRcd) {
		AddTRxWRecord();
	}
	Ex645.SCIx->DataBuf[1] = 0;
	if (pWt ->BitSet.FeilvChg) {
		RamData.Event_Feilv = 1;
	}
}

void ReadSafeData(strDispCode const *pCode, const strWriteCode *pWt) {//读数据处理

	u8 Buf[7];
	u8 Lth;
	u8 Lth2;
	uc8* Addr;
	u16 Code16;

	Code16 = (u16) (pCode ->Code);
	Code16 -= pWt ->Code;
	Addr = (pWt ->pAddr) + (Code16 << (pWt ->BitSet.Rol));
	if (SUCCESS != Check_Datatype_Mod_Enable(pWt->cMod))
	{
		SetErrReturn(_ERR_SAFE_);
		return ;
	}
	if (RamData.InDisp) {
		if (pWt ->DispSet & _DISP_NO_DISP_) { //
			return;
		}
		if (pWt ->DispSet) {
			RamData.Disp.DispSet = pWt ->DispSet;
		}
		if (pWt ->pBitLst) {
			AddDispBits((char *)pWt ->pBitLst);
			if ((Code16) && (RamData.Disp.BitLst[0] == _DISP_DIGIT_)) {
				RamData.Disp.BitLst[1] += Code16;
			}
		}
	}
	if (pWt ->BitSet.ReadProg) {
		((void(*)(strDispCode const *, uc8*)) (pWt ->MinMax))(pCode, Addr);
		return;
	}
	if (pWt ->BitSet.HexBCD >= 2) {
		Lth = pWt ->BitSet.Lth;
		Lth2 = Lth;
	} else {
		Lth = (pWt ->BitSet.Lth) & 7;
		Lth2 = (pWt ->BitSet.Lth >> 3);
		if (Lth2 == 0) {
			Lth2 = Lth;
		}
	}
	if (pWt ->BitSet.HexBCD < 2) {
		*(u32*) Buf = 0;
		CopyRam(Buf, Addr, Lth2);
		if ((Lth2 == 2) && (Buf[1] & 0x80) && (pWt ->BitSet.HexBCD == 1)) {
			*(u16*) (Buf + 2) = 0xFFFF;
		}
		if (pWt ->BitSet.HexBCD == 1) {
			Hex_BCD(*(u32*) Buf, Buf);
			if ((RamData.Use_Neg) && (RamData.InDisp == 0)) {
				Buf[Lth - 1] |= 0x80;
			}
		}
		if (pWt ->BitSet.Dir) {
			RevCopy(Buf, Lth);
		} else {
			NormCopy(Buf, Lth);
		}
		return;
	} else if ((RamData.InDisp == 0) || (Lth <= 4)) {
		if (pWt ->BitSet.Dir) {
			RevCopy(Addr, Lth);
		} else {
			NormCopy(Addr, Lth);
		}
	}
}
/*
void ClrEvtState(u8 *pEvtState) { //清除主动上报状态

	u32 Cnt;
	u8 stemp[_cEvnCnt_Len_];
	CopyRam(stemp, (char*) &FlashInfo.RunInfo.EvtCnt, sizeof(stemp));
	for (Cnt = 0; Cnt < NUMOF(FlashInfo.RunInfo.EvtCnt); Cnt++) {
		if ((pEvtState[Cnt >> 3] & (1 << (Cnt & 7))) == 0) {
			//Up_u8Data(&FlashInfo.RunInfo.EvtCnt[Cnt], 0);
			stemp[Cnt] = 0;
		}
	}
	Copy_FlashInfo((char*) &FlashInfo.RunInfo.EvtCnt, stemp, sizeof(stemp));
	ChkEvtState();
	RamData.EvtClrTm = 0;
	//	ClrRcdTime(&FlashInfo.RunInfo.RcdTm.EvtReadClrTime);
}

void cClrEvtState(u8 *pEvtState) { //清除主动上报状态
	u8 NEvt[8];
	CopyRam(NEvt, (pEvtState), 8);
	ClrEvtState(NEvt);
}

u8 ReadEvtState(strDispCode const *pCode, u8* pAddr) { //读主动上报状态字

	u32 Cnt;
	u32 k;
	u32 EvtMask[2];
	u8* pData;
	static const strMvData mvSetEvtState[] = { FILL_RAM( 0, 12), FILL_RAM( 0xAA, _RET_|1), };
	if (Ex645.WBit == 0) {
		if ((RamData.InDisp == 0) && ((u16) (pCode ->Code) != 0x1503)) {
			pData = tpChar;
			MoveDataLst(mvSetEvtState);
			for (Cnt = 0; Cnt < NUMOF(FlashInfo.RunInfo.EvtCnt); Cnt++) {
				if (FlashInfo.RunInfo.EvtCnt[Cnt]) {
					k = Cnt >> 3;
					if (k >= 4) {
						k += 4;
					}
					pData[k] |= (1 << (Cnt & 7));
					SetpCharInc(FlashInfo.RunInfo.EvtCnt[Cnt]);
				}
			}
			SetpCharInc(0xAA);
			Ex645.SCIx->DataBuf[2] = 0x01;
			Ex645.SCIx->DataBuf[3] = 0x15;//  *(u16*)(Ex645.SCIx->DataBuf+2) = 0x1501;
			Ex645.SCIx->DataBuf[4] = 0x00;
			Ex645.SCIx->DataBuf[5] = 0x04;//*(u16*)(Ex645.SCIx->DataBuf+4) = 0x0400;
			RamData.EvtClrTm = FlashInfo.SetInfo.EvtClrTmSet * 60;
			Copy_4((u8*) &EvtMask[0], pData);
			Copy_4((u8*) &EvtMask[1], pData + 8);
			EvtMask[0] = ~EvtMask[0];
			EvtMask[1] = ~EvtMask[1];
			Copy_FlashInfo((u8*) FlashInfo.RunInfo.LastEvtState, (u8*) EvtMask,
					8);
		}
		return SUCCESS;
	} else if ((pCode->Code & 0xFF) == 0x03) {
		//		if(FlashInfo.RunInfo.RcdTm.EvtReadClrTime == 0){
		if (RamData.EvtClrTm == 0) {
			return SetErrReturn(_ERR_OTHER_);
		}
		pData = Ex645.SCIx->DataBuf + 6;
		Copy_4((u8*) &EvtMask[0], pData);
		Copy_4((u8*) &EvtMask[1], pData + 8);
		//for (Cnt = 0; Cnt < NUMOF(FlashInfo.RunInfo.EvtCnt); Cnt++) {
		//	if (FlashInfo.RunInfo.EvtCnt[Cnt]) {
		//		k = Cnt >> 3;
		//		if (((((u8*) &EvtMask)[k] & (1 << (Cnt & 7))) == 0)
		//				&& (FlashInfo.RunInfo.LastEvtState[k]
		//						& (1 << (Cnt & 7)))) {
		//			return SetErrReturn(_ERR_OTHER_);
		//		}
		//	}
		//}
		//EvtMask[0] |= (*(u32*) FlashInfo.RunInfo.LastEvtState);
		//EvtMask[1] |= (*(u32*) (FlashInfo.RunInfo.LastEvtState + 4));
		{
			u8 Cnt, i, j;
			char *pEvtState = (u8*) &EvtMask;
			for (Cnt = 0; Cnt < NUMOF(FlashInfo.RunInfo.EvtCnt); Cnt++) {
				i = Cnt >> 3;
				j = Cnt & 7;
				if ((0 == (pEvtState[i] & (1 << (j))))
						&& (FlashInfo.RunInfo.LastEvtState[i] & (1 << (j)))
						&& (FlashInfo.RunInfo.EvtCnt[Cnt])) {
					return SetErrReturn(_ERR_OTHER_);

				}
			}
		}
		ClrEvtState((u8*) &EvtMask);
	}
	return SUCCESS;
}
*/

u8 ReadWriteTRxNum(strDispCode const *pCode, u8* pAddr) { //读写通讯地址/表号/户号(2007)

	if (Ex645.WBit == 0) {
		if (RamData.InDisp) {
			if (pCode ->SubCode == 0) {
				RevCopy(pAddr, 2); //Hi 4 Bit
			} else if (pCode ->SubCode == 1) {
				RevCopy(pAddr + 2, 4); //Lo 8 Bit
			}
		} else {
			RevCopy(pAddr, 6);
		}
		return SUCCESS;
	}
		
	if (( _Hu_Datano_ == ((u16) (pCode ->Code)) ) && (_ISMETER_LOCAL()))
	{
		SetErrReturn(_ERR_SAFE_); 
			return FAIL;
	}
	if ((Chk_PubAddr(Ex645.SCIx->DataBuf + 6)) < _PUB_LTH_) {
		return SetErrReturn(_ERR_OTHER_);
	}
	if (RamData.Mac_99) {
		if (Chk_Mac_99()) {
			return FAIL;
		}
		//#ifndef		_METER_485_
	} else if (((u16) (pCode ->Code) == 0x0401) && (_ISCOMMODE_ZB())) { //修改通讯地址
		RamData.TRxNumChgTm = 5;
		//#endif
	}
	Copy_FlashInfo(pAddr, Ex645.SCIx->DataBuf + 6, 6);
	if (((pCode->Code) & 0xFF) == 2) {
		RamData.IDACCess = 1;
	}
	return SUCCESS;
}

void RxOrderFroze(void) { //冻结指令

	u8 *pData;
	pData = Ex645.SCIx->DataBuf + 2;
	if (CheckPubMode()) { //广播地址检查
		return;
	}
	if (CheckBCD(pData, 4)) {
		SetErrReturn(_ERR_OTHER_);
		return;
	}
	GetFrozeMinCnt((char *)&RamData.FrozeTime);
	if (Cmp_4(pData, (Const699)) == 0) {
		Up_NormFroze(_FROZE_ACT_);
	} else {
		Rev_4(pData);
		Copy_FlashInfo(FlashInfo.SetInfo.FrozeCode, pData, 4);
	}
	pData--;
	*pData = 0;
	SetTrxDisp();
}

u8 Chk5Min(u32 *Tm1, u32 *Tm2) {

	s32 Diff;
	Diff = *Tm1 - *Tm2;
	if (labs(Diff) <= 300) {
		return SUCCESS;
	}
	return FAIL;
}

typedef struct {
	uc8* pTime;
	u8 Bias;
	u8 Lth;
	u8 Repeat;
} strTmN5;

void RxPublicWriteTime(void) { //广播校时

	u32 Cnt;
	u32 CntNow;
	u32 CntDiff;
	s32 Diff;
	u16 MaxDiff;
	u8 N;
	u8 N2, N3;
	u8 TmBCD[6];
	u8 Tm_2[6];
	u8 *pData;
	static const strTmN5 TmN5[] = { { (FileSet.ZeroP), 3, 1, 1 }, //每日零点
			{ (u8*) &FlashInfo.SetInfo.FrozeDT[0], 2, 2, 3 }, //结算日
			//	{	(u8*)&FlashInfo.PriceTbl[0].YStepTm[0],	1, 3, 4 },		//年结算日
			};
	strTmN5 const *pTmN5;
	pData = Ex645.SCIx->DataBuf;
	if (CheckPubMode()) {
		return;
	}
	Cnt = GetTimeCnt(pData + 2, (_REV_ | S_SEC | T_SEC));
	GetCntTime(TmBCD, 0, S_RTC | T_YMDHM);
	CntNow = GetTimeCnt(0, S_RTC | T_SEC);
	if (CheckDataAct((u8*) &Cnt)) { //时间无效错误
		SetErrReturn(_ERR_OTHER_);
		return;
	}
#ifdef		_USE_RTC_
	if (FlashInfo.RunInfo.EState & _ESTATE_DAY_CHG_) { //每天只能校时一次
		SetErrReturn(_ERR_OTHER_);
		return;
	}
	pTmN5 = TmN5;
	for (N = 0; N < NUMOF(TmN5); N++) {
		N2 = 0;
		while (N2 < pTmN5 ->Repeat) {
			*(u32*) Tm_2 = *(u32*) TmBCD;
			*(u16*) (Tm_2 + 4) = 0;
			CopyRam(Tm_2 + pTmN5 ->Bias, pTmN5 ->pTime + (N2 * pTmN5 ->Lth),
					pTmN5 ->Lth);
			CntDiff = GetTimeCnt(Tm_2, (S_SEC | T_SEC));
			if (Chk5Min(&CntDiff, &CntNow) == SUCCESS) {

				//	Diff = CntDiff - CntNow;
				//	if(labs(Diff) < 300){
				SetErrReturn(_ERR_OTHER_);
				return;
			}
			if (pTmN5 ->Bias == 3) {
				CntDiff += 86400;
				if (Chk5Min(&CntDiff, &CntNow) == SUCCESS) {
					//				if(labs(Diff) < 300){
					SetErrReturn(_ERR_OTHER_);
					return;
				}
			}
			N2++;
		}
		pTmN5++;
	}
	for (N = 0; N < 4; N++) {
		if (Chk5Min((((u32*) &FlashInfo.SafeInfo.StartTm_1) + N), &CntNow)
				== SUCCESS) {
			//	Diff = *(((u32*)&FlashInfo.SafeInfo.StartTm_1)+N) - CntNow;
			//	if(labs(Diff) < 300){
			SetErrReturn(_ERR_OTHER_);
			return;
		}
	}
	if (Chk5Min(&Cnt, &CntNow)) {
		//			(((u32*)&FlashInfo.SafeInfo.StartTm_1)+N)
		//	if(labs(CntNow - Cnt) > (5*60)){								//超过5分钟返回
		SetErrReturn(_ERR_OTHER_);
		return;
	}
	/*

	 MaxDiff = *(u16*)(TmBCD+3);
	 if((MaxDiff < 0005)||(MaxDiff >= 0x2355)){						//每日零点前后5分钟禁止校时
	 SetErrReturn(_ERR_OTHER_);
	 return;
	 }
	 for( N = 0; N < 3; N++){										//结算日时前后5分钟禁止校时
	 if((FlashInfo.SetInfo.FrozeDT[N]&0xFF)&&(TmBCD[2] == (FlashInfo.SetInfo.FrozeDT[N]>>8))){
	 if(TmBCD[3] == (FlashInfo.SetInfo.FrozeDT[N]&0xFF)){
	 if(TmBCD[4] < 5){
	 SetErrReturn(_ERR_OTHER_);
	 return;
	 }
	 } else if(BCD_Inc(TmBCD[3]) == (FlashInfo.SetInfo.FrozeDT[N]&0xFF)){
	 if(TmBCD[4] >= 0x55){
	 SetErrReturn(_ERR_OTHER_);
	 return;
	 }
	 }
	 }
	 }
	 MaxDiff = 5*60;
	 Diff = (Cnt - CntNow);
	 Diff = labs(Diff);
	 if(Diff > MaxDiff){												//校时超过5分钟禁止校时
	 SetErrReturn(_ERR_OTHER_);
	 return;
	 }	*/
#endif
	RamData.Write = 0;
	ChangeTime(Cnt);
	SetEBit(_BIT_DAY_CHG_); //设置当日校时标志
	RamData.DelayTmChg = 1;
	pData[1] = 0;
	SetTrxDisp();
}

u8 ReadWriteTime(strDispCode const *pCode, u8* pAddr) { //读写日期时间
	u8 BCDTm[6];
	u32 Cnt;
	u32 Rcd[3];
	u8 *pData;
	u8 D0;
	u32 Week;
	D0 = (u8) (pCode->Code);
	if (Ex645.WBit == 0) {
		if (D0 == 1) {
			MvData(BCDTm, _REV_ | OUT_TIME(S_RTC, T_YMDW)); //读取年月日周
			if (RamData.InDisp) {
				Copy_4(tpChar - 4, tpChar - 3);
				tpChar--;
			}
		} else if (D0 == 2) {
			MvData(BCDTm, _REV_ | OUT_TIME(S_RTC, T_HMS)); //读取时分秒
		} else {
			static const strMvData mvTime[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_RTC, T_HMS)), READ_PBASE( 0, _RET_|_REV_|OUT_TIME(S_RTC, T_YMDW)), };
			tpBase = BCDTm;
		MoveDataLst(mvTime);
		}
		return SUCCESS;
	}
	GetCntTime(BCDTm, RamData.RTCTime, S_RTC | T_SEC); //
	pData = Ex645.SCIx->DataBuf + 6;
	if (D0 == 1) { //
		RevCopyRam(BCDTm, pData + 1, 3);
		Week = pData[0];
	} else if (D0 == 2) { //
		RevCopyRam(BCDTm + 3, pData, 3);
	} else { //
		RevCopyRam(BCDTm, pData + 4, 3);
		RevCopyRam(BCDTm + 3, pData, 3);
		Week = pData[3];
	}
	Cnt = GetTimeCnt(BCDTm, 0);
	if (Cnt == 0) {
		return SetErrReturn(_ERR_OTHER_);
	}
	if ((RamData.FacProgTime == 0) && (D0 != 2) && (GetWeek(&Cnt) != Week)) {
		return SetErrReturn(_ERR_OTHER_); //
	}
	Rcd[0] = RamData.Write;
	Rcd[1] = RamData.RTCTime;
	ChangeTime(Cnt);
	SetEvtState(_EVT_CHGTIME_);
	Rcd[2] = RamData.RTCTime;
	Up_FrozeData((u8*) Rcd, _FROZE_CHG_TM_);
	RamData.DelayTmChg = 2;
	return SUCCESS;
}

u8 ReadWriteStartTm(strDispCode const *pCode, u8* pAddr) { //读写起始时间(年/月/日/时/分)

	u32 Cnt;
	u8 TmBCD[5];

	if (Ex645.WBit == 0) {
		MvData(pAddr, _REV_ | OUT_TIME(S_SEC, T_YMDHM));
		return SUCCESS;
	} else {
#ifdef		_HOUR_FROZE_0_30_
		if(((u16)(pCode->Code) == 0x1201)&&(Ex645.SCIx->DataBuf[10] != 0)&&(Ex645.SCIx->DataBuf[10] != 0x30)) {
			return SetErrReturn(_ERR_OTHER_); //整点冻结起始时间只能设整点或半点
		}
#endif
		Cnt = GetTimeCnt(Ex645.SCIx->DataBuf + 6, _REV_ | S_MIN | T_SEC);
		GetCntTime(TmBCD, Cnt, _REV_ | S_SEC | T_YMDHM);
		if (strCmp(TmBCD, Ex645.SCIx->DataBuf + 6, 5)) {
			return SetErrReturn(_ERR_OTHER_);
		}
		if (Chk_Mac_99()) {
			return FAIL;
		}
		Up_u32Data((u32*) pAddr, Cnt);
		if ((pAddr >= ((u8*) &FlashInfo.SafeInfo.StartTm_3)) && (pAddr
				<= ((u8*) &FlashInfo.SafeInfo.StartTm_4))) {
			TmBCD[0] = ((u8) (pAddr - ((u8*) &FlashInfo.SafeInfo.StartTm_1)))
					>> 2;
			ClrEBit(_BIT_YTB_STATE+TmBCD[0]);
		}
	}
	return SUCCESS;
}

u8 SetFacMode(strDispCode const *pCode, u8* pAddr) { //设置工厂模式
	u8 Bk;
	if (Ex645.WBit == 0) {
		NormCopy(pAddr, 4);
	} else {
		Bk = Ex645.SCIx->DataBuf[6];
		if ((RunState_EOpen == 0) &&(0) &&(Bk == 0)) {
			RamData.FacProgTime = 10;
		} else {
			if (Bk) {
				RamData.FacProgTime = 0;
				SetEBit(_BIT_FACOFF_);
			} else {
				RamData.FacProgTime = 1;
				SetEBit(0x80 | _BIT_FACOFF_);
			}
		}
	}
	return SUCCESS;
}

const u8 MaxFLMode[7] = { 14, //年时区数
		_TmTbl_StoreMaxNum_, //NUMOF(GAddr.pTmTbl[0]),										//日时段表数
		14, //日时段数
		4, //费率数
		_cpHld_StoreMaxNum,//NUMOF(GAddr.pHld),											//假日表数
		0, //谐波次数
		_MAX_LADDER_, //阶梯数
		};
static const u8 ErrMode[7] = { 0x10, 0x01, 0x20, 0x40, 0x01, 0x01, 0x01 };

u8 WriteFLMode(strDispCode const *pCode, u8* pAddr) { //写费率数/时段数....

	u8 D0;
	u8 Data;
	u16 Code;
	D0 = (u8) (pCode->Code - 1);
	if (D0 == 0x04) {
		Code = (u16) (BCD_Hex((u16) Ex645.FastData));
		if (Code > 254) {
			return SetErrReturn(ErrMode[D0]);
		}
		Data = Code;
	} else {
		Data = HEX((u8) Ex645.FastData);
	}
	if ((D0 <= 6) && (Data <= MaxFLMode[D0])) {
		Up_u8Data(&FlashInfo.SetInfo.FLMode[D0], Data);
		return SUCCESS;
	}
	return SetErrReturn(ErrMode[D0]);
}

u8 ReadSTD(strDispCode const *pCode, u8* pAddr) { //读电表参数信息

	static const u8 Tail[4] = { 'V', 'A', 'A', 0 };
	u8 Num;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt;

	ClrRam(Lst, sizeof(Lst));
	Num = (u8) (pCode->Code) - 4;
	if (Num < 3) {
		Data = (&FData.DftSet.V_Std)[Num];
		L = 6;
	} else {
		Data = FData.DftSet.Act_Grd;
		L = 4;
	}
	Hex_BCD(Data, B_Data);
	Data = *(u16*) (B_Data);
	N = 0;
	for (Cnt = 0; Cnt < 4; Cnt++) {
		k = (Data >> 12) & 0x0F;
		if (k || (N)) {
			if (Cnt == 3) {
				if (k == 0) {
					break;
				} else {
					Lst[N++] = '.';
				}
			}
			Lst[N++] = '0' + k;
		}
		Data <<= 4;
	}
	Lst[N++] = Tail[Num];
	RevCopy(Lst, L);
	return SUCCESS;
}

u8 ReadWriteActUseMode(strDispCode const *pCode, u8* pAddr) { //修改有功组合特征字

	u32 Tmp[_MAX_TARRIF_ + 1];
	u8 Cnt;
	u8 Mode;
	u16 Addr;
	if ((u8) Ex645.FastData > 0xF) {
		return SetErrReturn(_ERR_OTHER_);
	}
	Up_ProgFroze(_FROZE_COM_SET_, FlashInfo.SetInfo.ActUseMode,
			(((u8*) &Ex645.FastData)));
	SetEvtState(_EVT_COM_SET_);
	if ((u8) Ex645.FastData == FlashInfo.SetInfo.ActUseMode) {
		return SUCCESS;
	}
#ifdef			_COM_MODE_
	for( Cnt = 0; Cnt <= _MAX_TARRIF_; Cnt++) {
		Tmp[Cnt] = GetComUse(Cnt);
	}
#endif
	Mode = FlashInfo.SetInfo.ActUseMode;
	Up_u8Data(&FlashInfo.SetInfo.ActUseMode, (u8) Ex645.FastData);
#ifdef			_COM_MODE_
	for( Cnt = 0; Cnt <= _MAX_TARRIF_; Cnt++) {
		Ex645.FastData = GetComUse(Cnt);
		//Tmp[Cnt] = FlashInfo.RunInfo.ComUseDiff[Cnt] + Tmp[Cnt] - Data;
		tpData32 = &Tmp[Cnt];
		Add_u32((u32*)&FlashInfo.RunInfo.ComUseDiff[Cnt]);
		Sub_u32(&Ex645.FastData);
	}
	Copy_FlashInfo( (u8*)FlashInfo.RunInfo.ComUseDiff, (u8*)Tmp, sizeof(FlashInfo.RunInfo.ComUseDiff));
#endif
	return SUCCESS;
}

u8 WriteWeekTblNum(strDispCode const *pCode, u8* pAddr) { //修改周休日特征字

	u8 D0;
	D0 = (u8) (pCode->Code);
	if (D0 == 1) {
		if ((u8) Ex645.FastData > 0x7F) {
			return SetErrReturn(_ERR_OTHER_);
		}
	} else {
		if (((u8) Ex645.FastData == 0) || ((u8) Ex645.FastData > 8)) {
			return SetErrReturn(_ERR_OTHER_);
		}
	}
	Up_ProgFroze(_FROZE_WEEK_SET_, FlashInfo.SetInfo.WeekTblNum, RamData.Buf645);
	SetEvtState(_EVT_WEEKTBL_);
	Up_u8Data(pAddr, (u8) Ex645.FastData);
	return SUCCESS;
}

u8 WriteFMode(strDispCode const *pCode, u8* pAddr) { //冻结模式字

	static const u8 Mask[] = { ~0x0F, ~0x83, ~0x83, ~0x83, ~0x03, ~0x83 };
	if (((u8) Ex645.FastData) & Mask[((u8) (pCode->Code)) - 1]) {
		return SetErrReturn(_ERR_OTHER_);
	}
	Up_u8Data(pAddr, (u8) Ex645.FastData);
	return SUCCESS;
}

u8 WriteFrozeDT(strDispCode const *pCode, u8* pAddr) { //冻结日时

	if ((((u8) (Ex645.FastData) > 0x28) || ((u8) (Ex645.FastData >> 8) >= 0x24))
			&& (((u16) Ex645.FastData) != 0x9999)) {
		return SetErrReturn(_ERR_OTHER_);
	}
	if (((u8) (pCode->Code) == 1)
			&& (((u16) Ex645.FastData) != (*(u16*) pAddr))) {
		ChangeFDT1Froze();
	}
	Up_ProgFroze(_FROZE_FDT_SET_, (u8) (pCode->Code - 1), pAddr);
	Up_u16Data((u16*) pAddr, (u16) Ex645.FastData);
	SetRcdMin(&FlashInfo.RunInfo.LastMFrozeTm);
	SetEvtState(_EVT_FRZDT_);
	return SUCCESS;
}

u8 WriteRunType(strDispCode const *pCode, u8* pAddr) { //运行状态字
if ( _ISMETER_REMOTE()){ //#ifndef		_USE_CARD_
	if(Ex645.FastData&2) {
		return SetErrReturn(_ERR_OTHER_);
	}
} //#endif
if ( _ISJDMODE_IN()){ //#if		_JD_MODE_ == 1
	if (Ex645.FastData & 1) {
		return SetErrReturn(_ERR_OTHER_);
	}
}//#endif
	if (Ex645.FastData > 7) {
		return SetErrReturn(_ERR_OTHER_);
	}
	Up_u8Data(pAddr, (u8) Ex645.FastData);
	return SUCCESS;
}

u8 WriteHFrozeStep(strDispCode const *pCode, u8* pAddr) { //整点冻结时间间隔

	u8 Data;
	Data = HEX((u8) Ex645.FastData);
	if ((Data == 60) || (Data == 30) || (Data == 15)) {
		Up_u8Data(&FlashInfo.SetInfo.HourFrozeStep, Data);
		return SUCCESS;
	}
	return SetErrReturn(_ERR_OTHER_);
}

u8 WriteFrozeHM(strDispCode const *pCode, u8* pAddr) { //修改日冻结时间

	if (((u8) (Ex645.FastData >> 8) <= 0x23) && ((u8) Ex645.FastData <= 0x59)) {
		Up_u16Data(&FlashInfo.SetInfo.FrozeHM, (u16) Ex645.FastData);
		return SUCCESS;
	}
	return SetErrReturn(_ERR_OTHER_);
}

u8 GetHex(const u8 *pTm) {

	u8 D;
	D = (pTm[0] - '0') * 10 + (pTm[1] - '0');
	return D;
}

u8 GetMonth(const u8 *pTm) {

	static const u8 M[] = { 8, 7, 6, 2, 0x12, 0, 4, 0x11, 9, 1, 3, 0x10, 5 }; //"ihgcsaerjbdqf"
	u8 Data;
	Data = ((pTm[2] - 'a') + 14 - ((pTm[1] - 'a') & 0xFC)) / 3;
	return M[Data];
}

u8 ReadVer(strDispCode const *pCode, u8* pAddr) { //读取版本信息

#define	vDftSet		(*(strDftSet *)0)

	static const strMvData mvVer[] = { 
		READ_PBASE( (u16)(u32)(&vDftSet.AreaCode), 4), 
		READ_PBASE( (u16)(u32)vDftSet.VTime, _REV_|4), 
		READ_PBASE( (u16)(u32)&vDftSet.kWhImp, _REV_|BCD_OUT(2, 2, 0)), 
		READ_PBASE( (u16)(u32)&vDftSet.A_Std[1], _REV_|BCD_OUT(2, 1, 1)), 
		READ_PBASE( (u16)(u32)&vDftSet.A_Std[0], _RET_|_REV_|BCD_OUT(2, 1, 1)), 
	};
	u8* pTime;
	u8   stemp[16];
	tpBase = (u8*) (&FData.DftSet);
	MoveDataLst(mvVer);
	pTime = tpChar - 8;
	pTime[3] = BCD(GetHex(FData.DftSet.InterVersion + 18));
	pTime[2] = GetMonth(FData.DftSet.InterVersion + 9);
	if (0x20 == *(FData.DftSet.InterVersion + 13))
		stemp[0]= '0';
	else
		stemp[0]=*(FData.DftSet.InterVersion + 13);
	stemp[1]= *(FData.DftSet.InterVersion + 14);
	
	pTime[1] = (stemp[0]-'0')*16 + (stemp[1] -'0');
	pTime[0] = GetHex(FData.DftSet.InterVersion + 21) * 10 + GetHex(FData.DftSet.InterVersion + 24) / 6;
	return SUCCESS;
}

u8 WriteMSet(strDispCode const *pCode, u8* pAddr) { //读写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;
	static const u8
			LthLst[][2] = { { 0x2F, 57 }, { 0x3F, 122 }, { 0x5F, 32 }, };
	D0 = (u8) (pCode->Code);
	pData = Ex645.SCIx->DataBuf + 6;
	Cnt = 0;
	while (1) {
		if (LthLst[Cnt][0] == D0) {
			 	//WriteFData((u32)pAddr, pData, LthLst[Cnt][1]);
			break;
		}
		if (++Cnt >= NUMOF(LthLst)) {
			return SetErrReturn(_ERR_NODATA_);
		}
	}
	Ex645.SCIx->DataBuf[1] = 0;
	RamData.DelayRst = 5;
	return SUCCESS;
}

u8 WriteFlashSet(strDispCode const *pCode, u8* pAddr) { //读写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;
	static const u8 LthLst[][2] =
	{
			{ 0x6F, sizeof (struct STCHIPSET) },
			{ 0x7F,	sizeof(FlashInfo.RTCSet) },
	};
	D0 = (u8) (pCode->Code);
	pData = Ex645.SCIx->DataBuf + 6;
	
	if ( LthLst[0][0]== D0 )
	{
				Emu_Var_Write_io( (struct STCHIPSET *) pData);
				Ex645.SCIx->DataBuf[1] = 0;
				RamData.DelayRst = 3;
			return SUCCESS;
	}	
	Cnt = 0;
	
	while (1) {
		if (LthLst[Cnt][0] == D0) {
			Copy_FlashInfo(pAddr, pData, LthLst[Cnt][1]);
			break;
		}
		if (++Cnt >= NUMOF(LthLst)) {
			return SetErrReturn(_ERR_NODATA_);
		}
	}
	Ex645.SCIx->DataBuf[1] = 0;
	RamData.DelayRst = 5;
	return SUCCESS;
}

u8 WriteVolEffect(strDispCode const *pCode, u8* pAddr) { //读写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;

	D0 = (u8) (pCode->Code);
	pData = Ex645.SCIx->DataBuf + 6;
	Cnt = 0;
	 if( ((D0 == 0xCF)||(D0 == 0xDF)||(D0 == 0xEF)||(D0 == 0xDE)||(D0 == 0xCE)))
	 {

		Copy_FlashInfo( pAddr, pData,2);
	}
	Ex645.SCIx->DataBuf[1] = 0;

	return SUCCESS;
}

#ifdef	_USE_32768_
u8 ReadWriteRTCBias(strDispCode const *pCode, u8* pAddr) { //设置温度校正
	s16 Tmp;
	u8 Ret;
	if(Ex645.WBit) {
		Tmp = (s16)(BCD_Hex(((u16)Ex645.FastData)&0x7FFF));
		if(Ex645.FastData&0x8000) {
			Tmp = -Tmp;
		}
		Ret = SetRTCTrim(Tmp, (pCode ->Code&3));
		if(Ret) {
			return SetErrReturn(Ret);
		}
	} else if(RamData.InDisp == 0) {
		//	RevCopy( (u8*)&RamData.HCKCnt, 13);
		NormCopy( (u8*)FlashInfo.RTCSet, sizeof(FlashInfo.RTCSet));
		//	RevCopy( (u8*)&RTCData.LastCal, 2);
		//	RevCopy( (u8*)&RTCData.TmCnt, 1);
	}
	return SUCCESS;
}
#endif

u8 ReadIDTime(strDispCode const *pCode, u8* pAddr) { //读取身份认证时间

	//	u32	Data;
	Ex645.FastData = 59;
	if ((((u8) (pCode->Code)) & 1) == 0) {
		Ex645.FastData += RamData.IDACCess;
	} else if (RunState_IR) {
		Ex645.FastData += RamData.IRACCessTm;
	} else {
		Ex645.FastData = 0;
	}
	Up_BCDData(Ex645.FastData / 60, 0x24 - ((u8) (pCode->Code)));
	return SUCCESS;
}

ucode DS_Iz[] = { _DISP_N_, _DISP_N_ };
ucode DS_Freq[] = { _DISP_DIGIT_, 0xF7, _DISP_7_E_, _DISP_7_G_, _DISP_DOT_2_,
		_DISP_DOT_2_ };
ucode DS_Date[] = { _DISP_DANGQIAN_, _DISP_TM_SHI_, _DISP_TM_JIAN_,
		_DISP_DOT_2_, //日期
		_DISP_DOT_4_, _DISP_DOT_4_ };
ucode DS_Time[] = { _DISP_DANGQIAN_, _DISP_TM_SHI_, _DISP_TM_JIAN_,
		_DISP_DOT_2_, _DISP_DOT_4_, _DISP_DOT_2_2_, _DISP_DOT_4_2_,
		_DISP_DOT_4_2_ };
ucode DS_E_Num[] = { _DISP_BIAO_, _DISP_HAO_, _DISP_HAO_ };
ucode DS_H_Num[] = { _DISP_HU_, _DISP_HAO_, _DISP_HAO_ };

ucode DS_MaxCos[] = { _DISP_DOT_3_, _DISP_DOT_3_ };
ucode DS_Temperature[] = { _DISP_8_A_, _DISP_8_E_, _DISP_8_F_, _DISP_7_G_,
		_DISP_DOT_1_, _DISP_DOT_1_ }; //显示温度
ucode DS_Bat_V[] = { _DISP_V_, _DISP_DOT_2_, _DISP_DOT_2_ };
#define		DS_Dot_2	(DS_Bat_V+1)

ucode DS_RevUse[] = { _DISP_DANGQIAN_, _DISP_FAN_, _DISP_XIANG_, _DISP_XIANG_ }; //显示反向电量
ucode DS_SPrice[] = { _DISP_JIETI_, _DISP_DANGQIAN_, _DISP_DIAN_, _DISP_JIA_,
		_DISP_DOT_4_, _DISP_YUAN_, _DISP_YUAN_ }; //显示当前电价(阶梯电价)
ucode DS_LastMth[] = { _DISP_10_1_, _DISP_10_1_ }; //显示小数字1
ucode DS_MthComUse[] = { _DISP_DIGIT_, 0x81, _DISP_SHANG_, _DISP_YUE_,
		_DISP_YUE_ }; //显示上月组合电量
ucode DS_MthFwdUse[] = { _DISP_DIGIT_, 0x81, _DISP_SHANG_, _DISP_YUE_,
		_DISP_ZHENG_, _DISP_XIANG_, _DISP_XIANG_ }; //显示上月正向电量
ucode DS_MthRevUse[] = { _DISP_DIGIT_, 0x81, _DISP_SHANG_, _DISP_YUE_,
		_DISP_FAN_, _DISP_XIANG_, _DISP_XIANG_ }; //显示上月反向电量

ucode DS_FLPrice[] = { _DISP_DIAN_, _DISP_JIA_, _DISP_DOT_4_, _DISP_YUAN_,
		_DISP_YUAN_ }; //显示当前电价
ucode DS_Alm[] = { _DISP_DIGIT_, 0x01, _DISP_DIGIT_, 0x1F, _DISP_7_G_,
		_DISP_7_G_ };

ucode DS_DangQianJSUse[] = { _DISP_DANGQIAN_, _DISP_ZONG_, _DISP_YONG_,
		_DISP_YONG_ };
ucode
		DS_JSUse[] = { _DISP_DIGIT_, 0x81, _DISP_SHANG_, _DISP_YONG_,
				_DISP_YONG_ };

ucode DS_DangQianUse[] = { _DISP_DIGIT_, 0x81, _DISP_DANGQIAN_, _DISP_YUE_,
		_DISP_YONG_, _DISP_YONG_ };
ucode DS_MonthUse[] = { _DISP_DIGIT_, 0x81, _DISP_SHANG_, _DISP_YUE_,
		_DISP_YONG_, _DISP_YONG_ };

ucode DS_KwhImp[] = { _DISP_K_,_DISP_W_,_DISP_H_,_DISP_IMP_, _DISP_IMP_ };

ucode DS_FL_1[] = { _DISP_DIGIT_, 0x81, 0x81 };
ucode DS_FL_2[] = { _DISP_DIGIT_, 0x82, 0x82 };
ucode DS_10_1[] = { _DISP_10_1_, _DISP_10_1_ };

ucode DS_Err[] = { _DISP_DIGIT_, 0x2E, _DISP_5_E_, _DISP_5_G_, _DISP_4_E_,
		_DISP_4_G_, _DISP_3_G_, _DISP_3_G_ };

u8 WriteDisCharge(strDispCode const *pCode, u8* pAddr); //退费
u8 ReadCardState(strDispCode const *pCode, u8* pAddr); //读插卡状态

const strWriteCode Wx645Code[] = { // 0400XXXX
				//	  标识码  DI0, 安全级 {长度/方向/(H/B)/移位数/编程记录/费率表改变/读调用子程序/写调用子程序} 显示设置, 目标地址, 最大/最小值(子程序), 显示点阵
				{ 0x0101, 0, _SAFE_D_2_, { 4, 0, 3, 0, 1, 0, 1, 1 }, 0,
						(u8*) &RamData.RTCTime,(u32)ReadWriteTime, DS_Date,
						_bData_All_ }, //日期
				{ 0x0102, 0, _SAFE_D_2_, { 3, 0, 3, 0, 1, 1, 1, 1 }, 0,
						(u8*) &RamData.RTCTime, (u32)ReadWriteTime, DS_Time,
						_bData_All_ }, //时间
				{ 0x0105, 0, _SF_ROL_, { _DLTH_(2,1), 0, 0, 0, 0, 0, 0, 0 }, 0, (u8*) (&FileSet.X80),
						0, 0, _bData_All_ }, //脉冲宽度
				{ 0x0106, 7, _SAFE_D_2_, { 5, 0, 3, 2, 1, 1, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.StartTm_1, (u32)ReadWriteStartTm,
						0, _bData_All_ }, //读写起始时间
				//#ifdef	_USE_CARD_
				{ 0x0108, 0, _SAFE_D_1_, { 5, 0, 3, 2, 1, 1, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.StartTm_3, (u32)ReadWriteStartTm,
						0, _bData_Loc_ }, //读写起始时间
				{ 0x0109, 0, _SAFE_D_1_, { 5, 0, 3, 2, 1, 1, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.StartTm_4, (u32)ReadWriteStartTm,
						0, _bData_Loc_ }, //读写起始时间
				//#endif
				{ 0x010C, 0, _SAFE_D_2_, { 7, 0, 3, 0, 1, 1, 1, 1 },
						_DISP_NO_DISP_, (u8*) &RamData.RTCTime, (u32)ReadWriteTime,
						0, _bData_All_ }, //写日期时间
				{ 0x010F, 0, _SF_FAC_1_, { 1, 0, 3, 2, 0, 1, 1, 1 },
						_DISP_NO_DISP_, (u8*) &FlashInfo.RunInfo.EState,
						(u32)SetFacMode, 0, _bData_All_ }, //设置工厂模式

				{ 0x0201, 4, _SAFE_D_2_, { 1, 0, 1, 0, 1, 1, 0, 1 }, 0,
						FlashInfo.SetInfo.FLMode, (u32)WriteFLMode, 0, _bData_All_ }, //年时区数/日时段表数/日时段数/费率数
				{ 0x205, 0, _SAFE_D_2_, { _DLTH_(2,1), 0, 1, 0, 1, 1, 0, 1 }, 0,
						FlashInfo.SetInfo.FLMode + 4, (u32)WriteFLMode, 0,
						_bData_All_ }, //年时区数/日时段表数/日时段数/费率数
				//#ifdef		_USE_CARD_
				{ 0x0207, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 1, 0, 1 }, 0,
						FlashInfo.SetInfo.FLMode + 6, (u32)WriteFLMode, 0,
						_bData_Loc_ }, //阶梯数
				//#endif
				{ 0x0208, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.KeyNum, (u32)_MIN_MAX_(1, 32), 0, _bData_All_ }, //密钥总条数

				{ 0x0301, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.DispLength[0], (u32)_MIN_MAX_(1, 99), 0, _bData_All_ }, //自动循环显示屏数
				{ 0x0302, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.AutoDispTm, (u32)_MIN_MAX_(5, 20), 0, _bData_All_ }, //自动循环显示时间
				{ 0x0303, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.DispEnergyDots, (u32)_MIN_MAX_(0, 2), 0, _bData_All_ }, //显示电能小数位
				{ 0x0304, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.DispPowerDots, (u32)_MIN_MAX_(0, 4), 0, _bData_All_ }, //显示功率小数位
				{ 0x0305, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.DispLength[1], (u32)_MIN_MAX_(1, 99), 0, _bData_All_ }, //自动循环显示屏数
				{ 0x0306, 7, _SAFE_D_1_, { _DLTH_(3,4), 0, 1, 2, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SafeInfo.I_Mul, _MAX_1_(999999), 0, _bData_All_ }, //互感器变比
				{ 0x0308, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.DispAllTm, (u32)_MIN_MAX_(5, 30), 0, _bData_All_ }, //上电全显时间


				{ 0x0401, 0, _SAFE_D_2_, { 6, 1, 3, 0, 1, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.TRx_Num,(u32) ReadWriteTRxNum, 0,
						_bData_All_ }, //通讯地址
				{ 0x0402, 0, _SF99NCHG_, { 6, 1, 3, 0, 1, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.E_Num, (u32)ReadWriteTRxNum,
					(ucode	*)DS_E_Num, _bData_All_ },//表号
				{ 0x0403, 0, _SAFE_D_2_, { 32, 1, 2, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.F_Num, 0, 0, _bData_All_ }, //资产管理编码
				{ 0x0404, 7, _SF_ROL_, { 1, 0, 1, 0, 0, 0, _prog_set_, 0 },
						_DISP_NO_DISP_, (u8*) (&FData.DftSet.A_Std[0]),
						(u32)ReadSTD, 0, _bData_All_ }, //电压/电流规格
				{ 0x0409, 0, _SF_ROL_, { _DLTH_(3,2), 0, 1, 0, 0, 0, _prog_set_, 0 }, _DISP_LTH_2_,
						(u8*) (&FData.DftSet.kWhImp), (u32)ReadEEPROMFData,
						DS_KwhImp, _bData_All_ }, //脉冲常数
				{ 0x040B, 0, _SF_ROL_, { 10, 1, 2, 0, 0, 0, _prog_set_, 0 }, 0,
						(u8*) (&FData.DftSet.IMType), (u32)ReadEEPROMFData, 0,
						_bData_All_ }, //电表型号
				{ 0x040C, 0, _SF_ROL_, { 10, 1, 2, 0, 0, 0, _prog_set_, 0 }, 0,
						(u8*) (&FData.DftSet.MakeTime), (u32)ReadEEPROMFData, 0,
						_bData_All_ }, //生产日期
				{ 0x040D, 0, _SF_ROL_, { 16, 1, 2, 0, 0, 0, _prog_set_, 0, },
						0, (u8*) (&FData.DftSet.VerSion), (u32)ReadEEPROMFData, 0,
						_bData_All_ }, //协议版本
				//#ifdef		_USE_CARD_
				{ 0x040E, 0, _SF99NCHG_ , { 6, 1, 3, 0, 1, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.H_Num, (u32)ReadWriteTRxNum,
						DS_H_Num, _bData_All_ },//户号
				//#else
				//				{	0x040E, 0, _SF99NCHG_, {6, 1, 3, 0, 1, 0, 1, 1}, 0, (u8*)&FlashInfo.SafeInfo.H_Num, ReadWriteTRxNum, DS_H_Num},//户号
				//#endif
				{ 0x040F, 0, _SAFE_D_2_, { 11, 0, 2, 0, 1, 0, 0, 0 }, 0,
						FlashInfo.SetInfo.GPSCode, 0, 0, _bData_All_ }, //位置信息

				{ 0x0501, 7, _SF_ROL_, { 2, 0, 0, 1, 0, 0, 0, 0 }, 0,
						(u8*) &RamData.RunState[0], 0, 0, _bData_All_ }, //状态字
				{ 0x0508, 0, _SF_ROL_, { 4, 0, 0, 1, 0, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.EKeyState, 0, 0, _bData_All_ }, //密钥状态字

				{ 0x0601, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 1, 0, 1 }, 0,
						&FlashInfo.SetInfo.ActUseMode, (u32)ReadWriteActUseMode, 0,
						_bData_All_ }, //有功/组合特征字

				{ 0x0701, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 0, 0, 1 }, 0,
						FlashInfo.SetInfo.Baud, (u32)WriteBaud, 0, _bData_All_ }, //红外波特率特征
				{ 0x0703, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 0, 0, 1 }, 0,
						FlashInfo.SetInfo.Baud + 1, (u32)WriteBaud, 0, _bData_All_ }, //通讯口特率特征
				{ 0x0705, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 0, 0, 1 }, 0,
						FlashInfo.SetInfo.Baud + 2, (u32)WriteBaud, 0, _bData_All_ }, //通讯口特率特征

				{ 0x0801, 0, _SAFE_D_2_, { 1, 0, 0, 0, 1, 1, 0, 1 }, 0,
						&FlashInfo.SetInfo.WeekMode, (u32)WriteWeekTblNum, 0,
						_bData_All_ }, //周休表
				{ 0x0802, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 1, 0, 1 }, 0,
						&FlashInfo.SetInfo.WeekTblNum, (u32)WriteWeekTblNum, 0,
						_bData_All_ }, //周休表费率表号

				{ 0x0901, 6, _SAFE_D_2_, { 1, 0, 0, 0, 1, 0, 0, 1 }, 0,
						&FlashInfo.SetInfo.FHMode, (u32)WriteFMode, 0, _bData_All_ }, //负荷记录模式字/冻结模式字

				{ 0x0A01, 0, _SAFE_D_2_, { 4, 0, 3, 0, 1, 0, 0, 1 }, 0,
						(u8*) &FlashInfo.SetInfo.FuheStartTm, (u32)WriteFHStartTm,
						0, _bData_All_ },//负荷记录起始时间

				{ 0x0A02, 5, _SAFE_D_2_, { _DLTH_(2,1), 0, 1, 0, 1, 0, 0, 0 }, 0,
						&FlashInfo.SetInfo.FHTmStep[0], (u32)_MIN_MAX_(1, 60), 0, _bData_All_ }, //负荷记录间隔时间

				{ 0x0B01, 3, _SAFE_D_2_, { 2, 1, 3, 1, 1, 0, 0, 1 }, 0,
						(u8*) FlashInfo.SetInfo.FrozeDT, (u32)WriteFrozeDT,
						DS_Dot_2, _bData_All_ }, //冻结日时
				{ 0x0C01, 5, _SAFE_D_2_, { 4, 0, 0, 0, 1, 1, 1, 1 },
						_DISP_NO_DISP_, (u8*) FlashInfo.SetInfo.TRxKey[0],
						(u32)RxWriteKey, 0, _bData_All_ }, //修改密码
#ifdef		_USE_CARD_
				{ 0x1001, 2, _SAFE_D_1_, { 4, 0, 1, 2, 1, 0, 0, 0 },
						(_USE_UINT_ | _DISP_MIN_BITS_),
						(u8*) &FlashInfo.SafeInfo.Alm1, _MAX_0_(99999999), DS_Alm, _bData_Loc_ }, //报警金额1/2
				{ 0x1003, 5, _SAFE_D_2_, { 4, 0, 1, 2, 1, 0, 0, 0 },
						(_USE_UINT_ | _DISP_MIN_BITS_),
						(u8*) &FlashInfo.SetInfo.OverDraftLimit, _MAX_0_(99999999), 0, _bData_Loc_ }, //透支限额/囤积限额/合闸允许限额
				{ 0x1006, 0, _SAFE98_, { 4, 0, 0, 2, 1, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.SafeInfo.DisCharge, WriteDisCharge, 0,
						_bData_Loc_ }, //退费金额
#endif

				{ 0x1101, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 1 }, 0,
						(u8*) &FlashInfo.SetInfo.RunType, (u32)WriteRunType, 0,
						_bData_All_ }, //运行特征字
				{ 0x1104, 0, _SAFE_D_2_, { 8, 0, 2, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.EvtMode, 0, 0, _bData_All_ }, //主动上报模式字

				{ 0x1201, 0, _SAFE_D_2_, { 5, 0, 3, 0, 1, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.SetInfo.HourFrozeStartTm,
						(u32)ReadWriteStartTm, 0, _bData_All_ },//读写整点冻结起始时间
				{ 0x1202, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 1 }, 0,
						&FlashInfo.SetInfo.HourFrozeStep, (u32)WriteHFrozeStep, 0,
						_bData_All_ }, //写整点冻结时间间隔
				{ 0x1203, 0, _SAFE_D_2_, { 2, 0, 3, 0, 1, 0, 0, 1 }, 0,
						(u8*) &FlashInfo.SetInfo.FrozeHM, (u32)WriteFrozeHM,
						DS_Dot_2, _bData_All_ }, //写日冻结时间
				{ 0x1204, 0, _SF_ROL_, { 4, 1, 0, 0, 0, 0, 0, 0 }, 0,
						FlashInfo.SetInfo.FrozeCode, 0, 0, _bData_All_ }, //定时冻结代码

				{ 0x1401, 0, _SAFE_D_2_, { 2, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.JDOffDelayTm, _MAX_0_(9999), 0, _bData_All_ }, //继电器跳闸延时
				{ 0x1402, 0, _SAFE_D_2_, { _DLTH_(3,4), 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.JDWaitAM, _MAX_0_(999999), 0, _bData_All_ }, //继电器跳闸电流
				{ 0x1403, 0, _SAFE_D_2_, { 2, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.AMOffDelayTm, _MAX_0_(9999), 0, _bData_All_ }, //继电器过流跳闸延时
				{ 0x1404, 5, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.IRAccessTmSet, _MAX_0_(99), 0, _bData_All_ }, //红外认证时效/主动上报状态字复位时效

	//			{ 0x1501, 0, _SF_ROL_, { 2, 0, 2, 1, 0, 0, 1, 0 }, 0,
		//				(u8*) &FlashInfo.RunInfo.EvtCnt, (u32)ReadEvtState, 0,
			//			_bData_All_ }, //主动上报状态字
#ifdef	_USE_CARD_
				{ 0x1502, 0, _SF_ROL_, { 2, 0, 0, 0, 0, 0, 1, 0 }, 0,
						(u8*) &RamData.CardInState, ReadCardState, 0,
						_bData_Loc_ }, //插卡状态字
#endif
//				{ 0x1503, 0, _SFNONE_, { 12, 0, 2, 0, 0, 0, 1, 1 }, 0,
//						(u8*) &FlashInfo.RunInfo.EvtCnt, (u32)ReadEvtState, 0,
//						_bData_All_ }, //清除主动上报状态字
						
                      
                      
		
				// { 0xF11F,	0,	_SF_ROL_, 	{ 4, 1, 0, 0, 0, 0, 1, 0 }, 0,				(u8*)&RamData.LVSave[0].SumUse.Use, ReadVer},				//读版本信息
				{ 0xF11F, 0, _SF_ROL_, { 4, 1, 0, 0, 0, 0, 1, 0 }, 0,
						(u8*) &RamData.stdianlian[0].LVSave.SumUse.Use,
						(u32)ReadVer, 0, _bData_All_ }, //读版本信息
				{ 0xF12F, 0, _SF_FAC_0_, { 57, 1, 2, 0, 0, 0, 0, 1 }, 0,
						(u8*) (&FData.DftSet), (u32)WriteMSet, 0, _bData_All_ }, //读设置参数
				{ 0xF13F, 0, _SF_FAC_0_, { 122, 1, 2, 0, 0, 0, 0, 1 }, 0,
						(u8*) (&FData.DftSet.SoftVer), (u32)WriteMSet, 0,
						_bData_All_ }, //读设置参数
#ifdef	_USE_32768_
				//PXOPT { 0xF14D,0x4F,	_SF_FAC_0_,	{ 2, 0, 0, 1, 0, 0, 1, 1 },	0,				(u8*)&FlashInfo.RTCSet, ReadWriteRTCBias },					//读写温度校正
#endif
				{ 0xF15F, 0, _SF_FAC_0_, { 32, 1, 2, 0, 0, 0, 0, 1 }, 0,
						(u8*) (&FData.DftSet.MSet), (u32)WriteMSet, 0, _bData_All_ }, //读设置参数


			//	{ 0xF16E, 0, _SF_FAC_0_, { sizeof(RamData.stjlxb.cXbConData),
				//		1, 2, 0, 0, 0, 0, 0 }, 0,
					//	(u8*) &RamData.stjlxb.cXbConData, WriteMSet, 0,
				//		_bData_All_ }, // 校表参数回零

				{ 0xF16F, 0, _SF_FAC_0_, { sizeof (struct STCHIPSET), 1, 2, 0, 0, 0, 0, 1 },
						0, (char *)&stChipSet, (u32)WriteFlashSet, 0, _bData_All_ }, //	读写校表参数

				{ 0xF17F, 0, _SF_FAC_0_, { sizeof(FlashInfo.RTCSet), 1, 2, 0,
						0, 0, 0, 1 }, 0, (u8*) &FlashInfo.RTCSet,
						(u32)WriteFlashSet, 0, _bData_All_ }, //读写时钟校正参数
		//		{ 0xF18F, 0, _SF_FAC_0_, { sizeof(FlashInfo.cRTCOFFSET), 1, 0,
		//				0, 0, 0, 1, 1 }, 0, (u8*) &FlashInfo.cRTCOFFSET,
		//				RTCOFFSETREAD, 0, _bData_All_ }, //读写时钟校正参数
						
		//		{ 0xF18C, 0, _SF_FAC_0_, { sizeof(FlashInfo.sRTCTrim), 1, 2, 0,
			//				0, 0, 1, 1 }, 0, WriteRTCSet,WriteRTCSet,0, _bData_All_  }, //读写时钟校正参数
						
			//	{ 0xF18D, 0, _SF_FAC_0_, { 1, 1, 0, 0,
			//				0, 0, 1, 1 }, 0, (u8*) &FlashInfo.iOSC16_OFFSET,RTC_Trim_Enable, 0, _bData_All_  }, //读写时钟校正参数
				
		//		{ 0xF18E, 0, _SF_FAC_0_, { 2, 1, 0, 0,
		//					0, 0, 1, 1 }, 0, (u8*) &FlashInfo.iOSC16_OFFSET,RTC_DIV_READ , 0, _bData_All_}, //读写时钟校正参数
				
			//	{ 0xF18F, 0, _SF_FAC_0_, { sizeof(FlashInfo.iOSC16_OFFSET), 1, 0, 0,
			//				0, 0, 1, 1 }, 0, (u8*) &FlashInfo.iOSC16_OFFSET,RTCOFFSETREAD , 0, _bData_All_}, //读写时钟校正参数
			//	{ 0xF19F, 0, _SF_FAC_0_, { 1, 1, 0, 0,
		//					0, 0, 1, 1 }, 0, (u8*) &FlashInfo.iOSC16_OFFSET,RTCOFFSEINIT, 0, _bData_All_ }, //读写时钟校正参数
				{ 0xF1AF, 0, _SF_ROL_, { 4, 0, 0, 0, 0, 0, 0, 0 }, 0,
						(u8*) &RamData.TestData, 0, 0, _bData_All_ }, //读测试数据
				
						
			//	{ 0xF1CE, 0, _SF_FAC_0_, { 2, 0, 2, 0, 0, 0, 0, 1 },
			//			0,
			//			(u8 *) &ivol_lowoffset80, WriteVolEffect, 0,
			//			_bData_All_ }, //读写低电压OFFSET校表参数
		//		{ 0xF1CF, 0, _SF_FAC_0_, { 2, 0, 2, 0, 0, 0, 0, 1 },
			//			0,
			//			(u8 *) &ivol_lowoffset, WriteVolEffect, 0, _bData_All_ }, //读写低电压OFFSET校表参数
			//	{ 0xF1DE, 0, _SF_FAC_0_, { 2, 0, 2, 0, 0, 0, 0, 1 },
			//			0,
						//(u8 *) &ivol_highoffset115, WriteVolEffect, 0,
						//_bData_All_ }, //读写高电压OFFSET校表参数
				//{ 0xF1DF, 0, _SF_FAC_0_, { 2, 0, 2, 0, 0, 0, 0, 1 },
						//0,
						//(u8 *) &ivol_highoffset, _P_PROG_(WriteVolEffect), 0, _bData_All_ }, //读写高电压OFFSET校表参数
				//{ 0xF1EF, 0, _SF_FAC_0_, { 2, 0, 2, 0, 0, 0, 0, 1 },
					//	0,
//						(u8 *) &ivol_offset_en, _P_PROG_(WriteVolEffect), 0, _bData_All_ }, //读写高电压OFFSET校表参数
				{ 0xF301, 0, _SF_FAC_0_, { 1, 1, 2, 0, 0, 0, 0, 1 }, 0,
						(u8*) (&FData.DftSet.MSet), (u32)WriteEXTEEPROM, 0,
						_bData_All_ }, //读设置参数  //
				{ 0xF302, 0, _SF_FAC_0_, { 6, 1, 2, 0, 0, 0, 1, 1 }, 0,
					(u8*)ReadFlashData, (u32)ReadFlashData, 0, _bData_All_ }, //读内部ＦＬＡＳＨ
				{ 0xF303, 0, _SF_FAC_0_, { 6, 1, 2, 0, 0, 0, 1, 1 }, 0,
						(u8*) &FlashInfo.RTCSet,(u32) ReadExtEEprom, 0, _bData_All_ }, //读外部外部
#ifdef _DEBUG_
				{	0xF304, 0, _SF_FAC_0_, {2, 0, 1, 0, 1, 0, 0, 0}, 0, (u8*)&RamData.LVD_Volt, _MAX_0_(9999), DS_MaxCos}, //LVD点电压
#endif
				{ 0xF305, 0, _SF_FAC_0_, { _METERCONFIG_LEN_, 1, 2, 0, 0, 0, 0,
						1 }, 0, (u8*) &FlashInfo.meterinfo, (u32)Write_meterconfig,
						0, _bData_All_ }, //读外部外部
				
						 { 0xF310,0xfe , _SF_FAC_0_, { 4, 1, 2, 0, 0, 0, 1, 1 },
						0, (u8 *)(&(FlashInfo.RTCSet)) , _P_PROG_(Jlrestore), 0, _bData_All_ }, //	??????
						 
						 { 0xF311,0xfe , _SF_FAC_0_, { 4, 1, 2, 0, 0, 0, 1, 1 },
						0, (u8 *)(&(FlashInfo.RTCSet)) , _P_PROG_(effective_value), 0, _bData_All_ }, //	??????
						 
				 			 
				    { 0xF400,0xfe , _SF_FAC_0_, { 4, 1, 2, 0, 0, 0, 1, 1 },
						0, (u8 *)(&(FlashInfo.RTCSet)) , _P_PROG_(Jlregread), 0, _bData_All_ }, //	??????
								
            { 0xF500,0xfe , _SF_FAC_0_, { 4, 1, 2, 0, 0, 0, 1, 1 },
						0, (u8 *)(&(FlashInfo.RTCSet)) , _P_PROG_(Jbregread), 0, _bData_All_ }, //	??????

						{ 0xF600,0xfe , _SF_FAC_0_, { 4, 1, 2, 0, 0, 0, 1, 1 },
						0, (u8 *)(&(FlashInfo.RTCSet)) , _P_PROG_(Jbregread), 0, _bData_All_ }, //	??????
						
				 

		};

const strWriteCode Rx645_0280[] = { // 0280XXXX
		//	  标识码  DI0, 安全级 {长度/方向/(H/B)/移位数/编程记录/费率表改变/读调用子程序/写调用子程序} 显示设置, 目标地址, 最大/最小值(子程序), 显示点阵
		//		{ 0x0001, 0, _SF_ROL_, { _DLTH_(3,4), 0, 1, 0, 0, 0, 0, 0 }, _A_,
			//			(u8*) &RamData.I_z.sVI, 0, DS_Iz, _bData_All_ }, //零线电流
				{ 0x0002, 0, _SF_ROL_, { 2, 0, 1, 0, 0, 0, 0, 0 }, 0,
						(u8*) &RamData.Freq, 0, DS_Freq, _bData_All_ }, //线频率

				{ 0x0007, 0, _SF_ROL_, { 2, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_LTH_2_, (u8*) &RamData.Temperature, 0,
						DS_Temperature, _bData_All_ }, //温度
				{ 0x0008, 0, _SF_ROL_, { 2, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_LTH_3_, (u8*) &RamData.VBat[0], 0, DS_Bat_V,
						_bData_All_ }, //电池电压
				{ 0x000A, 0, _SF_ROL_, { 4, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_NO_DISP_, (u8*) &FlashInfo.RunInfo.BatUseTime, 0,
						0, _bData_All_ }, //电池使用时间
#ifdef		_USE_CARD_
				{ 0x000B, 0, _SF_ROL_, { 4, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_LTH_5_, (u8*) &FlashInfo.RunInfo.StepPrice, 0,
						DS_SPrice, _bData_Loc_ }, //当前阶梯电价
				{ 0x0020, 0, _SF_ROL_, { 4, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_LTH_5_, (u8*) &FlashInfo.RunInfo.ePrice, 0,
						DS_SPrice + 1, _bData_Loc_ }, //当前电价
				{ 0x0021, 0, _SF_ROL_, { 4, 0, 1, 0, 0, 0, 0, 0 },
						_DISP_LTH_5_, (u8*) &FlashInfo.RunInfo.FLPrice, 0,
						DS_SPrice + 1, _bData_Loc_ }, //当费率前电价
#endif
				{ 0x0022, 0x23, _SF_ROL_, { 2, 0, 1, 0, 0, 0, 1, 0 }, 0,
						(u8*) &RamData.IDACCess, (u32)ReadIDTime, 0, _bData_All_ }, //身份认证剩余时间
		};

const strWriteCode Wx645LmtCode[] = { // 0409XXXX
		//	  标识码  DI0, 安全级 {长度/方向/(H/B)/移位数/编程记录/费率表改变/读调用子程序/写调用子程序} 显示设置, 目标地址, 最大/最小值(子程序), 显示点阵
				{ 0x0801, 0, _SAFE_D_2_, { 2, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.OverIphLimit, _MAX_0_(1000), 0, _bData_All_ }, //过流下限
				{ 0x0802, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.OverIphTm, (u32)_MIN_MAX_(10, 99), 0, _bData_All_ }, //过流延时
				{ 0x1001, 0, _SAFE_D_2_, { _DLTH_(3,4), 0, 1, 0, 1, 0, 0, 0 }, _KW_,
						(u8*) &FlashInfo.SetInfo.RevPwrLimit, _MAX_0_(999999), 0, _bData_All_ }, //功率反向触发下限
				{ 0x1002, 0, _SAFE_D_2_, { 1, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.RevPwrTm, _MAX_0_(99), 0, _bData_All_ }, //功率反向判定延时延时

				{ 0xF101, 0, _SAFE_D_2_, { 2, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.SetInfo.BatLoLmt, _MAX_0_(400), DS_MaxCos, _bData_All_ }, //电池电压下限
				{ 0xF10A, 0, _SAFE_D_2_, { 4, 0, 1, 0, 1, 0, 0, 0 }, 0,
						(u8*) &FlashInfo.RunInfo.BatUseTime, _MAX_0_(9999), DS_MaxCos, _bData_All_ }, //电池使用时间

		};

void FillNullData(u8 *pAddr, u8 Lth) { //填充数据

	if (RamData.Read_F1) {
		if (pAddr) {
			SetpCharRam(Lth);
		} else {
			ClrpCharRam(Lth);
		}
	}
}

void MvTimeYMDHMS(u8* Addr) { //输出年月日时分秒
	MvData(Addr, _REV_ | OUT_TIME(S_SEC, T_YMDHMS));
}

void OutYTBL(u16 Data) { //输出年时区表

	SetpCharInc(Data & 0x0F);
	Data >>= 4;
	SetpCharInc(BCD(Data & 0x1F));
	Data >>= 5;
	SetpCharInc(BCD(Data));
}

void OutTTBL(u16 Data) { //输出日时段表

	SetpCharInc(BCD(Data & 0x1F));
	Data >>= 5;
	SetpCharInc(BCD(Data % 60));
	SetpCharInc(BCD(Data / 60));
}

void OutHTBL(u8 *pAddr) { //输出假日表

	MvData(pAddr, _REV_ | OUT_TIME(S_DAY, T_YMDW));
	*(tpChar - 4) = GetAddrData((u32) (pAddr + 2));
}

void OutLTBL(u8 *p) { //阶梯表记录转换

	N_MvData(p, _REV_ | BCD_OUT( 4, 4, 0), ((_MAX_LADDER_) | _LTH_4_));
	N_MvData(p + 24, _REV_ | BCD_OUT( 3, 4, 0), ((_MAX_LADDER_ + 1) | _LTH_3_));
	N_MvData(p + 24 + 21, REV_COPY_L(3), 4 | _LTH_3_);

}

u8 SetDBit5(u8 Frm) { //设置后续帧标志
	if (Ex645.Frm645Read < Frm) {
		Ex645.SCIx->DataBuf[0] |= 0x20;
	} else if (Ex645.Frm645Read > Frm) {
		Ex645.Frm645 = Frm;
		return FAIL;
	}
	return SUCCESS;
}

void SetAAInc(void) { //根据条件填充AA
	if (RamData.Read_F1) {
		SetpCharInc(0xAA);
	}
}

void GetLRcdLst(u8* pAct, u8 FType, u8 FCnt, u8 Start, u8 Lth) { //搜索更改项

	u8 Cnt;
	u8 K;
	u8* Addr;
	ClrRam(pAct, Lth);
	for (Cnt = 0; Cnt < FCnt; Cnt++) {
		Addr = GetFrozeStart(FType, Cnt);
		if (Addr == NULL) {
			return;
		}
		K = Addr[8];
		if ((K >= Start) && (K < (Start + Lth))) {
			pAct[K - Start] = Cnt + 1;
		}
	}
}

u8 ChkPhBC(u8 Ph) { //填充B相C相数据
	if ((RamData.InDisp == 0) && (RamData.Read_F1) && ((Ph == 2) || (Ph == 3))) {
		return SUCCESS;
	}
	return FAIL;
}

void RxProgRcd(strDispCode const *pCode) { //读编程记录

	static const strMvData mvRxRcd_1[] = {
	//FILL_RAM( 0xFF, 6),
			READ_PBASE( 8, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 0, _RET_|8), };
	static const strMvData mvRxRcd[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 4, 22), READ_PBASE( 4+22, _RET_|22), };
	u16 Cnt;
	u8 Cnt1;
	u16 Addr;
	u32 Writer;
	u8 cd;
	cd = (u8) (pCode->Code);
	if (cd == 0) {
		Cnt = GetFrozeCnt(_FROZE_PROG_);
#ifdef		_PROG_CNT_INC_
		if (FlashInfo.RunInfo.EState & _ESTATE_IN_PROG_) {
			Cnt++;
		}
#endif
		Up_BCDData(Cnt, 3);
		return;
	} else if (cd > 10) {//((strFrozeType *)(GetFileSetAddr(FileSet.FrozeSet.FrozeBias)))[_FROZE_PROG_].MinCnt){
		return;
	}
	cd--;
#ifdef		_PROG_CNT_INC_
	if (FlashInfo.RunInfo.EState & _ESTATE_IN_PROG_) {
		if (cd == 0) {
			RamData.LastFType = 0;
			GetNewProgRcd(RamData.FrozeBuf);
			tpBase = RamData.FrozeBuf;
		} else {
			cd--;
			tpBase = (u8*) GetFrozeStart(_FROZE_PROG_, cd);
		}
	} else {
		tpBase = (u8*) GetFrozeStart(_FROZE_PROG_, cd);
	}
#else
	tpBase = (u8*)GetFrozeStart(_FROZE_PROG_, cd);
#endif
	if (tpBase == NULL) {
		ClrpCharRam(50);
	} else {
		MoveDataLst(mvRxRcd);
	}
}

void RxOverIphRecord(strDispCode const *pCode) { //读过流记录

	u8 Cnt;
	u8 D1;
	u8 Over;
	u8* Addr;
	D1 = (u8) (pCode->Code >> 8);
	Cnt = (u8) (pCode->Code - 1);
	if (D1 <= 0x20) {
		Addr = (u8*) GetFrozeStart(_FROZE_OVERIM_START_, Cnt);
		Over = 0;
	} else {
		Addr = (u8*) GetFrozeStart(_FROZE_OVERIM_END_, Cnt);
		D1 -= 0x20;
		Over = 1;
	}
	if (D1 == 1) {
		MvTimeYMDHMS(Addr);
		//MvData(Addr, _REV_|OUT_TIME(S_SEC, T_YMDHMS));
	} else if ((D1 <= 3) || ((D1 >= 6) && (D1 <= 7))) {
		MvData((Addr + 4 + (D1 & 1) * 4), _REV_ | BCD_OUT( 4, 4, 0));
	} else if (Over == 1) {
		FillNullData(Addr, 4);
	} else if (D1 == 0x0A) {
		MvData((Addr + 12), _REV_ | BCD_OUT(2, 2, 0));
	} else if ((D1 >= 0x0B) && (D1 <= 0x0C)) {
		MvData((Addr + 14 + ((D1 - 0x0B) * 3)), _REV_ | BCD_OUT(3, 3, 0));
	} else if (D1 == 0x0E) {
		MvData((Addr + 20), _REV_ | BCD_OUT(2, 2, 0));
	} else if (D1 < 0x0A) {
		FillNullData(Addr, 4);
	} else {
		D1 = (D1 - 0x0A) % 9;
		if ((D1 == 0) || (D1 == 4)) {
			FillNullData(Addr, 2);
		} else if ((D1 >= 1) && (D1 <= 3)) {
			FillNullData(Addr, 3);
		} else {
			FillNullData(Addr, 4);
		}
	}
}

void RxJDRecord(strDispCode const *pCode) { //读拉合闸记录

	u8 D1;
	D1 = (u8) (pCode->Code >> 8);
	if (tpBase == NULL) {
		if (D1 == 1) {
			ClrpCharRam(6);
		} else {
			ClrpCharRam(4);
		}
	} else if (D1 == 1) {
		MvTimeYMDHMS(tpBase);
		//MvData( tpBase, _REV_|OUT_TIME(S_SEC, T_YMDHMS));
	} else if (D1 == 2) {
		NormCopy((tpBase + 4), 4);
	} else if (D1 <= 4) {
		MvData(tpBase + ((D1 - 1) * 4), _REV_ | BCD_OUT( 4, 4, 0));
	} else if (D1 <= 8) {
		SetpCharRam(4);
	}
}

void RxPwrOffRcd(strDispCode const *pCode, u8 FType) { //读掉电记录

	u8 *Addr;
	MvTimeYMDHMS(tpBase);
	Addr = (u8*) GetFrozeStart(((FType | 0x80) & (~0x40)), (u8) (pCode->Code
			- 1));
	MvData(Addr, _REV_ | OUT_TIME(S_SEC, T_YMDHMS));
	return;
}

void RxClrUseRcd(strDispCode const *pCode, u8 FType) { //读清零记录
	static const strMvData mvClrRecord[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), //
			READ_PBASE( 4, COPY_L(4)), //
			READ_PBASE( 8, _REV_|BCD_OUT(4, 4, 0)), //
			READ_PBASE( 12 , _REV_|BCD_OUT(4, 4, 0)), //
			FILL_RAM( 0xFF, 16), //
			READ_PBASE( 8, _REV_|BCD_OUT(4, 4, 0)), //
			READ_PBASE( 12 , _REV_|BCD_OUT(4, 4, 0)), //
			FILL_RAM( 0xFF, _RET_|64), //
			};
	MoveDataLst(mvClrRecord);
}

void RxClrEventRcd(strDispCode const *pCode, u8 FType) { //读取事件清零记录
	static const strMvData mvClrEvtRecord[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 4, _RET_|COPY_L(8)), };
	MoveDataLst(mvClrEvtRecord);
}

void RxTimeRcd(strDispCode const *pCode, u8 FType) { //读取修改时间记录
	static const strMvData mvTmChgRecord[] = { READ_PBASE( 0, COPY_L(4)), READ_PBASE( 4 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 8 , _RET_|_REV_|OUT_TIME(S_SEC, T_YMDHMS)), };
	MoveDataLst(mvTmChgRecord);
}

static const strMvData mvTmWriteCode[] = { //生成记录时间及操作者代码
		READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 4, _RET_|COPY_L(4)), };
void RxTmTblSetRcd(strDispCode const *pCode, u8 FType) { //日时段表编程记录

	u8 Cnt;
	u8 T1;
	u16 Data;
	u8 ActTbl[2];
	u8* pFroze;
	unsigned char sTmTbl[_TmTbl_Eeprom_First_Len]; //日时段表(2*8*28B)

	if (SetDBit5(7)) { //后续帧处理
		return;
	}
	Ex645.Frm645 = Ex645.Frm645Read;
	if (tpBase == NULL) {
		if (Ex645.Frm645Read == 0) {
			ClrpCharRam(94);
		} else {
			ClrpCharRam(84);
		}
		return;
	}
	if (Ex645.Frm645Read == 0) {
		MoveDataLst(mvTmWriteCode);
	}
	GetLRcdLst(ActTbl, _FROZE_TMTBL_SET_, (u8) (pCode->Code), Ex645.Frm645 * 2,
			2); //检测更改项
	for (Cnt = 0; Cnt < 2; Cnt++) {
		if (ActTbl[Cnt]) {
			pFroze = GetFrozeStart(_FROZE_TMTBL_SET_, (ActTbl[Cnt] - 1)) + 9;
		} else {
			//pFroze = (u8*)(GAddr.pTmTbl[(Ex645.Frm645>>2)&1][((Ex645.Frm645<<1)+Cnt)&7]);
			CopyExteeprom((u8*)sTmTbl, ((int)&(((struct ST24256_DATACONFIG *)0)->TmTbl[(Ex645.Frm645>>2)&1][((Ex645.Frm645<<1)+Cnt)&7])),sizeof(sTmTbl));
			pFroze = sTmTbl;
		}
		CopyRam(RamData.Buf645, pFroze, 28);
		for (T1 = 0; T1 < 14; T1++) {
			OutTTBL(((u16*) RamData.Buf645)[T1]);
		}
	}
}

void RxYTblSetRcd(strDispCode const *pCode, u8 FType) { //时区表编程记录

	u8 T1;
	u8 Cnt;
	u16 Data;
	u8 ActTbl[2];
	u8 YearTbl[_YearTbl_Eeprom_First_Len]; //年时区表(2*28)u8
	u8* pFroze;
	GetLRcdLst(ActTbl, _FROZE_YTBL_SET_, (u8) (pCode->Code), 0, 2);
	MoveDataLst(mvTmWriteCode);
	for (Cnt = 0; Cnt < 2; Cnt++) {
		if (ActTbl[Cnt]) {
			pFroze = GetFrozeStart(_FROZE_YTBL_SET_, (ActTbl[Cnt] - 1)) + 9;
		} else {
			//pFroze = (u8*)GAddr.pYTbl[Cnt];
			CopyExteeprom((unsigned char *)YearTbl,_Year_GetT(Cnt),sizeof(YearTbl));
			pFroze = (u8*) YearTbl;
		}
		CopyRam(RamData.Buf645, pFroze, 28);
		for (T1 = 0; T1 < 14; T1++) {
			OutYTBL(((u16*) RamData.Buf645)[T1]);
		}
	}
}

void RxOneByteRcd(strDispCode const *pCode, u8 FType) { //读取周休日/有功组合特征字编程记录
	static const strMvData mvRecord[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 4, _RET_|COPY_L(5)), };
	MoveDataLst(mvRecord);
}

void RxHLDRcd(strDispCode const *pCode, u8 FType) { //节假日编程记录

	u8 Cnt;
	u8 Start, Lth;
	u8 ActTbl[26];
	u8* pAddr;
	strHolidayTbl stholidaytbl;
	if (SetDBit5(9)) {
		return;
	}
	Ex645.Frm645 = Ex645.Frm645Read;
	if (tpBase == NULL) {
		if (Ex645.Frm645Read == 0) {
			ClrpCharRam(10 + 24 * 4);
		} else if (Ex645.Frm645Read < 9) {
			ClrpCharRam(26 * 4);
		} else {
			ClrpCharRam(22 * 4);
		}
		return;
	}
	if (Ex645.Frm645Read == 0) {
		MoveDataLst(mvTmWriteCode);
		Start = 0;
		Lth = 24;
	} else if (Ex645.Frm645Read < 9) {
		Start = (26 * Ex645.Frm645Read) - 2;
		Lth = 26;
	} else {
		Start = 232;
		Lth = 22;
	}
	GetLRcdLst(ActTbl, _FROZE_HLD_SET_, (u8) (pCode->Code), Start, Lth);
	for (Cnt = 0; Cnt < Lth; Cnt++) {
		if (ActTbl[Cnt]) {
			pAddr = GetFrozeStart(_FROZE_HLD_SET_, ActTbl[Cnt] - 1) + 9;
		} else {
			//pAddr = (u8*)(GAddr.pHld[Start+Cnt]);
			CopyExteeprom((unsigned char *)&stholidaytbl, _Hld_Get(Start+Cnt),_cpHld_Unit_Len);
			pAddr = (unsigned char *) &stholidaytbl;
		}
		OutHTBL(pAddr);
	}
}

void RxFrozeDTRcd(strDispCode const *pCode, u8 FType) { //结算日编程记录

	u8 pLst[3];
	u8 Cnt;
	GetLRcdLst(pLst, _FROZE_FDT_SET_, (u8) (pCode->Code), 0, 3);
	MoveDataLst(mvTmWriteCode);
	for (Cnt = 0; Cnt < 3; Cnt++) {
		if (pLst[Cnt]) {
			RevCopy((u8*) (GetFrozeStart(_FROZE_FDT_SET_, pLst[Cnt] - 1) + 9),
					2);
		} else {
			RevCopy((u8*) &FlashInfo.SetInfo.FrozeDT[Cnt], 2);
		}
	}
}

void RxEOpenRcd(strDispCode const *pCode, u8 FType) { //读开盖记录

	static const strMvData mvEOpenRcd[] = { 
				READ_RAM( vRamData.Buf645 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), 
				READ_PBASE( 0 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), 
				READ_RAM( (vRamData.Buf645 +4), _REV_|BCD_OUT( 4, 4, 0)), 
				READ_RAM( (vRamData.Buf645 +8), _REV_|BCD_OUT( 4, 4, 0)), 
				FILL_RAM( 0xFF, 16), 
				READ_PBASE( 4, _REV_|BCD_OUT( 4, 4, 0)), 
				READ_PBASE( 8, _REV_|BCD_OUT( 4, 4, 0)), 
				FILL_RAM( 0xFF, _RET_|16), };
	u16 Addr;
	u8 Type;
	CopyRam(RamData.Buf645, tpBase, 12);
	Type = (u8) (_FROZE_ECLOSE_1_ + (((pCode->Code >> 8) & 0xFF) - 0x0D));
	tpBase = (u8*) GetFrozeStart((Type | 0x80), (u8) (pCode->Code - 1));
	MoveDataLst(mvEOpenRcd);
}

void RxKeyChgRcd(strDispCode const *pCode, u8 FType) { //修改密钥记录

	static const strMvData mvKeyRecord[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 4, _RET_|COPY_L(9)), };
	MoveDataLst(mvKeyRecord);
}

void RxJDErrRcd(strDispCode const *pCode, u8 FType) { //负荷开关误动作记录

	static const strMvData mvPwrErrRcd[] = { READ_RAM( vRamData.Buf645 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 0 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_RAM( (vRamData.Buf645 +12), COPY_L(1)), READ_RAM( (vRamData.Buf645 +4), _REV_|BCD_OUT( 4, 4, 0)), READ_RAM( (vRamData.Buf645 +8), _REV_|BCD_OUT( 4, 4, 0)), READ_PBASE( 4, _REV_|BCD_OUT( 4, 4, 0)), READ_PBASE( 8, _RET_|_REV_|BCD_OUT( 4, 4, 0)), };
	CopyRam(RamData.Buf645, tpBase, 12);
	if (RamData.Buf645[0] & 1) {
		RamData.Buf645[12] = 1;
	} else {
		RamData.Buf645[12] = 0;
	}
	tpBase = (u8*) GetFrozeStart(((FType | 0x80) & (~0x40)), (u8) (pCode->Code
			- 1));
	MoveDataLst(mvPwrErrRcd);
}

void RxPwrErrRcd(strDispCode const *pCode, u8 FType) { //电源异常记录

	static const strMvData mvPwrErrRcd[] = { READ_RAM( vRamData.Buf645 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_PBASE( 0 , _REV_|OUT_TIME(S_SEC, T_YMDHMS)), READ_RAM( (vRamData.Buf645 +4), _REV_|BCD_OUT( 4, 4, 0)), READ_RAM( (vRamData.Buf645 +8), _RET_|_REV_|BCD_OUT( 4, 4, 0)), };
	CopyRam(RamData.Buf645, tpBase, 12);
	tpBase = (u8*) GetFrozeStart(((FType | 0x80) & (~0x40)), (u8) (pCode->Code
			- 1));
	MoveDataLst(mvPwrErrRcd);
}

#ifdef		_USE_CARD_

void RxPriceRcd(strDispCode const *pCode, u8 FType) { //费率电价表编程记录

	u8 Cnt;
	u8 Start;
	u8 ActTbl[2];
	u8 *pFroze;
	MoveDataLst(mvTmWriteCode);
	N_MvData(tpBase + 8, _REV_ | BCD_OUT( 3, 4, 0), (4 | _LTH_3_));
	N_MvData(tpBase + 8 + 48, _REV_ | BCD_OUT( 3, 4, 0), (4 | _LTH_3_));

	/*
	 for(Cnt = 0; Cnt < 2; Cnt++){
	 if(ActTbl[Cnt]){
	 pFroze = GetFrozeStart(_FROZE_TPRICE_SET_, (ActTbl[Cnt]-1))+9;
	 } else {
	 pFroze = (u8*)GAddr.pPriceTbl[Cnt];
	 }
	 N_MvData(pFroze, _REV_|BCD_OUT( 3, 4, 0), (4|_LTH_3_));
	 }	*/
}

void RxLPriceRcd(strDispCode const *pCode, u8 FType) { //阶梯表编程记录

	u8 Cnt;
	u8 Start;
	u8 ActTbl[2];
	u8 *pFroze;
	MoveDataLst(mvTmWriteCode);
	OutLTBL(tpBase + 8);
	OutLTBL(tpBase + 8 + 57);
}

u8 ReadCardState(strDispCode const *pCode, u8* pAddr) { //读插卡状态字

	RevCopy((u8*) &RamData.CardInState, 2);
	RamData.CardInState = 0;
	return SUCCESS;
}

u8 WriteDisCharge(strDispCode const *pCode, u8* pAddr) { //退费

	if (Ex645.WBit) {
		static const strMvData mvDisCharge[] = { SET_PCHAR( vComData+4, 0), NORM_COPY( (u8*)&vRamData.RTCTime, 4), NORM_COPY( (u8*)&vFlashInfo.SafeInfo.ChargeCnt, 8), //4),
				//		READ_RAM( (u8*)&Ex645.FastData, HEX_IN(4,4,0)),	//BCD_IN(4,4,0)),
				NORM_COPY( (u8*)&vRamData.u_Remain, _RET_|4), };
		//u32	NewSumCharge;
		tpBase = (u8*) GetFrozeStart(_FROZE_OFF_MONEY_, 0);
		if (tpBase) {
			if ((*(u32*) (tpBase + 4)) == FlashInfo.SafeInfo.ChargeCnt) {
				return SetErrReturn(_ERR_OTHER_);
			}
		}
		MoveDataLst(mvDisCharge);
		Rev_4((u8*) &Ex645.FastData);
		*(u32*) (ComData + 12) = Ex645.FastData;
		if ((Ex645.FastData == 0) || (Ex645.FastData > RamData.u_Remain)) {
			return SetErrReturn(_ERR_OTHER_);
		}
		Ex645.FastData = FlashInfo.SafeInfo.SumCharge - Ex645.FastData;
		Up_u32Data(&FlashInfo.SafeInfo.SumCharge, Ex645.FastData);
		Up_UseData();
		RamData.Alm_Chg = 1;
		*(u32*) (ComData + 20) = RamData.ePurse;
		Up_FrozeData(ComData + 4, _FROZE_OFF_MONEY_);
		return SUCCESS;
	} else {
		return SetErrReturn(_ERR_SAFE_);
	}
	return SUCCESS;
}

void RxOffMoneyRcd(strDispCode const *pCode, u8 FType) { //退费记录
	static const strMvData mvOffMoneyRcd[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHM)), //退费日期
			READ_PBASE( 4, _REV_|BCD_OUT(4, 2, 0)), //退费前购电次数
			READ_PBASE( 8 , _REV_|BCD_OUT(4, 4, 0)), //退费金额
			READ_PBASE( 12 , _REV_|BCD_OUT(4, 4, 0)), //退费前剩余金额
			READ_PBASE( 16 , _RET_|_REV_|BCD_OUT(4, 4, 0)), //退费后剩余金额
			};
	MoveDataLst(mvOffMoneyRcd);
}

void RxICErrRcd(strDispCode const *pCode, u8 FType) { //异常插卡记录
	static const strMvData mvICErrRcd[] = { READ_PBASE( 0, _REV_|OUT_TIME(S_SEC, T_YMDHMS)), //清零时间
			READ_PBASE( 4, REV_COPY_L(8)), //序列号
			READ_PBASE( 12, COPY_L(1)), //错误信息字
			READ_PBASE( 13, REV_COPY_L(5)), //操作命令头
			READ_PBASE( 18, REV_COPY_L(2)), //错误响应状态
			READ_PBASE( 20, _REV_|BCD_OUT(2, 2, 0)), //购电次数
			READ_PBASE( 22, _REV_|BCD_OUT(4, 4, 0)), //剩余金额
			READ_PBASE( 26, _REV_|BCD_OUT(4, 4, 0)), //正向有功
			READ_PBASE( 30 , _RET_|_REV_|BCD_OUT(4, 4, 0)), //反向有功
			};
	MoveDataLst(mvICErrRcd);
}

void RxChargeRcd(strDispCode const *pCode, u8 FType) { //读购电记录

	u8 D1;
	D1 = (u8) ((pCode->Code) >> 8);
	if (D1 == 1) {
		MvData(tpBase, _REV_ | OUT_TIME(S_SEC, T_YMDHM));
	} else if (D1 == 2) {
		MvData(tpBase + 4, _REV_ | BCD_OUT( 4, 2, 0));
	} else {
		MvData(tpBase + 8 + ((D1 - 3) * 4), _REV_ | BCD_OUT( 4, 4, 0));
	}
}

#endif

void GetFLUse(u8 *Addr, u8 D1, u8 Mode) { //获取用电数据
	static const strMvData mvReadFL2[] = { READ_PBASE( 0, _REV_|BCD_OUT(4, 4, 0)), READ_PBASE( 8, _REV_|BCD_OUT(4, 4, 0)), READ_PBASE( 16, _REV_|BCD_OUT(4, 4, 0)), READ_PBASE( 24, _REV_|BCD_OUT(4, 4, 0)), READ_PBASE( 32, _RET_|_REV_|BCD_OUT(4, 4, 0)), };
	static const strMvData mvReadFL3[] = { READ_PBASE( 0, _REV_|BCD_OUT(4, 3, 0)), READ_PBASE( 0, _REV_|BCD_OUT(4, 3, 0)), FILL_RAM_PBASE( 0xFF, 18|_RET_), };
	u8 FLNum;
	if (D1 == 0) {
		MvData(Addr, _REV_ | OUT_TIME(S_MIN, T_YMDHM));
	} else if (D1 <= 2) {
		FLNum = FlashInfo.SetInfo.FLMode[3];
		if ((FLNum == 0) || (FLNum >= _MAX_TARRIF_)) {
			FLNum = _MAX_TARRIF_;
		}
		if (D1 == 1) {
			if (Mode & 1) {
				tpBase = (u8*) (Addr + 8);
				goto _FL_USE_1;
				//	MoveDataLst(mvReadFL2);
				//	tpChar -= ((4-FLNum)*4);
			}
		} else {
			if (Mode & 2) {
				tpBase = (u8*) (Addr + 12);
				_FL_USE_1: MoveDataLst(mvReadFL2);
				tpChar -= ((4 - FLNum) * 4);
			}
		}
	} else if (D1 <= 0x0A) {
	} else if (D1 == 0x10) {
		if (Mode & 0x80) {
			tpBase = (u8*) (Addr + 4);
			MoveDataLst(mvReadFL3);
		}
	} else {
		return;
	}
	SetAAInc();
}

const strRcdLst RxRcdLst[] = { // 03xxxxxx

				{ 0x1100, 0, _FROZE_PWR_OFF_, 12, RxPwrOffRcd, _bData_All_ }, //读掉电记录
				{ 0x3001, 0, _FROZE_CLEAR_, 106, RxClrUseRcd, _bData_All_ }, //清零记录
				{ 0x3003, 0, _FROZE_EVENT_CLEAR_, 14, RxClrEventRcd,
						_bData_All_ }, //事件清零记录
				{ 0x3004, 0, _FROZE_CHG_TM_, 16, RxTimeRcd, _bData_All_ }, //读时钟记录
				{ 0x3005, 0, _FROZE_TMTBL_SET_, 0, RxTmTblSetRcd, _bData_All_ }, //日时段表编程记录
				{ 0x3006, 0, _FROZE_YTBL_SET_, 94, RxYTblSetRcd, _bData_All_ }, //时区表编程记录
				{ 0x3007, 0, _FROZE_WEEK_SET_, 11, RxOneByteRcd, _bData_All_ }, //周休日编程记录
				{ 0x3008, 0, _FROZE_HLD_SET_, 0, RxHLDRcd, _bData_All_ }, //节假日编程记录
				{ 0x3009, 0, _FROZE_COM_SET_, 11, RxOneByteRcd, _bData_All_ }, //有功组合特征字编程记录
				{ 0x300C, 0, _FROZE_FDT_SET_, 16, RxFrozeDTRcd, _bData_All_ }, //结算日编程记录
				{ 0x300D, 0, _FROZE_EOPEN_1_, 60, RxEOpenRcd, _bData_All_ }, //开盖记录
				#ifdef		_USE_CARD_
				{ 0x300F, 0, _FROZE_TPRICE_SET_, 42, RxPriceRcd, _bData_Loc_ }, //费率电价编程记录
				{ 0x3010, 0, _FROZE_LPRICE_SET_, 138, RxLPriceRcd, _bData_Loc_ }, //阶梯电价编程记录
				#endif
				{ 0x3012, 0, _FROZE_KEYCHG_, 15, RxKeyChgRcd, _bData_All_ }, //密钥更新记录
				#ifdef		_USE_CARD_
				{ 0x3013, 0, _FROZE_IC_ERR_, 36, RxICErrRcd, _bData_Loc_ }, //异常插卡记录
				{ 0x3301, 0x06, _FROZE_CHARGE_, 0, RxChargeRcd, _bData_Loc_ }, //读购电记录
				{ 0x3400, 0, _FROZE_OFF_MONEY_, 19, RxOffMoneyRcd, _bData_Loc_ }, //退费记录
				#endif
				{ 0x3600, 0, _FROZE_JDERR_START_, 29, RxJDErrRcd, _bData_All_ }, //读继电器故障记录
				{ 0x3700, 0, _FROZE_PERR_START_, 20, RxPwrErrRcd, _bData_All_ }, //读电源异常记录
		};

void Get645Data(strDispCode const *pCode) { //读645数据
/*
 static const u32 IRRead[] = { //红外允许读数据
 0x00900200, 0x00900201, 0x04000101, 0x04000102, 0x0400010C,
 0x04000401, 0x04000402, 0x04800004, 0x0000FF00, 0x0001FF00,
 0x0002FF00, 0x000b0000, 0x000c0000,
 #ifdef _METER_LOC_
 0x000d0000,
 #endif
 };
 */

	struct STIRREADDATALIST stirreadList[] = { { 0x00900200, _bData_All_ }, {
			0x00900201, _bData_All_ }, { 0x04000101, _bData_All_ }, {
			0x04000102, _bData_All_ }, { 0x0400010C, _bData_All_ }, {
			0x04000401, _bData_All_ }, { 0x04000402, _bData_All_ }, {
			0x04800004, _bData_All_ }, { 0x0000FF00, _bData_All_ }, {
			0x0001FF00, _bData_All_ }, { 0x0002FF00, _bData_All_ }, {
			0x000b0000, _bData_All_ }, { 0x000c0000, _bData_All_ }, {
			0x000d0000, _bData_Loc_ }, };

	u32 L;
	s32 Data;
	s32 DData[2];
	u8* Addr;
	u8 DI[4];
	u8 Type;
	u8 Mode;
	const strWriteCode *pWx;
	u8 FLNum;
	Data = 0;
	Copy_4(DI, (u8*) (&(pCode ->Code)));
	if (ERROR == GetIRSafe()) {
		if ((DI[3]) || (DI[2] > 2) || (DI[1] > _MAX_TARRIF_) || (DI[0])) { //当前用电量以外数据
			L = 0;
			while (1) {
				if (Cmp_4(((u8*)&stirreadList[L].lData), SCI[2].DataBuf + 2) == 0) {
					if (SUCCESS == Check_Datatype_Mod_Enable(
							stirreadList[L].cMod))
						break;
				}
				if (++L >= NUMOF(stirreadList)) {
					SetErrReturn(_ERR_SAFE_);
					return;
				}
			}
		}
	}
	L = 0;
	if ((pCode->Code == 0xFFFFFFFF) && (RamData.InDisp)) {
		ClrpCharRam(1);
		return;
	} else if (DI[3] == 0) { //用电数据(00XXXXXX)
		if (DI[2] == 0x90) { //剩余/透支
			#ifdef		_USE_CARD_
			if ((DI[1] == 2) && (_ISMETER_LOCAL())) {
				if ((DI[0] == 0) || (DI[0] == 0xFA)) { //剩余
					static ucode DS_Remain[] = {
							(_USE_UINT_ | _DISP_MIN_BITS_), _DISP_DANGQIAN_,
							_DISP_SHENGYU_, _DISP_JIN_E_, _DISP_JIN_E_ }; //显示剩余金额
					static ucode DS_OverDraft[] = { (_USE_UINT_
							| _DISP_MIN_BITS_), _DISP_DANGQIAN_,
							_DISP_TOUZHI_H_, _DISP_JIN_E_, _DISP_JIN_E_ };//显示透支金额
					if (RamData.InDisp) {
						if (DI[0] == 0) {
							Data = RamData.s_Remain;
						} else {
							Data = RamData.Last_Remain;
						}
						if (Data >= 0) {
							AddDispSetBits(DS_Remain); //显示剩余
						} else {
							AddDispSetBits(DS_OverDraft); //显示透支
						}
					} else {
						Data = RamData.u_Remain; //645剩余
					}
					L = 4;
				} else if ((DI[0] == 1) && (RamData.InDisp == 0)) { //645透支
					Data = RamData.u_OverDraft;
					L = 4;
				}
			}
			#endif
		} else if (DI[2] == 0x0B) { //读取结算周期用电量
			if ((DI[1] == 0) && (DI[0] <= 5)) {
				Addr = GetFrozeStart(_FROZE_MONTH_, DI[0]);
				if (Addr) {
					Data = (*(u32*) (Addr + 12));
				}
				if (DI[0] == 0) {
					Data = GetComUse(DI[1]) - Data;
				} else {
					Addr = GetFrozeStart(_FROZE_MONTH_, (DI[0] - 1));
					if (Addr) {
						Data = (*(u32*) (Addr + 12)) - Data;
					}
				}
				if (RamData.InDisp) {
					if (DI[0] == 0) {
						AddUseLst(DS_DangQianJSUse, 5);
					} else {
						AddUseLst(DS_JSUse, 5);
						RamData.Disp.BitLst[1] = (0x80 | (DI[0]));
					}
				}
				L = 4;
			}
		} else if (DI[2] == 0x0C) {
			if ((DI[1] == 0) && (DI[0] <= 1)) {
				if (DI[0] == 0) {
					Data = GetComUse(0) - FlashInfo.RunInfo.LastUse;
				} else {
					Addr = GetFrozeStart(_FROZE_MSTEP_, DI[0] - 1);
					if (Addr) {
						Data = *(u32*) Addr;
					}
				}
				if (RamData.InDisp) {
					if (DI[0] == 0) {
						AddUseLst(DS_DangQianUse, 5);
					} else {
						AddUseLst(DS_MonthUse, 5);
						RamData.Disp.BitLst[1] = (0x80 | (DI[0]));
					}
				}
				L = 4;
			}
			//#ifdef		_USE_CARD_
		} else if ((DI[2] == 0x0D) && (_ISMETER_LOCAL())) { //年结算周期用电量
			if ((DI[1] == 0) && (DI[0] <= 4) && (RamData.InDisp == 0)) {
				if (DI[0] == 0) {
					if (FlashInfo.RunInfo.EState & _ESTATE_YSTEP_) {
						MvData((u8*) &RamData.RTCTime, _REV_ | OUT_TIME(S_SEC, T_YMDW));
						Data = GetComUse(0) - FlashInfo.RunInfo.LastYUse;
						L = 4;
					} else {
						ClrpCharRam(8);
					}
				} else {
					static const strMvData mvYFroze[] = { READ_PBASE(0, _REV_|OUT_TIME(S_MIN, T_YMDW)), READ_PBASE(4, _RET_|_REV_|BCD_OUT(4, 4, 0)), };
					tpBase = (u8*) GetFrozeStart(_FROZE_YSTEP_, (DI[0] - 1));
					MoveDataLst(mvYFroze);
					return;
				}
			}
			//#endif
		} else if (DI[2] == 0xFE) {
			if ((DI[1] == 0) && (DI[0]) && (DI[0] <= 0x0C)) {
				Addr = GetFrozeStart(_FROZE_MONTH_, (DI[0] - 1));
				MvData((u8*) Addr, _REV_ | OUT_TIME(S_MIN, T_YMDW));
			}
		} else if (DI[0] == 0) { //当前用电量
			if (DI[2] == 0) { //组合
				static ucode DS_ComUse[] = { _DISP_DANGQIAN_, _DISP_DIAN_,
						_DISP_LIANG_, _DISP_LIANG_ };
				if (DI[1] <= _MAX_TARRIF_) {
					Data = GetComUse(DI[1]);
					L = 4;
					AddUseLst(DS_ComUse, DI[1]);
				}
			} else if (DI[2] == 1) { //正向
				static ucode DS_FwdUse[] = { _DISP_ZHENG_, _DISP_XIANG_,
						_DISP_DANGQIAN_, _DISP_DANGQIAN_ };
				if (DI[1] <= _MAX_TARRIF_) {
					CopyRam((u8*) &DData, (u8*) GetFLBase(DI[1]), 8);
					Data = DData[0] - DData[1];
					L = 4;
					AddUseLst(DS_FwdUse, DI[1]);
				}
			} else if (DI[2] == 2) { //反向
				static ucode DS_RevUse[] = { _DISP_DANGQIAN_, _DISP_FAN_,
						_DISP_XIANG_, _DISP_XIANG_ };
				if (DI[1] <= _MAX_TARRIF_) {
					Copy_4((u8*) &Data, ((u8*) GetFLBase(DI[1])) + 4);
					L = 4;
					AddUseLst(DS_RevUse, DI[1]);
				}
			}
		} else if (DI[0] <= 0x0C) { //月结算电量
			if (DI[2] <= 2) {
				static const u8 BLst[] = { 12, 4, 8 };
				if (DI[1] > _MAX_TARRIF_) {
					return;
				}
				Addr = GetFrozeStart(_FROZE_MONTH_, (DI[0] - 1));
				L = DI[1] * 12;
				Copy_4((u8*) &Data, (Addr + L + BLst[DI[2]]));
				if (0 == Addr)
					Data = 0;
				L = 4;
				if ((RamData.InDisp) && (L == 4)) {
					if (DI[2] == 0) {
						static ucode DS_MthComUse[] = { _DISP_DIGIT_, 0x81,
								_DISP_SHANG_, _DISP_YUE_, _DISP_DIAN_,
								_DISP_LIANG_, _DISP_LIANG_ }; //显示反向电量
						AddUseLst(DS_MthComUse, DI[1]);
					} else if (DI[2] == 1) {
						static ucode DS_MthFwdUse[] = { _DISP_DIGIT_, 0x81,
								_DISP_SHANG_, _DISP_YUE_, _DISP_DIAN_,
								_DISP_LIANG_, _DISP_ZHENG_, _DISP_XIANG_,
								_DISP_XIANG_ };
						;
						AddUseLst(DS_MthFwdUse, DI[1]);
					} else {
						static ucode DS_MthRevUse[] = { _DISP_DIGIT_, 0x81,
								_DISP_SHANG_, _DISP_YUE_, _DISP_DIAN_,
								_DISP_LIANG_, _DISP_FAN_, _DISP_XIANG_,
								_DISP_XIANG_ };
						AddUseLst(DS_MthRevUse, DI[1]);
					}
					if (DI[0] < 10) {
						RamData.Disp.BitLst[2] = DI[0] | 0x80;
					} else {
						static ucode
								DS_LastMth[] = { _DISP_10_1_, _DISP_10_1_ };
						RamData.Disp.BitLst[2] = (DI[0] - 10) | 0x80;
						AddDispBits((char *)DS_LastMth);
					}
				}
			}
		}
	} else if (DI[3] == 0x02) {
		if (DI[2] == 1) {
			if (DI[0] == 0) {
				if (DI[1] == 1) {
					static ucode DS_V[] = { (_V_ | _DISP_MIN_BITS_),
							_DISP_8_B_, _DISP_8_C_, _DISP_8_D_, _DISP_8_E_,
							_DISP_8_F_, _DISP_8_F_ }; //显示电压
					Data = RamData.Vph.sVI;
					L = 2;
					AddDispSetBits(DS_V);
				} else if (ChkPhBC(DI[1]) == SUCCESS) {
					SetpCharRam(2);
					return;
				}
			}
		} else if (DI[2] == 2) {
			if (DI[0] == 0) {
				if (DI[1] == 1) {
					static ucode DS_I[] = { (_A_ | _DISP_MIN_BITS_), _DISP_L_,
							_DISP_L_ }; //显示电流
					Data = RamData.Iph.sVI;
					L = 3;
					AddDispSetBits(DS_I);
				} else if (ChkPhBC(DI[1]) == SUCCESS) {
					SetpCharRam(3);
					return;
				}
			}
		} else if (DI[2] == 3) {
			if (DI[0] == 0) {
				if (DI[1] < 2) {
					static ucode DS_P[] = { (_KW_ | _DISP_USE_DOTS_
							| _DISP_MIN_BITS_), _DISP_8_A_, _DISP_8_B_,
							_DISP_8_E_, _DISP_8_F_, _DISP_8_G_, _DISP_8_G_ };
					Data = RamData.Pph.sVI;
					L = 3;
					AddDispSetBits(DS_P);
				} else if (ChkPhBC(DI[1]) == SUCCESS) {
					SetpCharRam(3);
					return;
				}
			}
		} else if (DI[2] == 4) {
			if ((DI[0] == 0) && (DI[1] == 0)) {
				Data = RamData.Varh;
				L = 3;
			}
		} else if (DI[2] == 6) {
			if (DI[0] == 0) {
				if (DI[1] < 2) {
					static ucode DS_Cos[] = { _DISP_DOT_3_, _DISP_COS_,
							_DISP_COS_ };
					Data = RamData.Pfph;
					L = 2;
					AddDispBits((char *)DS_Cos);
				} else if (ChkPhBC(DI[1]) == SUCCESS) {
					SetpCharRam(2);
					return;
				}
			}
		} else if (DI[2] == 0x80) {
			pWx = (const strWriteCode *) Get_Code_Lst((ucode *) Rx645_0280,
					(u16) (pCode->Code), sizeof(Rx645_0280[0]), NUMOF(Rx645_0280));
			if (pWx) {
				ReadSafeData(pCode, pWx);
				return;
			}
		}
	} else if (DI[3] == 3) {
		#ifdef _DEL
	{==== //事件记录(03XXXXXX)
		const strRcdLst *pRcd;
		pRcd = (const strRcdLst*) Get_Code_Lst((ucode *) &RxRcdLst,
				(u16) (pCode->Code >> 8), sizeof(RxRcdLst[0]), sizeof(RxRcdLst)
						/ sizeof(RxRcdLst[0]));

		if (pRcd) {
			if (SUCCESS != Check_Datatype_Mod_Enable(pRcd->cMod))
			{
				SetErrReturn(_ERR_SAFE_);
				return ;
			}
			if ((pRcd ->FType & 0x3F) < _FROZE_NUM_) {
				if (DI[0] == 0) {
					if (RamData.Read_F1 == 0) {
						#ifdef	_USE_CARD_
						if ((pRcd ->FType != _FROZE_CHARGE_)
								&& (_ISMETER_LOCAL())) {
							Data = GetFrozeCnt(pRcd ->FType);
							L = 3;
						} else { //#else
							Data = GetFrozeCnt(pRcd ->FType);
							L = 3;
						}
						#endif
					}
				} else {
					if (pRcd ->FType & 0xC0) {
						L = FileSet.FrozeSet[(pRcd ->FType) & 0x3F].MinCnt / 2;
					} else {
						L = FileSet.FrozeSet[pRcd ->FType].MinCnt;
					}
					if (DI[0] <= L) {
						#ifdef		_USE_CARD_
						if ((_ISMETER_LOCAL()) && ((pRcd ->FType == _FROZE_CHARGE_)
								|| (RamData.InDisp == 0))) {
							tpBase = (u8*) GetFrozeStart(pRcd ->FType, DI[0]
									- 1);
							if ((tpBase == NULL) && (pRcd->ZeroLth)) {
								ClrpCharRam(pRcd->ZeroLth);
							} else {
								pRcd ->DoProg(pCode, pRcd ->FType);
							}
							return;
						} else {
							#else
							if (RamData.InDisp == 0) {
								tpBase = (u8*) GetFrozeStart(pRcd ->FType,
										DI[0] - 1);
								if ((tpBase == NULL) && (pRcd->ZeroLth)) {
									ClrpCharRam(pRcd->ZeroLth);
								} else {
									pRcd ->DoProg(pCode, pRcd ->FType);
								}
								return;
							}
						}
						#endif
					}
					return;
				}
			}
			#ifdef		_USE_CARD_
		} else if ((pCode ->Code == 0x03301400) && (_ISMETER_LOCAL())) { //非法插卡次数
			Data = FlashInfo.RunInfo.ErrCardCnt;
			L = 3;
			#endif
		} else if (((pCode->Code) & 0x00FFFF00) == 0x300000) {
			RxProgRcd(pCode);
			return;
		}
	}=====
	#endif  
  }
	else if (DI[3] == 0x04) {
		if (DI[2] == 0) {
			pWx = (const strWriteCode *) Get_Code_Lst((ucode *) &Wx645Code,
					(u16) (pCode->Code), sizeof(Wx645Code[0]), NUMOF(Wx645Code));
			if (pWx) {
				ReadSafeData(pCode, pWx);//================================================
				return;
			}

		} else if ((DI[2] == 1) || (DI[2] == 2)) { //(040100XX/040200XX)年时区表/日时段表
			if (DI[1] == 0) {
				if (DI[0] == 0) {
					//CopyRam(RamData.Buf645, (u8*)(GAddr.pYTbl[DI[2]-1]), 28);
					CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->YearTbl[DI[2]-1])), 28);
					for (L = 0; L < FlashInfo.SetInfo.FLMode[0]; L++) {
						if ((RamData.InDisp == 0) || (pCode->SubCode == L)) {
							OutYTBL(((u16*) RamData.Buf645)[L]);
						}
					}
					return;
				} else if (DI[0] <= 8) {
					//CopyRam(RamData.Buf645, (u8*)(GAddr.pTmTbl[DI[2]-1][DI[0]-1]), 28);
					CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->TmTbl[DI[2]-1][DI[0]-1])), 28);
					for (L = 0; L < FlashInfo.SetInfo.FLMode[2]; L++) {
						if ((RamData.InDisp == 0) || (pCode->SubCode == L)) {
							OutTTBL(((u16*) RamData.Buf645)[L]);
						}
					}
					return;
				}
			}
		} else if (DI[2] == 3) { //(040300XX)假日表
			if ((DI[1] == 0) && (DI[0] > 0) && (DI[0] <= _cpHld_StoreMaxNum)) {
				CopyExteeprom(RamData.Buf645, _Hld_Get(DI[0]-1), sizeof(_cpHld_Unit_Len));
				OutHTBL((u8*) RamData.Buf645); //OutHTBL((u8*)GAddr.pHld[DI[0]-1]);
			}
		} else if (DI[2] == 4) { //(040401XX/040402XX)显示表
			if ((DI[1]) && (DI[1] <= 2) && (DI[0]) && (DI[0]
					<= FlashInfo.SetInfo.DispLength[DI[1] - 1])) {
				CopyExteeprom(RamData.Buf645, ((int)&(((struct ST24256_DATACONFIG *)0)->DispArry[DI[1]-1][DI[0]-1])), 5);
				NormCopy((u8*) RamData.Buf645, 5); //NormCopy((u8*)GAddr.pDisp[DI[1]-1][DI[0]-1], 5);
			}
			//#ifdef		_USE_CARD_
		} else if ((DI[2] == 5) && (_ISMETER_LOCAL())) { //(040501XX/040502XX)费率电价表
			if ((DI[1]) && (DI[1] <= 2)) {
				if ((DI[0]) && (DI[0] <= 32)) {
					DI[1]--;
					//Copy_4((u8*)&Data, (u8*)(GAddr.pPriceTbl[DI[1]]+(DI[0]-1)*3));
					Data = 0;
					CopyExteeprom((unsigned char *)&Data, _Price_Get(DI[1],(DI[0]-1)) , _Price_Unit_Len_);
					AddUseLst(DS_FLPrice, DI[0]);
					RamData.Disp.DispSet = _DISP_LTH_5_;
					if (DI[1] == 1) {
						AddDispBits((char *)DS_FL_1);
					} else {
						AddDispBits((char *)DS_FL_2);
					}
					L = 4;
				} else if ((RamData.InDisp == 0) && (RamData.Read_F1 == 0)
						&& (RamData.Read_F0 == 0)) {
					SetErrReturn(ErrMode[3]);
					return;
				}
			}
			//#endif
		} else if (DI[2] == 6) { //(040600XX~040603XX)阶梯表
			if (((DI[1] == 4) || (DI[1] == 5)) && (_ISMETER_LOCAL()) && (RamData.Read_F0) && (DI[0] == 0)) { //阶梯表集合
				strLTbl LTBl;
				//OutLTBL((u8*)GAddr.pLTbl[DI[1]&1]);
				CopyExteeprom((unsigned char *)&LTBl, _LTBl_Get(DI[1]&1),sizeof( strLTbl));
				OutLTBL((u8*) &LTBl);
			}
		} else if (DI[2] == 9) { //Limit(0409XXXX)限额信息
			pWx = (const strWriteCode*) Get_Code_Lst((ucode *) &Wx645LmtCode,
					(u16) (Ex645.Code645.Code), sizeof(Wx645LmtCode[0]), NUMOF(Wx645LmtCode));
			if (pWx) {
				ReadSafeData(pCode, pWx);
				return;
			}
		} else if (DI[2] == 0x80) { //版本信息(048000XX)
			if ((DI[1] == 0) && (RamData.InDisp == 0)) {
				if ((DI[0]) && (DI[0] <= 3)) {
					RevCopy((FData.DftSet.SoftVer + (32 * (DI[0] - 1))), 32);
				} else if (DI[0] == 4) {
					RevCopy((FData.DftSet.SoftID), 8);
				}
			}
#ifdef		__VH_TEST__
		} else if(DI[2] == 0xFE) {
			GetGTest(pCode);
#endif
		}
	} else if ((DI[3] == 5) && (RamData.InDisp == 0)) { //(05XXXXXX)冻结数据
		if (DI[2] == 0) { //(0500XXXX)
			if ((DI[0]) && (DI[0] <= _FRZ_MINCNT_(FFTime))) {
				Type = _FROZE_TM_;
				Mode = FlashInfo.SetInfo.FrozeMode[0];
				goto _FL_USE_;
			}
		} else if (DI[2] == 1) { //(0501XXXX)
			if ((DI[0]) && (DI[0] <= 3)) {
				Type = _FROZE_ACT_;
				Mode = FlashInfo.SetInfo.FrozeMode[1];
				goto _FL_USE_;
			}
		} else if ((DI[2] == 2) || (DI[2] == 3)) { //(0502XXXX/0503XXXX)
			if ((DI[0] == 1) || (DI[0] == 2)) {
				Type = _FROZE_YTBL_CHG_ + (DI[2] - 2);
				_FL_MODE2_: Mode = FlashInfo.SetInfo.FrozeMode[2];
				_FL_USE_: Addr = GetFrozeStart(Type, DI[0] - 1);
				_FL_USE_2_: GetFLUse(Addr, DI[1], Mode);
				return;
			}
		} else if (DI[2] == 4) { //(0504XXXX)
			if ((DI[0]) && (DI[0] <= _FRZ_MINCNT_(FFHour))) {
				Addr = GetFrozeStart(_FROZE_HOUR_, DI[0] - 1);
				if (DI[1] == 0) {
					MvData((u8*) Addr, _REV_ | OUT_TIME(S_SEC, T_YMDHM));
					goto _HOUR_AA_;
				} else if (DI[1] == 1) {
					if (FlashInfo.SetInfo.FrozeMode[3] & 1) {
						if (Addr) {
							Data = *(u32*) (Addr + 4);
						}
						Up_BCDData(Data, 4);
					}
					goto _HOUR_AA_;
				} else if (DI[1] == 2) {
					if (FlashInfo.SetInfo.FrozeMode[3] & 2) {
						if (Addr) {
							Data = *(u32*) (Addr + 8);
						}
						Up_BCDData(Data, 4);
					}
					_HOUR_AA_: SetAAInc();
					return;
				}
			}
			#ifdef		_USE_CARD_
		} else if ((DI[2] == 5) && (_ISMETER_LOCAL())) { //费率电价切换冻结(0505XXXX)
			if ((DI[0] == 1) || (DI[0] == 2)) {
				Type = _FROZE_FL_PRICE_CHG_;
				goto _FL_MODE2_;
			}
		} else if ((DI[2] == 7) && (_ISMETER_LOCAL())) { //阶梯电价切换冻结(0507XXXX)
			if ((DI[0] == 1) || (DI[0] == 2)) {
				Type = _FROZE_STEP_PRICE_CHG_;
				goto _FL_MODE2_;
			}
		} else if ((DI[2] == 8) && (_ISMETER_LOCAL())) {
			if ((DI[1] == 2) && (DI[0]) && (DI[0] <= _FRZ_MINCNT_(FFzDay))) {
				Addr = GetFrozeStart(_FROZE_DAY_, DI[0] - 1);
				MvData((u8*) Addr, _REV_ | BCD_OUT(2, 2, 0));
				Copy_4((u8*) &Data, Addr + 2);
				if (((s32) Data) >= 0) {
					Up_BCDData(Data, 4);
					ClrpCharRam(4);
				} else {
					ClrpCharRam(4);
					Data = -Data;
					L = 4;
				}
			}
			#endif
		} else if (DI[2] == 6) { //日冻结(0506XXXX)
			if ((DI[0]) && (DI[0] <= _FRZ_MINCNT_(FFzDay))) {
				Addr = GetFrozeStart(_FROZE_DAY_, DI[0] - 1);
				Addr += 6;
				Mode = FlashInfo.SetInfo.FrozeMode[4];
				goto _FL_USE_2_;
			}
		}
	} else if (DI[3] == 6) { //(06XXXXXX)
		if ((RamData.InDisp) && ((u16) (pCode->Code >> 8) == 0x0606)) {
			if (DI[0] == 0x06) {
				Data = RamData.DispErr; //北京故障代码显示
				L = 1;
				AddDispBits((char *)DS_Err);
				/*	} else if(DI[0] == 0x07){								//显示日时段表
				 Addr = (u16)(&FlashInfo.PriceTbl[TMTBL_State].TmTbl[FlashInfo.RunInfo.YTbl_Num-1]);
				 for(L = 0; L < FlashInfo.SetInfo.FLMode[2]; L++){
				 Data  = ((u32)BCD(((strTmTbl *)Addr)[L].FLNum)<<16)
				 | ((u16)BCD(((strTmTbl *)Addr)[L].HM%60)<<8)
				 | (BCD(((strTmTbl *)Addr)[L].HM/60));
				 if((RamData.InDisp == 0)||(pCode->SubCode == L)){
				 NormCopy( (u8*)(&Data)+1, 3);
				 AddDispBits((char *)DS_TmTbl);
				 }
				 }
				 return;	*/
			}
		}

	} else if ((DI[3] == 0x19) && (DI[2] == 0x01) && (RamData.InDisp == 0)) { //过流反向记录(1901XXXX)
		if (DI[1] == 0) {
			if (RamData.Read_F1 == 0) {
				if (DI[0] == 1) { //次数
					Data = GetFrozeCnt(_FROZE_OVERIM_START_);
					L = 3;
				} else if (DI[0] == 2) { //累计时间
					Data = FlashInfo.RunInfo.OverIphTime;
					if (FlashInfo.RunInfo.RcdTm.LastOverIphTime) {
						Data += GetTimeDiff(
								&FlashInfo.RunInfo.RcdTm.LastOverIphTime);
					}
					L = 3;
				}
			}
		} else if ((DI[1] <= 0x31) && (DI[0]) && (DI[0] <= 0x0A)) {
			RxOverIphRecord(pCode);
		}
	} else if (((DI[3] == 0x1D) || (DI[3] == 0x1E)) && (RamData.InDisp == 0)) { //拉闸记录(1D00XXXX/1E00XXXX)
		if (DI[2] == 0) {
			if (DI[1] == 0) {
				if ((RamData.Read_F1 == 0) && (RamData.Read_F0 == 0) && (DI[0]
						<= 1)) {
					Data = (GetFrozeCnt(_FROZE_JD_) + (DI[3] & 1)) / 2;
					L = 3;
				}
			} else if ((DI[0]) && (DI[0] <= 10) && (DI[1] <= 8)) {
				tpBase = GetFrozeStart(_FROZE_JD_,
						((((u8) (DI[0] - 1)) << 1)
								+ (((u8) (FlashInfo.FrozeCnt[_FROZE_JD_])
										^ DI[3]) & 1)));
				if ((0 == RamData.Read_F0) && (0 == RamData.Read_F1) && (DI[1]
						> 4))
					return;
				RxJDRecord(pCode);
			}
		}
	} else if ((DI[3] == 0xFA) && (DI[2] == 0xFA)) {
		L = *((u16*) DI);
		//	if((L&63) == 0){
		RevCopy((u8*) (*((u16*) DI)), 64);
		//	} else if(L == 0x0001){
		//		RevCopy( (u8*)(&QUANT_W0), 4);
		//	} else if(L == 0x01FE){
		//		RamData.LastKeySec = 76543210;
		//		Wait_Rst(1);
		//	}
		return;
	}
	Up_BCDData(Data, L);
}

u8 Chk_TRxNum(void) { //检查通讯地址

	u8 Cnt;
	u8 Lth;
	if (Ex645.SCIx->DataBuf[8] & 0x80) { //表发送数据不应答
		return FAIL;
	}
	Cnt = Chk_PubAddr(Ex645.SCIx->DataBuf + 1);
	if (Cnt < _PUB_LTH_) {
		Ex645.SCIx->AddrMode = Cnt + 2;
	} else {
		Ex645.SCIx->AddrMode = 0;
		Cnt = 0;
		do {
			if (!_localAddr) {
				if (Ex645.SCIx->DataBuf[Cnt + 1] != 0xAA) {
					return FAIL;
				}
			} else if (Ex645.SCIx->DataBuf[Cnt + 1] == 0xAA) {
				Ex645.SCIx->AddrMode = 1;
			} else if (Ex645.SCIx->DataBuf[Cnt + 1]
					!= FlashInfo.SafeInfo.TRx_Num[5 - Cnt]) {
				return FAIL;
			}
		} while (++Cnt < 6);
	}
	Tx_Enable(Ex645.SCIx);
	Ex645.SCIx->AddrAct = 1;
	Lth = Ex645.SCIx->DataBuf[9] + 2;
	Ex645.SCIx->DataBuf += 8;//CopyRam(Ex645.SCIx->DataBuf, Ex645.SCIx->DataBuf+8, Lth);
	//SetRamData(Ex645.SCIx->DataBuf+2, _SUB_33_, (Lth-2));			//按字节减33
	Ex645.SCIx->NoRet = 0;
	return SUCCESS;
}

void Chk_Rx68(void) { //在接收数据中搜索0x68

	u8 Cnt1;
	Cnt1 = 1;
	return;
	while (1) {
		if (Ex645.SCIx->DataBuf[Cnt1] == 0x68) {
			if ((Cnt1 == 8) && (Ex645.SCIx->DataBuf[7] != 0xFE)) {
				Cnt1 = 1;
				Ex645.SCIx->DataBuf[1] = 0x68;
			}
			SubSCIBuf(Cnt1);
			return;
		}
		if (++Cnt1 >= Ex645.SCIx->Head) {
			SubSCIBuf(Cnt1);
			return;
		}
	}
}

void RxSend645(void) { //通讯设置发送数据状态
	void stream_send_enable(unsigned char *p, unsigned int ilen);

	//unsigned short GetSCINUM(strSCIBuf *p);
	unsigned char stream_send_over(unsigned short);
	enum ENUMCHANDEL iSCINum = GetSCINUM(Ex645.SCIx);
	u8 Sum;
	u8 Lth;
	unsigned int itemp;
	/*

	 Ex645.SCIx->AddrAct = 0;
	 if((Ex645.SCIx->NoRet)||(Ex645.SCIx->AddrMode == _ADDR_99_)){				//全99通讯地址不返回
	 Rx_Enable(Ex645.SCIx);
	 return;
	 }
	 Ex645.SCIx->DataBuf[0] |= 0x80;
	 Lth = Ex645.SCIx->DataBuf[1];
	 SetRamData(Ex645.SCIx->DataBuf+2, _ADD_33_, Lth);
	 Sum = GetSum(FlashInfo.SafeInfo.TRx_Num, 6)+0xD0;
	 Lth += 2;
	 Sum += GetSum(Ex645.SCIx->DataBuf, Lth);
	 Ex645.SCIx->DataBuf[Lth] = (u8)Sum;
	 Ex645.SCIx->DataBuf[Lth+1] = 0x16;
	 Ex645.SCIx->SendLth = Lth+2;
	 #ifdef		_TRX_NO_FE_
	 Ex645.SCIx->Head = 0xF8;
	 #else
	 Ex645.SCIx->Head = 0xF4;
	 #endif
	 Ex645.SCIx->Act = 2;
	 Ex645.SCIx->TRxEvt = 1;
	 */

	USARTCOM[iSCINum].bEventRec645 = 0;
	Ex645.SCIx->AddrAct = 0;
	if ((Ex645.SCIx->NoRet) || (Ex645.SCIx->AddrMode == _ADDR_99_)) { //全99通讯地址不返回
		//	Rx_Enable(Ex645.SCIx);
		_RSet_Safe(USARTCOM[iSCINum].cSilence,1);//  stream_refresh (iSCINum )   ;
		return;
	}
	Ex645.SCIx->DataBuf[0] |= 0x80;

	Lth = Ex645.SCIx->DataBuf[1];
	CopyRam(Ex645.SCIx->DataBuf - 8, Ex645.SCIx->DataBuf, Lth + 2);
	RSet_WDG(Lth / 5 + 1);
	stream_send_enable((unsigned char *) Ex645.SCIx, Lth + 2);

	//do {
	//	itemp = stream_send_over(iSCINum ) ;
	//}while  ( _cnWaitSend_  == itemp);
	//while ( _cnWaitSend_  == stream_send_over ());
	//	   stream_refresh (iSCINum )   ;

	SetTrxDisp();
}

void SendErrReturn(u8 Err) { //设置错误状态并发送
	SetErrReturn(Err);
	RxSend645();
}

#define   _cFr645BUF_Len_ 		200 								//多帧数据返回最大长度
u8 Check645Length(u8 *RxBuf, u8 Lth) { //检查645数据长度并设置后续帧

	u8 NCnt;
	u8* pData;
	u8 MaxLth;
	pData = Ex645.SCIx->DataBuf;
	MaxLth = _cFr645BUF_Len_;
	if (RamData.WDGTm < 3) {
		return SUCCESS;
	}
	if (Lth == 0) {
		return FAIL;
	}
	if (Ex645.Frm645) {
		MaxLth--;
	}
	if (Ex645.Frm645 == Ex645.Frm645Read) {
		if ((pData[1] + Lth) > MaxLth) {
			pData[0] |= 0x20;
			Lth = MaxLth - pData[1];
			CopyRam(&pData[pData[1] + 2], RxBuf, Lth);
			pData[1] = MaxLth;
			return SUCCESS;
		} else {
			CopyRam(&pData[pData[1] + 2], RxBuf, Lth);
			pData[1] += Lth;
			return FAIL;
		}
	} else if ((pData[1] + Lth) >= MaxLth) {
		Ex645.Frm645++;
		NCnt = MaxLth - pData[1];
		Lth -= NCnt;
		if ((Ex645.Frm645 == Ex645.Frm645Read) && (Lth)) {
			CopyRam(pData + 6, RxBuf + NCnt, Lth);
		}
		pData[1] = Lth + 4;
	} else {
		pData[1] += Lth;
	}
	return FAIL;
}

void SetReadOver(void) { //数据返回处理

	u8 *pData;
	pData = Ex645.SCIx->DataBuf;
	if (Ex645.Frm645 < Ex645.Frm645Read) {
		if ((FlashInfo.RunInfo.EState & _ESTATE_EVT_OUT_)
				&& (FlashInfo.SetInfo.RunType & 4) && ((Ex645.Frm645 + 1)
				== Ex645.Frm645Read)) {
			goto S_Over;
		} else {
			SetErrReturn(_ERR_NODATA_);
		}
	} else if (pData[1] > 4) {
		if (Ex645.Frm645Read) {
			pData[pData[1] + 2] = Ex645.Frm645;
			pData[1]++;
		}
		if ((FlashInfo.RunInfo.EState & _ESTATE_EVT_OUT_)
				&& (FlashInfo.SetInfo.RunType & 4)) {
			pData[0] |= 0x20;
		}
	} else if (RamData.Read_FH) {
		if ((pData[1] == 4) && (FlashInfo.RunInfo.EState & _ESTATE_EVT_OUT_)
				&& (FlashInfo.SetInfo.RunType & 4)) {
			S_Over: 
					tpChar = pData + 6;
		//opt	ReadEvtState((strDispCode const *) pData, NULL);
			pData[1] = tpChar - (pData + 1);
			pData[pData[1] + 1] = Ex645.Frm645Read;
		}
	} else {
		SetErrReturn(_ERR_NODATA_);
	}
}

void RxSetDisp(void) { //设置显示项

	strDispCode DCode;
	u8 *pData;
	pData = Ex645.SCIx->DataBuf + 6;
	CopyRam((u8*) &DCode, Ex645.SCIx->DataBuf + 6, 5);
	if (ChkDispItem(&DCode) == SUCCESS) {
		SetDispItem((uc8*) &DCode);
		RamData.Disp.NextTm = 10;
		//	RamData.Disp.DispArryNum = 0x80;
		RamData.Disp_State = 1;
		if (DCode.Code == 0xFFFFFFFF) {
			RamData.TDispTm = 10;
		}
	} else {
		SetErrReturn(_ERR_OTHER_);
	}
}

void Clr_Comm(void);
#define		YTbl645		((u16 *)(RamData.Buf645))
#define		TmTbl645	((u16 *)(RamData.Buf645))
void Write645Data(void) { //strDispCode *pCode){					//写参数(04XXXXXX)

	u8 Safe;
	u32 Data;
	u8 Lth;
	u8 CBuf[5];
	u8* pAddr;
	u8 DI[4];
	const strWriteCode *pWx;
	u8 *pData;
	pData = Ex645.SCIx->DataBuf;
	Copy_4(DI, (u8*) (&(Ex645.Code645.Code)));
	if (DI[2] == 0) { //参数项(0400XXXX)
		pWx = (const strWriteCode*) Get_Code_Lst((ucode *) &Wx645Code[0],
				(u16) (Ex645.Code645.Code), sizeof(Wx645Code[0]),
				(sizeof(Wx645Code) / sizeof(Wx645Code[0])));
		if (pWx) {
			WriteSafeData(pWx);
			//#ifdef		_USE_CARD_
			if ((_ISMETER_LOCAL()) && ((u16) (Ex645.Code645.Code)
					&& ((u16) (Ex645.Code645.Code) <= 0x1005))) {
				RamData.Alm_Chg = 1;
			}
			//#endif
			return;
		}
	} else if ((DI[2] == 1) || (DI[2] == 2)) { //第一/二套费率表(040100XX/040200XX)
		if (DI[1]) {
			SetErrReturn(_ERR_NODATA_);
			return;
		} else if (DI[0] > 8) {
			SetErrReturn(_ERR_NODATA_); //超日时段表数
			return;
		}
		if ((pData[6] == 0x98) && (_CODE_DEL_))	{
			Ex645.ExLth = 3;
			if (GetSafeData(pData, 0)) {
				return;
			}
			Lth = RWSamic.Cmd[4];
		} else {
			Lth = pData[1] - 12;
#ifdef		_USE_24_CODE_
			if(GetSafeState(_SAFE_D_2_, Lth)) {
				return;
			}
#else
			if (GetSafeState(_SF_FAC_, Lth)) {
				return;
			}
#endif
		}
		if (CheckBCD(pData + 6, Lth)) { //非BCD数据返回
			SetErrReturn(_ERR_OTHER_);
			return;
		}
		if (Lth > (14 * 3)) {
			if (DI[0] == 0) {
				SetErrReturn(_ERR_OVER_YTB_);
			} else {
				SetErrReturn(_ERR_OVER_TMTB_);
			}
			return;
		} else if (Lth == 0) {
			SetErrReturn(_ERR_OTHER_);
			return;
		}
		while (Lth < (14 * 3)) {
			Copy_4(pData + 6 + Lth, pData + 3 + Lth); //			CopyRam( pData+6+Lth, pData+3+Lth, 3);
			Lth += 3;
			if (Lth > (14 * 3)) {
				SetErrReturn(_ERR_OTHER_);
				return;
			}
		}
		pAddr = pData + 6;
		for (Lth = 0; Lth < 13; Lth++) {
			if (strCmp(pAddr + 1, pAddr + 4, 2) == 0) {
				if (pAddr[0] != pAddr[3]) {
					SetErrReturn(_ERR_OTHER_);
					return;
				}
			}
			pAddr += 3;
		}
		pAddr = pData + 6;
		if (DI[0] == 0) { //修改年时区表
			for (Lth = 0; Lth < 14; Lth++) {
				if ((pAddr[2] == 0) || (pAddr[2] > 0x12) || (pAddr[1] == 0)
						|| (pAddr[1] > 0x31)) {
					SetErrReturn(_ERR_OTHER_);
					return;
				} else if ((pAddr[0] == 0) || (pAddr[0] > 8)) { //FlashInfo.SetInfo.FLMode[1])){
					SetErrReturn(_ERR_OTHER_); //年时区表数超
					return;
				}
				HEXLst(pAddr, 3);
				YTbl645[Lth] = YTB(pAddr[2], pAddr[1], pAddr[0]);
				pAddr += 3;
			}
			//Up_ProgFroze(_FROZE_YTBL_SET_, (DI[2]-1), (u8*)GAddr.pYTbl[DI[2]-1]);
			{
				u8 YearTbl[_YearTbl_Eeprom_First_Len]; //年时区表(2*28)u8
				CopyExteeprom((unsigned char *)YearTbl,_Year_GetT(DI[2]-1),sizeof(YearTbl));
				Up_ProgFroze(_FROZE_YTBL_SET_, (DI[2] - 1), YearTbl);
			}
			SetEvtState(_EVT_YTBL_);
			//Up_TTbl((u8*)YTbl645, YTBL_START+DI[2]-1);
			Write_EEData((u8*) YTbl645, _Year_GetT(DI[2]-1), TblSetLst[YTBL_START + DI[2] - 1].Lth);
		} else { //修改日时段表
			for (Lth = 0; Lth < 14; Lth++) {
				if ((pAddr[2] > 0x23) || (pAddr[1] > 0x59)) {
					SetErrReturn(_ERR_OTHER_);
					return;
				} else if ((pAddr[0] == 0) || (pAddr[0] > 0x32)) { // _MAX_TARRIF_)){	//FlashInfo.SetInfo.FLMode[3])){
					SetErrReturn(_ERR_OVER_FL_); //费率数超
					return;
				}
				HEXLst(pAddr, 3);
				TmTbl645[Lth] = TTB(pAddr[2], pAddr[1], pAddr[0]);
				pAddr += 3;
			}
			{
				unsigned char sTmTbl[_TmTbl_Eeprom_First_Len]; //日时段表(2*8*28B)
				//Up_ProgFroze(_FROZE_TMTBL_SET_, ((DI[2]-1)<<3)|((DI[0]-1)&0x7), (u8*)GAddr.pTmTbl[DI[2]-1][DI[0]-1]);
				CopyExteeprom(sTmTbl, _Tmtbl_GetB(DI[2]-1,DI[0]-1),sizeof(sTmTbl));
				Up_ProgFroze(_FROZE_TMTBL_SET_, ((DI[2] - 1) << 3) | ((DI[0]
						- 1) & 0x7), sTmTbl);
			}
			SetEvtState(_EVT_TMTBL_);
			//Up_TTbl((u8*)TmTbl645, TTBL_START+((DI[2]-1)<<3)+(DI[0]-1));
			Write_EEData(
					(u8*) TmTbl645,
					((int) &(((struct ST24256_DATACONFIG *) 0)->TmTbl[DI[2] - 1][DI[0]
							- 1])), _TmTbl_Eeprom_1_1 );
		}
		RamData.Event_Feilv = 1;
		AddTRxWRecord();
		return;
	} else if (DI[2] == 3) { //假日表(040300XX)
		if ((DI[1] == 0) && (DI[0]) && (DI[0] <= _cpHld_StoreMaxNum)) {
			if (GetSafeState(_SAFE_D_2_, 4)) {
				return;
			}
			tpChar = pData + 12;
			MvData(pData + 7, _REV_ | READ_BCD_TIME(S_DAY, T_DAY));
			if (CheckZero(pData + 12, 2) == 0) {
				if (CheckZero(pData + 6, 4) != 0) { //	if(((*(u16*)(pData+12)) == 0)&&((*(u32*)(pData+6)) != 0)){
					SetErrReturn(_ERR_OTHER_);
					return;
				}
			} else if ((pData[6] == 0) || (pData[6] > 8)) { //FlashInfo.SetInfo.FLMode[1])){
				SetErrReturn(_ERR_OTHER_); //超年时区表数
				return;
			}
			pData[14] = pData[6];
			//Up_ProgFroze(_FROZE_HLD_SET_, DI[0]-1, (u8*)GAddr.pHld[DI[0]-1]);
			{
				strHolidayTbl stholidaytbl;
				CopyExteeprom((unsigned char *)&stholidaytbl, _Hld_Get(DI[0]-1),_cpHld_Unit_Len);
				Up_ProgFroze(_FROZE_HLD_SET_, DI[0] - 1, (u8*) &stholidaytbl);
			}

			SetEvtState(_EVT_HLD_);
			//Up_Hld((strHolidayTbl *)(pData+12), (DI[0]-1));
			_Hld_Write((pData+12),(DI[0]-1));
			RamData.Event_Feilv = 1;
			AddTRxWRecord();
			return;
		}
	} else if (DI[2] == 4) { //显示项设置
		if ((DI[1]) && (DI[1] <= 2) && (DI[0]) && (DI[0]
				<= _DispArry_Unit_MaxNum_)) { // NUMOF(GAddr.pDisp[0]))){
			strDispCode DCode;
			if (GetSafeState(_SAFE_D_2_, 5)) {
				return;
			}
			if (RamData.Mac_98) {
				//	DCode.Code = (*(u32*)(pData+6));
				//	DCode.SubCode = pData[10];
				Copy_4((u8*) &DCode.Code, pData + 7);
				DCode.SubCode = pData[6];
			} else {
				//	DCode.Code = (*(u32*)(pData+7));
				//	DCode.SubCode = pData[6];
				CopyRam((u8*) &DCode, pData + 6, 5);
			}
			if (ChkDispItem(&DCode) == SUCCESS) {
				//Up_DispCode(&DCode, (((DI[1]-1)*NUMOF(GAddr.pDisp[0]))+DI[0]-1));
				PuttExtShowMenu(&DCode,(DI[1]-1),(DI[0]-1));
				AddTRxWRecord();
				return;
			} else {
				SetErrReturn(_ERR_OTHER_);
				return;
			}
		}
		#ifdef		_USE_CARD_
	} else if ((DI[2] == 5) && (_ISMETER_LOCAL())) { //费率电价
		if (((Ex645.Code645.Code) & 0xFFFF) == 0x02FF) {
			Lth = pData[1];
			if (((Lth & 3) != 0) || (Lth <= 16) || (Lth > (128 + 16))) {
				SetErrReturn(_ERR_OTHER_);
				return;
			}
			Lth /= 4;
			Lth -= 4;
			Rev4Lst(pData + 14, Lth);
			Ex645.ExLth = 4;
			if (GetSafeState(_SAFE99_, 0)) {
				return;
			}
			if (((RamData.MacBuf[5] & 3) != 0) || (RamData.MacBuf[5] > 128)) { //长度非法时返回错误(可精简
				SetErrReturn(_ERR_OTHER_);
				return;
			}
			if (Chk_Mac_99()) {
				return;
			}
			if (RamData.MacBuf[5] < 128) { //补齐数据
				DI[0] = 6 + RamData.MacBuf[5];
				while (DI[0] < (128 + 6)) {
					CopyRam(pData + DI[0], pData + RamData.MacBuf[5] + 2, 4);
					DI[0] += 4;
				}
			}
			Up_FPrice(pData + 6, 1);
			AddTRxWRecord();
			return;
		}
	} else if ((DI[2] == 6) && (_ISMETER_LOCAL())) { //阶梯电价
		if (((Ex645.Code645.Code) & 0xFFFF) == 0x05FF) {
			Lth = pData[1];
			if (pData[1] != (64 + 16)) {
				SetErrReturn(_ERR_OTHER_);
				return;
			}
			Rev4Lst(pData + 14, 13);
			RevStrLst(pData + 14 + 52, 3, 4);
			Ex645.ExLth = 4;
			if (GetSafeState(_SAFE99_, 0)) {
				return;
			}
			if (Chk_Mac_99()) {
				return;
			}
			Up_LData(pData + 6, 1); //刷新阶梯数据
			AddTRxWRecord();
			return;
		}
		#endif
	} else if (DI[2] == 9) { //限额设置(0409XXXX)
		pWx = (const strWriteCode*) Get_Code_Lst((ucode *) &Wx645LmtCode[0],
				(u16) (Ex645.Code645.Code), sizeof(Wx645LmtCode[0]),
				(sizeof(Wx645LmtCode) / sizeof(Wx645LmtCode[0])));
		if (pWx) {
			WriteSafeData(pWx);
			return;
		}
	} else {
		tpChar = pData + 8;
		Ex645.WBit = 0;
		Get645Data(&Ex645.Code645);
		if (tpChar == (pData + 8)) {
			SetErrReturn(_ERR_NODATA_);
		} else {
			SetErrReturn(_ERR_SAFE_);
		}
		return;
	}
	SetErrReturn(_ERR_NODATA_);
	return;
}

void SetFarFail(void) { //设置远程错误
	Ex645.SCIx->DataBuf[0] = 0xc3;
	Ex645.SCIx->DataBuf[1] = 0x02; //  *(u16*)(RWSamic.Cmd+2) = REV16(IDStart);
	CopyRam(Ex645.SCIx->DataBuf + 2, (u8*) &(RamData.FErrWord), 2);//_ipputp((Ex645.SCIx->DataBuf+2) , &(RamData.FErrWord)); //*((u16*)(Ex645.SCIx->DataBuf+2)) = RamData.FErrWord;

}

typedef struct {
	u32 Code32;
	u8 PubRead;
	u8 (*DoProg)(u8 *);
} strRxCmd4;
void IC_645Commniucate(void) { //远程通讯处理
	#ifdef _DEL
	static const strRxCmd4 RxCmd4[] = {
	      	 	{ 0x078003FF, 0, ID_IRAccess1 }, //红外认证请求
			{ 0x070003FF, 0, ID_IRAccess2 }, //红外身份认证
			{ 0x070000FF, 0, ID_Access }, //身份认证核对
			{ 0x070002FF, 0, ID_AccessTimeClose }, //访问时间关闭
			{ 0x078001FF, 1, ID_AccessDataRead }, //数据回抄
			{ 0x078002FF, 1, ID_AccessCodeCmp }, //程序比对命令
			{ 0x078102FF, 1, ID_AccessACK }, //查询状态
			{ 0x070001FF, 0, ID_AccessTimeSet }, //访问时间设定
			{ 0x070301FF, 0, ID_AccessChangeMi }, //修改密钥1	 //---------------密钥序列号未处理----------------//
#ifdef		_USE_CARD_
			{ 0x070101FF, 0, ID_AccessOpenCharge }, //开户
			{ 0x070102FF, 0, ID_AccessNextCharge }, //购电
			{ 0x070103FF, 0, ID_AccessRstCharge }, //预置金额
#endif
			{ 0x070004FF, 0, ID_MacWrite }, //明文写信息文件
			{ 0x070005FF, 0, ID_MacWrite }, //密文写信息文件
			};
	u32 Cnt;
	u32 CmdCode;
	strRxCmd4 const* pCmd;
	u8* pData;
	pData = Ex645.SCIx->DataBuf;
	RamData.WDGTm = 0xff;
	Copy_4((u8*) &RamData.Write, pData + 6);
	*((u16*) &RamData.FErrWord) = 0;
	Copy_4((u8*) &CmdCode, pData + 2);
	pCmd = RxCmd4;
	for (Cnt = 0; Cnt < (sizeof(RxCmd4) / sizeof(RxCmd4[0])); Cnt++) {
		if (pCmd ->Code32 == CmdCode) {
			if (Cnt < 2) {
				if (!_IsIRChangel()) { //红外认证只能通过红外通道
					RamData.FErrWord = _FEW_OTHER_; // = _FEW_OTHER_;
					SetFarFail();
					return;
				}
			} else if (Cnt == 2) {
				if (_IsIRChangel()) { //红外须先做远程认证
					if (RamData.IRACCessTm == 0) {
						RamData.FErrWord = _FEW_IDERR_; //.ErrID = 1;
						SetFarFail();
						return;
					}
				}
			} else if ((_IsIRChangel()) && (RunState_IR == 0)) { //红外须进行红外认证
				RamData.FErrWord = _FEW_IDERR_;
				SetFarFail();
				return;
			} else if (RamData.IDACCess == 0) {
				RamData.FErrWord = _FEW_IDERR_;
				SetFarFail();
				return;
#ifdef	_MAC_ERR_7_								//江机表
			} else if(Cnt >= 7) { //(NUMOF(RxCmd4)-2)){
#else
			} else if (Cnt >= 9) { //(NUMOF(RxCmd4)-2)){
#endif
				if (FlashInfo.RunInfo.MacErrCnt >= 200) {
					RamData.FErrWord = _FEW_HANGUP_; //.HangUp = 1;
					SetFarFail();
					return;
				}
			}
			if ((pCmd->PubRead == 0) && (Ex645.SCIx->AddrMode)) {
				RamData.FErrWord = _FEW_OTHER_;
				SetFarFail();
				return;
			}
			CopyRam(RamData.Rand_Num, RamData.Rand_Num_Far, 16); //复制Rand_Num + IC_Num
			if (pCmd ->DoProg(pData)) {
				SetFarFail();
				return;
			}
			if ((Cnt >= 7) && (Cnt <= 8)) {
				AddProgRecord(pData + 2);
			}
			return;
		}
		pCmd++;
	}
	RamData.FErrWord = _FEW_OTHER_;
	SetFarFail();
	#endif
}

void RxWriteSPMode(void) { //设置脉冲输出功能

	if (CheckPubMode()) { //未使用本表及广播地址返回
		return;
	}
	if ((Ex645.SCIx->DataBuf[2] != 0) && (Ex645.SCIx->DataBuf[2] != 2)) {
		SetErrReturn(_ERR_OTHER_);
		return;
	}
	Ex645.SCIx->DataBuf[1] = 1;
	RamData.SP_Mode = Ex645.SCIx->DataBuf[2] ? 1 : 0;
	SP_Config(!(RamData.SP_Mode));
	return;
}

void RxTRxRead(void) { //东软读地址指令

	if ( _u16get(Ex645.SCIx->DataBuf + 2)!=REV16(0xAF02)){// ( if ((*(u16*) (Ex645.SCIx->DataBuf + 2)) != REV16(0xAF02)) {
		SetErrReturn(_ERR_NODATA_);
		return;
	}
	Ex645.SCIx->AddrMode = _ADD_LOC_;
	//(*(u16*)(Ex645.SCIx->DataBuf)) = 0x9F08;
	Ex645.SCIx->DataBuf[1] = 8;
	RevCopyRam(Ex645.SCIx->DataBuf + 4, FlashInfo.SafeInfo.TRx_Num, 6);
	return;
}

void WaitTxOver(void) {

	while (Ex645.SCIx->Act >= 2) {
		if (Event_Ms) {
			Do_MsTask(); //毫秒任务
		}
		//asm("halt");
		//	IDle();
	}
}

extern const u8 _GCKey[];
void RxProgChip(void) { //芯片编程

	WaitTxOver();
	UpDisp();
	SCIProg();
}

u8 RxClrRecord(void) { //事件清零

	static const struct {
		u32 Code;
		u8 Type;
		enum ENUMDATACONFIG cMod; //	远程、本地抄读
	} ClrTypeLst[] = {
	//		{ 0x030E01FF, _FROZE_REV_ },								//反向记录
			{ 0x031100FF, _FROZE_PWR_OFF_, _bData_All_ }, //掉电记录
			{ 0x033000FF, _FROZE_PROG_, _bData_All_ }, //编程记录
			{ 0x033004FF, _FROZE_CHG_TM_, _bData_All_ }, //时钟修改记录
			{ 0x033005FF, _FROZE_TMTBL_SET_, _bData_All_ }, //日时段表编程记录
			{ 0x033006FF, _FROZE_YTBL_SET_, _bData_All_ }, //时区表编程记录
			{ 0x033007FF, _FROZE_WEEK_SET_, _bData_All_ }, //周休日编程记录
			{ 0x033008FF, _FROZE_HLD_SET_, _bData_All_ }, //节假日编程记录
			{ 0x033009FF, _FROZE_COM_SET_, _bData_All_ }, //有功组合特征字编程记录
			{ 0x03300CFF, _FROZE_FDT_SET_, _bData_All_ }, //结算日编程记录
			{ 0x03300DFF, _FROZE_EOPEN_1_, _bData_All_ }, //开盖记录
			#ifdef		_USE_CARD_
			{ 0x03300FFF, _FROZE_TPRICE_SET_, _bData_Loc_ }, //费率电价编程记录
			{ 0x033010FF, _FROZE_LPRICE_SET_, _bData_Loc_ }, //阶梯电价编程记录
			#endif
			{ 0x033012FF, _FROZE_KEYCHG_, _bData_All_ }, //密钥更新记录
			#ifdef		_USE_CARD_
			{ 0x033013FF, _FROZE_IC_ERR_, _bData_Loc_ }, //异常插卡记录
			{ 0x033014FF, _FROZE_FL_PRICE_CHG_, _bData_Loc_ }, //非法插卡记录(特殊处理)
			{ 0x0333FFFF, _FROZE_CHARGE_, _bData_Loc_ }, //购电记录
			{ 0x033400FF, _FROZE_OFF_MONEY_, _bData_Loc_ }, //退费记录
			#endif
			{ 0x033600FF, _FROZE_JDERR_START_, _bData_All_ }, //读继电器故障记录
			{ 0x033700FF, _FROZE_PERR_START_, _bData_All_ }, //读电源异常记录
			//		{ 0x1B0100FF, _FROZE_REV_ },								//反向记录
			//		{ 0x1B0101FF, _FROZE_REV_ },
			//		{ 0x1B01FFFF, _FROZE_REV_ },
			{ 0x1D0000FF, _FROZE_JD_OFF_, _bData_All_ }, //拉闸记录
			{ 0x1D0001FF, _FROZE_JD_OFF_, _bData_All_ }, { 0x1D00FFFF,
					_FROZE_JD_OFF_, _bData_All_ }, { 0x1D0000FF,
					_FROZE_JD_OFF_, _bData_All_ }, { 0x1E0001FF,
					_FROZE_JD_OFF_, _bData_All_ }, { 0x1E00FFFF,
					_FROZE_JD_OFF_, _bData_All_ }, { 0x1E0000FF,
					_FROZE_JD_OFF_, _bData_All_ }, { 0x190100FF,
					_FROZE_OVERIM_, _bData_All_ }, //过流记录
			{ 0x190101FF, _FROZE_OVERIM_, _bData_All_ }, { 0x1901FFFF,
					_FROZE_OVERIM_, _bData_All_ },
	//		{ 0x1F0000FF, _FROZE_PFPH_ },								//超功率因数
			//		{ 0x1F0001FF, _FROZE_PFPH_ },
			//		{ 0x1F00FFFF, _FROZE_PFPH_ },
			};
	u8 N;
	u8 k;
	u8 Act;
	u32 Code;
	u8* pData;
	pData = Ex645.SCIx->DataBuf;
	k = CheckDataAct(pData + 18);
	Act = 0;
	N = 0;
	RevCopy_4((u8*) &Code, pData + 18);
	//CopyRam(ComData, FlashInfo.FrozeCnt, (sizeof(FlashInfo.FrozeCnt)
	//		/ (sizeof(char))));
	do {
		if ((k || (ClrTypeLst[N].Code == Code)) && (SUCCESS
				== Check_Datatype_Mod_Enable(ClrTypeLst[N].cMod))) { //									(ClrTypeLst[N].Code == di){
		#ifdef	_USE_CARD_
			if ((ClrTypeLst[N].Type == _FROZE_FL_PRICE_CHG_)
					&& (_ISMETER_LOCAL())) { //非法插卡次数特殊处理
				Up_u16Data(&FlashInfo.RunInfo.ErrCardCnt, 0);
			} else
			#endif
			{
				if (ClrTypeLst[N].Type == _FROZE_PROG_) {
					ClrEBit(_BIT_IN_PROG_);
				} else if (ClrTypeLst[N].Type == _FROZE_EOPEN_1_) {
					RamData.EOpen_Cnt = 0;
					RamData.EOpen_State = 0;
				} else if (ClrTypeLst[N].Type == _FROZE_OVERIM_) {
					ClrRcdTime(&FlashInfo.RunInfo.OverIphTime);
				}
				ClrFrozeCnt(ClrTypeLst[N].Type);
				//	FlashInfo.FrozeCnt[(ClrTypeLst[N].Type) & 0x3f] = 0;
			}
			Act = 1;
		}
	} while (++N < NUMOF(ClrTypeLst));
	if (Act == 0) {
		return SetErrReturn(_ERR_NODATA_);
	}
	//Copy_FlashInfo((char *) FlashInfo.FrozeCnt, ComData,
	//		(sizeof(FlashInfo.FrozeCnt) / (sizeof(char))));
	CopyRam(RamData.Buf645, (char *) &RamData.RTCTime, 4);// *(u32*)(RamData.Buf645) = RamData.RTCTime;
	RevCopyRam(RamData.Buf645 + 8, pData + 18, 4);
	CopyRam(RamData.Buf645 + 4, (char *) &RamData.Write, 4);//*(u32*)(RamData.Buf645+4) = RamData.Write;
	Up_FrozeData(RamData.Buf645, _FROZE_EVENT_CLEAR_);
	if (k) {
		static const u8 EvtClrMask[] = { 0, 0, 0, 0, 0, 0, 0x0A, 0 };
		cClrEvtState(EvtClrMask);
		SetEvtState(_EVT_EVTCLEAR_);
		ClrEBit(_BIT_LOBAT_);
	}
	return SUCCESS;
}

u8 FarRetChk(u8 *pData) {

	if ((pData[2] == 0x98) && (pData[5] == 1) && ((pData[0] & 0x3F) == 0x1C)) {
		return SUCCESS;
	}
	return FAIL;
}

u8 FarCtrlRet(u16 Err) {
	u8 *pData;u16 itemp ;
	pData = Ex645.SCIx->DataBuf;
	if (FarRetChk(pData) == SUCCESS) {
		itemp = (u8) Err;
		CopyRam(pData + 2, (u8*) &itemp, sizeof(itemp));//_ipputp((pData+2) , &(itemp)); //*(u16*) (pData + 2) = (u8) Err;
		if (Err & 0x8000) {
			pData[3] = 1;
		}
		pData[0] |= 0xC0;
		pData[1] = 2;
		return FAIL;
	}
	return SetErrReturn((u8) ((Err >> 8) & 0x7F));
}

void TRX_Ctrl_Code(void) { //控制命令处理

	static const u8 FID[] = { 0x97, 0x94, 0x97, 0x88 };
	u8 Cnt;
	u32 CntData;
	u8 *pData;
	if (Ex645.SCIx->AddrMode) { //必须使用本机地址
		FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
		return;
	}
	pData = Ex645.SCIx->DataBuf;
	Copy_4((u8*) &RamData.Write, pData + 6);
	if (pData[1] == 0x1C) {
		if (FlashInfo.RunInfo.MacErrCnt >= 200) {
			FarCtrlRet(_FCS_SAFE_ | _FCS_HANG_);
			return;
		}
		if (SUCCESS!=GetIRSafe()) {
			FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
			return;
		}
		if (pData[2] != 0x98) {
			FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
			return;
		} else if (RamData.IDACCess == 0) {
			FarCtrlRet(_FCS_SAFE_ | _FCS_IDTIME_);
			return;
		}
//		if (Get98Data(pData + 10, FID[pData[0] - 0x19], 16)) {
//			pData[2] = 0x98;
//			FarCtrlRet(_FCS_SAFE_ | _FCS_MACERR_);
//			return;
//		}
	} else if ((RamData.FacProgTime) && (strCmp((FileSet.FacKey), pData + 2, 8)
			== 0)) {
		RevStr(pData + 12, pData[1] - 10);
	} else if ((pData[0] == 0x1C) && ((pData[10] == 0x1B)
			|| (pData[10] == 0x1C))) {
		Cnt = (pData[2] >> 1) - 1;
		if ((Cnt > 1) || (FlashInfo.RunInfo.KeyErrCnt[Cnt + 1] >= 3)) {
			FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
			return;
		}
		if (strCmp(pData + 2, FlashInfo.SetInfo.TRxKey[Cnt + 1], 4)) {
			if (FlashInfo.RunInfo.KeyErrCnt[Cnt + 1] < 3) {
				Up_u8Data(&FlashInfo.RunInfo.KeyErrCnt[Cnt + 1],
						(FlashInfo.RunInfo.KeyErrCnt[Cnt + 1] + 1));
			}
			/*	CntData = FlashInfo.RunInfo.KeyErrCnt[Cnt+1]+1;
			 if(CntData >= 3){
			 CntData = RamData.RTCTime;
			 }
			 UpU32Data(&FlashInfo.RunInfo.RcdTm.KeyErrTime[Cnt+1],(u8*)&CntData);	*/
			FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
			return;
		}
		Up_u8Data(&FlashInfo.RunInfo.KeyErrCnt[Cnt + 1], 0);
		//ClrRcdTime(&FlashInfo.RunInfo.RcdTm.KeyErrTime[Cnt+1]);
		RevStr(pData + 12, pData[1] - 10);
	} else {
		FarCtrlRet(_FCS_SAFE_ | _FCS_KEYERR_);
		return;
	}
	CntData = GetTimeCnt(pData + 12, S_SEC | T_SEC);
	if (CntData < RamData.RTCTime) { //if((CntData < RamData.RTCTime)||(CntData > (RamData.RTCTime+(86400*365)))){
		FarCtrlRet(_FCS_SAFE_ | _FCS_TIMEERR_);
		return;
	}
	if (pData[0] == 0x1B) { //事件清零
		if (RxClrRecord()) { //在子程序中设置错误代码
			return;
		}
	} else if (pData[0] == 0x1A) { //电表清零
if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		SetErrReturn(_ERR_SAFE_); //本地表不支持远程清零
		return;
}else{//#else
		RamData.EvtClrTm = 0xABCD;
		RamData.Event_Feilv = 1;
}//#endif
	} else if (pData[0] == 0x1C) { //远程控制
		if (Do_FarCtrl(pData + 10)) {
			return;
		}
		if (FarRetChk(pData) == SUCCESS) {
			pData[2] = RamData.FarCtrlState % 256;
			pData[3] = RamData.FarCtrlState / 256;//  *(u16*)(pData+2) = RamData.FarCtrlState;
			pData[1] = 2;
			return;
		}
	}
	pData[1] = 0;
	return;
}

typedef struct {
	u16 Code;
	void (*DoProg)(void);
} strRxCmd;

u8 Do_OtherTRx(void) { //特殊功能处理
	static const strRxCmd RxCmd[] = { { REV16(0x0806), RxPublicWriteTime }, //广播较时
			{ REV16(0x1604), RxOrderFroze }, //冻结指令
			{ REV16(0x1701), RxChgTRXBd }, //修改波特率
			{ REV16(0x1D01), RxWriteSPMode }, //写多功能输出端子模式
			{ REV16(0x1F02), RxTRxRead }, //东软载波特殊指令
			{ REV16(0x1A0A), RxProgChip }, //485编程启动指令
			//		{ REV16(0x1A28), ChgProg },									//程序修改
			};
	u8 Cnt;
	u8 *pData;
	u16 CmdLth;
	const strRxCmd *Addr;
	Cnt = (sizeof(RxCmd) / sizeof(RxCmd[0]));
	Addr = RxCmd;
	pData = Ex645.SCIx ->DataBuf;
	CmdLth = pData[1] * 256 + pData[0];//*(u16*)pData;
	do {
		if (Addr ->Code == CmdLth) {
			if ((GetIRSafe() == SUCCESS) || (Cnt <= 1)) { //红外修改程序不需要红外认证
				Addr ->DoProg();
				return SUCCESS;
			} else {
				SetErrReturn(_ERR_SAFE_);
				return SUCCESS;
			}
		}
		Addr++;
	} while (--Cnt);
	if ((((u8) CmdLth) >= 0x19) && (((u8) CmdLth) <= 0x1C)) { //0x19~0x1C 需量清零/清零/事件清零/闸控
		TRX_Ctrl_Code();
		return SUCCESS;
	}
	/*
	if ((FlashInfo.RunInfo.EState & _ESTATE_FACOFF_) == 0) {
		if (TRx_EChip(pData) == SUCCESS) { //校表
			return SUCCESS;
		}
	}
	*/
	return FAIL;
}

const struct {
	u16 Code;
	u8 FHLst;
	u8 Bias;
	u8 Lth;
} mFHOut[] = { { 0x0101, 0, 0, 2 }, //电压
		{ 0x0201, 0, 2, 3 }, //电流
		{ 0x0300, 1, 0, 3 }, //功率
		{ 0x0500, 2, 0, 2 }, //功率因数
		{ 0x0601, 3, 0, 4 }, //正向
		{ 0x0602, 3, 4, 4 }, //反向
		};

static const u8 SrcInc[4] = { 7, 3, 2, 8 };
#ifdef _DEL
u8 *GetFHLBias(u8* Src, u8 FHMode, u8 OutMode) {

	u8 Set;
	u8 Bit;
	u8 Cnt;
	u8* pRet;

	Set = (Src[3]) >> 2;
	Bit = 1;
	pRet = Src + 4;
	for (Cnt = 0; Cnt < 3; Cnt++) {
		if (Bit >= FHMode) {
			break;
		}
		if (Set & Bit) {
			pRet += SrcInc[Cnt];
		}
		Bit <<= 1;
	}
	return pRet + mFHOut[OutMode].Bias;
}

void GetNextFHTime(u32 *pTm, u8 Step) {

	u32 NewTm;
	NewTm = ChgMOD_2(pTm, 1440);
	ChgMOD_2(&NewTm, Step);
	NewTm += Step;
	if (NewTm >= 1440) {
		NewTm = 0;
		*pTm += 1440;
	}
	*pTm += NewTm;
}
#endif
void GetFuheLine(void) 
{}
	
#ifdef _DEL
void GetFuheLine(void) { //645读取负荷曲线

	u16 D01;
	u8 Cnt;
	u8 BLKNum;
	u16 FHBias;
	u32 FHTm;
	u32 ActUse;
	u8* BkpChar;
	u8* pFHRcd;
	u8* pData;
	u8 FHLst;
	u8 FHMode;
	u8 FHStep;
	u8 OutLth;
	u8 OutMode;
	u32 OutFHTm;
	u32 LastFHTm;
	u16 LastFHAddr;
	u32 OldFHTm;
	u16 Option;
	u8 FirstBlock;
	u8 RxBuf[81]; //最长负荷记录81字节
	u8 Start;
	u8 UseStart;
	u8 Over;
	u8 FHAct;
	u8 N;
	D01 = Ex645.Code645.Code & 0xFFFF;
	pData = Ex645.SCIx->DataBuf;
	N = 0;
	while (1) {
		if (D01 == mFHOut[N].Code) { //获取负荷记录类型
			FHLst = mFHOut[N].FHLst;
			OutLth = mFHOut[N].Lth;
			Option = _RET_ | _REV_ | BCD_OUT(OutLth, OutLth, 0);
			OutMode = N;
			FHStep = FlashInfo.SetInfo.FHTmStep[FHLst];
			break;
		}
		if (++N >= NUMOF(mFHOut)) {
			SetErrReturn(_ERR_NODATA_);
			return;
		}
	}
	LastFHTm = RamData.FHSet.LastFH[FHLst + 1].FHTime;
	LastFHAddr = RamData.FHSet.LastFH[FHLst + 1].FHAddr;
	FHMode = 1 << FHLst;
	if (pData[0] == 0x12) {
		Ex645.Frm645Read = pData[pData[1] + 1];
	} else {
		if (CheckBCD(pData + 6, 6)) {
			SetErrReturn(_ERR_OTHER_);
			return;
		}
		Ex645.Frm645Read = 0;
		RamData.RdFHBlks = HEX(pData[6]);
		RamData.RdFHTime = GetTimeCnt(pData + 7, _REV_ | S_MIN | T_MIN) - 1;
		GetNextFHTime(&RamData.RdFHTime, FHStep);
	}
	BLKNum = Get_FHFrozeBlock();
	if (BLKNum == 0xFF) {
		RamData.RdFHBlks = 0;
	}
	Ex645.Frm645 = 0;
	pData[1] = 4;
	RamData.WDGTm = 20;
	FHBias = _FH_BLTH_;
	RamData.Read_FH = 1;
	N = RamData.RdFHBlks;
	//	BLKNum = RamData.RdFHNum;
	FirstBlock = ((RamData.FHSet.LastFH[0].FHAddr / _FH_BLTH_) + 1)
			% _FH_BLOCKS_;
	GetFrozeMinCnt((char *)&OutFHTm);
	OutFHTm = RamData.RdFHTime;
	OldFHTm = 0;
	ActUse = 0;
	Start = 0;
	UseStart = 0;
	Over = 0;
	while (N) {
		if ((RamData.RdFHTime > LastFHTm) && ((FlashInfo.SetInfo.FHMode
				& FHMode) == 0)) {
			N = 0;
		} else if (RamData.RdFHTime == LastFHTm) {
			Read_FHRcd(ComData, LastFHAddr, 30);
			pFHRcd = ComData;
			FHTm = GetFHTime(pFHRcd);
			Over = 1;
		} else if (FHBias >= _FH_BLTH_) {
			Read_FHRcd(ComData, BLKNum * _FH_BLTH_, _FH_BLTH_);
			FHBias = 0;
			pFHRcd = ComData;
			FHTm = GetFHTime(pFHRcd);
			if ((FHTm == 0) || (FHTm <= OldFHTm)) {
				FHTm = 0;
				Over = 1;
			}
		} else {
			pFHRcd = ComData + FHBias;
			FHTm = GetFHTime(pFHRcd);
			if (FHTm == 0) {
				BLKNum = (BLKNum + 1) % _FH_BLOCKS_;
				FHBias = _FH_BLTH_;
				continue;
			}
		}
		while (N) {
			if (((OutFHTm - RamData.RdFHTime) / FHStep) >= RamData.RdFHBlks) {
				Over = 1;
				break;
			}
			if ((pFHRcd[3]) &(FHMode << 2)) {
				FHAct = 1;
			} else {
				FHAct = 0;
			}

			if ((FHTm) && (FHAct) && (OutFHTm >= FHTm) && ((BLKNum
					!= FirstBlock) || (FHBias > (RamData.FHSet.LastFH[0].FHAddr
					% _FH_BLTH_)))) {

				if ((pFHRcd[3]) & 0x20) {
					Copy_4((u8*) &ActUse, (u8*) GetFHLBias(pFHRcd, FHMode,
							OutMode));
				}
				UseStart = 1;
			}
			if ((OutFHTm > FHTm) && ((Over == 0) || (OutFHTm
					> RamData.FrozeTime))) {
				break;
			}
			if (UseStart) {
				if (Start == 0) {
					if (Ex645.Frm645Read == 0) {
						N -= ((OutFHTm - RamData.RdFHTime) / FHStep);
						RamData.RdFHBlks = N;
						RamData.RdFHTime = OutFHTm;
					} else {
						OutFHTm = RamData.RdFHTime;
					}
					GetCntTime(RxBuf, OutFHTm, _REV_ | S_MIN | T_YMDHM);
					Check645Length(RxBuf, 5);
					Start = 1;
				}
				BkpChar = tpChar;
				tpChar = RxBuf;
				if ((FHTm == OutFHTm) && (FHAct)) {
					MvData(GetFHLBias(pFHRcd, FHMode, OutMode), Option);
				} else {
					if (OutMode >= 4) {
						MvData((u8*) &ActUse, _RET_ | _REV_ | BCD_OUT(4, 4, 0));
					} else {
						SetpCharRam(OutLth);
					}
				}
				N--;
				tpChar = BkpChar;
				if (Check645Length(RxBuf, OutLth) == SUCCESS) {
					N = 0;
					break;
				}
			}
			GetNextFHTime(&OutFHTm, FHStep);

		}
		if (Over) {
			break;
		}
		FHBias += GetFHLth(pFHRcd[3]);
		if (FHBias >= (_FH_BLTH_ - 4)) {
			BLKNum = (BLKNum + 1) % _FH_BLOCKS_;
			FHBias = _FH_BLTH_;
		}
		if (RamData.WDGTm < 3) {
			break;
		}
		OldFHTm = FHTm;
	}
	SetReadOver();
	RamData.Read_FH = 0;
}
#endif
 
void Do_SCI(void) {

	unsigned short itemp;
	u16 CmdLth;
	u8 Cnt;
	u8 NCnt;
	/*
	 Ex645.WBit = 0;
	 if(Ex645.SCIx->AddrAct == 0)
	 {											//接收帧检测
	 while(1)
	 {
	 //if(Ex645.SCIx->Head < 12)
	 //{
	 //	Ex645.SCIx->AddrMode = 0;
	 //	Ex645.SCIx->TRxEvt = 0;
	 //	return;
	 //}
	 if((Ex645.SCIx->DataBuf[0] == 0x68)&&(Ex645.SCIx->DataBuf[7] == 0x68)){
	 //	&&((Ex645.SCIx->DataBuf[9]+12) <= sizeof(Ex645.SCIx->DataBuf))){//不接收超长帧
	 if(Ex645.SCIx->Head < (Ex645.SCIx->DataBuf[9]+12))
	 {
	 Ex645.SCIx->AddrMode = 0;
	 Ex645.SCIx->TRxEvt = 0;
	 return;
	 }
	 //if((Ex645.SCIx->DataBuf[Ex645.SCIx->DataBuf[9]+11] == 0x16)
	 //		&&(Ex645.SCIx->DataBuf[Ex645.SCIx->DataBuf[9]+10] == (u8)GetSum(Ex645.SCIx->DataBuf, Ex645.SCIx->DataBuf[9]+10))){
	 //	Ex645.SCIx->CSTime = RamData.msTime;
	 if(Chk_TRxNum() == SUCCESS)
	 {
	 for ( itemp  =7 ; itemp <Ex645.SCIx->DataBuf[9]+11; itemp )
	 Ex645.SCIx->DataBuf[Itemp -7] =Ex645.SCIx->DataBuf[Itemp ] ;
	 //break;
	 }
	 //SubSCIBuf(Ex645.SCIx->DataBuf[9]+11);
	 }
	 }
	 Chk_Rx68();
	 }
	 }
	 */
	if (_IsIRChangel())
					RamData.Event_IRKey=1;
	if (Chk_TRxNum() != SUCCESS) {
		enum ENUMCHANDEL GetSCINUM(strSCIBuf *p);
		Cnt = GetSCINUM(Ex645.SCIx);
		_RSet_Safe(USARTCOM[Cnt].cSilence,1);//  stream_refresh (Cnt)   ;
		return;
		//for ( itemp  =7 ; itemp <Ex645.SCIx->DataBuf[9]+11; itemp )
		//	Ex645.SCIx->DataBuf[itemp -7] =Ex645.SCIx->DataBuf[itemp ] ;
		//break;
	}
	RamData.bClr_Comm = 0;
	CmdLth = Ex645.SCIx->DataBuf[0] + Ex645.SCIx->DataBuf[1] * 256;// *(u16*)(Ex645.SCIx->DataBuf);
	//Ex645.Code645.Code =
	Copy_4((u8*) &Ex645.Code645.Code, Ex645.SCIx->DataBuf + 2);
	Ex645.Code645.SubCode = 0;
	if (CmdLth == REV16(0x1300)) { //读通讯地址
		RevCopyRam(Ex645.SCIx->DataBuf + 2, FlashInfo.SafeInfo.TRx_Num, 6);
		Ex645.SCIx->DataBuf[1] = 6;
		Ex645.SCIx->AddrMode = 3;
		RxSend645();
		return;
	} else if (RamData.TRx_Wait) {
		return;
#define  _readmemory()	 	((CmdLth==REV16(0x110A)) && (0xf3==Ex645.SCIx->DataBuf[3]) &&(0x0==Ex645.SCIx->DataBuf[4]) && (0x4 ==Ex645.SCIx->DataBuf[5]))
	} else if ((CmdLth == REV16(0x1104)) || _readmemory() || ((CmdLth == REV16(0x1205)) && (Ex645.SCIx->DataBuf[5] != 6))) { //读数据
		u8* BkpChar;
		u8* BkpBase;
		Ex645.Frm645 = 0;
		if (Ex645.SCIx->DataBuf[0] == 0x12) {
			Ex645.Frm645Read = Ex645.SCIx->DataBuf[6];
		} else {
			Ex645.Frm645Read = 0;
		}
		BkpChar = tpChar;
		BkpBase = tpBase;
		NCnt = 0;
		if ((Ex645.SCIx->DataBuf[2] == 0xFF)
				|| (Ex645.SCIx->DataBuf[3] == 0xFF)) {
			static u8 RxBuf[_cFr645BUF_Len_ + 20];
			u16 Bias;
			u8 Lth;
			u8 EvLth, EvCnt;
			u8 LFCnt;
			RamData.WDGTm = 20;
			EvLth = 0;
			EvCnt = 0;
			Ex645.SCIx->DataBuf[1] = 4;
			Bias = 0;
			if (Ex645.SCIx->DataBuf[2] == 0xFF) {
				RamData.Read_F0 = 1;
			} else {
				RamData.Read_F1 = 1;
			}
			for (Cnt = 0; Cnt <= 254; Cnt++) { //读数据块
				//if(Ex645.SCIx->DataBuf[2] == 0xFF){
				if (RamData.Read_F0) {
					//Ex645.Code645.Code = (Ex645.Code645.Code&(~((u32)0xFF)))|Cnt;
					((u8*) &Ex645.Code645.Code)[0] = Cnt;
				} else {
					//Ex645.Code645.Code = (Ex645.Code645.Code&(~((u32)0xFF00)))|(((u16)Cnt)<<8);
					((u8*) &Ex645.Code645.Code)[1] = Cnt;
				}
				//				if((EvLth)&&(EvCnt >= 5)&&((Ex645.Frm645+1) < Ex645.Frm645Read)){
				if ((EvLth) && (EvCnt >= 5) && (LFCnt != RamData.LastFCnt)
						&& (Ex645.Frm645 < Ex645.Frm645Read)
						&& (Ex645.SCIx->DataBuf[1] < (_cFr645BUF_Len_ - EvLth))) {
					tpChar = RxBuf + EvLth;
				} else {
					tpChar = RxBuf;
					LFCnt = RamData.LastFCnt;
					Get645Data(&Ex645.Code645);
				}
				if (tpChar == RxBuf) {
					if (++NCnt > 17) {
						break;
					}
				} else {
					Lth = (u8) (tpChar - RxBuf);
					if (Check645Length(RxBuf, Lth) == SUCCESS) {
						break;
					}
					NCnt = 0;
					if (EvLth == Lth) {
						EvCnt++;
					} else {
						EvLth = Lth;
						EvCnt = 0;
					}
				}
			}
			RamData.Read_F0 = 0;
			RamData.Read_F1 = 0;
			if (Ex645.SCIx->DataBuf[1] >= 4) {
				SetReadOver();
			}
		} else {
			tpChar = Ex645.SCIx->DataBuf + 6;
			Get645Data(&Ex645.Code645);
			if (Ex645.SCIx->DataBuf[1] >= 4) {
				Ex645.SCIx->DataBuf[1] = (u8) (tpChar - (Ex645.SCIx->DataBuf
						+ 2));
				SetReadOver();
			}
		}
		RxSend645();
		tpChar = BkpChar;
		tpBase = BkpBase;
		return;
	} else if ((RamData.InMsTask) || (RamData.InDisp)) { //其他指令不在毫秒任务中执行/不参与显示
		return;
	}
	/*			以上部分程序可在毫秒任务中执行			*/
	RamData.Mac_99 = 0;
	RamData.Mac_98 = 0;
	Ex645.ExLth = 0;
	if (Do_OtherTRx() == SUCCESS) { //特殊指令处理
#define		_RD_CODE_	0x04040300
	} else if (Ex645.SCIx->DataBuf[0] == 0x03) { //认证充值
		IC_645Commniucate();
	} else if (SUCCESS!=GetIRSafe()) { //以后必须在红外认证后执行
		SetErrReturn(_ERR_SAFE_);
	} else if ((Ex645.SCIx->DataBuf[0] == 0x11) && (Ex645.Code645.Code
			== _RD_CODE_)) {//显示项设置
		if (Ex645.SCIx->DataBuf[1] == 0x09) {
			RxSetDisp();
		} else {
			SetErrReturn(_ERR_OTHER_);
		}
	} else if (((CmdLth == 0x0A11) || (CmdLth == 0x0512))
			&& (((u16) (Ex645.Code645.Code >> 16)) == 0x0610)) {
		GetFuheLine();
	} else if (Ex645.SCIx->AddrMode) { //以下操作必须使用本机地址
		SetErrReturn(_ERR_SAFE_);
	} else if ((Ex645.SCIx->DataBuf[0] == 0x14)
			&& (Ex645.SCIx->DataBuf[5] == 4)) { //写数据
		Ex645.WBit = 1;
		Write645Data();//&Ex645.Code645);
		Ex645.WBit = 0;
	} else if (SUCCESS!=GetIRSafe()) { //以后必须在红外认证后执行
		SetErrReturn(_ERR_SAFE_);
#ifdef		_USE_24_CODE_
	} else if(CmdLth == 0x180C) { //修改密码
		if((Ex645.Code645.Code >= 0x04000C01)&&(Ex645.Code645.Code <= 0x04000C05)) {
			if(RamData.IDACCess == 0) {
				SetErrReturn(_ERR_SAFE_);
			} else {
				Ex645.WBit = 1;
				ChgKey(&Ex645.Code645);
				Ex645.WBit = 0;
			}
		} else {
			SendErrReturn(_ERR_NODATA_);
			return;
		}
#endif		//_USE_24_CODE_
	} else {
		SendErrReturn(_ERR_NODATA_);
		return;
	}
	RxSend645();
//#ifndef		_USE_CARD_
	if((RamData.EvtClrTm == 0xABCD)  &&(!_ISMETER_LOCAL()))
	{
		void Clr_AllUse(void);
		Clr_AllUse();
		RamData.EvtClrTm = 0;
	}

//#endif
	#ifdef _USE_CARD_
if ( (_ISMETER_LOCAL())) 
		Clr_Comm();
#endif
}
 
strSCIBuf SCI[3]; 
void Do_TRx(void) { //通讯处理
	
	  //extern strSCIBuf SCI[3];
	
	u8 Cnt, i,j;
	u16 Tm;
	/*for(Cnt = 0; Cnt < 3; Cnt++){
	 strSCIBuf *pSCI;
	 pSCI = &SCI[Cnt];
	 if(pSCI ->TRxEvt){
	 Tm = (RamData.msTime - pSCI ->TRxTm);
	 if(pSCI ->Act == 0){
	 pSCI ->TRxEvt = 0;
	 } else if(pSCI ->Act == 1){
	 u8	BkBuf[sizeof(Ex645)];
	 CopyRam( BkBuf, (u8*)&Ex645, sizeof(Ex645));
	 Ex645.SCIx = pSCI;
	 Do_SCI();
	 CopyRam( (u8*)&Ex645, BkBuf, sizeof(Ex645));
	 } else if(pSCI ->Act == 2){
	 if(Tm > 16){										//接收80毫秒后发送数据
	 pSCI ->Act = 3;
	 Send_SCI(pSCI);
	 }
	 }
	 }
	 }*/

	/*
	 * typedef	struct {
	 u32		FastData;
	 strSCIBuf *SCIx;						//当前处理SCI指针
	 u8		Frm645;							//645帧数
	 u8		Frm645Read;						//645后续帧
	 u8		ExLth;							//扩展长度(用于645数据分组调整)
	 strDispCode	Code645;
	 u8		WBit;							//645写标志
	 } strEx645;
	 *
	 *
	 * =================
	 *
	 * typedef struct {
	 u16			TRxTm;						//
	 u16			CSTime;						//
	 GBit		Act:2;						//当前状态(0-等待接收,1-接收,2-等待发送,3-发送
	 GBit		AddrMode:3;					//地址状态(0-本机地址,1-AA匹配地址,2-广播地址,3-校表地址
	 #define		_ADD_LOC_		0
	 #define		_ADDR_99_		2
	 #define		_ADDR_6162_		3
	 #define   _broadcast99Addr     ( _ADDR_99_ == Ex645.SCIx->AddrMode)	// 全9地址
	 #define   _localAddr           ( _ADD_LOC_ == Ex645.SCIx->AddrMode)	// 本地地址
	 GBit	AddrAct:1;						//地址匹配(0-未匹配 1-匹配)
	 GBit	TRxEvt:1;						//接收事件发生
	 GBit	NoRet:1;						//无返回
	 u8			Head;						//
	 u8			SendLth;					//
	 u8			Rev;						//4字节对齐
	 u8	*DataBuf;//[220]; 						//最大支持220字节长度数据
	 }strSCIBuf;
	 *
	 */
	strSCIBuf *pSCI;
	for (j = _Com_start_; j < _Com_End_; j++) {
			i  = j -_Com_start_;
		if (USARTCOM[i].bEventRec645) {
			//memset((char *) (SCI + i), 0, sizeof(SCI[0]));
			ClrRam((char *) (SCI + i),   sizeof(SCI[0]));
			SCI[i].Head = USARTCOM[i].cHead;
			SCI[i].DataBuf = USARTCOM[i].DataBuf;
			Ex645.SCIx = (SCI + i);
			Do_SCI();
		}else
		{
			if(( _IR_Chanel_ == i ) && ( 0 == RamData.Event_IRKey )  && ( USARTCOM[i].cStatu >= _second_68_))
					  RamData.Event_IRKey=1;
		}

	}

	//pxopt if((SCI[0].TRxEvt == 0)&&(SCI[1].TRxEvt == 0)&&(SCI[2].TRxEvt == 0)){
	//pxopt 	Event_USART = 0;
	//pxopt }
}
/*
 void	Chk_TRx(void){												//通讯超时处理

 u16	Tm;
 u16	Tm2;
 u8	Cnt;return ;
 for(Cnt = 0; Cnt < 3; Cnt++){
 if(SCI[Cnt].TRxTm){
 Tm = (RamData.msTime - SCI[Cnt].TRxTm);
 Tm2 = (RamData.msTime - SCI[Cnt].CSTime);
 if(Tm >= 400){											//总线空闲2秒复位
 Rst_SCI(Cnt);
 } else if((Tm2 > 400)&&(Tm > 10)){						//超时2秒未收到正确数据且总线空闲50毫秒复位
 Rst_SCI(Cnt);
 }
 }
 }
 }
 */

u8 WriteEXTEEPROM(strDispCode const *pCode, u8* pAddr) { //写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;
 
	Init_FlashData();
	Reset_LVSave();

	Ex645.SCIx->DataBuf[1] = 0;
	RamData.DelayRst = 5;
	return SUCCESS;
}

u8 ReadFlashData(strDispCode const *pCode, u8* pAddr) { //写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;
	unsigned long lAddr = 0;
	lAddr = _slpgetp(Ex645.SCIx->DataBuf+6);
	lAddr &= 0xffffff;

	CopyRam(Ex645.SCIx->DataBuf + 6, (u8 *) (lAddr), 32);
	tpChar = Ex645.SCIx->DataBuf + (32 + 6);
	return SUCCESS;
}

u8 ReadExtEEprom(strDispCode const *pCode, u8* pAddr) { //写设置参数
	extern const unsigned short iEEPROM_MAXADDR; //	外部E方最大地址
	u8 D0;
	u8 Cnt;
	u8 *pData;
	unsigned long lAddr = 0;
	lAddr = _slpgetp(Ex645.SCIx->DataBuf+6);
	lAddr &= 0xffffff;
	if (lAddr < iEEPROM_MAXADDR) {
		CopyExteeprom( Ex645.SCIx->DataBuf +6 , (lAddr) ,32);
		tpChar = Ex645.SCIx->DataBuf + (32 + 6);
		return SUCCESS;
	}
	return FALSE;
}

u8 Write_meterconfig(strDispCode const *pCode, u8* pAddr) { //读写设置参数

	u8 D0;
	u8 Cnt;
	u8 *pData;
	struct STMETER_CONFIG *sourcep;
	struct STMETER_CONFIG *desp;

	D0 = (u8) (pCode->Code);
	pData = Ex645.SCIx->DataBuf + 6;
	Cnt = 0;

	sourcep = (struct STMETER_CONFIG*) pAddr;
	desp = (struct STMETER_CONFIG*) pData;

	//  --if ( 0!=strCmp((char *)pAddr ,(char *)desp ,sizeof(struct STMETER_CONFIG)))//  if (*((u32 *) sourcep) != *((u32 *) desp))
	//  --	DispData_init(desp->cMeter_Type, desp->cMeter_ComType);

	Copy_FlashInfo(pAddr, pData, _METERCONFIG_LEN_);

	Ex645.SCIx->DataBuf[1] = 0;
	RamData.DelayRst = 5;
	return SUCCESS;
}

u8 WriteFHStartTm(strDispCode const *pCode, u8* pAddr) {

	u8 Tm[8];
	u32 Cnt;
	Tm[0] = 0;
	Tm[5] = 0;

	CopyRam(Tm + 1, (u8*) &Ex645.FastData, 4);
	Cnt = GetTimeCnt(Tm, _REV_ | T_SEC);
	if (Cnt == 0) {
		return SetErrReturn(_ERR_OTHER_);
	}
	Up_u32Data(&FlashInfo.SetInfo.FuheStartTm, Ex645.FastData);
	//ClrEBit(_BIT_FUHE_);
	return SUCCESS;
}


u8 Check_Datatype_Mod_Enable(enum ENUMDATACONFIG cmod) {
	unsigned char ctemp = (1
			<< (FlashInfo.meterinfo.stmeter_config.cMeter_Type));
	if (cmod & ctemp)
		return SUCCESS;
	return FAIL;

}

u8 ReadEEPROMFData(strDispCode const *pCode, u8* pAddr) { //读电表参数信息

	strDftSet sttemp;
	u8 Num;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt, clen;

	u32 ltemp;

//----	CopyExteeprom( (char*)&sttemp, _Dftset_start_ ,sizeof(sttemp));

	ltemp = (u32) & (FData.DftSet);

	switch ((u8) (pCode->Code)) {
	case 9: //脉冲常数
		clen = 3; sttemp.kWhImp = FData.DftSet.kWhImp;
		Hex_BCD((u32) (sttemp.kWhImp), (u8*) &sttemp.kWhImp);
		RevCopy((char *) &sttemp.kWhImp, clen);
		break;

	case 0xb: //电表型号
		clen = 10;
		RevCopy((char *) &sttemp.IMType, clen);
		break;

	case 0xc: //生产日
		clen = 10;
		RevCopy((char *) &sttemp.MakeTime, clen);
		break;

	case 0xd: //协议版本
		clen = 16;
		RevCopy((char *) &sttemp.VerSion, clen);
		break;

	default:
		return FAIL;
	}

	return SUCCESS;

}

u8 RTC_DIV_READ(strDispCode const *pCode, u8* pAddr)
{ //读电表参数信息


	u16 itemp;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt,clen;
	u8* pData;
	u32 ltemp ;
		pData = Ex645.SCIx->DataBuf;
	
//	ltemp = RTC->FDIV;
	if ( 0 == Ex645.WBit)
	{
		
			NormCopy((char *)&ltemp , 2);
	}else
	{
			ltemp = 0;
			CopyRam ( (char *)&ltemp ,  (pData + 6), 2);
		//Copy_FlashInfo(&FlashInfo.iOSC16_OFFSET , (pData + 6), sizeof(FlashInfo.iOSC16_OFFSET ));
//		RTC->FDIV = ltemp;
		 
		 return SUCCESS;
	}



return SUCCESS;

}



u8 RTC_Trim_Enable(strDispCode const *pCode, u8* pAddr)
{ //读电表参数信息

#ifdef _DEL

	u16 itemp;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt,clen;
	u8* pData;
	u32 ltemp ;
		pData = Ex645.SCIx->DataBuf;
	
	
	ltemp = RTC->FDIV;
	if ( 0 == Ex645.WBit)
	{
		
			NormCopy((char *)&RamData.cTrimEnable , 1);
	}else
	{
			ltemp = 0;
			CopyRam((char *)&RamData.cTrimEnable ,  (pData + 6), 1);		 
		 return SUCCESS;
	}

#endif

return SUCCESS;

}


u8 RTCOFFSETREAD(strDispCode const *pCode, u8* pAddr)
{ //读电表参数信息
#ifdef _DEL

	u16 itemp;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt,clen;
	u8* pData;
		pData = Ex645.SCIx->DataBuf;
	if ( 0 == Ex645.WBit)
	{
		
			NormCopy((char *)&FlashInfo.iOSC16_OFFSET , sizeof(FlashInfo.iOSC16_OFFSET));
	}else
	{
			//itemp = (*(pData + 6))*256 + &0x1f;
		
		Copy_FlashInfo(&FlashInfo.iOSC16_OFFSET , (pData + 6), sizeof(FlashInfo.iOSC16_OFFSET ));
		 RamData.DelayRst=2;
		 return SUCCESS;
	}

#endif

return SUCCESS;

}

u8 RTCOFFSEINIT(strDispCode const *pCode, u8* pAddr)
{ //读电表参数信息

#ifdef _DEL
	u16 itemp;
	u8 L;
	u8 k;
	u8 Lst[8];
	u16 Data;
	u8 B_Data[5];
	u8 N, Cnt,clen;
	u8* pData;
		pData = Ex645.SCIx->DataBuf;
	if ( 0 == Ex645.WBit)
	{
		//	Rtcunint_offsetRead(&ctemp);
		//	if ( ctemp != FlashInfo.cRTCOFFSET)
		//			Copy_FlashInfo(&FlashInfo.cRTCOFFSET,&ctemp,sizeof(FlashInfo.cRTCOFFSET));
			RevCopy((char *)&FlashInfo.cRTCOutMode , 1);
	}else
	{
			Copy_FlashInfo(&FlashInfo.cRTCOutMode,pData+6,1);
			 OSC16_init() ;// OSC16_Rfresh();
		 return SUCCESS;
	}

#endif

return SUCCESS;

}

u8 WriteRTCSet(strDispCode const *pCode, u8* pAddr) 
{ //读写设置参数
#ifdef _DEL
	u8 D0;
	u8 Cnt;
	u8 *pData;
	
	pData = Ex645.SCIx->DataBuf + 6;
	 if ( 0 == Ex645.WBit)
	 {
		RevCopy((char *)FlashInfo.sRTCTrim , sizeof(FlashInfo.sRTCTrim));
		return SUCCESS;
	 }
	Copy_FlashInfo((char *)FlashInfo.sRTCTrim , pData, sizeof(FlashInfo.sRTCTrim));
	Ex645.SCIx->DataBuf[1] = 0;
	RamData.DelayRst = 5;
	 #endif
	return SUCCESS;
}

u8	GetSafeData(u8 *pData, u8 Lth) {return 0;}
	u8		Chk_Mac_99(void)  { return 0;}
	
	
	
#ifdef		_USE_24_CODE_	//??02 04???
 

u8	ChkKey( u8* Key){												//????

	u32	Cnt;
	u8	KeyNum;

	if(Key[0] == 0){
		KeyNum = 0;
	} else if(Key[0] == 2){
		KeyNum = 1;
	} else if(Key[0] == 4){
		KeyNum = 2;
	} else {
		return 0;
	}
	if(FlashInfo.RunInfo.RcdTm.KeyErrTime[KeyNum] > 3){
		return 0;
	}
	if(strCmp( Key, FlashInfo.SetInfo.TRxKey[KeyNum], 3)){
		if ( 1) { //if(FlashInfo.RunInfo.RcdTm.LastProgTime){
			Cnt = FlashInfo.RunInfo.RcdTm.KeyErrTime[KeyNum]+1;
			if(Cnt >= 3){
				Cnt = RamData.RTCTime;
			}
			Up_u32Data(&FlashInfo.RunInfo.RcdTm.KeyErrTime[KeyNum], Cnt);
		//	Up_u32Data
		}
		return 0;
	}
	ClrRcdTime(&FlashInfo.RunInfo.RcdTm.KeyErrTime[KeyNum]);
	return KeyNum+1;
}

void ChgKey(strDispCode const *pCode){								//180C????

	u8*	pData;
	pData = Ex645.SCIx->DataBuf ;//RamData.SCIx->DataBuf;
	if((pData[6] > 9)||(pData[10]>9)||(pData[6]>pData[10])){
		SetErrReturn(_ERR_SAFE_);
		return;
	} else if((pData[6] != 2)&&(pData[6]!= 4)){
		SetErrReturn(_ERR_NODATA_);
		return;
	}
	if(ChkKey(pData+6) == 0){
		SetErrReturn(_ERR_SAFE_);
		return;
	}
	RamData.Write = 0;
	CopyRam( pData +6, pData+10, 4);
	RxWriteKey( pCode, 0);
}
#endif		//_USE_24_CODE_		??02 04???


u8 Jlregread(strDispCode const *pCode, u8* pAddr) { //??????
 unsigned long ltemp;
	u8 D0,D1,ctemp;
	char stemp[8];
	u8 *pData;
        unsigned long lAddr;
        D0 = (u8) (pCode->Code);
        D1=  (u8) (((pCode->Code)>>8)&0xf);
        lAddr = D0;//(unsigned long )HT_EMUEPA + D0;      
				
			pData = Ex645.SCIx->DataBuf + 6;
        
        if (Ex645.WBit) 
        {
            
                Ex645.SCIx->DataBuf[1] = 0;
        }
        else
        {
          if ( lAddr &0x3 )
					{
						SetErrReturn(_ERR_SAFE_);
					}else
					{
							ltemp=Read_EPADR(lAddr);                
							NormCopy((char *)&ltemp  ,4);
					}
          	return SUCCESS;//SetErrReturn(_ERR_SAFE_);
        }
	return SUCCESS;
}
	
u8 Jbregread(strDispCode const *pCode, u8* pAddr) { //??????
 unsigned long ltemp;
	u8 D0,D1,ctemp;
	char stemp[8];
	u8 *pData;
        unsigned long lAddr;
        D0 = (u8) (pCode->Code);
        D1=  (u8) (((pCode->Code)>>8)&0xf);
        lAddr = D0;//(unsigned long )HT_EMUECA  + D0;      
				if ( 6 == D1)
					lAddr +=0x100;
				
			pData = Ex645.SCIx->DataBuf + 6;
        	if ( lAddr &0x3 )
					{
					
						SetErrReturn(_ERR_SAFE_);
					}
					
        if (Ex645.WBit) 
        {
						RevCopyRam ( (char *)&ltemp , pData , sizeof(ltemp));
						Write_ECADR ( lAddr , ltemp);			
						Emu_Chip_to_StructData();		
						emu_ecr_reg.ECR.CheckSum = Read_EPADR(EPR_Checksum);					
            Ex645.SCIx->DataBuf[1] = 0;
					Emu_Var_Write();
        }
        else
        {       
					{					
								ltemp=Read_ECADR(lAddr);                
								NormCopy((char *)&ltemp  ,4);
					}
         	return SUCCESS;//SetErrReturn(_ERR_SAFE_);
        }
	return SUCCESS;
}


u8 Jlrestore(strDispCode const *pCode, u8* pAddr) { 
 unsigned long ltemp;
	u8 D0,D1,ctemp;
	char stemp[8];
	u8 *pData;
        unsigned long lAddr;
        D0 = (u8) (pCode->Code);
        D1=  (u8) (((pCode->Code)>>8)&0xf);
        lAddr = D0;//      
				
			pData = Ex645.SCIx->DataBuf + 6;
        
        if (Ex645.WBit) 
        {
								if ( 0x55 == pData[0])
									sEmuc_ResotreDef();
								if ( 0x56 == pData[0])
								{
									sEmuc_ResotreDef();
									Init_FlashData();
									Reset_LVSave();
								}
									
                Ex645.SCIx->DataBuf[1] = 0;
        }
        else
        {
            Ex645.SCIx->DataBuf[1] = 0;
          	return SUCCESS;//SetErrReturn(_ERR_SAFE_);
        }
	return SUCCESS;
}


u8 effective_value(strDispCode const *pCode, u8* pAddr) { 
 unsigned long ltemp;
	u8 D0,D1,ctemp;
	char stemp[8];
	u8 *pData;
        unsigned long lAddr;
        D0 = (u8) (pCode->Code);
        D1=  (u8) (((pCode->Code)>>8)&0xf);
        lAddr = D0;//      
				
			pData = Ex645.SCIx->DataBuf + 6;
        
        if (Ex645.WBit) 
        {
								if ( 0x56 == pData[0])
								{
								Chip_Trim_IB();
										Emu_Var_Write();
								}
                Ex645.SCIx->DataBuf[1] = 0;
        }
        else
        {
            Ex645.SCIx->DataBuf[1] = 0;
          	return SUCCESS;//SetErrReturn(_ERR_SAFE_);
        }
	return SUCCESS;
}
