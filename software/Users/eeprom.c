/** ****************************************************************************
 *
 * @file	port.c
 * @brief	This is Port program for S1C17M11 demonstration.
 *
 * @par		Copyright:
 * 			Copyright (C) SEIKO EPSON CORPORATION 2011-2012. All rights reserved.
 *
 * @warning -
 *
 * @par		History:
 *			- 08/10/2012    0.10
 *				- First revision.
 *			- 10/31/2012    0.90
 *				- Change source code.
 *
 **************************************************************************** */
//#include "system.h"

//#include <port.h>
//#include <lcd8a.h>
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

#include "hal_delay_us.h"
#include <string.h>
#include "eeprom.h"

#ifdef _DEBUG_
 #define		_SMC3_DEBUG_PIN_	P1DAT_P12OUT	  	//	继电器控制
#endif
 



void    SetGP1(u32 ChgBit, u8 BitNew);

u8 		Get_SYS_Statu ( void );
const unsigned char		_cEepromRetryNo  = 5;			//	 重试次数
const unsigned short  iEEPROM_MAXADDR =  0x800;		//	外部E方最大地址
extern struct 		STEEPROM_CACHE  steeprom_cache;

/* --- global variable --- */
// Interrupt flag.

 /*
u8	EDataErr(void){

	RunState_EEP = 1;
	SetEvtState(_EVT_EEP_);
	return FAIL;
}
*/

void	I2C_FStop(void) ;
u32	Send_I2C_Addr(u32 Addr);
u8	Read_I2C( u8 *pBuf, u32 Addr, u16 Num);
u8	Write_I2C( const u8 *pBuf, u32 Addr, u16 Num);
void I2C_Stop(void);
void I2C_FStop(void);
u8 I2C_Start(void);
u8 I2C_RByte(void);
void I2C_ACK(void);

//eeprom.c
unsigned char bOnLine; //		??,??,?0,??
unsigned char bI2cErr; //	RamData.I2C_Err
//volatile unsigned char		cEeprom;
enum ENUMI2CSTATE cI2cOper; //	 ???I2C
unsigned char 		cEepromDevice_statu;
struct STEEPROM_CACHE steeprom_cache; 


void   EEData_Fun_Init(void)
{
	ClrRam((unsigned char *)&steeprom_cache   , sizeof(steeprom_cache)/sizeof(char)  );
}

void DelayTBOC1(unsigned int x )
{
    DelayMS(x);
    return;   
}

void Set_SDA_OUT(void)
{
	//P1IOEN_P17IEN	=0;
	//P1IOEN_P17OEN 	=1;
	GPIO_SetAsOutputPin(IO_I2CSDA);
    NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
}

void Set_SDA_IN(void)
{
	
	GPIO_SetAsInputPin(IO_I2CSDA);
    NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
	NOP_ASM();	//asm("nop");
}


unsigned    long lAddset = 0xa000;


u8	Read_EEData1( u8* Buf, u16 Addr, u16 Lth)
{
 unsigned short itemp ;
 unsigned long	ltemp  ;
	u8	Ret;
	u8	Retry;
	Retry = 0;
	while(1)
	{
		itemp = 0;
		if ((steeprom_cache.cEepromCache) && (Addr >= steeprom_cache.iEeprom_Cache_Addr ) &&  (( steeprom_cache.iEeprom_Cache_Addr+steeprom_cache.iEeprom_Cache_Len) >=(Addr +Lth)))
		{
				cCopyRam( Buf ,steeprom_cache.sEeprom_Cache_Buf+(Addr -steeprom_cache.iEeprom_Cache_Addr ),Lth );return SUCCESS;
		}
		else
		{
			//Ret = Read_I2C( Buf, (u32)(0xA00000)+Addr, Lth);
			if ( Lth >=sizeof (steeprom_cache.sEeprom_Cache_Buf) )
			{
				Ret  = Read_I2C(  Buf, (u32)(lAddset)+Addr,Lth);
			}
			else
			{
				ltemp = Addr ;
				ltemp +=sizeof (steeprom_cache.sEeprom_Cache_Buf);
				if ( ltemp >=iEEPROM_MAXADDR)
					itemp  = iEEPROM_MAXADDR - Addr ;
				else
					itemp  = sizeof (steeprom_cache.sEeprom_Cache_Buf);
				Ret  = Read_I2C(  steeprom_cache.sEeprom_Cache_Buf, (u32)(lAddset)+Addr,itemp);
			}
			if(Ret == 0)
			{
				if ( 0 == itemp )
				{
						itemp = sizeof (steeprom_cache.sEeprom_Cache_Buf);
						cCopyRam (steeprom_cache.sEeprom_Cache_Buf ,Buf ,sizeof (steeprom_cache.sEeprom_Cache_Buf));
				}
				else
						cCopyRam (Buf,steeprom_cache.sEeprom_Cache_Buf ,Lth);
				steeprom_cache.iEeprom_Cache_Addr= Addr;
				steeprom_cache.iEeprom_Cache_Len=itemp ;
				steeprom_cache.cEepromCache = 1;
				return SUCCESS;
			}
		}
		if(++Retry > _cEepromRetryNo)
		{
			EEData_Fun_Init();
			return FAIL;  // return EDataErr();
		}
	}
	return Ret;
}

