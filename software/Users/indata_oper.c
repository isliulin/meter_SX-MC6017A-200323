#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "pubset.h"
#include "indata_oper.h"
#include "hal_delay_us.h"
#include "flash_filesystem.h"

#include "eeprom.h"
extern  const  strFileSet FileSet;
extern const  strFData FData;
extern unsigned char  ComData[];

extern const  strFlashInfo DftFlashInfo;

const unsigned short  _iFlashSaveTm_Sec_Max = 60 ;  //60*15
const unsigned char		_cRetryInitMax =5 ;
 
extern struct STMEMTER_STATU stmemter_statu[];

 

unsigned char	 DispData_init( unsigned char cMode,unsigned char cComConfig)
{
	unsigned int i ,j;
#ifdef _DEL

	Write_EEData((	unsigned char *)FileSet.stdisparrydatabak[cMode].cDispArrayNum,_DisArry_Num_EERom_startaddr_,2);
	Write_EEData((	unsigned char *)FileSet.stdisparrydatabak[cMode].DArryNorm ,_DisArry_Normal_EERom_startaddr ,  _DispArry_Len_	);
	Write_EEData((	unsigned char *)FileSet.stdisparrydatabak[cMode].DArryKey , _DispArry_key_EERom_startaddr ,  _DispArry_Len_	);

	i = (cMode == _bMETTER_LOCAL_)?0:2;
	j =(cComConfig == _bMETER_ZB_)?0:1;
	i=i+j;

	Write_EEData((	unsigned char *)stmemter_statu[i].IMType,_Dftset_METERSTAUE_,sizeof(FData.DftSet.stmemter_statu));//Write_EEData((	unsigned char *)stmemter_statu[i].IMType,_Dftset_IMType_,sizeof(FData.DftSet.stmemter_statu.IMType));
	//Write_EEData((	unsigned char *)stmemter_statu[i].SoftVer,_Dftset_SoftVer_	,sizeof(FData.DftSet.stmemter_statu.SoftVer));
	//Write_EEData((	unsigned char *)stmemter_statu[i].HardVer, _Dftset_HardVer_	,sizeof(FData.DftSet.stmemter_statu.HardVer));
#endif
	return SUCCESS;
}


char Init_FlashData(void)
{

	extern const  u16 DftYTbl1[2][14];
	extern const  u16 DftTmTbl[14]  ;
	extern const  u8  DftPrice11[2][32][3];
	extern const  strLTbl DftLTbl ;
	unsigned int 	i ,j,ilen ,itemp;//k;
	unsigned int	iset,jset;//,kset;



	Write_EEData((unsigned char*)&DftFlashInfo.RunInfo	, _RunInfo_eeprom_startaddr_, sizeof( DftFlashInfo.RunInfo )	);
	Write_EEData((char*)&DftFlashInfo.FrozeCnt, _FrozeCnt_eeprom_startaddr_, sizeof( DftFlashInfo.FrozeCnt )	);
	Write_EEData ( (char *)&FData.DftSet, _Dftset_start_,sizeof(strDftSet) );
  debug_break(_debug_errno_flashdatarestore_);
#ifdef _OPT
	Write_EEData(( unsigned char*)DftYTbl1,  _YearTbl_Eeprom_Startaddr   , _YearTbl_Eeprom_Len  );

	iset = _ST24256_ARRAY_UNIT(TmTbl);
	jset = _ST24256_ARRAY_UNIT(TmTbl[0]);
	for ( i = 0 ; i < iset;i++)
		for (j =0;j<jset;j++)
			Write_EEData(( unsigned char*)DftTmTbl,_Tmtbl_GetB(i,j),  _TmTbl_Eeprom_1_1 );

	Write_EEData(( unsigned char*)DftPrice11 ,  	_Price_Get(0,0)	  , _Price_Eeprom_Len );


	
	DispData_init(DftFlashInfo.meterinfo.stmeter_config.cMeter_Type,DftFlashInfo.meterinfo.stmeter_config.cMeter_ComType);
//--	Write_EEData((	unsigned char *)&DftLTbl,  _LTBl_Get(0)	  ,  sizeof( DftLTbl));
//--Write_EEData((	unsigned char *)&DftLTbl,  _LTBl_Get(1)	   , sizeof( DftLTbl));
	ClrRam ( ComData , sizeof(ComData));
	ilen = _PHld_Eeprom_Len ;
	i = 0;
	do {
		if ( ilen >= sizeof(ComData) )
		{
			itemp = sizeof(ComData);
			ilen  -=sizeof(ComData);
		}
		else
		{
			itemp = ilen;
			ilen =0;
		}
		Write_EEData(ComData ,_Hld_Get(0)+i, itemp );
		i+=itemp;
	} while ( ilen !=0);
#endif
 
	return SUCCESS;
}

