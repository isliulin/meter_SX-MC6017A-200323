//#include "system.h"
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
#include "lcdio.h"
#include "flash_filesystem.h"
 
#include "eeprom.h"
#include "pubdata.h"
#include "safedate.h"
//#include "flash.h"
extern const strFileSet FileSet;
extern const strFData FData;
//strGAddr GAddr;
extern u8* tpChar;
extern u8* tpBase;
 
void CheckpChar(void) { //数据地址检查
	//if (tpChar >= ((u8*) (0x20000000))) { //最正向k自由存取RAM空间
	//	Wait_Rst(RAM_ADDR_ERR); //地址错误
	//}
   if (( tpChar < ((u8*)_RAM_ADDR_START_ )) || ( tpChar >= ((u8*) _RAM_ADDR_END_)))
Wait_Rst(RAM_ADDR_ERR); //地址错误       0x20001800
}

u8 GetAddrData(u32 Addr) {
	return *(u8*) Addr;
//	if (Addr >= _CODE_ADDR_) {
//		if (Addr >= _FDATA_ADDR_) {
//			Addr |= _FLASH_DATA_MASK_;
//		} else {
//			Addr -= _CODE_ADDR_;
//		}
//		return *(const __code u8 *) Addr;
//	} else if (Addr >= 0x40) {
//		return *(u8*) Addr;
//	} else {
//		return 0;
//	}
}



 void	cCopyRam(u8 *Dst,u8 *Src, u16 Length)
  {

 while(Length--){
 *Dst++ = *Src++;
 }
 }

void CopyRam(u8 *Dst, u8 const *Src, u16 Length) { //复制数据
	unsigned char 	ctemp =(((u32)Src<_RAM_ZERO_END_) && ((u32) Src>=_RAM_ADDR_START_))?0xff:0;
	
	while(Length--){
		if ( ctemp)
			*Dst++ = 0;
		else
			*Dst++ = *Src++;
		}
	//memcpy(Dst, Src, Length);

}

s32 strCmp(uc8 *s1, uc8* s2, u16 cnt) { //比较数据

	u16 n;
	u8 a, b;
	/* for ( n = 0;n<cnt;n++,s1++,s2++)
	 {
	 if ( *s1>*s2)
	 return 1;
	 else if( *s1<*s2)
	 return -1;
	 }
	 return 0;
	 */
	for (n = 0; n < cnt; n++) {
		a = GetAddrData((u32) s1);
		b = GetAddrData((u32) s2);
		if (a > b) {
			return 1;
		} else if (a < b) {
			return -1;
		}
		s1++;
		s2++;
	}
	return 0;

}

u8 CheckBCD(u8 *buf, u8 Cnt) {
	u8 a;
	while (Cnt--) {
		a = GetAddrData((u32) buf);
		buf++;
		if ((a >= 0xA0) || ((a & 0x0F) >= 0x0A))
			return FAIL;
	}
	return SUCCESS;
}

u8 CheckZero(u8 *buf, u8 Cnt) {
	while (Cnt--) {
		if (GetAddrData((u32) buf)) {
			return FAIL;
		}
		buf++;
	}
	return SUCCESS;
}

u16 GetSum(u8 *buf, u8 Cnt) { //计算校验和
	u16 i = 0;
	while (Cnt--) {
		i += GetAddrData((u32) buf);
		buf++;
	}
	return i;
}
/*
void GetDispCode(u8 DispNum) { //获取显示代码

	if (GAddr.pDisp[0][DispNum]) {
		CopyRam((u8*) &RamData.Disp.DispCode, (u8*) (GAddr.pDisp[0][DispNum]),
				sizeof(strDispCode));
	} else {
		ClrRam((u8*) &RamData.Disp.DispCode, sizeof(strDispCode));
	}
}
*/
#ifdef		_USE_EEPROM_		/*		以下为外置EEPROM处理程序		*/
u8 EDataErr(void) {

	RunState_EEP = 1;_cEepromDevice_SetErr();
	if((FlashInfo.RunInfo.EState&_ESTATE_EEP_) == 0){
			SetEvtState(_EVT_EEP_);
			SetEBit(_BIT_EEP_);
	}
	return FAIL;
}
#endif

/*				FlashInfo数据处理				*/
extern const __code strFlashInfo DftFlashInfo;
#define		RUNINFO_CHG_START	((sizeof(FlashInfo)+3)&0xFFFC)
void Reset_FlashInfo(void) { //充置FlashInfo数据

	/*u32 Addr;

	 Addr = Get_BlankBlock(FLASH_INFO_START);
	 FlashInfo.StartCode = FLASH_INFO_START;
	 FlashInfo.SNum++;
	 FlashInfo.End = 0x16;
	 WriteFlashData(Addr, (u8*)&FlashInfo, sizeof(FlashInfo));
	 GAddr.FlashInfoNext = (u16)(Addr + RUNINFO_CHG_START);
	 ClrTypeBlock(Addr);
	 */
}


#define 	_FlashInfo_RunInfoStartAddr_  ((int)&((( strFlashInfo *)0)->RunInfo))
#define 	_FlashInfo_RunInfo_Len_				sizeof((( strFlashInfo *)0)->RunInfo)
#define 	_FlashInfo_RunInfoEndAddr_  ( _FlashInfo_RunInfoStartAddr_ + _FlashInfo_RunInfo_Len_)
	
#define 	_FlashInfo_FrozeCntStartAddr_ 	 		(int)&(((strFlashInfo *)0)->FrozeCnt)
#define 	_FlashInfo_FrozeCnt_Len_			sizeof(((strFlashInfo *)0)->FrozeCnt)
#define 	_FlashInfo_FrozeCntEndAddr_ 	 (_FlashInfo_FrozeCntStartAddr_ + _FlashInfo_FrozeCnt_Len_)

#define 		_FlashInfo_Runinfo_2_EepromAddr(sp)			((unsigned short)(_RunInfo_eeprom_startaddr_+ (unsigned short)((char *)sp-(char *)&(FlashInfo.RunInfo))))	
#define 		_FlashInfo_FrozeCnt_2_EepromAddr(sp)		((unsigned short)(_FrozeCnt_eeprom_startaddr_+(unsigned short)((char *)sp-(char *)&(FlashInfo.FrozeCnt))))	
	

