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
 

#ifdef _TestDisp_
extern unsigned char		cDispprt ;
#endif
const char  cKeyInput_Tm =3;
extern  const  strFileSet FileSet;
extern const  strFData FData;
void	Do_Keys(void){													//Key
	//extern unsigned 	char		cKeyInput_Tm   ;		//	 招键软件波波
	 if(RamData.cKeyOnPress >=cKeyInput_Tm){ //if(RamData.Last_Key){
#ifdef _TestDisp_
		 cDispprt++ ;
#endif


		if(RamData.Disp_State != 1){
			SetDispArry(_KEY_DISP_ARRY_);								//Key Disp
		} else {
			SetpArry(_KEY_DISP_ARRY_);
			NextItem(1);
		}
		RamData.Disp.NextTm = _KEY_DISP_TM_;
		RamData.Disp.DispArryLength = FlashInfo.SetInfo.DispLength[1];
		RamData.BkltTm = 60;
		RamData.TDispTm = 0;
		Init_Disp();
		UpDisp();
		//RamData.KeyDispCnt++;
	}
	 main_sequential_off();
	RamData.Event_Key = 0;
	 main_sequential_on();
}

void	Do_IRKeys(void){

	RamData.Event_IRKey = 0;
	if(RamData.PwrOn_Disp){
		return;
	}
	if((RamData.IRDispCnt == 0)&&(RamData.Disp_State == 0)){
		SetDispArry(_NORM_DISP_ARRY_);
		RamData.IRDispCnt = 2;
		Init_Disp();
		UpDisp();
#if		_FAC_ == _FAC_JINLING_
	} else {
		Do_Keys();
		RamData.Disp.NextTm = 20;
#endif
	}
}

#define		BIT_A		0x1
#define		BIT_B		0x2
#define		BIT_C		0x4
#define		BIT_D		0x8
#define		BIT_E		0x10
#define		BIT_F		0x20
#define		BIT_G		0x40
const __code u8 DigitBits[17] = {
	(BIT_A|BIT_B|BIT_C|BIT_D|BIT_E|BIT_F),							//'0'
	(BIT_B|BIT_C),													//'1'
	(BIT_A|BIT_B|BIT_D|BIT_E|BIT_G),								//'2'
	(BIT_A|BIT_B|BIT_C|BIT_D|BIT_G),								//'3'
	(BIT_B|BIT_C|BIT_F|BIT_G),										//'4'
	(BIT_A|BIT_C|BIT_D|BIT_F|BIT_G),								//'5'
	(BIT_A|BIT_C|BIT_D|BIT_E|BIT_F|BIT_G),							//'6'
	(BIT_A|BIT_B|BIT_C),											//'7'
	(BIT_A|BIT_B|BIT_C|BIT_D|BIT_E|BIT_F|BIT_G),					//'8'
	(BIT_A|BIT_B|BIT_C|BIT_D|BIT_F|BIT_G),							//'9'
	(BIT_A|BIT_B|BIT_C|BIT_E|BIT_F|BIT_G),							//'A'
	(BIT_C|BIT_D|BIT_E|BIT_F|BIT_G),								//'B'
	(BIT_A|BIT_D|BIT_E|BIT_F),										//'C'
	(BIT_B|BIT_C|BIT_D|BIT_E|BIT_G),								//'d'
	(BIT_A|BIT_D|BIT_E|BIT_F|BIT_G),								//'E'
	(BIT_A|BIT_E|BIT_F|BIT_G),										//'F'
	0,																//空格
};

