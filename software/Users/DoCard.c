#include "PubSet.h"
 
#include "docard.h"
//#include "cardtype.h"

#if   _CARDTYPE_  == _CARDCPU_
 void cFuntion_NULL(unsigned char );
#define				_EOTHER_			50
extern  const  strFileSet FileSet;
const __code strRWSamic Cmd0088 = {0};//{{ 0, 0x88, 0, 0, 0x08 }, WRITE_IC, RamData.Rand_Num };
const __code strRWSamic ReadCmd = {{0x00,0xB0,0x81,0x00,0x04}, READ_IC, ComData };
#ifdef DEL
const __code u8	MacTmTbl[4][6] = {
			{	0x83, FL_TBL_1_ID, 0,	2,		2,		130	},	//4,		4,		128	},
			{	0x84, FL_TBL_2_ID, 0,	2,		2,		130	},	//4,		4,		128	},
			{	0x83, FL_TBL_1_ID, 0,	132,	132,	64	},
			{	0x84, FL_TBL_2_ID, 0,	132,	132,	69	},
};
#endif
#define			_MACTBL_LENGTH_			(130+130+64+69+(4*4)+3)		//费率阶梯表长度(扩展至4字节对齐)
#

#define			UserCard	(*((strUserCard *)ComData))
#define			vUserCard	(*((strUserCard *)0))
#define			bUserCard	(*((strUserCard *)PBASE_ADDR))

#define			SetupCard	(*(strSetupCard *)ComData)

//#define			ESafeInfo	(*(strSafeInfo *)(ComData +sizeof(ComData)-sizeof(strSafeInfo)))
//#define			vESafeInfo	(*(strSafeInfo *)(vComData +sizeof(ComData)-sizeof(strSafeInfo)))

#define			ESafeInfo	(*(strSafeInfo *)(ComData +sizeof(strUserCard)+_MACTBL_LENGTH_))
#define			vESafeInfo	(*(strSafeInfo *)(vComData +sizeof(strUserCard)+_MACTBL_LENGTH_))

//#define			ERunInfo 	(*((strRunInfo *)(ComData+(sizeof(ComData)-(sizeof(strSafeInfo)+sizeof(strRunInfo))))))
#define			ERunInfo 	(*((strRunInfo *)(ComData+3+(sizeof(strUserCard)))))
#define			vESAMInfo	(*(strESAMInfo *)0)
/*			ComData 中最多放置 strUserCard, strRunInfo, strSafeInfo		*/


#define			fTmTbl		(*((strESAMTmTbl *)RamData.MacBuf))
#define			vfTmTbl		(*((strESAMTmTbl *)0))

const __code strDispCode DErr = {_D_DISP_ERR_, 0 };

#ifdef		_USE_CARD_
	u8		Read_Purse(void);
	void	Up_Far_Use(void);
#endif
#ifdef DEL 
void	GetRWSamic(strRWSamic const __code*pRWSamic){
	//CopyRam( (u8*)&RWSamic, RCODE(pRWSamic), sizeof(strRWSamic));
	CopyRam( (u8*)&RWSamic, (char *)pRWSamic, sizeof(strRWSamic));
}

u8	SamicNoCode(void){
	return Samic(&RWSamic);
}

u8	SamicRWSamic( u8 Code, u8 Num){
	RWSamic.Cmd[Num] = Code;
	return SamicNoCode();
}

u8	SamicCode( strRWSamic const __code *pRWSamic, u8 Code, u8 Num){

	GetRWSamic(pRWSamic);
	return SamicRWSamic( Code, Num);
}

u8	Read_Back( u8* Buf){											//读取返回数据,(Return_7816&0xFF)-长度

	static const __code strRWSamic ReadBack = { { 0, 0xC0, 0, 0, 0x08 }, READ_ESAM, NULL };
	GetRWSamic(&ReadBack);
	RWSamic.Addr = Buf;
	if ( _ISCARCHANEL()){ // if(RamData.Is_Card){
		RWSamic.RWCfg = READ_IC;
	}
	//return SamicRWSamic( (u8)(RamData.Return_7816), 4 ); //
	return SamicRWSamic( (u8)(RamData.Return_7816>>8), 4 );
}

#define	ReadMacRand(Mode) ReadRand((Mode)|2)
u8	ReadRand(u8 Mode){

	static const __code strRWSamic Cmd = { { 0, 0x84, 0, 0, 8 }, READ_ESAM, RamData.Rand_Num };
	GetRWSamic(&Cmd);
	if(Mode&1){
		RWSamic.RWCfg = READ_IC;
	}
	if(Mode&2){
		RWSamic.Cmd[4] = 4;
	}
	return	SamicNoCode();
}

#define	MAC_WCARD		BIT0
#define	MAC_PURSE		BIT1
#define	MAC_INITPURSE	BIT2
#define	MAC_READ_SN		BIT3
#define	MAC_WRITE_SN	BIT4
#define	MAC_NEXT		BIT7

	
void	GetMacCmd(uc8 *pLst, u8* Cmd){

	if((pLst[2] & MAC_PURSE)&&((pLst[2] & MAC_WCARD)==0)){
		if(pLst[2]&MAC_INITPURSE){
			Cmd[0] = 0x04;
			Cmd[1] = 0xdc;
			Cmd[2] = pLst[4];
			Cmd[3] = pLst[1];
		} else {
			Cmd[0] = 0x84;
			Cmd[1] = 0x32;
			Cmd[2] = pLst[4];
			Cmd[3] = pLst[1];
		}
	} else {
		Cmd[0] = 0x04;
		Cmd[1] = 0xD6;
		Cmd[2] = pLst[1];
		Cmd[3] = pLst[4];
	}
	if(pLst[2] & MAC_WRITE_SN){
		Cmd[4] = (u8)(pLst[5]+8+4);
	} else {
		Cmd[4] = (u8)(pLst[5]+4);
	}
}
 
 
u8	GetMacData(uc8 *pLst, u8 *Buf){
/*	pLst[]数据内容:		文件1, 文件2, 操作模式, 文件1偏移,文件2偏移,长度, 文件1偏移,文件2偏移,长度,......, 0, 0, 0,
	操作模式定义:	BIT0:0-WriteESAM, 1-WriteCard
			BIT1:0-写普通文件; 1-写钱包文件
			BIT2:0-正常操作, 1- 初始化钱包文件
			BIT3:0-读无分散, 1- 读有分散
			BIT4:0-写无分散, 1- 写有分散
			BIT7:0-无下1条,  1- 有下1条
	*/
	u8 *MacAddr;
	u8	OutCode[17];
	strRWSamic	MacCmd;
	MacAddr = Buf; RSet_WDG(100) ;
	while(1){
		Copy_4(OutCode, RamData.Rand_Num);							//读随机数(4Byte)至MacBuf
		GetMacCmd(pLst, (OutCode+4));
		if((pLst[2]&MAC_PURSE)&&(pLst[2]&MAC_WCARD)){
			MacCmd.Cmd[1] = 0xB2;
			MacCmd.Cmd[2] = pLst[3];
			MacCmd.Cmd[3] = pLst[0];
			MacCmd.Cmd[0] = 0x04;
		} else {
			MacCmd.Cmd[1] = 0xB0;
			MacCmd.Cmd[2] = pLst[0];
			MacCmd.Cmd[3] = pLst[3];
			MacCmd.Cmd[0] = 0x04;
		}
		if(pLst[2]&MAC_READ_SN){
			if(pLst[2]&MAC_WCARD){
				CopyRam( (OutCode+9), RamData.IC_Num, 8);
			} else {
				CopyRam( (OutCode+11), FlashInfo.SafeInfo.E_Num, 6);
				OutCode[9] = 0;
				OutCode[10] = 0;
			}
			MacCmd.Cmd[4] = 0x11;
		} else {
			MacCmd.Cmd[4] = 0x9;
		}
		if(pLst[2]&MAC_WCARD){
			MacCmd.RWCfg = WRITE_ESAM;
		} else {
			MacCmd.RWCfg = WRITE_IC;
		}
		MacCmd.Addr = OutCode;
		if(Samic(&MacCmd)){									//发送MAC读命令
			return FAIL;
		}
		*(((u8*)&RamData.Return_7816)+1) = (u8)(pLst[5]+4);
//		RamData.Return_7816 = (u16)((pLst[5]+4)<<8);
		if(pLst[2]&MAC_WCARD){
			if(Read_Back(MacAddr)){							//先加分散因子读处理
				return FAIL;
			}
		} else {
			if((pLst[2]&MAC_PURSE)&&(!(pLst[2]&MAC_INITPURSE))){	//钱包处理
				if(Read_Back(MacAddr)){
					return FAIL;
				}
				if(pLst[2]&MAC_WRITE_SN){
					CopyRam((MacAddr+pLst[5]+4), RamData.IC_Num, 8);
				}
			} else {
				if(pLst[2]&MAC_WRITE_SN){
					CopyRam(MacAddr, RamData.IC_Num, 8);
					if(Read_Back((MacAddr+8))){
						return FAIL;
					}
				} else {
					if(Read_Back(MacAddr)){
						return FAIL;
					}
				}
			}
		}
		MacAddr += OutCode[8];
		if((pLst[2]&0x80) == 0){
			return SUCCESS;
		}
		pLst += 6;
	}
}


u8	WriteMacData(uc8 *pLst, u8 *Buf){//0

	u8 *MacAddr;
	u8 *DstAddr;
	strRWSamic	MacCmd;
	MacAddr = Buf;
	DstAddr = Buf;
	while(1){  RSet_WDG(200);
		GetMacCmd(pLst, MacCmd.Cmd);
		if(pLst[2] & MAC_WCARD){
			MacCmd.RWCfg = WRITE_IC;
		} else {
			MacCmd.RWCfg = WRITE_ESAM;
		}
		MacCmd.Addr = MacAddr;
		if(Samic(&MacCmd)){											//写数据
			return FAIL;
		}
		if((RamData.Return_7816&0xFF) == 0x61){
			if(Read_Back(DstAddr))
				return FAIL;
			DstAddr += pLst[5];
		} else if((pLst[2] & MAC_WCARD) == 0){
			if(pLst[2] & MAC_WRITE_SN){
				CopyRam(DstAddr, (MacAddr+8), pLst[5]);
			} else {
				CopyRam(DstAddr, MacAddr, pLst[5]);
			}
			DstAddr += pLst[5];
		}
		MacAddr += MacCmd.Cmd[4];
		if((pLst[2]&0x80) == 0){
			return SUCCESS;
		}
		pLst += 6;
	}
}

u8	MacTrans(uc8 *pLst, u8 *Buf){
	if(GetMacData(pLst, Buf))
		return FAIL;
	return WriteMacData(pLst, Buf);
}

u8	cGetMacData(const __code u8 *pLst, u8 *Buf){					//从代码区
	u8	MacSet[12];
	CopyRam( MacSet, RCODE(pLst), 12);
	return GetMacData(MacSet, Buf);
}

u8	cWriteMacData(const __code u8 *pLst, u8 *Buf){
	u8	MacSet[12];
	CopyRam( MacSet, RCODE(pLst), 12);
	return WriteMacData(MacSet, Buf);
}

u8	cMacTrans(const __code u8 *pLst, u8 *Buf){
	u8	MacSet[12];
	CopyRam( MacSet, RCODE(pLst), 12);
	return MacTrans(MacSet, Buf);
}

				/*	以上为IC卡/ESAM基本操作		*/