void Copy_FlashInfo(u8* Dst, uc8 *Src, u8 Length) 
	{  
  char stemp[8];
		u8 cResult ;	 
		unsigned short iStartOffset = (unsigned char *) Dst - (unsigned char *) &FlashInfo;
		unsigned short iEndOffset = iStartOffset + Length;
		unsigned long lFlashAddr = (unsigned long )&(stflashdata.sFlashInfo);
		strFlashInfo 		*stp = (strFlashInfo 	*)stflashdata.sFlashInfo ;	 
	CopyRam(Dst, Src, Length);
		if (( iStartOffset >=_FlashInfo_RunInfoStartAddr_  ) && ( iEndOffset <=_FlashInfo_RunInfoEndAddr_ ))
		{					 
								if ( Length <sizeof(stemp) )
								{							 
									cResult  =Read_EEData(stemp, _FlashInfo_Runinfo_2_EepromAddr(Dst)	,(unsigned short) Length);
									if ( SUCCESS == cResult )
									{
											cResult  = strCmp( stemp , Src , Length);
											if ( SUCCESS == cResult )
												return ;
									}
								}
								Write_EEData((char *)Src, _FlashInfo_Runinfo_2_EepromAddr(Dst),(unsigned short) Length);
		}else 		if (( iStartOffset >=_FlashInfo_FrozeCntStartAddr_  ) &&  ( iEndOffset <=_FlashInfo_FrozeCntEndAddr_ ))
		{
		 
								if ( Length <sizeof(stemp) )
								{
									cResult  =Read_EEData(stemp, _FlashInfo_FrozeCnt_2_EepromAddr(Dst),(unsigned short) Length);
									if ( SUCCESS == cResult )
									{
											cResult  = strCmp( stemp , Src , Length);
											if ( SUCCESS == cResult )
												return ;
									}
								}
								Write_EEData((char *)Src,_FlashInfo_FrozeCnt_2_EepromAddr(Dst),(unsigned short) Length);
			 
		}else
		{
	 
				McuFlashRandWriteStack (lFlashAddr +iStartOffset ,  Src, (unsigned short) Length);
		}
}
#ifdef _DEL
/*				电量数据处理					*/
void Up_BkLVSave(void) { //设置电量校验数据

	/*
	 u32	Cnt;
	 RamData.LVSave[0].CS = GetSum((u8*)(&RamData.LVSave[0]), sizeof(strLVSave) - 2);
	 for(Cnt = 0; Cnt < sizeof(strLVSave); Cnt++){
	 ((u8*)&RamData.LVSave[1])[Cnt] = 0xFF - ((u8*)&RamData.LVSave[0])[Cnt];
	 }
	 CopyRam( (u8*)&RamData.LVSave[2], (u8*)&RamData.LVSave[0], sizeof(strLVSave)*2);
	 */
	nSafeData_set_upb(); 	//       更新状态标志
	nSafeData_update2bak();
	nSafeData_res_upb(); 	// 		清除状态标志
	_DIANLIANMOD_SET();		//	 	电量修改标志
}
#endif

strSectUse *GetFLBase(u8 Feilv) { //获取费率数据地址

	if (Feilv == 0) {
		return &(RamData.stdianlian[0].LVSave.SumUse);// RamData.LVSave[0].SumUse;
	}
	else
	{
		Feilv--;
		if (Feilv < _MAX_TARRIF_)
		{
			if ( RamData.stdianlian[0].LVSave.FeilvNum == Feilv)
							return 	(strSectUse*)(&RamData.stdianlian[0].LVSave.FLUse);//FlashInfo.pFLUse[Feilv]);
			else
							return (strSectUse*)(&RamData.stdianlian[0].FLUse[Feilv]);//FlashInfo.pFLUse[Feilv]);
		}
	}
		return (strSectUse*) 0;
}

void Reset_pFLUse(u32 Addr) {

	u32 Cnt;
	//for (Cnt = 0; Cnt < (_MAX_TARRIF_); Cnt++) {
	//	GAddr.pFLUse[Cnt]
	//			= (u16) ((u32) (&((strLVSaveBlock *) Addr)->FLUse[Cnt]));
	//}
}

void ReFresh_LVSave(void) { //刷新用电数据

	unsigned char cError;
	/*
	 u32	Cnt;
	 u32	Lst[2];
	 u32	Addr;
	 u32	Num;
	 return;
	 Addr = Get_BlankBlock(FLASH_SAVE_START);
	 Num = Get_FlashAddr(Lst, FLASH_SAVE_START);
	 Up_BkLVSave();
	 ClrRam(WFBuf.Data, sizeof(strLVSaveBlock));
	 ((strLVSaveBlock *)WFBuf.Data)->StartCode = FLASH_SAVE_START;
	 ((strLVSaveBlock *)WFBuf.Data)->Rev = ELV_CODE;
	 for(Cnt = 0; Cnt < _MAX_TARRIF_; Cnt++){
	 CopyRam((u8*)&(((strLVSaveBlock *)WFBuf.Data)->FLUse[Cnt]), (u8*)GAddr.pFLUse[Cnt], sizeof(strSectUse));
	 }
	 CopyRam((u8*)(&((strLVSaveBlock *)WFBuf.Data)->LVSave), (u8*)&RamData.LVSave, sizeof(strLVSave));
	 if(WriteFlashData(Addr, WFBuf.Data, sizeof(strLVSaveBlock))){
	 Wait_Rst(FLASH_ERR);
	 return;
	 }
	 Reset_pFLUse(Addr);
	 GAddr.pFLTmp = GAddr.pFLUse[RamData.LVSave[0].FeilvNum];
	 GAddr.pFLUse[RamData.LVSave[0].FeilvNum] = (u16)((u32)&RamData.LVSave[0].FLUse);
	 GAddr.LVSaveNext = Addr+sizeof(strLVSaveBlock);
	 ClrTypeBlock(Addr);
	 */
	cError = cSafeData_Data_Refreah();
	Do_Task ( _system_success == cError ,return);
}

void Reset_LVSave(void) { //清零所有用电数据
/*	u32	Cnt;
 Cnt = RamData.LVSave[0].FeilvNum;
 if(Cnt >= _MAX_TARRIF_){
 Cnt = 0;
 }
 ClrRam((u8*)&GAddr.pFLUse, sizeof(GAddr.pFLUse));
 ClrRam((u8*)&RamData.LVSave, sizeof(RamData.LVSave));
 RamData.LVSave[0].FeilvNum = Cnt;
 RamData.LVSave[0].LVCode = ELV_CODE;
 ReFresh_LVSave();
 */
/*	u8 a;
	u8 Cnt;
	a = RamData.LVSave[0].FeilvNum;
	if (a >= _MAX_TARRIF_) {
		a = 0;
	}
	ClrRam((u8*) &RamData.LVSave[0], sizeof(strLVSave));

	nSafeData_struct_Init((u8*) (&RamData.LVSave[0]),
			(u8*) (&RamData.LVSave[1]), (u8*) (&RamData.LVSave[2]),
			sizeof(strLVSave));

	nSafeData_set_upb();
	nSafeData_update2bak();
	nSafeData_res_upb();
	cFlashinfo_dianlian_backup();

	u8 a;
		u8 Cnt;
		a = RamData.LVSave[0].FeilvNum;
		if (a >= _MAX_TARRIF_) {
			a = 0;
		}*/
		ClrRam((u8*) &RamData.stdianlian, sizeof(struct   STDIANLIAN));

		nSafeData_struct_Init((u8*) (&RamData.stdianlian[0]),
				(u8*) (&RamData.stdianlian[1]), (u8*) (&RamData.stdianlian[2]),
				sizeof(struct   STDIANLIAN));

		nSafeData_set_upb();
		nSafeData_update2bak();
		nSafeData_res_upb();
		_DIANLIANMOD_SET();		//	 	电量修改标志
		cFlashinfo_dianlian_backup();

}

u32 Get_ELVSave(u32 Addr) { //获取用电数据
/*
 u32	Cnt;
 const __code strLVSave *pRet;
 const __code strLVSave *pLV;
 Reset_pFLUse(Addr);
 pLV = &((const __code strLVSaveBlock *)Addr)->LVSave;
 pRet = NULL;
 GAddr.LVSaveNext = 0;
 Cnt = ((u32)pLV)&(_FLASH_BLOCK_SIZE_-1);
 while(Cnt <= (_FLASH_BLOCK_SIZE_-sizeof(strLVSave))){
 if((pLV->LVCode == ELV_CODE)&&(pLV->FeilvNum < _MAX_TARRIF_)
 &&(pLV->CS == GetSum((u8*)pLV, sizeof(strLVSave) - 2))){
 pRet = pLV;
 GAddr.pFLUse[pLV->FeilvNum] = (u16)((u32)&pLV->FLUse);
 }
 if(*(const __code u32*)pLV == 0xFFFFFFFF){
 GAddr.LVSaveNext = (u16)((u32)pLV);
 break;
 }
 pLV++;
 Cnt += sizeof(strLVSave);
 }
 return (u32)pRet;
 */
}