void	UpBkltLED(void){						//背光LED处理

	if(RamData.Pwr_On == 0){
		JDLED_On();
	} else if(RamData.LoVolt){
		JDLED_Off();
	} else if((RamData.PwrOn_Disp)||(RamData.TDispTm)){
		JDLED_On();
	} else if(((FlashInfo.RunInfo.EState&_ESTATE_JD_STATE_) == 0)&&(RamData.Next_JD)&&((FlashInfo.RunInfo.EState&_ESTATE_ALM_OFF_) == 0)){
		if(RamData.Disp_BinkState){
			JDLED_On();
		} else {
			JDLED_Off();
		}
	} else if(JD_TState){
		JDLED_On();
	} else {
		JDLED_Off();
	}
	if((RamData.Pwr_On == 0)||(RamData.LoVolt)){
		Bklt_Off();
	} else if(RamData.PwrOn_Disp){									//上电状态
		Bklt_On();
	} else if((RamData.Disp_State)									//非自动轮显
			||(RamData.IRDispCnt)									//红外唤醒背光
			||(RamData.TDispTm)										//液晶查看
			||(RamData.BkltTm)										//背光延时关闭
			||(RamData.Alm_State)									//报警状态
			||((FlashInfo.RunInfo.RcdTm.FarJDOffTime)&&(JD_State == 0))				//延时跳闸
			||(FlashInfo.RunInfo.RcdTm.JDOverIphTime)				//过流延时跳闸
			||(FileSet.AlmMask&RamData.RunState[0])					//运行状态报警
			||(RamData.Disp.DispCode.Code == _D_DISP_ERR_)){		//显示错误代码
		Bklt_On();
	} else {
	//	if((RamData.Read_RP == 0)&&(RamData.Disp.NextTm == 1)){
	//		Bklt_On();
	//	} else {
			Bklt_Off();
	//	}
	}
}

u8		ChkDispItem(strDispCode const *pCode){						//检查显示项合法性

	u8	Buf[16];
	u8*	Bk;
	u8	Ret;
	Bk = tpChar;
	tpChar = Buf;
	RamData.InDisp = 1;
	Ex645.WBit = 0;
	Get645Data(pCode);
	RamData.InDisp = 0;
	if(tpChar > (Buf+4)){
		Wait_Rst(DISP_ITEM_ERR);
		return SUCCESS;
	} else if((tpChar == Buf)||(tpChar > (Buf+4))){
		Ret = FAIL;
	} else {
		Ret = SUCCESS;
	}
	tpChar = Bk;
	return Ret;
}

void	SetDispBit(u8	cBit){										//

	unsigned char *segram =(unsigned char *)RamData.Disp.DispData;
	if ( ((cBit)/8)<sizeof(RamData.Disp.DispData))
	  segram[cBit/8]|=1<<(cBit%8);	
	return ;
	//RamData.Disp.DispData[lBit>>5] |= (1<<(lBit&0x1F));	//Length 40Bytes
	//RamData.Disp.DispData[Bit/8] |= (1<<(Bit%8));		//Length 86Bytes	STM8:33Bytes	STARM:24Bytes
}

void	SetDigitDisp(u8 Num, u8 Bit){//0							//
	u8	n;
	u8	msk;
	const __code u8 *pDigit;
	msk = DigitBits[Num];
//	pDigit = (uc8*)(GetFileSetAddr(FileSet.DispSet.DispDigitBias)+(Bit*7));
	pDigit = FileSet.DispDigitSegs[Bit];
	for(n = 0; n < 7; n++){
		if((msk>>n)&1){
		   SetDispBit(pDigit[n]);
		} else {
			RamData.Disp.DispData[pDigit[n]>>5] &= (~(1<<(pDigit[n]&0x1F)));
		}
	}
}

void	AddDispBits( u8 *Src){
	u8	Cnt;
	if((RamData.InDisp == 0)||(Src == NULL)){
		return;
	}
	Cnt = 0;
	while(RamData.Disp.BitLst[Cnt] != RamData.Disp.BitLst[Cnt+1]){
		Cnt++;
	}
	if(RamData.Disp.BitLst[Cnt] != 0xFF){
		Cnt++;
	}
	while( Cnt < 30){
		RamData.Disp.BitLst[Cnt] = *Src;
		Src++;
		if(*Src == RamData.Disp.BitLst[Cnt]){
			RamData.Disp.BitLst[Cnt+1] = *Src;
			break;
		}
		Cnt++;
	}
}

void	AddDispSetBits(ucode *Src){

	RamData.Disp.DispSet = Src[0];
	AddDispBits((u8*)Src+1);
}