u8	Read_EEData( u8* Buf, u16 Addr, u16 iLth)
{
	unsigned short itemp ;
	u8	Ret;
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=1;
#endif
	if (_ISPowerOff()) return FALSE;
	_Rset_Global_Val( cI2cOper ,_bI2C_busy_);
	bOnLine = Get_SYS_Statu () ;
	 WDGTm_Refresh( iLth /_EE_PAGE_LENGTH_+10);
	Ret = Read_EEData1( Buf, Addr, iLth);
	_Rset_Global_Val( cI2cOper ,_bI2C_free_);
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=0;
#endif
		if ( 0 == Ret )
	{
		//	if(FlashInfo.RunInfo.EState&_ESTATE_EEP_)
			{
		//				ClrEBit(_BIT_EEP_);
			}
	}
	else
	{
		EEData_Fun_Init();
		return EDataErr();

	}
	return Ret ;
}
/*
void T16Ch3_IE_TURN_ON_OFF(unsigned char ctemp);
unsigned char	cCheckResult(void)
{
	extern volatile unsigned char		cEeprom;
	unsigned char	ctemp ;
	u8	DAddr;

	T16Ch3_IE_TURN_ON_OFF(0);
	cEeprom =3;
	T16Ch3_IE_TURN_ON_OFF(1);
	do
	{

		DAddr = Send_I2C_Addr((u32)(0xA00000));
		if(0 == (DAddr&0xFF))
			return 0; 	//I2C有应答返回
		T16Ch3_IE_TURN_ON_OFF(0);
		ctemp  =cEeprom ;
		T16Ch3_IE_TURN_ON_OFF(1);
	}while (ctemp);
	return 1;
}
*/
u8	Write_EEData1( uc8* Buf, u16 Addr, u16 iLth)
{//volatile u8 ctemp;
	u16	iNum;
	u8	Ret;
	u8	Retry;
	u8	RxBuf[_EE_PAGE_LENGTH_];

	Retry = 0;  EEData_Fun_Init();
	while(iLth){
		if(Addr&(_EE_PAGE_LENGTH_-1)){
			iNum = _EE_PAGE_LENGTH_ - (Addr&(_EE_PAGE_LENGTH_-1));
		} else {
			iNum = _EE_PAGE_LENGTH_;
		}
		if(iNum > iLth){
			iNum = iLth;
		}
		Ret = Write_I2C( Buf, (u32)(lAddset)+Addr, iNum);
		//ctemp=cCheckResult();
		DelayTBOC1(_EE_PAGE_WaitMS_);
		if(Ret){
			if(++Retry > _cEepromRetryNo){
				return FAIL;  // return EDataErr();
			}
			continue;
		}
		Read_I2C( RxBuf, (u32)(lAddset)+Addr, iNum);
		if(strCmp(RxBuf, Buf, iNum)){
			if(++Retry > _cEepromRetryNo){
				return FAIL;  // return EDataErr();
			}
			continue;
		}
		iLth -= iNum;
		Addr += iNum;
		Buf+= iNum;
	}
	
	return 0;
}

u8	Write_EEData( u8* Buf, u16 Addr, u16 iLth)
{
	u8	Ret;
	if ( SUCCESS != _EepromDevice_statu())
		{
			EEData_Fun_Init();
		    return EDataErr();
		}
	_Rset_Global_Val( cI2cOper ,_bI2C_busy_);
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=1;
#endif
	bOnLine = Get_SYS_Statu () ;
	WDGTm_Refresh( iLth *3/_EE_PAGE_LENGTH_);
	Ret = Write_EEData1( Buf, Addr, iLth);
	_Rset_Global_Val( cI2cOper ,_bI2C_free_);
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=0;
#endif
		if ( SUCCESS == Ret )
	{
		RunState_EEP = 0;
		if(FlashInfo.RunInfo.EState&_ESTATE_EEP_)
		{
					ClrEBit(_BIT_EEP_);
		}
	}else
	{
		EEData_Fun_Init();
		return EDataErr();

	}
	return Ret ;
}