void Read_ELVSave(void) { //读取用电数据
/*
 u8	Act;
 u8	Cnt;
 u32	Lst[2];
 u32	Addr;
 u8*	pData1;
 u8* pData2;


 Act = 0;
 pData1 = (u8*)&RamData.LVSave[0];
 pData2 = (u8*)&RamData.LVSave[1];
 while(pData1 == (u8*)&RamData.LVSave[0]){
 if(((strLVSave *)pData1)->CS == GetSum(pData1, sizeof(RamData.LVSave[0]) - 2)){
 for(Cnt = 0; Cnt < sizeof(strLVSave); Cnt++){
 if((pData1[Cnt]+pData2[Cnt]) != 0xFF){
 break;
 }
 }
 if(Cnt == sizeof(strLVSave)){
 if(pData1 == (u8*)&RamData.LVSave[2]){
 CopyRam((u8*)&RamData.LVSave[0], (u8*)&RamData.LVSave[2], sizeof(strLVSave)*2);
 }
 Act = 1;
 break;
 }
 }
 pData1 = (u8*)&RamData.LVSave[2];
 pData2 = (u8*)&RamData.LVSave[3];
 }
 Cnt = Get_FlashAddr(Lst, FLASH_SAVE_START);
 if(Cnt == 0){
 ClrRam((u8*)&GAddr.pFLUse, sizeof(GAddr.pFLUse));
 if(Act == 0){
 Reset_LVSave();
 } else {
 ReFresh_LVSave();
 }
 return;
 } else if(Cnt == 2){
 Addr = (u32)Get_ELVSave(Lst[0]);
 if(Addr){
 EraseFBlock(Lst[1]);
 } else {
 Addr = (u32)Get_ELVSave(Lst[1]);
 EraseFBlock(Lst[0]);
 }
 } else {
 Addr = (u32)Get_ELVSave(Lst[0]);
 }


 if(Addr == 0){
 if(Act == 0){
 Reset_LVSave();
 } else {
 ReFresh_LVSave();
 }
 return;
 } else if(Act == 0){
 CopyRam((u8*)&RamData.LVSave[0], (u8*)Addr, sizeof(strLVSave));
 Up_BkLVSave();
 }
 GAddr.pFLUse[RamData.LVSave[0].FeilvNum] = (u16)((u32)&RamData.LVSave[0].FLUse);
 */
	unsigned char i, cError = 9; 
	 
	RamData.WDGTm = 0x90;
/*
	do {

		i = Read_EEData((unsigned char *) &(RamData.LVSave[0]), _iDLDATA_START,
				_iDLDATA_SIZE);

	} while (SUCCESS != i);

	// Ram初始化
	nSafeData_struct_Init((u8*) (&RamData.LVSave[0]),
			(u8*) (&RamData.LVSave[1]), (u8*) (&RamData.LVSave[2]),
			sizeof(strLVSave));
	cError = cSafeData_Data_Refreah();
	Do_Task ( _system_success == cError ,return);

	//  初始化
	Reset_LVSave();
	return;
	*/
	if ( SUCCESS != _EepromDevice_statu())
	{
		Reset_LVSave();

				 return ;
	}
	do {

			i = Read_EEData((unsigned char *) &(RamData.stdianlian), _iDLDATA_START,
					_iDLDATA_SIZE);

		} while (SUCCESS != i);

		// Ram初始化
		nSafeData_struct_Init((u8*) (&RamData.stdianlian[0]),
				(u8*) (&RamData.stdianlian[1]), (u8*) (&RamData.stdianlian[2]),
				sizeof(struct   STDIANLIAN ));
		cError = cSafeData_Data_Refreah();
		Do_Task ( _system_success == cError ,return);

		//  初始化
		Reset_LVSave();
		return;


}


u8 Up_LVSave_io(void) { //保存用电数据




	if ( _IS_I2C_Busy()	)
		return FAIL;
	if ( RamData.bUp_LVSave)
		return FAIL;

	_Rset_Global_Val(RamData.bUp_LVSave,1);
	cSafeData_Data_Refreah();
	cFlashinfo_dianlian_backup();
	_Rset_Global_Val(RamData.bUp_LVSave,0);
	return SUCCESS;
}

#ifdef _DEL
void Up_LVSave(void) { //保存用电数据


 u8 cresult ;

	 do{
		 cresult = Up_LVSave_io();
	 } while ( cresult != SUCCESS);



}
#endif

/*
strFLUse *GetFLBase(u8	Feilv)
{			//获取费率数据地址
	if(Feilv == RamData.LVSave[0].FeilvNum){
		return &RamData.LVSave[0].FLUse;
	} else if(Feilv >= _MAX_TARRIF_){
		return (strFLUse*)0;
	} else if(FlashInfo.FLUse[Feilv].CS == GetSum((u8*)&FlashInfo.FLUse[Feilv], sizeof(strFLUse))){
		return (strFLUse*)&FlashInfo.FLUse[Feilv].FLUse;
	} else if(FlashInfo.FLUseBck[Feilv].CS == GetSum((u8*)&FlashInfo.FLUseBck[Feilv], sizeof(strFLUse))){
		return (strFLUse*)&FlashInfo.FLUseBck[Feilv].FLUse;
	} else {
		return ((strFLUse*)&FlashInfo.FLUse[Feilv].FLUse);
	}
}
*/
void Chg_Feilv(u8 NewFeilv) { //切换费率

	if (NewFeilv != RamData.LVSave[0].FeilvNum)
	{
		Up_LVSave();
		//		GAddr.pFLUse[RamData.LVSave[0].FeilvNum] = GAddr.pFLTmp;
	 	CopyRam((u8*)&(RamData.stdianlian[0].FLUse[RamData.stdianlian[0].LVSave.FeilvNum]) ,(unsigned char *)&(RamData.stdianlian[0].LVSave.FLUse),sizeof(strSectUse)   );
		//GAddr.pFLTmp = GAddr.pFLUse[NewFeilv];
		CopyRam((u8*)&(RamData.stdianlian[0].LVSave.FLUse), (u8*)&(RamData.stdianlian[0].FLUse[NewFeilv]),sizeof(strSectUse));
		RamData.stdianlian[0].LVSave.FeilvNum = NewFeilv;
		//GAddr.pFLUse[NewFeilv] = (u16) ((u32) & RamData.LVSave[0].FLUse);
		Up_BkLVSave();
		RamData.Use_Chg = 1;
	}
}

/*			时段表/费率表/电价表/阶梯表处理			*/
#define	vTTBlBlock	(*((strTTBlBlock *)0))
#define	TTBlSize(A)			(sizeof(vTTBlBlock.A))

 const  struct STTblSetLst TblSetLst[] = {
				{ TTBlSize(YearTbl[0]), (u8*) &DftYTbl },  //0
				{ TTBlSize(YearTbl[0]), (u8*) &DftYTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(TmTbl[0][0]), (u8*) &DftTmTbl },
				{ TTBlSize(Price[0]), 	 (u8*) &DftPrice },//0x12
				{ TTBlSize(Price[0]),    (u8*) &DftPrice },
				{ TTBlSize(LTBl[0]),   (u8*) &DftLTbl },
				{ TTBlSize(LTBl[0]), (u8*) &DftLTbl },
				};