void	AddUseLst( ucode* Src, u8 FL){

	if(RamData.InDisp){
		RamData.Disp.DispSet = (_KWH_|_DISP_USE_DOTS_|_DISP_MIN_BITS_);
		if(FL <= 4){
			RamData.Disp.BitLst[0] = FileSet.DispFeilvSet[FL];
		}
		AddDispBits((u8*)Src);
		AddDispBits((u8*)(FileSet.D_DianLiang));
	}
}

void	SetDispBitLst(uc8* Lst){										//设置位描述序列

	u8	BLst[20];
	u8	*pLst;
	pLst = BLst;
	CopyRam(pLst, Lst, sizeof(BLst));
	if((pLst[0] == 0)&&(pLst[1] == 0)){
		return;
	}
	while(1) {
		if(*pLst == _BLINK_BIT_){
			pLst++;
			if((RamData.Disp_BinkState)||(RamData.Pwr_On == 0)){
				SetDispBit(*pLst);
			}
		} else if(*pLst == _DISP_DIGIT_){
			pLst++;
			if(((*pLst)&0x0F) == 0x0F){
				SetDigitDisp(16,(((*pLst)>>4)&0xF));
			} else {
				SetDigitDisp((*pLst)&0x0F,(((*pLst)>>4)&0xF));
			}
		} else {
			SetDispBit(*pLst);
		}
		if(*pLst == *(pLst+1)){
			return;
		}
		pLst++;
	}
}