unsigned char eeprom_data_check(unsigned char *sp,unsigned int iLen)
{
	unsigned short i;
	for ( i = 0 ; i < iLen ;i++)
	{
			if ( 0 == i)
				continue;
			else
				if (( sp[i-1] != sp[i] ) || ((0xff != sp[i]) && (0 != sp[i])))
					break;
	}
	if ( i >= iLen)
		return FAIL;
	else
		return SUCCESS;

}

u8 	cGet_First_Data(void )
{
	unsigned short 	iStart = _Dftset_InterVersion_;
	unsigned short 	iLen =  _Dftset_InterVersion_Len_ ;
	unsigned char	i =0, cResult ;

	do{
		_cEepromDevice_SetOk();
		if ( i++ )
			DelayMS(3);
		cResult = EEData_Safe_Read(ComData,iStart,iLen );
		if ( SUCCESS!=cResult )
			continue ;
			DelayMS(3);
		cResult = EEData_Safe_Read(ComData+iLen ,iStart,iLen );
		if ( SUCCESS!=cResult )
			continue ;
		if (  0 == strCmp( ComData,ComData+iLen ,iLen ))
		{			 
					return SUCCESS;
		}
	}while ( i < _cRetryInitMax );

	_cEepromDevice_SetErr();
	return FAIL;
}


char		Exteeprom_2_FlashInfo(strFlashInfo *stp)
{
	extern const struct STFLASHDATA stflashdata ;
		memcpy( (char *)&FlashInfo,(char	*)(stflashdata.sFlashInfo) , sizeof(strFlashInfo));	 
	if ( SUCCESS ==	Read_EEData( (unsigned char *)&(stp->RunInfo), _RunInfo_eeprom_startaddr_  , sizeof( strRunInfo )))
	{
			 if ( SUCCESS ==	Read_EEData( (unsigned char *)&(stp->FrozeCnt), _FrozeCnt_eeprom_startaddr_  , sizeof( stp->FrozeCnt )))
				 					return SUCCESS;
		}
		return  ERROR;
} 

unsigned char	cFlashinfo_Restore( void )
{
	if ( SUCCESS ==	cGet_First_Data( )) //Read_EEData( ComData,  _Dftset_start_ ,sizeof(strDftSet)))
	{
		if ( 0 == strCmp( ComData,(char *)&FData.DftSet.InterVersion,  _Dftset_InterVersion_Len_))
		{
				if ( SUCCESS == Exteeprom_2_FlashInfo(&FlashInfo))
			//if ( SUCCESS ==	Read_EEData( (unsigned char *)&FlashInfo.RunInfo, _RunInfo_eeprom_startaddr  , sizeof( strRunInfo )))
			{
					 if (  _Flasheck_Check())
					 {
						 if (_SYS_ON_LINE() )
						 {
							 Init_FlashData();
							 Wait_Rst(FLASH_INIT);
						 }
						 return FALSE;
					 }
					 return SUCCESS;
			}
			else
				return FALSE;
		}else
		{
				if ( SUCCESS!=_EepromDevice_statu()  )
				{
					CopyRam(  (unsigned char *)&FlashInfo,(unsigned char *)&DftFlashInfo , sizeof( strFlashInfo ));
				}else
				 if (_SYS_ON_LINE() )
				 {
						 Init_FlashData();
						 Wait_Rst(FLASH_INIT);
				 }
		}

	}
	else
	{
		CopyRam(  (unsigned char *)&FlashInfo,(unsigned char *)&DftFlashInfo , sizeof( strFlashInfo ));
		return FALSE;
	}
	return FALSE;
} 


// 将电量写入外部E方
unsigned char	cFlashinfo_dianlian_backup( void )
{
		if (!_ISDIANLIAN_MOD() )
			return SUCCESS;
		if ( SUCCESS == Write_EEData(( unsigned char*)&(RamData.stdianlian), _iDLDATA_START , _iDLDATA_SIZE))
		{
				_DIANLIANMOD_CLR()	;
				return SUCCESS;
		}
		return  FAIL;

}