void Get_TTbl(u32 Dst) { //读取当前时段费率表

	/*u32 Addr;
	u32 Bias;
	u32 Cnt;
	u32 Lth;
	u32 N;

	Addr = Dst + 4;
	for (Cnt = 0; Cnt < NUMOF(TblSetLst); Cnt++) {

		GAddr.pYTbl[Cnt] = (u16) Addr;
		Addr += TblSetLst[Cnt].Lth;
	}
	GAddr.TTblNext = 0;
	Bias = (u16) ((sizeof(strTTBlBlock) + 31) & 0xFFE0);
	Addr = Dst + Bias;
	while (Bias < (_FLASH_BLOCK_SIZE_)) {
		N = *(const __code u32*) Addr;
		if (N == 0xFFFFFFFF) {
			GAddr.TTblNext = (u16) Addr;
			break;
		}
		N = (u8) N;
		if (N < NUMOF(TblSetLst)) {
			GAddr.pYTbl[N] = (u16) (Addr + 4);
			Lth = ((TblSetLst[N].Lth) + 35) & 0xE0; //调整为32的倍数
		} else {
			Lth = 32;
		}
		Bias += Lth;
		Addr += Lth;
	}*/
}

void ReFresh_TTbl(u32 Dst) { //时段表/费率表/电价表/阶梯表块刷新处理

	u32 Addr;
	u32 Cnt;
	u32 Lth;
	/*
	 if(*((const __code u8*)Dst) != FLASH_TBL_START){
	 RamData.FlashBuf[0] = FLASH_TBL_START;
	 *(u16*)(RamData.FlashBuf+2) = TBL_CODE;
	 WriteFlashData(Dst, RamData.FlashBuf, 4);
	 }
	 Addr = Dst+4;
	 for(Cnt = 0; Cnt < NUMOF(TblSetLst); Cnt++){
	 Lth = TblSetLst[Cnt].Lth;
	 if(GAddr.pYTbl[Cnt]){
	 CopyRam(RamData.FlashBuf, (u8*)GAddr.pYTbl[Cnt], Lth);
	 } else {
	 CopyRam(RamData.FlashBuf,
	 (TblSetLst[Cnt].pTbl),	Lth);
	 }
	 if(strCmp((u8*)Addr, RamData.FlashBuf, Lth)){
	 WriteFlashData(Addr, RamData.FlashBuf, Lth);
	 }
	 GAddr.pYTbl[Cnt] = (u16)Addr;
	 Addr += Lth;
	 }
	 GAddr.TTblNext = (u16)(Dst+((sizeof(strTTBlBlock)+31)&0xFFE0));
	 ClrTypeBlock(Dst);
	 */
}

void Read_TTbl(void) { //上电读取时段表/费率表/电价表/阶梯表

	u32 Cnt;
	u32 Lst[2];
	u32 Addr;
	/*
	 Cnt = Get_FlashAddr(Lst, FLASH_TBL_START);
	 if(Cnt == 0){
	 ClrRam((u8*)&GAddr.pYTbl, NUMOF(TblSetLst)*2);
	 Addr = Get_BlankBlock(FLASH_TBL_START);
	 ReFresh_TTbl(Addr);
	 } else if(Cnt == 1){
	 Get_TTbl(Lst[0]);
	 if(GAddr.TTblNext == 0){
	 Addr = Get_BlankBlock(FLASH_TBL_START);
	 ReFresh_TTbl(Addr);
	 }
	 } else {
	 Get_TTbl(Lst[1]);
	 ReFresh_TTbl(Lst[0]);
	 }
	 */
}

void Up_TTbl(u8* Buf, u8 Type) { //写时段表/费率表/电价表/阶梯表块

	u32 Lth;
	u32 Addr;
	/*
	 if(Type >= NUMOF(TblSetLst)){
	 return;
	 }
	 Lth = TblSetLst[Type].Lth;
	 if(strCmp(Buf, (u8*)GAddr.pYTbl[Type], Lth) == 0){
	 return;
	 }
	 RamData.Event_Feilv = 1;
	 if((GAddr.TTblNext)&&(((GAddr.TTblNext&(_FLASH_BLOCK_SIZE_-1))+ Lth) <= _FLASH_BLOCK_SIZE_)){
	 UpFlashData();
	 *(u32*)RamData.FlashBuf = Type;
	 CopyRam(RamData.FlashBuf+4, Buf, Lth);
	 ClrRam(RamData.FlashBuf+4+Lth, 32);
	 Lth = (Lth+35)&0xE0;
	 WriteFlashData(GAddr.TTblNext, RamData.FlashBuf, Lth);
	 GAddr.pYTbl[Type] = (u16)(GAddr.TTblNext+4);
	 if(((GAddr.TTblNext&(_FLASH_BLOCK_SIZE_-1))+ Lth) > (_FLASH_BLOCK_SIZE_-128)){
	 Addr = Get_BlankBlock(FLASH_TBL_START);
	 ReFresh_TTbl(Addr);
	 return;
	 }
	 GAddr.TTblNext += Lth;
	 return;
	 }
	 Addr = Get_BlankBlock(FLASH_TBL_START);
	 ReFresh_TTbl(Addr);
	 Up_TTbl(Buf, Type);
	 */
}

/*			假日表/显示表处理			*/
#define		HLD_NUM		254
#define		DISP_NUM	(99*2)
typedef struct {
	u16* pLst;
	u8 Type;
	u8 MaxN;
	u8 Lth;
} strExTblSet;

//const __code strExTblSet 		HldTblSet = { GAddr.pHld, FLASH_HLD_START, HLD_NUM, 4 }; //假日表设置
//const __code strExTblSet DispTblSet = { GAddr.pDisp[0], FLASH_DISP_START,		DISP_NUM, 8 }; //显示表设置
#ifdef _DEL
void Get_ExTbl(u32 Addr, const __code strExTblSet *pSet) { //从Flash块中读取假日表/显示表信息

	u32 Bias;
	u16* Lst;
	u32 N;
	u32 MaxN;
	u32 Lth; return ;
	Lst = pSet ->pLst;
	MaxN = pSet ->MaxN;
	Lth = pSet ->Lth;
	ClrRam((u8*) Lst, (MaxN + 1) * 2);
	Bias = 4;
	Addr += 4;
	while (Bias < _FLASH_BLOCK_SIZE_) {
		N = *(const __code u32*) Addr;
		if (N == 0xFFFFFFFF) {
			Lst[MaxN] = (u16) Addr;
			break;
		}
		N = (u8) N;
		if (N < MaxN) {
			Lst[N] = (u16) (Addr + 1);
		}
		Addr += Lth;
		Bias += Lth;
	}
}

void ReFres_ExTblh(u32 Dst, const strExTblSet *pSet) { //假日表显示表块刷新

	u32 Addr;
	u32 Cnt;
	u32 Type;
	u32 Lth;
	u16 *Lst;/*
	 if(*((const __code u8*)Dst) != pSet ->Type){
	 WFBuf.Data[0] = pSet ->Type;
	 *(u16*)(WFBuf.Data+2) = HLD_CODE;
	 WriteFlashData(Dst, WFBuf.Data, 4);
	 }
	 Lst = pSet ->pLst;
	 Lth = pSet ->Lth;
	 Addr = Dst+4;
	 for(Cnt = 0; Cnt < pSet->MaxN; Cnt++){
	 if(Lst[Cnt]){
	 WFBuf.Data[0] = Cnt;
	 CopyRam(WFBuf.Data+1, (u8*)Lst[Cnt], Lth);
	 if(strCmp((u8*)Addr, WFBuf.Data, Lth)){
	 WriteFlashData(Addr, WFBuf.Data, Lth);
	 }
	 Lst[Cnt] = (u16)(Addr+1);
	 Addr += Lth;
	 }
	 }
	 Lst[Cnt] = (u16)Addr;
	 ClrTypeBlock(Dst);
	 */
}