void	UpDisp(void){
	u8 n;
	//u16 m16;
	u32	N;
	u8	Dots;
	u8	Num;
	u8	Buf[16];
	ClrRam((u8*)RamData.Disp.DispData, sizeof(RamData.Disp.DispData));
	if(!Is_DispCode(0xFFFFFFFF)){
		FillRam((u8*)RamData.Disp.DispData, 0xFF, sizeof(RamData.Disp.DispData));
	UpdateDisp(( char *)RamData.Disp.DispData);											//Disp All
		return;
	} else if((RamData.Disp_Blink)&&(RamData.Disp_BinkState == 0)&&(_SYS_ON_LINE())){
	UpdateDisp(( char *)RamData.Disp.DispData);										//Disp Blink
		return;
	}
	tpChar = Buf;
	FillRam(RamData.Disp.BitLst, 0xFF, sizeof(RamData.Disp.BitLst));
	RamData.Use_Neg = 0;
	RamData.Disp.DispSet = 0;
	RamData.InDisp = 1;
	Get645Data(&RamData.Disp.DispCode);								//Get DispData
	RamData.InDisp = 0;
	if(RamData.Disp.DispSet&0x700){
		Num = (u8)(((RamData.Disp.DispSet>>8)&7)+1);
	} else {
		Num = (u8)((tpChar-Buf)*2);										//
	}
	ClrpCharRam(4);				//
//	Rev_4(Buf);
	N = *(u32*)Buf;
	Dots = 0;
	switch(RamData.Disp.DispSet&7){
		case _KWH_:				//kWh
		case _KVAH_:			//kVah
		case _YUAN_:			//Yuan
			if((RamData.Disp.DispSet&_DISP_USE_DOTS_)&&(FlashInfo.SetInfo.DispEnergyDots < 2)){
				Dots = FlashInfo.SetInfo.DispEnergyDots;
				if((!Is_DispCode(_D_DISP_REMAIN_))&&(RamData.Use_Neg == 0)){
					if(Dots == 0){
						N += 0xFF;
					} else {
						N += 0xF;
					}
				}
				N >>= ((2-Dots)*4);
			} else {
				Dots = 2;
			}
			if(RamData.Disp.DispSet&_DISP_MIN_BITS_){
				Num = Dots+1;
			}
			break;
		case _KW_:			//kW
		case _KVA_:			//kVa
			if((RamData.Disp.DispSet&_DISP_USE_DOTS_)&&(FlashInfo.SetInfo.DispPowerDots < 4)){
				Dots = FlashInfo.SetInfo.DispPowerDots;
				N >>= ((4-Dots)*4);
			} else {
				Dots = 4;
			}
			if(RamData.Disp.DispSet&_DISP_MIN_BITS_){
				Num = Dots+1;
			}
			break;
		case _V_:
			Dots = 1;
			Num = 2;
			break;
		case _A_:
			Dots = 3;
			Num = 4;
			break;
	}
	if(Dots){
		SetDispBit(FileSet.DigitDots[Dots-1]);
	}
	if(RamData.Disp.DispSet&7){
		Dots = (RamData.Disp.DispSet&7)-1;
		if((&FileSet.DispBias_kWh)[Dots]){
			SetDispBitLst((((ucode *)&FileSet)+(&FileSet.DispBias_kWh)[Dots]));
		}
	}
	Dots = 7;	// - ((RamData.Disp.DispSet2>>3)&7);
	while(Num--){
		SetDigitDisp( (N&0x0F), Dots);
		N >>= 4;
		Dots--;
	}
	while((s8)Dots >= 0){
		if(N == 0){
			break;
		}
		SetDigitDisp( (N&0x0F), Dots);
		N >>= 4;
		Dots--;
	}
	if(RamData.Use_Neg){
		if((((u16)(RamData.Disp.DispCode.Code>>16)) == 0x0090)&&((s8)Dots >= 0)){
			SetDigitDisp(16, Dots);
			SetDispBit(FileSet.DispDigitSegs[Dots][6]);
		} else {
			SetDispBit(_DISP_NEG_);
		}
	}
	if(_SYS_ON_LINE()){
		if((RamData.Use_Step)&&((RamData.Disp.DispSet&_DISP_NO_STEP_) == 0)){
			if(FlashInfo.RunInfo.LStep < 10){
				Num = FlashInfo.RunInfo.LStep+1;
				for( n = 4; n > 0; n--){
					if(Num >= n){
						SetDispBit(FileSet.DispStepDots[n-1]);
						Num -= n;
					}
				}
			}
			SetDispBit(_DISP_JIETI_1_);
			if(CheckDataAct((u8*)&FlashInfo.SafeInfo.StartTm_4) == SUCCESS){//
				SetDispBit(_DISP_JIETI_2_);
			}
		}
		if((FlashInfo.RunInfo.ActTblNum)&&((RamData.Disp.DispSet&_DISP_NO_TARIFF_) == 0)){
			//if((RamData.LVSave[0].FeilvNum < 4)&&((RamData.Disp_BinkState)||(RunState_EOpen == 0))){
			if((RamData.LVSave->FeilvNum < 4)&&((RamData.Disp_BinkState)||(RunState_EOpen == 0))){
				//SetDispBit(FileSet.DispFeilvDots[RamData.LVSave[0].FeilvNum]);
				SetDispBit(FileSet.DispFeilvDots[RamData.LVSave->FeilvNum]);
			}
	if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
			if((FlashInfo.SetInfo.RunType&2) == 0){
				if(FlashInfo.RunInfo.EState&_ESTATE_TTB_STATE){
					SetDispBit(_DISP_LO_2_);
				} else {
					SetDispBit(_DISP_LO_1_);
				}
			} else {
				SetDispBit(_DISP_LO_1_);
				if(CheckDataAct((u8*)&FlashInfo.SafeInfo.StartTm_3) == SUCCESS){//
					SetDispBit(_DISP_LO_2_);
				}
			}
	}else {  //#else
			if(FlashInfo.RunInfo.EState&_ESTATE_TTB_STATE){
				SetDispBit(_DISP_LO_2_);
			} else {
				SetDispBit(_DISP_LO_1_);
			}
} //#endif
		}
		if(RamData.Disp.SubDispTm){
			SetDispBitLst((((const __code u8 *)&FileSet)+FileSet.DispSubItemBias[RamData.Disp.SubItem]));
		}
	}
	SetDispBitLst(RamData.Disp.BitLst);
	if(JD_State){
		if((RamData.Next_JD == 0)||(FlashInfo.RunInfo.EState&_ESTATE_ALM_OFF_)){
			SetDispBit(_DISP_TIAOZHA_);								//
		}
	} else if(((FlashInfo.RunInfo.RcdTm.FarJDOffTime)||(FlashInfo.RunInfo.RcdTm.JDOverIphTime))&&(RamData.Disp_BinkState&&_SYS_ON_LINE())){
		SetDispBit(_DISP_TIAOZHA_);									//
	}
	if(RunState_OverDraft){
		if((RamData.Alm_State < ALM_OVEROFF)||(FlashInfo.RunInfo.EState&_ESTATE_PROTECT_CMD_)){
			if(RamData.Disp_BinkState){
				SetDispBit(_DISP_TOUZHI_);							//
			}
		} else {
			SetDispBit(_DISP_TOUZHI_);
		}
	}
	if((CheckKeyChg(&FlashInfo.SetInfo.EKeyState)) && (_CODE_DEL_)){
		SetDispBit(_DISP_PUB_);										//实验室标志
	}
	if(AUse_Dir_A){
		Disp_LedOn();
	 

		//SetDispBit(_DISP_REVERSE_);									//反向
	}
	else
	{	Disp_LedOff();
	} 