/*		IO_Driver
void	SetGP0(u32 ChgBit, u8 BitNew){

	IC = 0;
	if(BitNew&1){
		PO0 |= ChgBit;
	} else {
		PO0 &= ~ChgBit;
	}
	if(BitNew&2){
		PD0 |= ChgBit;
	} else {
		PD0 &= ~ChgBit;
	}
	IC = 1;
}

void	SetGP1(u32 ChgBit, u8 BitNew){

	IC = 0;
	if(BitNew&1){
		PO1 |= ChgBit;
	} else {
		PO1 &= ~ChgBit;
	}
	if(BitNew&2){
		PD1 |= ChgBit;
	} else {
		PD1 &= ~ChgBit;
	}
	IC = 1;
}


void	Init_GPIO(u8 Mode){

	if(Mode){
		PO0 = _PO0_INT_;
		PD0 = 0xFFFFFFFF;
		PD0 = _PD0_INT_;
		PO1 = _PO1_INT_;
		PD1 = 0xFFFFFFFF;
		PD1 = _PD1_INT_;
		if(RamData.LastKeySec = 76543210){
			JDLED_Off();
			Bklt_Off();
		}
	} else {
		PD0 = _PD0_INT_OFF_;
		PO0 = _PO0_INT_OFF_;
		PD1 = _PD1_INT_OFF_;
		PO1 = _PO1_INT_OFF_;
	}
}

*/
void	I2C_Delay(void);
u32 rev32(u32 *s){
	u8 d[4];

	d[0] = ((u8*)s)[3];
	d[1] = ((u8*)s)[2];
	d[2] = ((u8*)s)[1];
	d[3] = ((u8*)s)[0];
	return (*(u32*)d);
}

void	I2C_Delay(void)
{
	u16	Cnt;
	for ( Cnt =0;Cnt <5;Cnt++)
	 NOP_ASM();	//asm("nop");
	 return ;
	 if (!bOnLine) //if(RamData.Pwr_On)
	{
		 NOP_ASM();	//asm("nop"); Cnt = 1;//3;
		//while(--Cnt);
	}
	 else
	 {
		 NOP_ASM();	//asm("nop");
		 return ;
	 }

}

void	I2C_Stop(){//0					//发送I2C停止位
	Set_SDA_OUT();
	Res_SDA();
	I2C_Delay();
	Set_SCL();
	I2C_Delay();
	Set_SDA();
}
void	I2C_FStop(){//0					//强制中断I2C操作

	u32 i;
	i = 10;
	do {
		I2C_Stop();
		Set_SDA_IN();
		if(GC_ReadIO(Pin_SDA_In))
		{
			return;
		}
	} while(--i);
	i = 10;
	do {
		Res_SCL();
		I2C_Stop();
		Set_SDA_IN();
		if(GC_ReadIO(Pin_SDA_In)){
			return;
		}
	} while(--i);
}

u8	I2C_Start(){//0					//发送I2C起始位(返回0-成功,其他失败)

	u32	Cnt;
	u8 Err;

	Set_SDA_IN();
	if(!GC_ReadIO(Pin_SDA_In))
	{
		I2C_FStop();
		Err =1;
	}
	else
		Err =0;
	bI2cErr  = 0; //RamData.I2C_Err = 0;
	Set_SCL();
	Set_SDA_IN( ) ;//     ==============Set_SDA();
	if( Err)
		I2C_Delay();
	if ( bOnLine ) //if(RamData.Pwr_On)
	{
		Cnt = 2; //100
	} else {
		Cnt = 1;
	}
	do {
		I2C_Delay();
	} while(--Cnt);
	if(!GC_ReadIO(Pin_SDA_In)){
		bI2cErr  = 1;//RamData.I2C_Err = 1;
		return 1;
	}
	Set_SDA_OUT();Res_SDA();
	I2C_Delay();
	Res_SCL();
	I2C_Delay();
	return 0;
}
void	I2C_ACK(){//0					//发送I2C应答位
	Set_SDA_OUT();
	Res_SDA();
	I2C_Delay();
	Set_SCL();
	I2C_Delay();
	Res_SCL();
}
u8	I2C_WByte(u8 Data)
{	 		//I2C字节写操作(返回0-成功,2-应答失败)

	u32	Cnt;
	Set_SDA_OUT();
	/*Cnt = 8;
	do {
		if(Data&0x80)
		{	Set_SDA();}
		else
		{	Res_SDA();}

		Set_SCL();
		I2C_Delay();
		Data <<= 1;
		Res_SCL();
	} while(--Cnt);*/
	for ( Cnt =8;Cnt>0;Cnt--)
	{
		if(Data&0x80)
				{	Set_SDA();}
				else
				{	Res_SDA();}
		Set_SCL();
				I2C_Delay();
				Data <<= 1;
				Res_SCL();
	}
	Res_SCL();
	Set_SDA_IN() ;//GC_HizSDA();
	Set_SCL();

	//Set_SDA();
	I2C_Delay();
	//Set_SCL();
	if ( 0 ) //== bI2cErr ) // if(RamData.I2C_No_Err)
	{
		Cnt = 0;
	} else {
		Cnt = GC_ReadIO(Pin_SDA_In)<<1;
	}
	Res_SCL();
	I2C_Delay();
	return (u8)Cnt;
}
u8	I2C_RByte(){//0					//I2C字节读操作(返回读取字节)

	u32	Data;
	u32	Cnt;

	Set_SDA_IN( ) ; //Set_SDA();
	Cnt = 8;
	Data = 0;
	do {
		Set_SCL();
		I2C_Delay();
		if(GC_ReadIO(Pin_SDA_In)){
			Data = (Data<<1)|1;
		} else {
			Data = (Data<<1);
		}
		Res_SCL();
	} while(--Cnt);
	return Data;
}
u32	Send_I2C_Addr(u32 Addr){//0			//发送I2C起始地址(Addr-地址(低字节先发),返回0xXX00-成功,其他-失败码)

	u32	Ret;
	u32	Cnt;
	u32	DAddr;
	
	u32  ltemp =  ( Addr & 0x0f00) *2;
	 ltemp &=0x0f00;
	Addr = Addr & 0xf0ff;
	Addr |= ltemp;
	
	Ret = I2C_Start();
	if(Ret){
		return Ret;				//I2C_SDA电平错误
	}
	Addr = rev32(&Addr);
	Cnt = 4;
	while((Addr&0xFF)==0){
		Addr>>= 8;
		Cnt--;
	}
	DAddr = ((Addr&0xFF)|1)<<8;
	Addr &= (~1);
	do {
		Ret =(u32) I2C_WByte(Addr&0xFF);
		if(Ret){
			return Ret;			//I2C应答错误
		}
		Addr >>= 8;
	}while(--Cnt);

	return DAddr;
}