void Read_ExTbl(const __code strExTblSet *pSet) { //读取假日表/显示表

	u32 Cnt;
	u32 Lst[2];
	u32 Addr;
	u16 pArry;
	/*
	 Cnt = Get_FlashAddr(Lst, pSet->Type);
	 if(Cnt == 0){
	 Addr = Get_BlankBlock(pSet ->Type);
	 ClrRam((u8*)(pSet ->pLst), ((pSet->MaxN)*2));
	 if(pSet == &DispTblSet){									//初始化显示列表
	 pArry = (u16)((u32)(FileSet.DArryNorm));
	 for(Cnt = 0; Cnt < NUMOF(FileSet.DArryNorm); Cnt++){
	 GAddr.pDisp[0][Cnt] = pArry;
	 pArry += 8;
	 }
	 pArry = (u16)((u32)(FileSet.DArryKey));
	 for(Cnt = 0; Cnt < NUMOF(FileSet.DArryKey); Cnt++){
	 GAddr.pDisp[1][Cnt] = pArry;
	 pArry += 8;
	 }
	 }
	 ReFresh_ExTbl(Addr, pSet);
	 } else if(Cnt == 1){
	 Get_ExTbl(Lst[0], pSet);
	 if(pSet ->pLst[pSet ->MaxN] == 0){
	 Addr = Get_BlankBlock(pSet ->Type);
	 ReFresh_ExTbl(Addr, pSet);
	 }
	 } else {
	 Get_ExTbl(Lst[1], pSet);
	 ReFresh_ExTbl(Lst[0], pSet);
	 }
	 */
}

void Up_ExTbl(u8 *pData, u8 Num, const strExTblSet *pSet) {//写入新假日表/显示表

	u8 Buf[8];
	u32 Addr;
	u32 Next;
	u32 MaxN;
	u16 *Lst;

	u32 Lth;
	/*
	 MaxN = pSet ->MaxN;
	 if(Num >= MaxN){
	 return;
	 }
	 Lth = pSet ->Lth;
	 Lst = pSet ->pLst;
	 *(u32*)(Buf+4) = 0;
	 CopyRam(Buf+1, pData, 5);										//显示数据5字节/假日表数据3字节
	 Buf[0] = Num;
	 if(strCmp((u8*)Lst[Num], pData, (Lth-1)) == 0){
	 return;
	 }
	 Next = Lst[MaxN];
	 if(Next&&((Next&(_FLASH_BLOCK_SIZE_-1)) <= (_FLASH_BLOCK_SIZE_-Lth))){
	 UpFlashData();
	 WriteFlashData(Next, Buf, Lth);
	 Lst[Num] = (u16)(Next+1);
	 if(((Next&(_FLASH_BLOCK_SIZE_-1))+ Lth) >= (_FLASH_BLOCK_SIZE_-Lth)){
	 Addr = Get_BlankBlock(pSet ->Type);
	 ReFresh_ExTbl(Addr, pSet);
	 return;
	 }
	 Lst[MaxN] += Lth;
	 return;
	 }
	 Addr = Get_BlankBlock(pSet ->Type);
	 ReFresh_ExTbl(Addr, pSet);
	 Up_ExTbl(pData, Num, pSet);
	 */
}

void Up_Hld(strHolidayTbl *pHld, u8 Num) { //写入假日表

	Up_ExTbl((u8*) pHld, Num, &HldTblSet);

}

void Up_DispCode(strDispCode *pDispCode, u8 Num) { //写入显示表

	Up_ExTbl((u8*) pDispCode, Num, &DispTblSet);
}


void SetCardOverTbl(void)
{

	u8 Cnt;
	for (Cnt = 0; Cnt < NUMOF(GAddr.pDispCardOver); Cnt++)
	{
		GAddr.pDispCardOver[Cnt] = (u16) ((u32) ((&FileSet.DArryCardOver[Cnt])));
	}
}

void Chk_AllData(void) { //上电检查数据

	u32 N;return ;
	//pxopt UpFlashData();
	//pxopt N = Get_BlankBlock(0);
	if (N == 0) { //数据存储区满处理
		N = WFBuf.Sum;
		if ((N & (~0x0F)) == 0x12A0) {
			N &= 0x0F;
			switch (N) {
			case 0:
				Read_FlashInfo();
				break;
			case 1:
				Read_ELVSave();
				break;
			case 2:
				Read_TTbl();
				break;
			case 3:
				Read_ExTbl(&HldTblSet);
				break;
			case 4:
				Read_ExTbl(&DispTblSet);
				break;
			}
		}
	}
	Read_FlashInfo();
	Read_ELVSave();
	Read_TTbl();
	Read_ExTbl(&HldTblSet);
	Read_ExTbl(&DispTblSet);
	SetCardOverTbl();
}
#endif

/*			以下冻结数据处理			*/
u8 Chk_FrozeType(u8 Type) { //检查冻结类型

	u8 Cnt;
	Cnt = Type & 0x3F;
	if (Cnt >= _FROZE_NUM_) {
		return 0xFF;
	}
	if ((Type & 0x40) && (FlashInfo.FrozeCnt[Cnt] & 1)) {
		return 0xFF;
	} else if ((Type & 0x80) && ((FlashInfo.FrozeCnt[Cnt] & 1) == 0)) {
		return 0xFF;
	}
	return Cnt;
}

u16 GetFStartAddr(u8 Type, u16 Num) { //获取冻结偏移地址

	if (Type >= _FROZE_NUM_) {
		return 0;
	}
	if (Type == _FROZE_FUHE_) {
		return FileSet.FrozeSet[_FROZE_FUHE_].Bias + ((Num % _FH_BLOCKS_)
				* _FH_BLTH_);
	}
	return FileSet.FrozeSet[Type].Bias + ((Num % FileSet.FrozeSet[Type].MinCnt)
			* FileSet.FrozeSet[Type].Length);
}

u8* GetFrozeStart(u8 Type, u8 Num) { //获取前N次冻结数据并返回地址

	u8 Cnt;
	u8 Lth;
	u16 StartAddr;
	Cnt = Type & 0x3F;
	if (Cnt >= _FROZE_NUM_) {
		return 0;
	}
	if (Type & 0xC0) {
		Num <<= 1;
		if ((FlashInfo.FrozeCnt[Cnt] & 1) == 0) {
			Num++;
		}
		if (Type & 0x80) {
			Num--;
		}
	}
	if (Num >= FlashInfo.FrozeCnt[Cnt]) {
		return 0;
	}
	if ((Cnt >= _FROZE_NUM_) || (Num >= 254)) {
		return 0;
	}
#ifdef		_USE_EEPROM_
	if ((RamData.LastFType == (Cnt + 1)) && (RamData.LastFCnt == Num)) {
		return RamData.FrozeBuf;
	}
#endif
	StartAddr = GetFStartAddr(Cnt, FlashInfo.FrozeCnt[Cnt] - (Num + 1));
#ifdef		_USE_EEPROM_
	Lth = FileSet.FrozeSet[Cnt].Length;
	if (Lth > sizeof(RamData.FrozeBuf)) {
		Lth = sizeof(RamData.FrozeBuf);
	}
	Read_EEData(RamData.FrozeBuf, StartAddr, FileSet.FrozeSet[Cnt].Length);
	RamData.LastFType = (Cnt + 1);
	RamData.LastFCnt = Num;
	return RamData.FrozeBuf;
#else
	return (u8 *)(StartAddr);
#endif
}