//	if(RunState_IR){
//		SetDispBit(_DISP_PROG_);									//编程模式
//	} else 
	
		
if(_IsFactory_Mode())
	{		//工厂状态长显编程符号		 
		 SetDispBit(_DISP_GOU_);
		SetDispBit(_DISP_SHANGYUE_);				
		SetDispBit(_DISP_DIANLIAN_);				
		SetDispBit(_DISP_SHENGYU_);				
		SetDispBit(_DISP_PROG_); 								//
	} else if(RamData.FacProgTime){									//合闸状态进入工厂模式编程符号闪烁
		if(RamData.Disp_BinkState){
			SetDispBit(_DISP_PROG_);
		}
	}
	if((RamData.Disp_BinkState)||(_SYS_OFF_LINE())){
		if(Bat1_State){
			SetDispBit(_DISP_BAT1_);								//显示"电池符号"
		}
		if(RamData.Alm_State){
			SetDispBit(_DISP_GOUDIAN_);								//显示"清购电"
		}
	}
	if(((FlashInfo.RunInfo.KeyErrCnt[1] >= 3)
			||(FlashInfo.RunInfo.KeyErrCnt[2] >= 3)
			||(FlashInfo.RunInfo.MacErrCnt >= 200))&&(_CODE_DEL_)){
		SetDispBit(_DISP_LOCK_);									//显示锁标志
	}
	UpdateDisp(( char *)RamData.Disp.DispData);
}

void	NextItem(u8 Mode){											//

	RamData.Disp_Blink = 0;
	if((!Is_DispCode(_D_DISP_VER_))&&(RamData.Disp.DispCode.SubCode < 2)){
		RamData.Disp.DispCode.SubCode++;
	} else if(!(Is_DispCode(_D_DISP_TMTBL_))&&(RamData.Disp.DispCode.SubCode < (FlashInfo.RunInfo.ActTblNum-1))){
		RamData.Disp.DispCode.SubCode++;
	} else {	//if(Mode < 2){
		if((RamData.Disp_State == 1)&&(Mode == 0)){
			SetpArry(_NORM_DISP_ARRY_);
			RamData.Disp.DispArryNum = 0;
		} else if(++RamData.Disp.DispArryNum >= RamData.Disp.DispArryLength){
			if(Mode == 0){
				SetpArry(_NORM_DISP_ARRY_);
				if(RamData.IRDispCnt){
					--RamData.IRDispCnt;
				}
			}
			RamData.Disp.DispArryNum = 0;
		}
//		CopyRam((u8*)&RamData.Disp.DispCode, (u8*)&RamData.Disp.pDispArry[RamData.Disp.DispArryNum], sizeof(strDispCode));
		//===========

	//	CopyRam( (unsigned char *)&RamData.Disp.DispCode , (u8*)&(RamData.Disp.pDispArry[RamData.Disp.DispArryNum ]),sizeof (RamData.Disp.DispCode ) );
		GetExtShowMenu (RamData.Disp.DispCode,RamData.Disp.cDispArry,RamData.Disp.DispArryNum);

		//================;
		//GetDispCode(RamData.Disp.DispStart+RamData.Disp.DispArryNum);
	}
}