u8	GetETest(u8 Key){												//生成ESAM认证数据至Rand_Num+8(用于内部认证及返写认证认证)

	static const __code strRWSamic	Cmd80FA0X = { { 0x80, 0xFA, 0, 0, 0x08 }, WRITE_ESAM, RamData.IC_Num };
	static const __code strRWSamic Cmd80FA00 = { { 0x80, 0xFA, 0, 0, 0x08 }, WRITE_ESAM, RamData.Rand_Num };
	if(SamicCode(&Cmd80FA0X, Key, 3))								//生成过程密钥
		return FAIL;
	if(SUCCESS!=cSamic(&Cmd80FA00)){											//生成认证数据
		return FAIL;
	}
	return Read_Back(RamData.At_Back);
}

void	GetSafeInfo( strESAMInfo *pEInfo){

	static const __code strMvData mvSafeInfo[] = {
		SET_PCHAR( &vESafeInfo.Cmd, 0),
		NORM_COPY( &bUserCard.Cmd, 1),
		NORM_COPY( &bUserCard.UserType, 1),
		READ_RAM( &bUserCard.H_Num, COPY_L(sizeof(UserCard.H_Num))),//客户编号
		READ_RAM( &bUserCard.E_Num, COPY_L(sizeof(UserCard.E_Num))),//表号
		SET_PCHAR(&vESafeInfo.Alm1, 0),
		READ_RAM( &bUserCard.Alm1, BCD_IN(4, 4, 0) ),
		READ_RAM( &bUserCard.Alm2, BCD_IN(4, 4, 0) ),
		READ_RAM( &bUserCard.I_Mul, BCD_IN(3, 4, 0) ),				//电流互感器变比
		READ_RAM( &bUserCard.V_Mul, BCD_IN(3, 4, 0) ),				//电压互感器变比
		SET_PCHAR( &vESafeInfo.StartTm_3, 0),
		READ_RAM( &bUserCard.StartTm, _RET_|READ_BCD_TIME(S_MIN, T_SEC) ),
	};
	CopyRam( (u8*)&ESafeInfo, (u8*)&FlashInfo.SafeInfo, sizeof(strSafeInfo));
	tpBase = (u8*)pEInfo;
	MoveDataLst((strMvData *)mvSafeInfo);
}

u8	Read_ESAM(u8* Buf, u16 IDStart, u8 size){		//读取ESAM二进制文件,(ID-文件标识,size-长度)

	const strRWSamic RWSamic1 = { { 0, 0xB0, 0x0, 0, 0 }, READ_ESAM, RamData.MacBuf };
	GetRWSamic(&RWSamic1);
	RWSamic.Cmd[2] = (unsigned char )(IDStart>>8);  RWSamic.Cmd[3] = (unsigned char )IDStart;  //  *(u16*)(RWSamic.Cmd+2) = REV16(IDStart);
	RWSamic.Addr = Buf;
	while(size){
		if(size <= 120){
			RWSamic.Cmd[4] = size;
		} else {
			RWSamic.Cmd[4] = 120;
		}
		if(SUCCESS!=SamicNoCode()){
			//RamData.Return_7816 = REV16(RamData.Return_7816 ) ;
			if((u8)RamData.Return_7816!=0x6C)
				return FAIL;
			ClrRam(RWSamic.Addr+(u8)(RamData.Return_7816>>8), size-(u8)(RamData.Return_7816>>8));
			RWSamic.Cmd[4] = (u8)(RamData.Return_7816>>8);
			if(SUCCESS!=SamicNoCode())
				return FAIL;
			return SUCCESS;
		}
		*(u16*)(RWSamic.Cmd+2) = ((*(u16*)(RWSamic.Cmd+2))+RWSamic.Cmd[4]);
		RWSamic.Addr += RWSamic.Cmd[4];
		size -= RWSamic.Cmd[4];
	}
	return SUCCESS;
}

u8	ReadPureInfo(void){

	static const __code u8 MacFarReadPurse[] = {
		0x0C, 0x86, (MAC_WCARD|MAC_PURSE|MAC_READ_SN|MAC_WRITE_SN|MAC_NEXT),
		0x01, 0, 4,
		0x0C, 0x86, (MAC_WCARD|MAC_PURSE|MAC_READ_SN|MAC_WRITE_SN),
		0x03, 0, 4,
	};
	CopyRam( RamData.Rand_Num, RamData.Rand_Num_Far, 16);			//复制Rand_Num + IC_Num
	if(SUCCESS!= cGetMacData(MacFarReadPurse, RamData.MacBuf)){
		RamData.FErrWord = _FEW_ESAMERR_;
		return FAIL;
	}
	return SUCCESS;
}
#endif

void	Clr_AllUse(void){//0

	u8	N;
	u8	k;
	u32 ltemp ,ltemp1;
	if(RamData.Write == 0x86714200){
		k = 0;
	} else {
		k = 1;
	}
	for(N = 0; N < _FROZE_NUM_; N++){
		if(k == 0){
			ClrFrozeCnt(N);
		} else if(N != _FROZE_CLEAR_){
			ClrFrozeCnt(N);
		}
	}
	cClrEvtState(FileSet.ZeroP);
	if(k){
		Up_NormFroze(_FROZE_CLEAR_);								//增加清零记录
		if(CheckKeyChg(&FlashInfo.SetInfo.EKeyState) == SUCCESS){
			SetEvtState(_EVT_CLEAR_);
		}
	}
	Reset_LVSave();													//清除Flash用电数据
	ClrRam((u8*)&ERunInfo.LastUse, ((u8*)(&ERunInfo.EState)-(u8*)(&ERunInfo.LastUse)));
	GetFrozeMinCnt((unsigned char *)&ERunInfo.LastMFrozeTm);	//RamData.RTCTime/60;
	GetFrozeMinCnt((unsigned char *)&ERunInfo.LastYFrozeTm);	//RamData.RTCTime/60;
	GetFrozeMinCnt((unsigned char *)&ERunInfo.LastDFrozeTm);	//RamData.RTCTime/60;
	//ERunInfo.BatUseTime = FlashInfo.RunInfo.BatUseTime;
  memcpy( (char *)&ERunInfo.BatUseTime,(char *)&FlashInfo.RunInfo.BatUseTime,sizeof(ERunInfo.BatUseTime));
	ltemp1 =  ( 1<<(1+_BIT_METER_CLEAR_)) -1;
	if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		ltemp =  FlashInfo.RunInfo.EState&(~(ltemp1|_ESTATE_JD_STATE_|_ESTATE_JD_CMD_|_ESTATE_FAR_ALM_|_ESTATE_EEP_|_ESTATE_ESAM_));
	}else{ //#else
		ltemp=FlashInfo.RunInfo.EState&(~(ltemp1|_ESTATE_EEP_|_ESTATE_ESAM_));
	} //#endif
	if(ChkYMode() == SUCCESS){
		ltemp |= _ESTATE_YSTEP_;
		RamData.YTm_Chg = 0;
	}
if (_ISMETER_REMOTE()) { //#ifndef		_USE_CARD_
	if((_ISJDMODE_OUT()	)&&(FlashInfo.RunInfo.EState&_ESTATE_JD_STATE_)){	// if((_JD_MODE_ == 0)&&(FlashInfo.RunInfo.EState&_ESTATE_JD_STATE_)){	//外置表合闸清零后按合闸成功处理
		ltemp |= _ESTATE_LAST_STATE_;
	}
	CopyRam((u8*)&ERunInfo.RcdTm.JDOverIphTime, (u8*)&FlashInfo.RunInfo.RcdTm.JDOverIphTime, 12);	//JDOverIphTime,FarJDOffTime,FarJDOffPreTime保持
	ERunInfo.FarJDPreTm = FlashInfo.RunInfo.FarJDPreTm;
	ERunInfo.LastJDWriter = FlashInfo.RunInfo.LastJDWriter;
}//#endif
	Copy_FlashInfo((u8*)&FlashInfo.RunInfo.LastUse, (u8*)&ERunInfo.LastUse,((u8*)&ERunInfo.StepPrice-(u8*)&ERunInfo.LastUse));
	//Copy_FlashInfo((u8*)&FlashInfo.RunInfo.StepPrice, (u8*)&ERunInfo.StepPrice,((u8*)&ERunInfo.EState+sizeof(ERunInfo.EState)-(u8*)&ERunInfo.StepPrice));
   Copy_FlashInfo((u8*)&FlashInfo.RunInfo.StepPrice, (u8*)&ERunInfo.StepPrice,((u8*)&ERunInfo.EState -(u8*)&ERunInfo.StepPrice));
   Up_u32Data(&FlashInfo.RunInfo.EState, ltemp);
//	ClrRam( RamData.ErrLst, sizeof(RamData.ErrLst));
	Copy_FlashInfo(FlashInfo.RunInfo.IC_Num, RCODE(FileSet.ZeroP), 8);
	//ClrRcdTime((u32*)FlashInfo.RunInfo.IC_Num);
	//ClrRcdTime((u32*)(FlashInfo.RunInfo.IC_Num+4));
	RamData.Event_Min = 1;
//	RamData.OverPwrTm = 0;
//	RamData.OverVoltTm = 0;
//	RamData.OverPhTm = 0;
	RamData.RevPwrCnt = 0;
//	RamData.IRACCessTm = 0;
	RamData.ESAMErrCnt = 0;
	RamData.FarSubTm = 0;
	if (_ISJDMODE_OUT()){ //#if		_JD_MODE_ == 0
			RamData.ErrImpTm = 0;
			RamData.ErrImp = 0;
			JD_Err = 0;
	}//#endif
	RunState_LOpen = 1;
	RunState_FOpen = 1;
	ChkEvtState();
//#ifndef		_MAXIM_71315_
	CheckBatVolt();
//#endif
}
#ifdef _CARD
void	Chg_Mi(void){												//修改密钥

	strRWSamic WriteMi = { {0x84,0xd4,0x01,0x00,0x20}, WRITE_ESAM, ComData+4 };
	static const __code strRWSamic WriteKInfo = {{ 0x00, 0xD6, KEY_INFO_1_ID, 0, 4 }, WRITE_ESAM, RamData.IC_Num+4 };
	u8	Cnt;
	u8	K;
	u16	StartBias;
	u32	ChgState;

	if(RamData.Event_KeyChg == 0){
		return;
	}
	RamData.Event_KeyChg = 0;
	if(Read_ESAM(RamData.IC_Num, (KEY_INFO_1_ID<<8), 4)){
		return;
	}
	RevCopy_4(RamData.IC_Num+4, RamData.IC_Num);
	if((CheckKeyChg(&FlashInfo.RunInfo.NKeyState))||((FlashInfo.RunInfo.EState&_ESTATE_CHG_KEY_) == 0)){
		Up_u32Data(&FlashInfo.SetInfo.EKeyState, *(u32*)(RamData.IC_Num+4));
		return;
	}
	ChgState = 0;RSet_WDG(200);
	for(Cnt = 0; Cnt < FlashInfo.SetInfo.KeyNum; Cnt++){
		if(Cnt){
			WriteMi.Cmd[3] = 0xFF;
		}
		StartBias = GetFStartAddr(_FROZE_KEYDATA_, Cnt);
#ifdef	_USE_EEPROM_
		Read_EEData( ComData, StartBias, 36);
#else
		//待定
#endif
		for(K = 0; K < 3; K++){
			if(Samic(&WriteMi) == SUCCESS){
				ChgState |= (((u32)1)<<Cnt);
				break;
			}
		}
	}
	if(ChgState){
		ChgState |= 1;
	}
	if(FlashInfo.RunInfo.EState&_ESTATE_KEY_NEXT_){
		ChgState |= FlashInfo.SetInfo.EKeyState;
	} else {
		ChgState = (~ChgState)&FlashInfo.SetInfo.EKeyState;
		ChgState &= ((((u32)1)<<FlashInfo.SetInfo.KeyNum)-1);
	}
	Up_u32Data(&FlashInfo.SetInfo.EKeyState, ChgState);
	ClrEBit(_BIT_CHG_KEY_);
	RevCopy_4(RamData.IC_Num+4, (u8*)&ChgState);
	cSamic(&WriteKInfo);
	Up_NormFroze(_FROZE_KEYCHG_);
	SetEvtState(_EVT_KEYCHG_);
	return;
}