void Clr_EEData4(u16 Addr) {

	u32 N;
	N = 0;
	Write_EEData((u8*) &N, Addr, 4);
}

void ClrFrozeCnt(u8 Type) { //清除冻结次数

	u32 N;
	u8 Cnt;
	Cnt = Type & 0x3F;
	if (Cnt >= _FROZE_NUM_) {
		return;
	}
	if (Cnt == 0) {
#ifdef	_USE_EEPROM_
		for (Cnt = 0; Cnt < _FH_BLOCKS_; Cnt++) {
		//	Clr_EEData4(Cnt * _FH_BLTH_);
		}
#endif
	//	ClrRam((u8*) &RamData.FHSet, sizeof(RamData.FHSet));
	}
	Up_u16Data(&FlashInfo.FrozeCnt[Cnt], 0);
}

u16 GetFrozeCnt(u8 Type) { //获取冻结次数

	u8 Cnt;
	u16 Ret;
	Cnt = Type & 0x3F;
	if (Cnt >= _FROZE_NUM_) {
		return 0;
	} else if (FlashInfo.FrozeCnt[Cnt] > 40000) {
		Ret = 40000;
	} else {
		Ret = FlashInfo.FrozeCnt[Cnt];
	}
	if (Type & 0xC0) {
		Ret = (Ret + 1) / 2;
	}
	return Ret;
}

void Write_Froze(uc8 *Src, u8 Type, u16 N) { //写冻结数据
	u16 Bias;
	if (Type >= _FROZE_NUM_) {
		return;
	}
	Bias = GetFStartAddr(Type, N);
#ifdef	_USE_EEPROM_
	Write_EEData((char *)Src, Bias, FileSet.FrozeSet[Type].Length);
#endif
}

void Up_FrozeData(uc8 *Src, u8 Type) { //冻结数据
	u16 i;
	RamData.LastFType = 0;
	Type = Chk_FrozeType(Type);
	if (Type >= 0x80) {
		return;
	}
	Write_Froze(Src, Type, FlashInfo.FrozeCnt[Type]);
	i = FlashInfo.FrozeCnt[Type] + 1;
	if (i > 60100) {
		i -= FileSet.FrozeSet[Type].MinCnt;
	}
	Up_u16Data(&FlashInfo.FrozeCnt[Type], i);
}

u8 GetFHLth(u8 Mode) {

	static const __code u8 FHLth[16] = { //  bit3-8 bit2-2 bit1-3 bit0-7
			4, // 0000
					12, // 0001
					8, // 0010
					14, // 0011
					6, // 0100
					14, // 0101
					10, // 0110
					16, // 0111
					12, // 1000
					20, // 1001
					16, // 1010
					22, // 1011
					14, // 1100
					22, // 1101
					18, // 1110
					24, // 1111
			};
	Mode = (Mode >> 2) & 0x0F;
	return FHLth[Mode];
}


void Read_FHRcd(u8 *Buf, u16 Bias, u16 Lth) {

	while (Lth) {
		if (Lth < 128) {
			Read_EEData(Buf, Bias, Lth);
			return;
		} else {
			Read_EEData(Buf, Bias, 128);
			Buf += 128;
			Bias += 128;
			Lth -= 128;
		}
	}
}



void Up_FHMode(u8 Mode) {
	u8 Cnt;
	for (Cnt = 0; Cnt < 4; Cnt++) {
		if (Mode & (1 << Cnt)) {
			CopyRam((u8*) &RamData.FHSet.LastFH[Cnt + 1],
					(u8*) &RamData.FHSet.LastFH[0],
					sizeof(RamData.FHSet.LastFH[0]));
		}
	}

}
u32 GetFHTime(u8 *pData) {
	u32 Data;
	Copy_4((u8*) &Data, pData);
	return (Data & 0x3FFFFFF);
}

void ClrNextFH(void) {

	Clr_EEData4(RamData.FHSet.NextFH);
}

void Read_FHSet(void) {

#ifdef		_USE_EEPROM_
	u8 Cnt;
	u8 *pLst;
	u16 Addr;
	u16 Bias;
	u32 NowTm;
	u32 FHTm;
	u8 MaxCnt;
	u8 MinCnt;
	u8 FMode;
	u8 ChkCnt;

	GetFrozeMinCnt((char *)&NowTm);//RamData.RTCTime/60;
	ClrRam((u8*) &RamData.FHSet, sizeof(RamData.FHSet));
	MaxCnt = 0;
	for (Cnt = 0; Cnt < _FH_BLOCKS_; Cnt++) {
		Addr = Cnt * _FH_BLTH_;
		Read_FHRcd((u8*) &RamData.FHSet.FHStartTm[Cnt], Addr, 4);
		FMode = (RamData.FHSet.FHStartTm[Cnt] >> 26) & 0x0F;
		RamData.FHSet.FHStartTm[Cnt] &= (0x3FFFFFF);
		if ((RamData.FHSet.FHStartTm[Cnt] > NowTm) || (FMode == 0)) {
			RamData.FHSet.FHStartTm[Cnt] = 0;
			Write_EEData((u8*) &RamData.FHSet.FHStartTm[Cnt], Addr, 4);
		} else if (RamData.FHSet.FHStartTm[Cnt]
				> RamData.FHSet.FHStartTm[MaxCnt]) {
			MaxCnt = Cnt;
		}
	}
	if (RamData.FHSet.FHStartTm[MaxCnt] == 0) {
		return;
	}
	ChkCnt = (MaxCnt + _FH_BLOCKS_ - 1) % _FH_BLOCKS_;
	while (1) {
		Addr = ChkCnt * _FH_BLTH_;
		Read_FHRcd(ComData, Addr, _FH_BLTH_);
		pLst = ComData;
		while (1) {
			FMode = (pLst[3] >> 2) & 0x0F;
			//FHTm = (*(u32*)pLst)&0x3FFFFFF;
			FHTm = GetFHTime(pLst);
			if ((FHTm > NowTm) || (FHTm < RamData.FHSet.LastFH[0].FHTime)
					|| (FMode == 0)) {
				RamData.FHSet.NextFH = Addr + (pLst - ComData);
				if (FHTm) {
					ClrNextFH();
					//Write_EEData(zeroP, RamData.FHSet.NextFH, 4);
					MaxCnt = ChkCnt;
				}
				break;
			}
			RamData.FHSet.LastFH[0].FHTime = FHTm;
			RamData.FHSet.LastFH[0].FHAddr = Addr + (pLst - ComData);
			Up_FHMode(FMode);
			pLst += GetFHLth(pLst[3]);
			if (pLst >= (ComData + _FH_BLTH_ - 4)) {
				break;
			}

		}
		if (ChkCnt == MaxCnt) {
			if (pLst >= (ComData + _FH_BLTH_ - 4)) {
				Cnt = ((MaxCnt + 1) % _FH_BLOCKS_);
				RamData.FHSet.NextFH = Cnt * _FH_BLTH_;
				RamData.FHSet.FHStartTm[Cnt] = 0;
				ClrNextFH();
				//Write_EEData(zeroP, RamData.FHSet.NextFH, 4);
			}
			break;
		}
		ChkCnt = (ChkCnt + 1) % _FH_BLOCKS_;
	}
#endif

}