void	SetDispItem(uc8 *pCode){						//设置显示项编码


	strDispCode DispCode;
	CopyRam((u8*)&DispCode, (u8*)pCode, sizeof(strDispCode));
	if(ChkDispItem(&DispCode)){
		return;
	}
	CopyRam((u8*)&RamData.Disp.DispCode, (u8*)pCode, sizeof(strDispCode));
	RamData.Disp.NextTm = FlashInfo.SetInfo.AutoDispTm;
	RamData.Disp_Blink = 0;
	UpDisp();
	UpBkltLED();
}

void	SetpArry(u8 Num){											//

	if(Num == _CARD_OVER_DISP_ARRY_){
		RamData.Disp_State = 2;
		RamData.Disp.DispStart = (99*2)+1;							//RamData.Disp.pDispArry = FlashInfo.DArryCardOver;
		RamData.Disp.DispArryLength = _CARD_OVER_DLTH_;
	} else {
		if(Num < 2){
			RamData.Disp_State = Num;
		} else {
			RamData.Disp_State = 2;
			Num = 0;
		}
		if(Num == 0){
			RamData.Disp.DispStart = 0;
		} else {
			RamData.Disp.DispStart = 99;
		}															//pDispArry = FlashInfo.DispArry[Num];
		RamData.Disp.DispArryLength = FlashInfo.SetInfo.DispLength[Num];
		//RamData.Disp.pDispArry =  ( 0 == Num )?FlashInfo.DispArry_normal:FlashInfo.DispArry_key;
		RamData.Disp.cDispArry = Num ;
		{

		}

	}
	RamData.Disp_Blink = 0;
}

void	SetDispArry(u8 Num){										//设置显示序列

	SetpArry(Num);
	RamData.Disp.DispArryNum = 0x80;
	NextItem(1);
	RamData.Disp.NextTm = FlashInfo.SetInfo.AutoDispTm;
}

void	SetDispSubItem(u16 ItemSecs){							//设置子显示项

	u8	Item;
	Item = (u8)(ItemSecs>>8);
	if(Item < NUMOF(FileSet.DispSubItemBias)){
		RamData.Disp.SubItem = Item;
		RamData.Disp.SubDispTm = ((u8) ItemSecs) * 2+1;
	}
}

void	Do_Disp(void){													//

	//if(_SYS_ON_LINE()){
		//if((RamData.PwrUpCnt < (FlashInfo.SetInfo.DispAllTm*2))&&(!_Is_MeterApp_OnLine())&&((RamData.Disp_State == 2)||(RamData.Disp_State == 0))){
//			RamData.PwrOn_Disp = 1;
	//		UpBkltLED();
			//return;
//		}
		_Set_MeterApp_OnLine() ;//RamData.PwrOnCode = 52707000;
//	}
	if(RamData.Disp.SubDispTm){
		RamData.Disp.SubDispTm--;
	}
	if ( Event_Sec ) { //if(RamData.HSecCnt >= _main_sequential_start_){
		if(RamData.TDispTm){
			--RamData.TDispTm;
		}
		if(RamData.BkltTm){
			--RamData.BkltTm;
		}
		if(--RamData.Disp.NextTm == 0){
		//-	NextItem(0);
		//-	RamData.Disp.NextTm = FlashInfo.SetInfo.AutoDispTm;
			//-Init_Disp();
		}
	}
	UpDisp();
	RamData.PwrOn_Disp = 0;
	UpBkltLED();
}

void	InitDispProg(void){

	Init_Disp();
	if(!_Is_MeterApp_OnLine()){ //(RamData.PwrOnCode != 52707000){
		FillRam((u8*)RamData.Disp.DispData, 0xFF, sizeof(RamData.Disp.DispData));
	}
	UpdateDisp(( char *)RamData.Disp.DispData);
	SetDispArry(_NORM_DISP_ARRY_);								//设置为正常显示
	RamData.BkltTm = 0;
}

void Turn_OnDisplay( )
{

	 	Init_Disp();
		UpDisp ();

}