u8	Read_I2C( u8 *pBuf, u32 Addr, u16 iNum){//0		//I2C读操作(pBuf-数据区,Addr-地址,Num-长度,返回0-成功,其他失败码)

	u32	DAddr;
	DAddr = Send_I2C_Addr(Addr);
	if(DAddr&0xFF){
		return DAddr;				//I2C错误返回
	}
	I2C_Start();
	if(I2C_WByte(DAddr>>8)){
		I2C_Stop();
		bI2cErr  = 1;//RamData.I2C_Err = 1;
		return 2;				//I2C应答错误
	}
	while(1){
		*pBuf++ = I2C_RByte();
		if((--iNum)==0){
			break;
		}
		I2C_ACK();
	}
	I2C_Delay();
	Set_SCL();
	I2C_Delay();
	Res_SCL();
	I2C_Delay();
	I2C_Stop();
	bI2cErr  = 0;//RamData.I2C_Err = 0;
	return 0;					//正确返回
}
u8	Write_I2C( const u8 *pBuf, u32 Addr, u16 iNum){//0	//I2C写操作(pBuf-数据区,Addr-地址,Num-长度,返回0-成功,其他失败码)

	u8	DAddr;
	DAddr = Send_I2C_Addr(Addr);
	if(DAddr&0xFF){
		return DAddr;				//I2C错误返回
	}
	while(iNum--){
		if(I2C_WByte(*pBuf++)){
			I2C_Stop();
			bI2cErr =1 ; //RamData.I2C_Err = 1;
			return 2;			//I2C应答错误
		}
	}
	I2C_Stop();
	bI2cErr =0 ; // RamData.I2C_Err = 0;
	return 0;
}

/*
unsigned char	Read_EEData_IO( unsigned char* Buf, u16 Addr, u16 Lth){

	unsigned char	Ret;
	unsigned char	Retry;
	Retry = 0;
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=1;
#endif
	while(1){
		cI2cOper =_bI2C_busy_;
		bOnLine = Get_SYS_Statu () ;
		WDGTm_Refresh( Lth /_EE_PAGE_LENGTH_+10);
		Ret = Read_I2C( Buf, (u32)(0xA00000)+Addr, Lth);
		cI2cOper =_bI2C_free_;
		if(Ret == 0){
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=0;
#endif
			return SUCCESS;
		}
		if(++Retry > 5){
			EEData_Fun_Init();
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=0;
#endif
						return FAIL;  // return EDataErr();
		}
	}
#ifdef _DEBUG_
	_SMC3_DEBUG_PIN_=0;
#endif
	return Ret;
}
8*/

u8 	EEData_Safe_Read( u8* Buf, u16 Addr, u16 iLth)
{
	EEData_Fun_Init();
	return Read_EEData( Buf,  Addr,  iLth);
}