void Up_FHFroze(u8 *pData) {

#ifdef		_USE_EEPROM_
	u8 Lth;
	u8 n;
	u16 Bias;
	u8 FMode;
	Bias = RamData.FHSet.NextFH % _FH_BLTH_;
	Lth = GetFHLth(pData[3]);
	if ((Bias + Lth) > _FH_BLTH_) {
		ClrNextFH();
		//Write_EEData(zeroP, RamData.FHSet.NextFH, 4);
		n = ((RamData.FHSet.NextFH + 256) / _FH_BLTH_) % _FH_BLOCKS_;
		RamData.FHSet.NextFH = n * _FH_BLTH_;
		Bias = 0;
		//RamData.FHSet.FHStartTm[n] = (*(u32*)pData)&0x3FFFFFF;
		RamData.FHSet.FHStartTm[n] = GetFHTime(pData);
	} else if (Bias == 0) {
		n = ((RamData.FHSet.NextFH) / _FH_BLTH_);
		//RamData.FHSet.FHStartTm[n] = (*(u32*)pData)&0x3FFFFFF;
		RamData.FHSet.FHStartTm[n] = GetFHTime(pData);
	}
	Write_EEData(pData, RamData.FHSet.NextFH, Lth);
	//RamData.FHSet.LastFH[0].FHTime = (*(u32*)pData)&0x3FFFFFF;
	RamData.FHSet.LastFH[0].FHTime = GetFHTime(pData);
	RamData.FHSet.LastFH[0].FHAddr = RamData.FHSet.NextFH;
	FMode = ((pData[3]) >> 2);
	Up_FHMode(FMode);
	RamData.FHSet.NextFH += Lth;
	if ((Bias + Lth) >= (_FH_BLTH_ - 4)) {
		n = ((RamData.FHSet.NextFH + 256) / _FH_BLTH_) % _FH_BLOCKS_;
		RamData.FHSet.NextFH = n * _FH_BLTH_;
		RamData.FHSet.FHStartTm[n] = 0;
	}
	ClrNextFH();
	//Write_EEData(zeroP, RamData.FHSet.NextFH, 4);
#endif
}

u8 Get_FHFrozeBlock(void) {

	u8 Cnt;
	u8 Cnt_1;
	u8 Cnt_2;
	u32 StartTm;
	//	if(RamData.RdFHTime > RamData.FHSet.LastFH[0].FHTime){
	//		StartTm = RamData.FHSet.LastFH[0].FHTime - 60;				//1小时提前量
	//	} else {
	//		StartTm = RamData.RdFHTime - 60;							//1小时提前量
	//	}

	StartTm = RamData.RdFHTime;
	for (Cnt = 0; Cnt < _FH_BLOCKS_; Cnt++) {
		Cnt_1 = (Cnt + 1) % _FH_BLOCKS_;
		Cnt_2 = (Cnt + 2) % _FH_BLOCKS_;
		if ((StartTm >= RamData.FHSet.FHStartTm[Cnt_1]) && (StartTm
				< RamData.FHSet.FHStartTm[Cnt_2])) {
			if (RamData.FHSet.FHStartTm[Cnt_1]) {
				if ((RamData.FHSet.FHStartTm[Cnt])
						&& (RamData.FHSet.FHStartTm[Cnt]
								< RamData.FHSet.FHStartTm[Cnt_1])) {
					return Cnt;
				}
				return Cnt_1;
			} else {
				return Cnt_2;
			}
		} else if (RamData.FHSet.FHStartTm[Cnt_1]
				> RamData.FHSet.FHStartTm[Cnt_2]) {
			if (StartTm >= RamData.FHSet.FHStartTm[Cnt_1]) {
				if ((RamData.FHSet.FHStartTm[Cnt])
						&& (RamData.FHSet.FHStartTm[Cnt]
								< RamData.FHSet.FHStartTm[Cnt_1])) {
					return Cnt;
				}
				return Cnt_1;
			} else if (StartTm < RamData.FHSet.FHStartTm[Cnt_2]) {
				return Cnt_2;
			}
		}
	}
	return 0xFF;
}




#ifdef DEL_pop

u8 Read_EEData( u8* Buf, u16 Addr, u8 Lth) { //读取EEPROM

	u8 Ret;
	u8 Retry;
	Retry = 0;
	while(1) {
		Ret = Read_I2C( Buf, (u32)(lAddset)+Addr, Lth);
		if(Ret == 0) {
			return SUCCESS;
		}
		if(++Retry> 5) {
			return EDataErr();
		}
	}
	return Ret;
}

u8 Write_EEData( uc8* Buf, u16 Addr, u8 Lth) { //写入EERPOM

	u8 Num;
	u8 Ret;
	u8 Retry;
	u8 RxBuf[_EE_PAGE_LENGTH_];
	Retry = 0;
	while(Lth) {
		if(Addr&(_EE_PAGE_LENGTH_-1)) {
			Num = _EE_PAGE_LENGTH_ - (Addr&(_EE_PAGE_LENGTH_-1));
		} else {
			Num = _EE_PAGE_LENGTH_;
		}
		if(Num> Lth) {
			Num = Lth;
		}
		Ret = Write_I2C( Buf, (u32)(lAddset)+Addr, Num);
		DelayTBOC(10000);
		if(Ret) {
			if(++Retry> 5) {
				return EDataErr();
			}
			continue;
		}
		Read_I2C( RxBuf, (u32)(lAddset)+Addr, Num);
		if(strCmp(RxBuf, Buf, Num)) {
			if(++Retry> 5) {
				return EDataErr();
			}
			continue;
		}
		Lth -= Num;
		Addr += Num;
		Buf+= Num;
	}
	RunState_EEP = 0;
	return 0;
}

/*		以下为MAX71315 Flash擦写处理程序		*/
#define		ELV_CODE		0x1234
#define		TBL_CODE		0x2234
#define		HLD_CODE		0x3234

u8 EraseFBlock(u32 pDst) {  //块擦长K啿
	int n;
	int Ret;
	if((pDst < _FLASH_DATA_START_)&&(pDst&((u32)(_FLASH_BLOCK_SIZE_-1)))) {
		return FAIL;
	}
	for(n = 0; n < (_FLASH_BLOCK_SIZE_/_FLASH_PAGE_SIZE_); n++) {
		Ret = flash_page_erase(pDst,FLASH_WR_ERS_PASSCODE);
		pDst += _FLASH_PAGE_SIZE_;
	}
	return SUCCESS;
}

u8 Get_FlashAddr(u32* Lst, u8 Type ) { //获取类型Flash块地址列表(Lst:地址列表位置,Type:类型,返回找到数量Lst[0]-最新数据Lst[1]-之前数据)

	u32 Cnt;
	u32 Addr;
	u32 K;

	Addr = _FLASH_DATA_START_;
	K = 0;
	for(Cnt = 0; Cnt < _FLASH_DATA_BLOCKS_; Cnt++) {
		if((*(const __code u8*)Addr) == Type) {
			if(K < 2) {
				Lst[K] = Addr;
				K++;
			}
		}
		Addr += _FLASH_BLOCK_SIZE_;
	}
	if(K == 0) {
		WFBuf.Data[1] = 0;
		return 0;
	} else if(K <= 1) {
		WFBuf.Data[1] = (*(const __code u8*)(Lst[0]+1))+1;
		return K;
	}
	if((s8)((*(u8*)(Lst[0]+1))-(*(u8*)(Lst[1]+1))) < 0) {
		K = Lst[0];
		Lst[0] = Lst[1];
		Lst[1] = K;
	}
	WFBuf.Data[1] = (*(const __code u8*)(Lst[0]+1))+1;

	return 2;
}