const __code u8 DF01[2] = { 0xDF, 0x01 };
const __code strRWSamic CmdSelect = {{0x00,0xA4,0x00,0x00,0x02}, WRITE_IC, (u8*)RCODE(DF01) };
const __code strRWSamic MacWriteCmd = { { 0x04, 0xD6, 0x88, 0x00, 0x14 }, WRITE_ESAM, RamData.MacBuf };

u8	ID_SuccessRet(void){

	Ex645.SCIx->DataBuf[1] = 4;
	return SUCCESS;
}

u8	SetMacErrCnt(void){

	//u8	N;
	if(FlashInfo.RunInfo.MacErrCnt < 250){
		Up_u8Data( &FlashInfo.RunInfo.MacErrCnt, FlashInfo.RunInfo.MacErrCnt + 1);
	}
	RamData.FErrWord = _FEW_ESAMERR_;
	return FAIL;
}

u8	SetUserBack(void){												//生成用户卡返写数据

#define			UserBack	(*((strUserBack *)ComData))
#define			vUserBack	(*((strUserBack *)0))

	static const __code strMvData mvUserBack[] = {
		SET_PCHAR( vComData, 0),
		SET_RAM_2( 0x6811, 0),
//		SET_RAM_2((sizeof(strUserBack)-6), 0),
		SET_RAM_2((u16)(&vUserBack.Sum - &vUserBack.UserType), 0),
		NORM_COPY( &vFlashInfo.SafeInfo.UserType, 1),
		READ_RAM( &vFlashInfo.SafeInfo.I_Mul, BCD_OUT(4, 3, 0)),
		READ_RAM( &vFlashInfo.SafeInfo.V_Mul, BCD_OUT(4, 3, 0)),
		NORM_COPY( vFlashInfo.SafeInfo.E_Num, 6),
		NORM_COPY( vFlashInfo.SafeInfo.H_Num, 6),
		READ_RAM( &vRamData.u_Remain, HEX_OUT(4, 4, 0)),
		READ_RAM( (u8*)&vFlashInfo.SafeInfo.ChargeCnt, HEX_OUT(4, 4, 0)),
		READ_RAM( &vRamData.u_OverDraft, BCD_OUT(4, 4, 0)),
		FILL_RAM( 0, 4),
#ifdef		_USE_CARD_
		READ_RAM( &vFlashInfo.RunInfo.ErrCardCnt, BCD_OUT( 2, 3, 0)),
#else
		FILL_RAM( 0, 3),
#endif
		READ_RAM( &vRamData.RTCTime, OUT_TIME( S_RTC, T_YMDHM)),
		SET_RAM_2( 0x0016, _RET_),
	};
	static const __code strRWSamic WrUserBack = {{ 0x00, 0xD6, ESAM_WBACK_ID, 0, (&vUserBack.End - &vUserBack.Start)+1 }, WRITE_ESAM, (u8*)&UserBack };
	MoveDataLst(mvUserBack);
	UserBack.Sum = GetSum(&UserBack.Cmd, (&vUserBack.Sum - &vUserBack.Cmd));
	if (_ISMETER_REMOTE())  ClrRam(UserBack.ErrCardCnt,sizeof(UserBack.ErrCardCnt));
	return cSamic(&WrUserBack);
}