const __code u8 TypeLst[] = {FLASH_INFO_START, FLASH_SAVE_START, FLASH_TBL_START, FLASH_HLD_START, FLASH_DISP_START};
u32 Get_BlankBlock(u8 Type) { //获取空数据块

	u32 Cnt;
	u32 Cnt_2;
	u32 Addr;
	u32 Lst[2];
	u8 TypeCnt[sizeof(TypeLst)];

	if(Type) {
		Cnt = Get_FlashAddr(Lst, Type);
		if(Cnt == 2) {
			EraseFBlock(Lst[1]);
			return Lst[1];
		}
	}
	ClrRam(TypeCnt, sizeof(TypeCnt));
	Addr = _FLASH_DATA_START_;
	for(Cnt = 0; Cnt < _FLASH_DATA_BLOCKS_; Cnt++) {
		if(*((const __code u32*)Addr) == 0xFFFFFFFF) {
			return Addr;
		}
		for(Cnt_2 = 0; Cnt_2 < sizeof(TypeLst); Cnt_2++) {
			if(TypeLst[Cnt_2] == (*(const __code u8*)Addr)) {
				TypeCnt[Cnt_2] += 1;
				Cnt_2 = 0;
				break;
			}
		}
		if(Cnt_2) {
			EraseFBlock(Addr);
			return Addr;
		}
		Addr += _FLASH_BLOCK_SIZE_;
	}
	for(Cnt_2 = 0; Cnt_2 < sizeof(TypeCnt); Cnt_2++) {
		if(TypeCnt[Cnt_2]> 1) {
			if(Type == 0) {
				WFBuf.Sum = 0x12A0+Cnt_2; //设置空数据块类型
				return 0;
			} else {
				Cnt = Get_FlashAddr(Lst, TypeLst[Cnt_2]);
				EraseFBlock(Lst[0]);
				return Lst[0];
			}
		}
	}

	return 0;
}

void ClrTypeBlock(u32 DAddr) { //擦除同类数据
	u8 Type;
	u32 Cnt;
	u32 Addr;
	Addr = _FLASH_DATA_START_;
	Type = *(const __code u8*)DAddr;
	for(Cnt = 0; Cnt < _FLASH_DATA_BLOCKS_; Cnt++) {
		if((Addr != DAddr)&&((*(const __code u8*)Addr) == Type)) {
			EraseFBlock(Addr);
		}
		Addr += _FLASH_BLOCK_SIZE_;
	}
}

void UpFlashData(void) { //写FLASH数据

	u32 Cnt;
	u32 Sum;
	u32 Addr;

	if(WFBuf.StartAddr < _CODE_ADDR_) {
		Addr = WFBuf.StartAddr + _CODE_ADDR_;
	} else {
		Addr = WFBuf.StartAddr;
	}
	if((WFBuf.Lth)&&(((u16)(~WFBuf.Lth)) == WFBuf.Lth_2)) {
		Sum = 0x87654321;
		for(Cnt = 0; Cnt < ((sizeof(strWFBuf)/4)-1); Cnt++) {
			Sum += ((u32*)&WFBuf)[Cnt];
		}
		if(WFBuf.Sum == Sum) {
			Cnt = 3;
			while(1) {
				if(strCmp((u8*)Addr, WFBuf.Data, (WFBuf.Lth<<2)) == 0) {
					break; //正确返回
				}
				if(((WFBuf.StartAddr&(_FLASH_PAGE_SIZE_-1)) == 0)&&(WFBuf.Lth == (_FLASH_PAGE_SIZE_>>2))) {
					flash_page_write(WFBuf.StartAddr, (u32*)WFBuf.Data);
				} else {
					flash_write32(WFBuf.StartAddr, (u32*)WFBuf.Data, WFBuf.Lth, FLASH_WR_ERS_PASSCODE);
				}
				if(--Cnt == 0) {
					Wait_Rst(0);
					break; //错误返回
				}
			}
		}
	}
	WFBuf.Lth = 0;
	WFBuf.Lth_2 = 0;

}

u8 WriteFlashData(u32 Dst, uc8* Src, u16 Lth) { //写FLASH区数据
	u32 Cnt;
	u32 SLth;
	u32 DLth;
	u32 Sum;
	u8 IsPageData;
	if((Lth == 0)||(Dst&3)) {
		return FAIL;
	}
	if(((u16)Dst) >= _FDATA_ADDR_) {
		Dst |= _FLASH_DATA_MASK_;
	}
	if(Dst >= _FLASH_DATA_START_) {
		IsPageData = 0;
	} else if((Dst >= (u32)&FData)&&((Dst+Lth) <= (((u32)&FData)+sizeof(FData)))) {
		IsPageData = 1;
	} else {
		return FAIL;
	}
	UpFlashData();
	for(Cnt = 0; Cnt < Lth; Cnt++) {
		if(((const __code u8*)Dst)[Cnt] != (GetAddrData((u32)Src)+Cnt)) {
			break;
		}
	}
	if(Cnt == Lth) {
		return SUCCESS;
	}
	while(Lth) {
		if(Dst&(_FLASH_PAGE_SIZE_-1)) {
			SLth = _FLASH_PAGE_SIZE_ - (Dst&(_FLASH_PAGE_SIZE_-1));
			if(SLth> Lth) {
				SLth = Lth;
			}
		} else if(Lth> _FLASH_PAGE_SIZE_) {
			SLth = _FLASH_PAGE_SIZE_;
		} else {
			SLth = Lth;
		}
		if(IsPageData) {
			WFBuf.StartAddr = (Dst&(~(_FLASH_PAGE_SIZE_-1)));
			CopyRam(WFBuf.Data, (WFBuf.StartAddr), _FLASH_PAGE_SIZE_);
			CopyRam(WFBuf.Data+(Dst&(_FLASH_PAGE_SIZE_-1)), Src, SLth);
			DLth = (_FLASH_PAGE_SIZE_>>2);
		} else {
			for(Cnt = 0; Cnt < SLth; Cnt += 4) {
				if(*(const __code u32*)(Dst+Cnt) != 0xFFFFFFFF) {
					IsPageData = 1;
					break;
				}
			}
			if(IsPageData == 0) {
				CopyRam(WFBuf.Data, Src, SLth);
				WFBuf.StartAddr = Dst;
				DLth = SLth>>2;
			} else {
				WFBuf.StartAddr = (Dst&(~(_FLASH_PAGE_SIZE_-1)));
				CopyRam(WFBuf.Data, (u8*)WFBuf.StartAddr, (Dst&(_FLASH_PAGE_SIZE_-1)));
				CopyRam(WFBuf.Data+(Dst&(_FLASH_PAGE_SIZE_-1)), Src, SLth);
				DLth = ((Dst&(_FLASH_PAGE_SIZE_-1)) + SLth)>>2;
			}
			if(SLth&3) {
				ClrRam(WFBuf.Data+(DLth<<2), ((4-SLth)&3));
				DLth++;
			}
		}
		WFBuf.Lth = DLth;
		WFBuf.Lth_2 = (u16)(~DLth);
		Sum = 0x87654321;
		for(DLth = 0; DLth < ((sizeof(strWFBuf)/4)-1); DLth++) {
			Sum += ((u32*)&WFBuf)[DLth];
		}
		WFBuf.Sum = Sum;
		UpFlashData();
		if(WFBuf.Lth) {
			return FAIL;
		}
		Src += SLth;
		Dst += SLth;
		Lth -= SLth;
	}
	return SUCCESS;
}
#endif