u8	ID_IRAccess1(u8* pData){										//红外认证1

	static const __code strMvData mvIRAccess1[] = {
		SET_PCHAR( vRamData.IC_Num, 0),
		FILL_RAM( 0, 2),
		NORM_COPY( vFlashInfo.SafeInfo.E_Num, 6),
		SET_PCHAR( vRamData.Rand_Num_Far, 0),
		REV_COPY( PBASE_ADDR+2+8, 8),
		SET_PCHAR( vRamData.Rand_Num, 0),
		REV_COPY( PBASE_ADDR+2+8, 8|_RET_),
	};
	static const __code strMvData mvIRAccess2[] = {
		SET_PCHAR( PBASE_ADDR+1, 0),
		FILL_RAM( (4+30), 1),
		SET_PCHAR( PBASE_ADDR+6, 0),
		REV_COPY( vFlashInfo.SafeInfo.E_Num, 6),
		REV_COPY( vRamData.IC_Num_Far, 8),
		REV_COPY( vRamData.At_Back, 8),
		REV_COPY( vRamData.Rand_Num, 8|_RET_),
	};
	static const __code strRWSamic Cmd1 = {{0x00,0xB0,0x99, 0x01, 0x08}, READ_ESAM, RamData.IC_Num_Far };

	RamData.IDACCess = 1;
	if(SUCCESS!=Samic_Rst(_RW_SAM_)){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	if(SUCCESS!=cSamic(&Cmd1)){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	tpBase = Ex645.SCIx->DataBuf;
	MoveDataLst(mvIRAccess1);
	if(SUCCESS!=GetETest(0x03)){												//生成ESAM认证数据
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	if(ReadRand(0)){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	MoveDataLst(mvIRAccess2);
	RamData.IDACCess = 0;
	RamData.IRACCessTm = 59;
	RunState_IR = 0;
	return SUCCESS;
}

u8	ID_IRAccess2(u8* pData){										//红外认证2

	static const __code strRWSamic Cmd = {{ 0, 0x82, 0, 0x08, 0x08 }, WRITE_ESAM, RamData.At_Back };
	if(RamData.IRACCessTm == 0){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	RevCopyRam(RamData.At_Back, pData+10, 8);
	if(SUCCESS!=cSamic(&Cmd)){
		RamData.FErrWord = _FEW_IDERR_;
		RamData.IRACCessTm = 1;
		return FAIL;
	}
	RamData.IRACCessTm = FlashInfo.SetInfo.IRAccessTmSet*60;		//认证有效时间30分钟
	RunState_IR = 1;
	return ID_SuccessRet();
}

static const __code strMvData mvAccess[] = {
	SET_PCHAR( vRamData.IC_Num_Far, 0),
	//FILL_RAM( 0, 2),
	//NORM_COPY( FlashInfo.SafeInfo.E_Num, 6),
	REV_COPY( PBASE_ADDR+2+8+16, 8),
	SET_PCHAR( vRamData.Rand_Num, 0),
	REV_COPY( PBASE_ADDR+2+8+8, 8),
	SET_PCHAR( &vRamData.Rand_Num_Far, 0),
	NORM_COPY( vRamData.Rand_Num, 8),
	SET_PCHAR( &vRamData.IC_Num, 0),
	NORM_COPY( vRamData.IC_Num_Far, 8|_RET_),
};
static const __code strMvData mvAccessOver[] = {
	SET_PCHAR( PBASE_ADDR+1, 0),
	FILL_RAM( (4+4+8), 1),
	SET_PCHAR( PBASE_ADDR+6, 0),
	REV_COPY( vRamData.Rand_Num, 4),								//返回4字节随机数2
	REV_COPY( vRamData.ESAM_Sn, 8|_RET_),						//返回ESAM序列码
};
u8	ID_Access(u8* pData){											//身份认证

#ifndef		_CODE_CHANGE_
	static const __code strRWSamic CmdReadAccessTime = {
		{ 0x00, 0xB0, SAFE_INFO_ID, (u8)(vESAMInfo.ID_Tm - &vESAMInfo.Start), 0x02 }, READ_ESAM,
		RamData.MacBuf	};
#endif

	u32	Data;

	CloseProgRecord();
	RamData.IDACCess = 1;

	if(SUCCESS!=Samic_Rst(_RW_SAM_)){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}

	tpBase = pData;
	MoveDataLst(mvAccess);
	if((*(u16*)RamData.IC_Num_Far)||(strCmp(RamData.IC_Num_Far+2, FlashInfo.SafeInfo.E_Num, 6))){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}

	if(SUCCESS!=GetETest(0x03)){												//生成ESAM认证数据
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	RevStr( pData+2+8, sizeof(RamData.IC_Num));						//获取认证数据

	if(strCmp(RamData.At_Back, pData+2+8, 8) != 0){					//对比认证数据一致性
		RamData.FErrWord = _FEW_IDERR_;
		return FAIL;
	}

	if(SUCCESS!=ReadMacRand(2)){												//从ESAM取随机数
		RamData.FErrWord = _FEW_IDERR_;
		return FAIL;
	}

#ifdef		_CODE_CHANGE_
	if(FlashInfo.SetInfo.ID_Tm){
		RamData.IDACCess=FlashInfo.SetInfo.ID_Tm*60;				//以秒为单位
	}else{
		RamData.IDACCess=300;										//默认身份有效时间
	}
#else
	if(!cSamic(&CmdReadAccessTime)){
		Data = BCD_Hex(REV16(*(u16*)RamData.MacBuf));
		if(Data){
			RamData.IDACCess=Data*60;								//以秒为单位
		}else{
			RamData.IDACCess=300;									//默认身份有效时间
		}
	}else{
		RamData.IDACCess=300;										//默认身份有效时间
	}
#endif

	MoveDataLst(mvAccessOver);
	RunState_ID = 1;
	return SUCCESS;
}

u8	ID_AccessTimeSet(u8* pData){									//访问时间设定

	static const __code u8 MacIDTime[] = {
		SAFE_INFO_ID, SAFE_INFO_ID, 0,			//File1, File2, Mode:
		(u8)(vESAMInfo.ID_Tm - &vESAMInfo.Start), (u8)(vESAMInfo.ID_Tm - &vESAMInfo.Start), 2,
	};u16 itemp;

	RevStr(pData+10, 2);											//调整分散因子
	Rev_4(pData+10+2);												//调整MAC
	if (  0 == _u16get(pData+10)) { //	  if(*(u16*)(pData+10) == 0){										//不可设置零
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	} else if(cWriteMacData(MacIDTime, pData+10)){					//验证失败
		return SetMacErrCnt();
	}
#ifdef		_CODE_CHANGE_
	Up_u16Data(&FlashInfo.SetInfo.ID_Tm, BCD_Hex(_u16get(pData+10)));    //Up_u16Data(&FlashInfo.SetInfo.ID_Tm, BCD_Hex(*(u16*)(pData+10)));
	RamData.IDACCess = FlashInfo.SetInfo.ID_Tm*60;					//以秒为单位
#else
	RevCopyRam((char*)&itemp, pData+10,sizeof(itemp));
	RamData.IDACCess = BCD_Hex((u32)itemp)*60;				//以秒为单位  //RamData.IDACCess = BCD_Hex(REV16(*(u16*)(pData+10)))*60;				//以秒为单位
#endif
	return ID_SuccessRet();
}

u8	ID_AccessTimeClose(u8* pData){									//访问时间关闭

	static const __code strMvData mvACK[] = {
		REV_COPY(vFlashInfo.SafeInfo.H_Num, 6),
		REV_COPY( vRamData.MacBuf, 4),
		REV_COPY( vRamData.MacBuf+16, 4),	//REV_COPY( MacBuf+8, 4),
		NORM_COPY( (u8*)&vFlashInfo.SetInfo.EKeyState, 4|_RET_),
	};
	pData[1] = 0x16;
	if(ReadPureInfo()){
		return FAIL;
	}
	tpChar = pData+6;
	MoveDataLst(mvACK);
	RamData.IDACCess = 1;
	return SUCCESS;
}

u8 ID_AccessACK(u8* pData){											//状态查询

	static const __code strMvData mvACK[] = {
		REV_LST( vRamData.MacBuf, 4, 2),
		REV_LST( vRamData.MacBuf+16, 4, 2),	//		REV_LST( MacBuf+8, 4, 2),
		REV_COPY( vFlashInfo.SafeInfo.H_Num, 6),
		NORM_COPY( (u8*)&vFlashInfo.SetInfo.EKeyState, 4|_RET_),
	};
if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
	Up_Far_Use();
}//#endif
	pData[1]=0x1e;
	if(ReadPureInfo()){
		return FAIL;
	}
	tpChar = pData+6;
	MoveDataLst(mvACK);
	return SUCCESS;
}

u8 ID_AccessChangeMi(u8* pData){									//修改密钥

	static const __code u8 MacMi[] = {
		0x82,	0x96, MAC_WRITE_SN,		//File1, File2, Mode:
		0,      0x0,  36,
	};
	u8	KeyNum;
	u8	Cnt;
	u8	K;
	u32	KState;
	//u32	NewEState;
	u8	*pKey;
	if((pData[1]%36) != 12){
		return FAIL;
	}
	KeyNum = pData[1]/36;

	KState = 0;
	pKey = pData+10;
	for(Cnt = 0; Cnt < KeyNum; Cnt++){
		RevStr(pKey, 36);
		if(Cnt == 0){
			CopyRam(ComData+18, pKey, 36);
		} else {
			for(K = 0; K < 36; K++){
				ComData[K+18] ^= pKey[K];
			}
		}
		if(pKey[0] != FlashInfo.SetInfo.KeyNum){					//密钥条数检查
			RamData.FErrWord = _FEW_OTHER_;
			return FAIL;
		}
		if(pKey[2] >= FlashInfo.SetInfo.KeyNum){					//密钥编号检查
			RamData.FErrWord = _FEW_OTHER_;
			return FAIL;
		}
		if(pKey[3] > 1){
			RamData.FErrWord = _FEW_OTHER_;
			return FAIL;
		}
		if(pKey[3] != pData[13]){									//密钥状态检查
			RamData.FErrWord = _FEW_OTHER_;
			return FAIL;
		}
		KState |= (((u32)1)<<(pKey[2]));
		pKey += 36;
	}
	RevCopyRam(ComData+18+36, pKey, 4);								//MAC数据
	Ex645.FastData = FlashInfo.RunInfo.EState|_ESTATE_CHG_KEY_;
	if((FlashInfo.RunInfo.EState&_ESTATE_CHG_KEY_) == 0){
		if(pData[13]){
			Ex645.FastData |= _ESTATE_KEY_NEXT_;
		} else {
			Ex645.FastData &= ~(u32)(_ESTATE_KEY_NEXT_);
		}
	} else if((pData[13])&&((FlashInfo.RunInfo.EState&_ESTATE_KEY_NEXT_) == 0)){
		Ex645.FastData |= _ESTATE_KEY_NEXT_;
	} else if((pData[13] == 0)&&(FlashInfo.RunInfo.EState&_ESTATE_KEY_NEXT_)){
		Ex645.FastData &= (~(u32)(_ESTATE_KEY_NEXT_));
	} else{
		KState |= FlashInfo.RunInfo.NKeyState;
	}

	*(u16*)(ComData+10) = 0;									//分散因子设为表号
	CopyRam(ComData+12, FlashInfo.SafeInfo.E_Num, 6);
	if(cWriteMacData(MacMi, ComData+10)){
		if(pData[13]){
			return SetMacErrCnt();												//验证失败
		}
		ClrRam((ComData+10), 7);
		ComData[17] = 1;											//分散因子设为1
		if(cWriteMacData(MacMi, ComData+10)){
			return SetMacErrCnt();												//验证失败
		}
	}
	pKey = pData+10;
	for(Cnt = 0; Cnt < KeyNum; Cnt++){
		Write_Froze(pKey, _FROZE_KEYDATA_, pKey[2]);
		pKey += 36;
	}
	UpEState(Ex645.FastData);
	UpJDWriter();													//记录操作者代码
	Up_u32Data(&FlashInfo.RunInfo.NKeyState, KState);
	RamData.Event_KeyChg = 1;
	RamData.TRx_Wait = 1;
	return ID_SuccessRet();
}

u8 ID_AccessDataRead(u8* pData){									//数据回抄

	u8 MacDataRead[] = {
		SAFE_INFO_ID, 0x86, (MAC_WCARD|MAC_READ_SN|MAC_WRITE_SN), 0, 0,   _strESAMInfo_Len_};// sizeof(strESAMInfo) };
//		SAFE_INFO_ID, 0x86, (MAC_WCARD|MAC_READ_SN), 0, 0, sizeof(strESAMInfo) };
	u8 Length;
	if (( 0x1 != pData[16]) || ( 0xdf != pData[17])){//  if((*(u16*)(pData+16)) != 0xDF01){
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	if(pData[14] == 0x01){
if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
		Up_Far_Use();
}//#endif
		pData[1] = 0x14+8;
		if(ReadPureInfo()){
			return FAIL;
		}
		CopyRam(pData+6, pData+10, 8);
		CopyRam(pData+14, RamData.MacBuf, 8);
		CopyRam(pData+14+8, RamData.MacBuf+16, 8);		//CopyRam(pData+14+8, MacBuf+8, 8);
		Rev4Lst(pData+14, 4);									//剩余金额/MAC/购电次数/MAC
	} else {
		if(pData[14] == (ESAM_WBACK_ID-0x80)){
if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
			Up_Far_Use();
}else{ //#else
	if(RamData.FarSubTm == 0){
		SetUserBack();
		RamData.FarSubTm = 15;
	}
} //#endif
		}
		Length = pData[10];
		MacDataRead[0] = 0x80+pData[14];
		MacDataRead[3] = pData[12];
		MacDataRead[4] = pData[12];
		MacDataRead[5] = Length;
		if(GetMacData(MacDataRead, RamData.MacBuf)){
			RamData.FErrWord = _FEW_OTHER_;
			return FAIL;
		}
		CopyRam(pData+6, pData+10, 8);
		RevCopyRam(pData+14, RamData.MacBuf, Length);						//数据颠倒，
		RevCopyRam((pData+14+Length), (RamData.MacBuf+Length), 4);			//MAC颠倒
		pData[1] = 4+8+Length+4;
	}
	return SUCCESS;
}

u8		ID_MacWrite(u8 *pData){

	u16	FCode;
	RevStr(pData+13, (pData[1]-15));
	Rev_4(pData+pData[1]-2);
	GetRWSamic(&CmdSelect);
	RWSamic.Addr = (u8*)&FCode;
	RWSamic.RWCfg = WRITE_ESAM;
	FCode = pData[10]<<8;
	if(SamicNoCode()){
		RamData.FErrWord = _FEW_OTHER_;									//选择文件错误
		return FAIL;
	}
	if(pData[3] == 0x04){											//明文写信息文件
		u8	Far_MacWrite[] = {
			0x00, 0x00, 0,
			0, 0x05, 0x06	};
		Far_MacWrite[1] = pData[12];
		Far_MacWrite[4] = pData[11];
		Far_MacWrite[5] = pData[1]-15;
		if(WriteMacData(Far_MacWrite, pData+13)){
			Far_MacWrite[1] = pData[10]+0x80;
			if(WriteMacData(Far_MacWrite, pData+13)){
				return SetMacErrCnt();										//验证MAC错
			}
		}
	} else {														//密文写信息文件
		GetRWSamic(&MacWriteCmd);
		RWSamic.Cmd[2] = pData[12];
		RWSamic.Cmd[3] = pData[11];
		RWSamic.Cmd[4] = pData[1]-11;
		RWSamic.Addr = pData+13;
		if(SamicNoCode()){
			RWSamic.Cmd[2] = pData[10]+0x80;
			if(SamicNoCode()){
				return SetMacErrCnt();										//验证MAC错
			}
		}
	}
	return ID_SuccessRet();
}

u8 ID_AccessCodeCmp(u8* pData){										//程序比对

	u8*	p1;
	u8	k;
	u8	Cnt;
	static const __code strRWSamic Cmd1 = { { 0x80, 0xFA, 0x00, 0x00, 0x08 }, WRITE_ESAM, RamData.At_Back };
	static const __code strRWSamic Cmd2 = { { 0x80, 0xFA, 0x01, 0x00, 0x40 }, WRITE_ESAM, ComData };
	if(pData[10]){													//CPU编号错误
		RamData.FErrWord = _FEW_OTHER_;
		return FAIL;
	}
	if(GetCompData(0)){
		RamData.FErrWord = _FEW_ADDRERR_;							//地址超限
		return FAIL;
	}
	for(Cnt = 0; Cnt < 8; Cnt++){									//获取分散因子
		RamData.At_Back[Cnt] = ComData[Cnt]^ComData[Cnt+8];
	}
	if(GetCompData(1)){
		RamData.FErrWord = _FEW_ADDRERR_;							//地址超限
		return FAIL;
	}
	p1 = ComData+64;
	for(k = 0; k < 3; k++){
		for(Cnt = 0; Cnt < 64; Cnt++){
			ComData[Cnt] ^= p1[Cnt];
		}
		p1 += 64;
	}
	if(SamicCode(&Cmd1, pData[11], 3)){								//密钥分散
		RamData.FErrWord = _FEW_ESAMERR_;
		return FAIL;
	}
	if(cSamic(&Cmd2)){												//计算密文
		RamData.FErrWord = _FEW_ESAMERR_;
		return FAIL;
	}
	if(Read_Back(pData+6)){
		RamData.FErrWord = _FEW_ESAMERR_;
		return FAIL;
	}
	RevStr(pData+6, 64);
	pData[1] = 68;
	return SUCCESS;
}

void	Set_ESAMErr(void){
	RunState_ESAM = 1;
	if(RamData.ESAMErrCnt < 10){
		if((FlashInfo.RunInfo.EState&_ESTATE_ESAM_) == 0){
			SetEvtState(_EVT_ESAM_);
			SetEBit(_BIT_ESAM_);
		}
	}
	RamData.ESAMErrCnt = 10;
}

u8	Get98Data(u8 *pData, u8 FID, u8 Lth){

	static const __code strRWSamic ReadCmd = { { 0x00, 0xB0, 0x88, 0, 0x08 }, READ_ESAM, RamData.MacBuf };

	GetRWSamic(&MacWriteCmd);
	RWSamic.Cmd[2] = FID;
	RWSamic.Cmd[4] = Lth+4;
	RWSamic.Addr = pData+4;
	RevStr(pData, Lth+4);
	Copy_4((pData+Lth+4), pData);
//	*(u32*)(pData+Lth+4) = *(u32*)pData;

	if ( SUCCESS !=SamicNoCode())//  - if(FAIL==SamicNoCode())
	{
		RWSamic.Addr = pData;
		if(SamicNoCode()){
			SetErrReturn(_ERR_SAFE_);
				SetMacErrCnt();
			return FAIL;
		}
	}
	GetRWSamic(&ReadCmd);
	RWSamic.Addr = pData;
	RWSamic.Cmd[2] = FID;
	if((FID == 0x88)||(FID == 0x97)||(FID == 0x94)||(FID == 0x8C)){
		if(FID == 0x97){
			RWSamic.Cmd[4] = 12;
		}
		if(SamicNoCode()){
			return SetErrReturn(_ERR_SAFE_);
		}
	} else {
		RWSamic.Cmd[2] = FID;
		RWSamic.Cmd[4] = 1;
		if(SamicNoCode()){
			return SetErrReturn(_ERR_SAFE_);
		}
		RWSamic.Cmd[3]=0x01;
		RWSamic.Cmd[4]=pData[0];
		if(SamicNoCode()){//读全部数据
			return SetErrReturn(_ERR_SAFE_);
		}
	}
	return SUCCESS;
}

u8	Chk_Mac_99(void){


	if(RamData.Mac_99){
		if(WriteMacData(RamData.MacBuf, RamData.MacBuf+6)){
			SetErrReturn(_ERR_SAFE_);
			return SetMacErrCnt();
		}
	}
	return SUCCESS;
}

typedef struct {
	u16	Code;
	u8	FileID;
	u8	StartA;
}strMac99;
u8	GetSafeData(u8 *pData, u8 Lth){

	u8	Far_Mac_99[] = {
		0x84, 0x82, 0,				//File1, File2, Mode:
									//BIT0:0-WriteCard;1-WriteSAM
									//BIT1:0-先加分散因子,1-后加分散因子
									//BIT2:0-写普通文件; 1-写钱包文件
									//BIT3:0-正常操作, 1- 初始化钱包文件
		0, 0x05, 0x06	};
	static const __code strMac99 Mac99[] = {
		{	0x1001, SAFE_INFO_ID, (u8)((u32)(vUserCard.Alm1 - &vUserCard.Start)) },
		{	0x1002, SAFE_INFO_ID, (u8)((u32)(vUserCard.Alm2 - &vUserCard.Start)) },
		{	0x0306, SAFE_INFO_ID, (u8)((u32)(vUserCard.I_Mul - &vUserCard.Start)) },
		{	0x0307, SAFE_INFO_ID, (u8)((u32)(vUserCard.V_Mul - &vUserCard.Start)) },
		{	0x0402, SAFE_INFO_ID, (u8)((u32)(vUserCard.E_Num - &vUserCard.Start)) },
//#ifdef		_USE_CARD_
		{	0x0108, SAFE_INFO_ID,(u8)((u32)(vUserCard.StartTm - &vUserCard.Start)) },
		{	0x0109, FL_TBL_2_ID, (u8)((u32)(vfTmTbl.StartTm_4 - &vfTmTbl.Start)) },
//#else
		{	0x040E, SAFE_INFO_ID,(u8)((u32)(UserCard.H_Num - &UserCard.Start)) },
//#endif
	};
	u8	Cnt;
	u8	FID;
	u8	Lth645;
	u16	Code16;
	u32	Code32;
//	u32	NCode;
	if((RamData.IDACCess == 0)||(FlashInfo.RunInfo.MacErrCnt >= 200)){
		return SetErrReturn(_ERR_SAFE_);
	}
	Copy_4((u8*)&RamData.Write, pData+10);
	Copy_4((u8*)&Code32, pData+2);
	Lth645 = pData[1];
	if(pData[6] == 0x99){											//第一类数据
		if(Ex645.ExLth == 0){
			RevStr(pData+14, (Lth645-16));
		}
		Rev_4(((pData)+(Lth645-2)));
		if(Code32 == 0x040502FF){
			Far_Mac_99[1] = FL_TBL_2_ID;
			Far_Mac_99[4] = (u8)((u8*)&fTmTbl.Price-&fTmTbl.Start);
		} else if(Code32 == 0x040605FF){
			Far_Mac_99[1] = FL_TBL_2_ID;
			Far_Mac_99[4] = (u8)((u8*)&fTmTbl.Price[32]-&fTmTbl.Start);
		} else if(((u16)(Code32>>16)) == 0x0400){
			Code16 = (u16)Code32;
			Cnt = 0;
			while(1){
				if(Code16 == Mac99[Cnt].Code){
					Far_Mac_99[1] = Mac99[Cnt].FileID;
					Far_Mac_99[4] = Mac99[Cnt].StartA;
					break;
				}
				if(++Cnt >= (sizeof(Mac99)/sizeof(Mac99[0]))){
					return SetErrReturn(_ERR_NODATA_);
				}
			}
		} else {
			return SetErrReturn(_ERR_NODATA_);
		}
		Far_Mac_99[5] = (Lth645-16);
		CopyRam(RamData.MacBuf, Far_Mac_99, 6);
		CopyRam(RamData.MacBuf+6, pData+14, Lth645-12);
		RamData.Mac_99 = 1;

		if((Lth)&&(Lth != (Lth645-16))){
			return SetErrReturn(_ERR_OTHER_);
		}
		Lth645 -= 16;
		CopyRam(pData+6, pData+14, Lth645);
		if(Ex645.ExLth == 0){
			RevStr(pData+6, Lth645);								//处理数据反向
		}
	} else if(pData[6] == 0x98){
		if(Code32 == 0x04001006){
			if(Get98Data(pData+14, 0x8C , (Lth645-16))){
				return SetErrReturn(_ERR_SAFE_);
			}
			CopyRam( pData+6, pData+14, 4);
		} else {
			FID = BCD(89+pData[4]%5);
			if(Get98Data(pData+14, FID , (Lth645-16))){
				return SetErrReturn(_ERR_SAFE_);
			}
			Rev_4(pData+14);
			if(Cmp_4(pData+2, pData+14)){
				return SetErrReturn(_ERR_SAFE_);
			}
			if(RWSamic.Cmd[4] <= 4){
				return SetErrReturn(_ERR_OTHER_);
			}
			RWSamic.Cmd[4] -= 4;
		//	Lth645 = RWSamic.Cmd[4]-4;
			if((Lth)&&(Lth != RWSamic.Cmd[4])){
				return SetErrReturn(_ERR_OTHER_);
			}
			CopyRam( pData+6, pData+18, RWSamic.Cmd[4]);
			if(Code32 == 0x0400040F){								//GPS信息特殊处理
				Rev4Lst(pData+6, 2);
				RevStr(pData+6+8, 3);
			} else if(Ex645.ExLth){									//处理分组数据反向(日时段表等信息)
				RevStrLst(pData+6, Ex645.ExLth, RWSamic.Cmd[4]/Ex645.ExLth);
				Ex645.ExLth = 0;
			} else {
				RevStr(pData+6, RWSamic.Cmd[4]);								//处理数据反向
			}
		}
		RamData.Mac_98 = 1;
	}
	return SUCCESS;
}

				/*	以上为远程表本地表基本操作	*/
#ifdef		_USE_CARD_

u8	SetCardErr(u16 ECode){											//SetUserErr(u8 Num, u8 ErrCode)	插卡错误处理

	u8	ErrCode;
	u8	Mask;
	Mask = (u8)(ECode&0xC0);
	ErrCode = (u8)(ECode&0x3F);
	if((RamData.CardInState != 2)||(RamData.ICECode)){
		return FAIL;
	}
	if((ErrCode == 19)&&(RamData.Card_In == 0)){
		RamData.ICECode = 20;
	} else {
		RamData.ICECode = ErrCode;
	}
	if(Mask&_ERR_CARD_){
		Up_u16Data(&FlashInfo.RunInfo.ErrCardCnt, (u16)(FlashInfo.RunInfo.ErrCardCnt+1));
	}
	if(Mask&_ERR_NCMD_){
		ClrRam(RamData.ICECmd, 7);
	}
	Up_NormFroze(_FROZE_IC_ERR_);
	RamData.Disp.DispArryNum = 0x80;
	SetDispSubItem(CARD_FAIL|15);
#ifdef		_USE_CARD_BEEP_
	StartBeep(CARD_ERR_BEEP);										//蜂鸣器响
#endif
	RamData.HSecCnt = 20;
	RamData.Disp_BinkState = 1;
	RamData.DispErr = (u8)(ECode>>8);
	SetDispItem(RCODE(&DErr));
#if		_FAC_ == _FAC_XILI_
	RamData.Disp_Blink = 0;
#else
	RamData.Disp_Blink = 1;
#endif
	RamData.Disp.NextTm = 11;
	RamData.BkltTm = 11;
	return FAIL;
}

void	Up_Far_Use(void){

	if(RamData.FarSubTm == 0){
		Up_UseData();
		SetUserBack();
		RamData.FarSubTm = 15;
	}
}
extern const  struct STTblSetLst TblSetLst[];
static const __code strMvData mvPriceRcdStart[] = {
		SET_PCHAR( vRamData.FrozeBuf, 0),
		NORM_COPY( &vRamData.RTCTime, 4),
		NORM_COPY( &vRamData.Write, 4|_RET_),
};

void	Up_FPrice(u8 *pData, u8 Num)
{
	unsigned char	stemp[_MAX_TARRIF_][_cPrice_Unit_Len_];

	MoveDataLst(mvPriceRcdStart);
	CopyExteeprom( (u8 *)stemp ,_Price_Get(0,0) ,sizeof(stemp) /sizeof(char) ) ;
	NormCopy((u8*)stemp, sizeof(stemp) /sizeof(char));
	CopyExteeprom( (u8 *)stemp ,_Price_Get(1,0),sizeof(stemp) /sizeof(char)) ;
	NormCopy((u8*)stemp, sizeof(stemp) /sizeof(char) );
	Up_FrozeData(RamData.FrozeBuf, _FROZE_TPRICE_SET_);
	SetEvtState(_EVT_PRICE_);
	tpChar = pData;
	N_MvData(pData, BCD_IN(4, 3, 0), _LTH_4_|_cPrice_Max_Num_);
	Write_EEData( pData,	_Price_Get(Num,0)	,TblSetLst[PTBL_START+Num].Lth);
}

void	Up_LData(u8 *pData, u8 Num)
{
	strLTbl  stLTbl,stLTbl1;

	MoveDataLst(mvPriceRcdStart);

	//NormCopy((u8*)GAddr.pLTbl[0], 57);
	CopyExteeprom( (u8 *)&stLTbl ,_LTBl_Get(0)	 ,sizeof(strLTbl)	 ) ;
	NormCopy((u8*)&stLTbl, 57);
	//NormCopy((u8*)GAddr.pLTbl[1], 57);
	CopyExteeprom( (u8 *)&stLTbl1 ,_LTBl_Get(1)	 ,sizeof(strLTbl)	 ) ;
	NormCopy((u8*)&stLTbl1, 57);
	Up_FrozeData(RamData.FrozeBuf, _FROZE_LPRICE_SET_);
	tpChar = pData;
	N_MvData(pData, BCD_IN(4, 4, 0), _LTH_4_|6);
	N_MvData(pData+24, BCD_IN(4, 3, 0), _LTH_4_|7);
	NormCopy(pData+52, 12);
	SetEvtState(_EVT_LSTEP_);

	//if((Num == 0)&&(strCmp((u8*)(GAddr.pLTbl[0]+45), pData+45, sizeof(12)))){
	//	RamData.YTm_Chg = 1;
	//}
	if((Num == 0)&&(strCmp((u8*)(stLTbl.YStepTm), pData+45, 12)))
	{
			RamData.YTm_Chg = 1;
	}
	//Up_TTbl(pData, LTBL_START+Num);
	Write_EEData( pData,_LTBl_Get(Num), TblSetLst[LTBL_START+Num].Lth);

}

u8	Read_Purse(void){													//读取钱包

	static const __code u8 MacReadPurseCnt[] = {
		0x0C, 0x82, (MAC_WCARD|MAC_PURSE|MAC_READ_SN|MAC_NEXT),
		0x01, 0x00, 4,
		0x0C, 0x82, (MAC_WCARD|MAC_PURSE|MAC_READ_SN),
		0x03, 4, 4,
	};
	u32	Lst[4];
	if(cGetMacData(MacReadPurseCnt, (u8*)Lst)){
		return FAIL;
	}
	RevStrLst((u8*)Lst, 4, 3);
	RamData.ePurse = Lst[0];
	Up_u32Data(&FlashInfo.SafeInfo.ChargeCnt, Lst[2]);
	return SUCCESS;
}

u8	ChkMaxPurse(u32 Inc){											//检查囤积金额

	if(FlashInfo.SetInfo.MaxPurse == 0){
		return SUCCESS;
	}
	if((RamData.u_Remain + Inc) <= FlashInfo.SetInfo.MaxPurse){
		return SUCCESS;												//不超过囤积金额
	}
	return FAIL;													//超囤积金额
}

u8	ChkCharge(u8 *pData){											//远程充值检查
unsigned long ltemp ;
	u32	IsCharge;
	if(CheckKeyChg(&FlashInfo.SetInfo.EKeyState) != 0){
		RamData.FErrWord = _FEW_IDERR_;
		return FAIL;
	}
	CopyRam(pData+8, pData+10, 22);									//调整4字节对齐
	RevStr(pData+8+12, 6);											//调整用户号
	Rev_4(pData+8+12+6);											//调整MAC
	//IsCharge = (*((u32*)(pData+12))) - FlashInfo.SafeInfo.ChargeCnt;
	ltemp =  _slpgetp(pData+12);
	IsCharge = ltemp - FlashInfo.SafeInfo.ChargeCnt;
	if(IsCharge!=1){
		if(!IsCharge){
			RamData.FErrWord = _FEW_REPEATCHARGE_;
			//if((*(u32*)(pData+12)) <= 1){
			if(ltemp <= 1){
				return 2;
			}
		}else{
			RamData.FErrWord = _FEW_CHARGECNTERR_;
		}
		return 	FAIL;
	} else {
		ltemp = _slpgetp(pData+8);
		//if(ChkMaxPurse(*(u32*)(pData+8))){									//超囤积金额
		if(ChkMaxPurse(ltemp)){									//超囤积金额
			RamData.FErrWord = _FEW_OVERCHARGE_;
			return FAIL;
		}
	}
	return SUCCESS;
}

void	Up_ChargeFroze(u32 Inc, u32 LastRemain){					//生成购电记录
	static const __code strMvData mvChargeFroze[] = {
		READ_RAM( &vRamData.RTCTime, COPY_L(4)),
		READ_RAM( &vFlashInfo.SafeInfo.ChargeCnt, COPY_L(12)),
		READ_RAM( &vRamData.u_Remain, COPY_L(4)),
		READ_RAM( &vFlashInfo.SafeInfo.SumCharge, COPY_L(4)|_RET_),
	};
	u32	Lst[6];

	Up_Remain();
	tpChar = (u8*)Lst;
	MoveDataLst(mvChargeFroze);
	Lst[2] = Inc;
	Lst[3] = LastRemain;
	Up_FrozeData( (u8*)Lst, _FROZE_CHARGE_);
}

void	Up_UseData(void){

	u32	Lst[2];
	u32	Remain;
	static const __code strRWSamic SubCmd = {{0x80,0x30,0x00,0x04,4}, WRITE_ESAM, (u8*)&RamData.ePurse };
	static const __code u8 MacReadPurse[] = {
		0x0C, 0x82, (MAC_WCARD|MAC_PURSE|MAC_READ_SN),
		0x01, 0x00, 4,
	};
	if(cGetMacData(MacReadPurse, (u8*)Lst)){
		return;
	}
	if(FlashInfo.SafeInfo.SumCharge > RamData.LVSave[0].SumMoney){
		Remain = FlashInfo.SafeInfo.SumCharge - RamData.LVSave[0].SumMoney;
	} else {
		Remain = 0;
	}
	Rev_4((u8*)&Lst[0]);
	if(Lst[0] > Remain){
		RamData.ePurse =  (Lst[0] - Remain);
		Rev_4((u8*)&RamData.ePurse);
		if(cSamic(&SubCmd)){
			Read_Purse();
			return;
		}
		Read_Back((u8*)Lst);
		Rev_4((u8*)&Lst[0]);
		RamData.ePurse =  (Lst[0]);
		RamData.UseChgTm = 0;
	}
	Up_Remain();
	RamData.Use_Chg = 0;
	RamData.NotChg = 0;
}

static const __code u8 MacUserPurse[] = {
		0x82,	0x0C, (MAC_PURSE),		//File1, File2, Mode:
		0, 0x01, 0x08,
};
/*			远程充值			*/
u8 	FarIncPurse(u8 *pData){

	u32 Remain; u32 ltemp;
	Remain = RamData.u_Remain;
	Rev4Lst(pData+8, 3);											//调整购电金额/次数/MAC
	if(cWriteMacData(MacUserPurse,pData+8)){						//增加电量
		RamData.FErrWord = _FEW_ESAMERR_;	//RamData.FErrWord.ErrESAMAccess=1;
		return SetMacErrCnt();
	}
	RamData.FarSubTm = 0;
	RevCopyRam((char *)&ltemp,pData+8+4,4);//Rev_4(pData+8+4);
	ESafeInfo.SumCharge = FlashInfo.SafeInfo.SumCharge + ltemp ;   //ESafeInfo.SumCharge = FlashInfo.SafeInfo.SumCharge + (*(u32*)(pData+8+4));
	ESafeInfo.ChargeCnt = FlashInfo.SafeInfo.ChargeCnt+1;
	Copy_FlashInfo( (u8*)&FlashInfo.SafeInfo.SumCharge, (u8*)&ESafeInfo.SumCharge, sizeof(ESafeInfo.SumCharge)+sizeof(ESafeInfo.ChargeCnt));
	Up_ChargeFroze(ltemp, Remain); //Up_ChargeFroze(*(u32*)(pData+8+4), Remain);
	return SUCCESS;
}

/*			远程充值结束处理			*/

u8	ID_ChargeOver(void){

	SetDispSubItem( CHARGE_OK|5);
	return ID_SuccessRet();
}

u8	FarIncOver(u8 *pData){

//	u32	N;
	UpJDWriter();
	if(FlashInfo.RunInfo.FarJDPreTm){
		Up_u32Data(&FlashInfo.RunInfo.RcdTm.FarJDOffPreTime, 1);
	} else if(_ISJDMODE_OUT()){ // } else if(_JD_MODE_ == 0){
		ChkAlmJD();
	} else {
		GetAlmState();
	}
	if(RamData.Next_JD){
		ClrEBit(_BIT_ALM_OFF_);
	}
	Up_UseData();
	return ID_ChargeOver();
}

static const u8 MacUseIDAdd[] = {//增加用户号
	0x84, 0x82, 0,				//File1, File2, Mode:
	//BIT0:0-WriteCard;1-WriteSAM
	//BIT1:0-先加分散因子,1-后加分散因子
	//BIT2:0-写普通文件; 1-写钱包文件
	//BIT3:0-正常操作, 1- 初始化钱包文件
	(u8)(vESAMInfo.H_Num - &vESAMInfo.Start), (u8)(vESAMInfo.H_Num - &vESAMInfo.Start), 6,
};

void	CardClrUse(u8 *pData){

	Rev4Lst(pData, 2);
	Copy_FlashInfo((u8*)&FlashInfo.SafeInfo.SumCharge, pData, 8);	//刷新累计购电金额/购电次数
	RamData.Evt_JDClr = 1;
	Clr_AllUse();
	Up_ChargeFroze(FlashInfo.SafeInfo.SumCharge, 0);				//购电记录
}

u8	ID_AccessRstCharge(u8* pData){

	static const __code u8 MacResCharge[] = {								//Mac重置钱包指令
		0x82,	0x0C, (MAC_PURSE|MAC_INITPURSE|MAC_NEXT),
		0x00, 0x01, 4,
		0x82,	0x0C, (MAC_PURSE|MAC_INITPURSE),
		0x00, 0x03, 4,
	};
	//u8 Cnt;

	if ( !_ISMETER_LOCAL())
	{
			RamData.FErrWord = _FEW_OTHER_	;
			return FAIL;
	}
	if((CheckKeyChg(&FlashInfo.SetInfo.EKeyState) != 2)&&(RamData.Write != 0x52707000)){
		RamData.FErrWord = _FEW_IDERR_;
		return FAIL;
	}
	Rev4Lst(pData+10, 4);									//调整购电金额/MAC/购电次数/MAC
	if(CheckZero(pData+10+8, 4)){
		RamData.FErrWord = _FEW_CHARGECNTERR_;
		return FAIL;
	}
	if(cWriteMacData( MacResCharge, pData+10)){						//重置钱包数据
		RamData.FErrWord = _FEW_ESAMERR_;
		return SetMacErrCnt();
	}
	// 此处三行替代补注释掉三行，返返回再操作
	CopyRam ( RamData.MacBuf  ,pData+10 , 16  );
	RamData.bClr_Comm = 1 ;
	return ID_ChargeOver();

	//CardClrUse(pData+10);
	//ChkAlmJD();
	//return ID_ChargeOver();
}
void  Clr_Comm(void)
{
	if ( RamData.bClr_Comm )
	{
		CardClrUse(RamData.MacBuf);
		ChkAlmJD();
	}
	RamData.bClr_Comm =0;
}


u8 ID_AccessOpenCharge(u8* pData){

	u8	Ret;unsigned long ltemp;
	if ( !_ISMETER_LOCAL())
		{
				RamData.FErrWord = _FEW_OTHER_	;
				return FAIL;
		}
	Ret = ChkCharge(pData);
	if(FAIL==Ret ){
		return FAIL;
	}
	if(FlashInfo.RunInfo.EState&(_ESTATE_LOC_OPEN_|_ESTATE_FAR_OPEN_)){
		if(strCmp(pData+8+12, FlashInfo.SafeInfo.H_Num, 6 ) != 0){	//户号号不同
			RamData.FErrWord = _FEW_HNUMERR_;
			return FAIL;
		} else
		{
			ltemp =  _slpgetp((pData+8+4));
			//if((*((u32*)(pData+8+4))) > 1){						//重复开户
			if ( ltemp >1){
			RamData.FErrWord = _FEW_OTHER_;		//.Other = 1;
			return FAIL;
		}
		}
	}
	if(WriteMacData(MacUseIDAdd,pData+8+12)){						//记录户号
		RamData.FErrWord = _FEW_ESAMERR_;
		return SetMacErrCnt();
	}
	Copy_FlashInfo(FlashInfo.SafeInfo.H_Num, pData+8+12, 6 );		//更新户号
	if(SUCCESS == Ret){
		if(FarIncPurse(pData)){
			return FAIL;
		}
	}
	SetEBit(_BIT_FAR_OPEN_);
	RunState_FOpen = 0;
	return FarIncOver(pData);
}

u8 ID_AccessNextCharge(u8* pData){

	u8	Ret;
	if ( !_ISMETER_LOCAL())
	{
			RamData.FErrWord = _FEW_OTHER_	;
			return FAIL;
	}
	if((FlashInfo.RunInfo.EState&(_ESTATE_LOC_OPEN_|_ESTATE_FAR_OPEN_)) == 0){
		RamData.FErrWord = _FEW_CHARGECNTERR_;
		return FAIL;
	}
	Ret = ChkCharge(pData);
	if(strCmp(pData+8+12, FlashInfo.SafeInfo.H_Num, 6 ) != 0){		//户号号不同
		RamData.FErrWord = _FEW_HNUMERR_;
		return FAIL;
	}
	if(Ret){
		return FAIL;
	}
	if(WriteMacData(MacUseIDAdd,pData+8+12)){						//验证户号
		RamData.FErrWord = _FEW_ESAMERR_;
		return SetMacErrCnt();
	}
	if(FarIncPurse(pData)){
		return FAIL;
	}
	return FarIncOver(pData);
}


void	RefreshESAM(void){
	if(RamData.IDACCess == 0){
		if(Samic_Rst(_RW_SAM_)){
			if(++RamData.ESAMErrCnt > 5){
				Set_ESAMErr();
			}
			Card_Over();
			return;
		}
		if(FlashInfo.RunInfo.EState&_ESTATE_ESAM_){
					ClrEBit(_BIT_ESAM_);
		}
	}
	if(RunState_ESAM == 1){
		Up_LVSave();
		Wait_Rst(ESAM_OK);
//		return;
	}
	if(RamData.Use_Chg){
		Up_UseData();
	}
	if(RamData.IDACCess == 0){
		Card_Over();
	}
}

u8	ReadICFile(u8 size){											//读取购电文件

	if(SamicCode(&ReadCmd, size, 4))
		return FAIL;												//未发行卡片
	if(ComData[0] != 0x68)
		return SetUserErr(34, 14);									//文件格式不合法
	if((*(u16*)(ComData+2)) != REV16(size-6))
		return SetUserErr(34, 14);									//文件格式不合法
	if(ComData[size-1] != 0x16)
		return SetUserErr(34, 14);									//文件格式不合法
	if(ComData[size-2] != (u8)GetSum(ComData+1, size-3))
		return SetUserErr(34, 14);									//文件格式不合法
	return SUCCESS;
}

u8	ChkUserMac( u8* Buf, u8 Bias, u8 Length){						//检查用户卡数据MAC

	u8	macUser[] = {
		0x81, SAFE_INFO_ID, 0,
		0, 0, 0,
	};
	if((Bias + Length) > (sizeof(UserCard)-2)){
		return SetUserErr(32, 22|_ERR_NCMD_);
	}
	macUser[3] = Bias;
	macUser[4] = Bias;
	macUser[5] = Length;
	if(GetMacData(macUser, Buf)){
		return FAIL;
	}
	if(strCmp( Buf, (ComData+Bias), Length)){
		return SetUserErr(32, 23|_ERR_CARD_);
	}
	return WriteMacData(macUser, Buf);
}

u8	Do_TmTblMac(u8 Mode){											//Mode: 0-读MAC数据 1-验证MAC数据 2-刷新数据

	u8	Cnt;
	//u8	Mask;
	u16	Bias;
	Bias = 0;
	for(Cnt = 0; Cnt < 4; Cnt++){
		if(UserCard.Chg&(1<<Cnt)){
			if(Mode == 0){
				if(cGetMacData(MacTmTbl[Cnt], ComData+sizeof(UserCard)+Bias)){
					return FAIL;
				}
			} else if(Mode == 1){
				if(cWriteMacData(MacTmTbl[Cnt], ComData+sizeof(UserCard)+Bias)){
					return FAIL;
				}
			} else if(Mode == 2){
				if(Cnt < 2){
					Up_FPrice(ComData+sizeof(UserCard)+Bias+2, (Cnt&1));
					if(Cnt == 1){
						Up_u32Data(&FlashInfo.SafeInfo.StartTm_3, ESafeInfo.StartTm_3);
						ClrEBit(_BIT_FL_PRICE_);
					}
				} else {
					Up_LData(ComData+sizeof(UserCard)+Bias, (Cnt&1));
					if(Cnt == 3){
						ESafeInfo.StartTm_4 = GetTimeCnt(ComData+sizeof(UserCard)+Bias+64, S_MIN|T_SEC);
						Up_u32Data(&FlashInfo.SafeInfo.StartTm_4, ESafeInfo.StartTm_4);
						ClrEBit(_BIT_STEP_PRICE_);
					}
				}
			}
			Bias += MacTmTbl[Cnt][5]+4;
		}
	}
	return SUCCESS;
}

u8	Do_SetupCard(void){												//参数预置卡处理

	static const __code u8 MacResPurse[] = {						//Mac重置钱包指令
		0x85,	0x0C, (MAC_PURSE|MAC_INITPURSE|MAC_NEXT),
		0x00, 0x01, 4,
		0x85,	0x0C, (MAC_PURSE|MAC_INITPURSE),
		4, 0x03, 4,
	};
	u8	N, L;
	if(ReadICFile(sizeof(SetupCard)))
		return FAIL;
	if(SetupCard.Cmd != CMD_SETUPCARD)
		return SetUserErr(34, 14);									//文件格式不合法
	ReadMacRand(0);
	if(ChkUserMac(ComData+sizeof(SetupCard), 0, (SetupCard.StartTm - &SetupCard.Start))){
		return FAIL;												//MAC校验错
	}
	Up_u16Data((u16*)&FlashInfo.SafeInfo.Cmd, SetupCard.Cmd|(((u16)SetupCard.UserType)<<8));	//刷新命令字/用户类型
	if(cGetMacData( MacResPurse, RamData.MacBuf)){					//读取钱包数据
		return FAIL;
	}
	if(*(u32*)(RamData.MacBuf+8)){									//购电次数不为零返回
		return SetUserErr(34,17|_ERR_NCMD_);
	}
	if(Do_TmTblMac(0)){												//读取费率表信息
		return FAIL;
	}
	ClrEBit( _BIT_FL_PRICE_);
	if(SetupCard.Chg&BIT1){
		N = (SetupCard.StartTm - &SetupCard.Start);
		if(SetupCard.Chg&BIT7){
			L = (&SetupCard.Sum - SetupCard.StartTm);
		} else {
			L = (&SetupCard.FLNum - SetupCard.StartTm);
		}
	} else if(SetupCard.Chg&BIT7){
		N = (SetupCard.Alm1 - &SetupCard.Start);
		L = (&SetupCard.Sum - SetupCard.Alm1);
	} else {
		N = 0;
		L = 0;
	}
	if(L){
		if(ChkUserMac(RamData.MacBuf+24, N, L)){					//验证费率表修改信息
			return FAIL;
		}
	}
	if(Do_TmTblMac(1)){												//验证费率表信息
		return FAIL;												//不成功返回
	}
	GetSafeInfo((strESAMInfo*)&UserCard);
	if(SetupCard.Chg&BIT7){
//		ESafeInfo.MaxPurse = 99999999;								//恢复默认参数
//		ESafeInfo.OverDraftLimit = 0;
//		ESafeInfo.JDOnMinPurse = 0;
		Copy_FlashInfo((u8*)&FlashInfo.SafeInfo.Alm1, (u8*)&ESafeInfo.Alm1, (u8*)&ESafeInfo.SumCharge - (u8*)&ESafeInfo.Alm1);
	}
	Do_TmTblMac(2);
	if(cWriteMacData( MacResPurse, RamData.MacBuf)){							//读取钱包数据
		return FAIL;
	}
	CardClrUse(RamData.MacBuf);
	RamData.IRACCessTm = 0;
	RunState_IR = 0;
	return SUCCESS;
}

u8		Up_HNum(void){												//用户卡更新户号


	if(ChkUserMac(RamData.MacBuf, 0, 4)){							//检查文件头
		return FAIL;
	}
	if(ChkUserMac(RamData.MacBuf, (UserCard.E_Num-&UserCard.Start), 12)){	//刷新户号/表号
		return FAIL;												//ESAM验证错误
	}
	Copy_FlashInfo(FlashInfo.SafeInfo.H_Num, ESafeInfo.H_Num, sizeof(ESafeInfo.H_Num));
	return SUCCESS;
}

u8		Chk_HNum(void){												//检查户号

	if(strCmp(FlashInfo.SafeInfo.H_Num, ESafeInfo.H_Num, sizeof(ESafeInfo.H_Num))){
		return SetUserErr(33,12|_ERR_NCMD_);
	}
	return SUCCESS;
}

void	ClrJDPreState(void){										//预跳闸立即合闸

	if(FlashInfo.RunInfo.FarJDPreTm){
		ClrJDOffTime();
		ClrEBit(_BIT_JD_CMD_);
	}
}

u8	Do_UserCard(void){												//用户卡处理

	static const __code strRWSamic RdUserBack = {{ 0x00, 0xB0, 0x85, 0x00, 0x04 }, READ_IC, RamData.Rand_Num };
	static const __code strRWSamic CmdICTest = {{ 0x00, 0x82, 0x00, 0x02, 0x08}, WRITE_IC, RamData.At_Back };
	static const __code u8 MacUserBack[] = {						//返写用户卡
		ESAM_WBACK_ID, 0x85, (MAC_WCARD|MAC_READ_SN),
		0, 0, (&vUserBack.End - &vUserBack.Start)+1,				//sizeof(strUserBack),
	};
	struct {														//增加钱包数据
		u32	NewInc;
		u32	ChargeCnt;
		u32	Mac;
		u8	S_Num[8];
	} IncPurseMac;
	u8	WBack;
	u32	N;
	u8	L;
	u32	Remain;
//	u16 n16;
	u8 n8;
	u32 ltemp[2];

	if(cSamic(&RdUserBack))											//读取返写文件
		return FAIL;
	if(CheckDataAct(RamData.Rand_Num)){
		WBack = 0;
	} else {
		WBack = 1;
	}
	if(ReadICFile((&UserCard.End - &UserCard.Start)+1))				//读取购电文件
		return FAIL;
	if(UserCard.Cmd != CMD_USERCARD)
		return SetUserErr(34, 14);									//卡信息错误返回
	if((UserCard.CardType == 0)||(UserCard.CardType > 3)){			//卡类型错返回
		return SetUserErr(32,8);
	}
	if(ReadRand(1)) return FAIL;									//取随机数
	if(GetETest(2)) return FAIL;									//生成认证数据
	if(cSamic(&CmdICTest)) return FAIL;								//外部认证
	if(strCmp(FlashInfo.SafeInfo.E_Num, UserCard.E_Num, sizeof(ESafeInfo.E_Num))){
		return SetUserErr(33,11);
	}
	if(ReadMacRand(0))												//读取MAC随机数
		return FAIL;
	if(cGetMacData(MacUserPurse, (u8*)&IncPurseMac))				//获得钱包信息
		return FAIL;
	GetSafeInfo((strESAMInfo*)&UserCard);
	RevCopy_4((u8*)&N, (u8*)&IncPurseMac.ChargeCnt);				//获得购电次数
	if(UserCard.CardType == 0x01){
		if(FlashInfo.RunInfo.EState&(_ESTATE_FAR_OPEN_|_ESTATE_LOC_OPEN_)){
			if(Chk_HNum()){
				return(FAIL);
			}
		}
		if(N > 1){
			return SetUserErr(34, 17|_ERR_NCMD_);					//开户卡购电次数错
		}
	} else {
		if((FlashInfo.RunInfo.EState&(_ESTATE_FAR_OPEN_|_ESTATE_LOC_OPEN_)) == 0){
			if(UserCard.CardType == 0x02){
				return SetUserErr(34, 15|_ERR_NCMD_);				//未开户插购电卡
			} else {
				return SetUserErr(34, 16|_ERR_NCMD_);				//未开户插补卡
			}
		}
		if(Chk_HNum()){
			return(FAIL);
		}
		if(UserCard.CardType == 0x02){
			if(strCmp(RamData.IC_Num, FlashInfo.RunInfo.IC_Num, 8)){//用户卡检查卡序号
				return SetUserErr(33, 13|_ERR_NCMD_);				//非上次卡
			}
		}
	}
	if(N > (FlashInfo.SafeInfo.ChargeCnt+1)){						//购电次数过大
		SetUserErr(34, 17|_ERR_NCMD_);
		goto User_WBack;
	} else if(N < FlashInfo.SafeInfo.ChargeCnt){					//购电次数过小
		if(UserCard.CardType == 0x01){
			goto User_L;
		} else if(UserCard.CardType == 0x03){
			goto User_L;
		} else {
			goto User_WBack;
		}
	} else if(N == FlashInfo.SafeInfo.ChargeCnt){					//已购电卡/补卡/零次开户卡处理
		if(WBack){
			if(strCmp(RamData.IC_Num, FlashInfo.RunInfo.IC_Num, 8)){//有返写时检查卡序号
				return SetUserErr(33, 13|_ERR_NCMD_);				//非上次开户卡(21)
			} else {
				goto 	User_WBack;
			}
		}
		if((UserCard.CardType == 0x01)&&((FlashInfo.RunInfo.EState&(_ESTATE_FAR_OPEN_|_ESTATE_LOC_OPEN_)) == 0)){
			if(Up_HNum()){
				return FAIL;
			}
			ClrJDPreState();										//预跳闸立即合闸
		}
		goto	W_ESAMSet;
	}
	if(WBack){														//有返写不购电
		SetUserErr(34, 18|_ERR_NCMD_);
		goto	User_WBack;
	}
	RevCopy_4((u8*)&N, (u8*)&IncPurseMac.NewInc);
	if(ChkMaxPurse(N)){												//超囤积金额返回
		SetUserErr(36, 21|_ERR_NCMD_);
		return FAIL;
	}
	if((UserCard.CardType == 0x01)&&((FlashInfo.RunInfo.EState&(_ESTATE_FAR_OPEN_|_ESTATE_LOC_OPEN_)) == 0)){//开户卡记录户号
		if(Up_HNum()){
			return FAIL;
		}
	}
	Remain = RamData.u_Remain;
	if(cWriteMacData(MacUserPurse, (u8*)&IncPurseMac)){				//增加购电金额
		return FAIL;
	}
	Rev_4((u8*)&IncPurseMac.ChargeCnt);
	//((u32*)RamData.MacBuf)[0] = FlashInfo.SafeInfo.SumCharge +  IncPurseMac.ChargeCnt;	//此时IncPurseMac.ChargeCnt 为本次购电金额
	//((u32*)RamData.MacBuf)[1] = FlashInfo.SafeInfo.ChargeCnt+1;
	ltemp[0] = FlashInfo.SafeInfo.SumCharge + N;// IncPurseMac.ChargeCnt;	//此时IncPurseMac.ChargeCnt 为本次购电金额
	ltemp[1] = FlashInfo.SafeInfo.ChargeCnt+1;
	CopyRam ( RamData.MacBuf , (u8 *)&ltemp ,8);
	Copy_FlashInfo( (u8*)&FlashInfo.SafeInfo.SumCharge, RamData.MacBuf, 8);
	//Up_ChargeFroze( IncPurseMac.ChargeCnt, Remain);					//此时IncPurseMac.ChargeCnt 为本次购电金额
	Up_ChargeFroze( N, Remain);  //Up_ChargeFroze( IncPurseMac.NewInc, Remain);					//此时IncPurseMac.ChargeCnt 为本次购电金额
	ClrJDPreState();												//预跳闸立即合闸
W_ESAMSet:															//更新ESAM参数
	WBack = UserCard.Chg;
	if(UserCard.CardType != 0x01){
		UserCard.Chg &= ~0x85;
	}
	if(Do_TmTblMac(0)){
		return FAIL;
	}
	if(UserCard.Chg&(BIT7|BIT1)){
		if(UserCard.Chg&BIT7){
			n8 = (UserCard.Alm1 - &UserCard.Start);
			if(UserCard.CardType == 0x01){
				L = 14;
			} else {
				L = 8;
			}
			if(UserCard.Chg&BIT1){
				n8 = (UserCard.StartTm - &UserCard.Start);
				L += 6;
			}
		} else {
			n8 = (UserCard.StartTm - &UserCard.Start);
			L = 5;
		}
		if(ChkUserMac(RamData.MacBuf, n8, L)){
			return FAIL;											//不成功返回
		}
/*		if(WriteUserMac(MacBuf, n8, L)){
			return FAIL;											//不成功返回
		}	*/
	}
	if(Do_TmTblMac(1)){
		return FAIL;												//不成功返回
	}
	GetSafeInfo((strESAMInfo*)&UserCard);
	Do_TmTblMac(2);
	if(UserCard.Chg&BIT7){
		Copy_FlashInfo((u8*)&FlashInfo.SafeInfo.I_Mul, (u8*)&ESafeInfo.I_Mul, 8);	//修改倍率
		Copy_FlashInfo((u8*)&FlashInfo.SafeInfo.Alm1, (u8*)&ESafeInfo.Alm1, 8);
	}
	if(UserCard.Chg){
		N = (0x99980000|(((u16)UserCard.Cmd)<<8)|(UserCard.Chg));
		AddProgRecord((u8*)&N);
	}
User_L:
	if(UserCard.CardType == 0x01){
		SetEBit(_BIT_LOC_OPEN_);
		RunState_LOpen = 0;
		Copy_FlashInfo(FlashInfo.RunInfo.IC_Num, RamData.IC_Num, 8);
	} else if(UserCard.CardType == 0x03){
		Copy_FlashInfo(FlashInfo.RunInfo.IC_Num, RamData.IC_Num, 8);
	}
User_WBack:
	SetUserBack();
	ReadMacRand(1);
	if(cMacTrans(MacUserBack, RamData.MacBuf)){
		return FAIL;
	}
	return SUCCESS;
}

typedef struct {
		u8	Cmd;
		u8	Safe;
		u8	(* const Prog)(void);
	} strTypeTbl;


void	Do_Card(void){												//读卡处理

	static const __code strTypeTbl TypeTbl[] = {
//		命令字			认证/公开状态/处理函数
		{ CMD_USERCARD,		0, Do_UserCard },				//用户卡
		{ CMD_SETUPCARD,	2, Do_SetupCard },				//参数预置卡_
	};
	strTypeTbl const __code *pTypeTbl = TypeTbl;
	u8	CardNum;
	u8	K;

	if(RamData.Card_In == 0){
		RamData.Event_CardIn = 0;
		return;
	}
	if((RamData.LVD_Volt < _GetLVD_RST())||((RamData.Vph.sVI)&&(RamData.Vph.sVI < _iVolStand60_))){
		return;
	}
	CloseProgRecord();
	RamData.ICECode = 0;
	RamData.CardInState = 2;
	CardNum = sizeof(TypeTbl)/sizeof(TypeTbl[0]);
	Up_LVSave();
	RamData.Disp.SubDispTm = 0;
	RamData.BkltTm = 60;
	SetDispSubItem( CARD_IN|5);
	RamData.Last_Remain = RamData.s_Remain;
	SetDispItem(RCODE(&FileSet.DArryCardOver[0]));							//插卡显示
	SetRam(RamData.IC_Num, 8);
	if((RamData.Lo_Pwr)||((RamData.Vph.sVI)&&(RamData.Vph.sVI < _iVolStand75_))){
		SetUserErr(31, 1|_ERR_NCMD_);								//电压低处理
		goto _End_DoCard;
	}
	if(Samic_Rst(_RW_KA_)){
		goto _End_DoCard;
	}
	//RamData.WriterCode = REV32(*(u32*)(RamData.IC_Num+4));			//记录操作者代码
	RevCopy_4((u8*)&RamData.Write, RamData.IC_Num+4);
	if(cSamic(&CmdSelect)){											//选择主文件
		goto _End_DoCard;
	}
	Up_UseData();													//扣减金额
	if(cSamic(&ReadCmd)){											//读文件头
		goto _End_DoCard;
	}
	if(UserCard.Start != 0x68){										//检查起始字节
		SetUserErr(34,14);
		goto _End_DoCard;
	}
	CardNum = 0;
	while(1){
		if(pTypeTbl ->Cmd == (UserCard.Cmd)){
			K = CheckKeyChg(&FlashInfo.SetInfo.EKeyState);
			if(K != pTypeTbl ->Safe){
				SetUserErr(32, 9|_ERR_NCMD_);
				goto _End_DoCard;
			}
			if(ReadRand(0)){										//读取随机数
				goto _End_DoCard;
			}
			if(GetETest(0x01)){										//生成ESAM认证数据
				goto _End_DoCard;
			}
			if(SamicCode(&Cmd0088, 0x01, 3)){						//生成认证数据
				goto _End_DoCard;
			}
			if(Read_Back(RamData.Rand_Num_Far)){					//读取认证数据
				goto _End_DoCard;
			}
			if(strCmp(RamData.At_Back, RamData.Rand_Num_Far, 8) != 0){	//比较认证数据
				SetUserErr(32, 5|_ERR_CARD_);
				goto _End_DoCard;
			}
			RamData.TRx_Wait = 1;
			if(pTypeTbl ->Prog()){
				SetUserErr(_EOTHER_, 0);
				goto _End_DoCard;
			}
			UpJDWriter();
			if(RamData.ICECode == 0){
				RamData.CardInState = 1;
				SetDispArry(_CARD_OVER_DISP_ARRY_);					//四川表新增插卡显示
				SetDispSubItem( CARD_OK|5);
			//	RamData.Disp_State = 1;
				RamData.Disp.NextTm = 6;RamData.Disp.DispArryNum = 0x80;
#ifdef		_USE_CARD_BEEP_
				StartBeep(CARD_OK_BEEP);							//设置蜂鸣器输出
#endif
			}
			break;
		}
		if(++CardNum >= NUMOF(TypeTbl)){
			SetUserErr(32, 8);
			break;
		}
        pTypeTbl++;
	}
_End_DoCard:
	Up_UseData();													//刷新电量数据
	Card_Over();
	RamData.Event_Feilv = 1;
	if(FlashInfo.RunInfo.FarJDPreTm == 0){
		ChkAlmJD();
	}
	ChkYFroze();
	if(RamData.Next_JD){
		ClrEBit(_BIT_ALM_OFF_);
	}
	RamData.Event_CardIn = 0;
	RamData.FarSubTm = 0;
}
#else
u8	SetCardErr(u16 ECode)
{}
#endif


u8	Read_ESAMData(void){

//	u32	Sum;

	u8	E_Num_Chg;
	E_Num_Chg = 0;
	if(Samic_Rst(_RW_SAM_)){
		Card_Over();
		return FAIL;
	}
	if(Read_ESAM( RamData.MacBuf, ((u16)SAFE_INFO_ID)<<8,  _strESAMInfo_Len_)){ //)sizeof(strESAMInfo))){
		Card_Over();
		return FAIL;
	}
	GetSafeInfo((strESAMInfo *)RamData.MacBuf);
	if((ESafeInfo.Cmd!= 0xFF)&&(ESafeInfo.Cmd != 0)&&(strCmp((u8*)&ESafeInfo, (u8*)&FlashInfo.SafeInfo, sizeof(ESafeInfo)-24))){
		if(Read_ESAM((u8*)&ESafeInfo, ((u16)SAFE_INFO_ID)<<8,  _strESAMInfo_Len_ )){//sizeof(strESAMInfo))){
			Card_Over();
			return FAIL;
		}
		if(strCmp((u8*)&ESafeInfo, RamData.MacBuf, _strESAMInfo_Len_ )){//sizeof(strESAMInfo))){
			Card_Over();
			return FAIL;
		}
		GetSafeInfo( (strESAMInfo *)RamData.MacBuf);
		if(strCmp(ESafeInfo.E_Num, FlashInfo.SafeInfo.E_Num, sizeof(ESafeInfo.E_Num))){
			E_Num_Chg = 1;
		}
		Copy_FlashInfo( (u8*)&FlashInfo.SafeInfo, (u8*)&ESafeInfo, sizeof(ESafeInfo)-24);
	}
if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
	RSet_WDG(0x78);if(Read_Purse()){
		Card_Over();
		return FAIL;
	}
	if(E_Num_Chg){
		Up_u32Data(&FlashInfo.SafeInfo.SumCharge, RamData.LVSave[0].SumMoney + RamData.ePurse);
	}
	Up_UseData();
	if((FlashInfo.SafeInfo.ChargeCnt)&&((FlashInfo.RunInfo.EState&(_ESTATE_LOC_OPEN_|_ESTATE_FAR_OPEN_)) == 0)){
		SetEBit(_BIT_FAR_OPEN_);			//有购电次数按已开户处理
	}
}//#endif
	if((FlashInfo.RunInfo.EState&_ESTATE_LOC_OPEN_) == 0){
		RunState_LOpen = 1;
	}
	if((FlashInfo.RunInfo.EState&_ESTATE_FAR_OPEN_) == 0){
		RunState_FOpen = 1;
	}
	RamData.Event_KeyChg = 1;
	Chg_Mi();
	Card_Over();
	return SUCCESS;
}

#endif
#endif
