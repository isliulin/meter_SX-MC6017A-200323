#ifndef		_EEPROM_HPP_
#define		_EEPROM_HPP_

#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
 

// I2C??

#define         Pin_SDA         4
#define         Pin_SCL         8

#define         Pin_SDA_IN      (HT_GPIOE->PTDAT & Pin_SDA)
#define         Pin_SDA_In      Pin_SDA_IN

#define  IO_I2CSDA  Pin_SDA_IN
#define         GC_ReadIO(IO_Num)   (IO_Num)

void 	Set_SDA_OUT(void);
void 	Set_SDA_IN(void);

#define         GC_SetSDA()     { } //  Set_SDA_OUT(); GC_SetIO(Pin_SDA);   }
#define         GC_ResSDA()     { } //{   Set_SDA_OUT(); GC_ResIO(Pin_SDA);   }
#define         GC_SetSCL()     { } //{   Set_SDA_OUT(); GC_SetIO(Pin_SCL);   }
#define         GC_ResSCL()     { } //{   Set_SDA_OUT(); GC_ResIO(Pin_SCL);   }
#define         GC_ReadSDA()     { } //   (GC_ReadIO(Pin_SDA_IN)?1:0)

#define 		Set_SDA()		{ HT_GPIOE->PTSET |= 0x4;} 
#define 		Res_SDA()	  { HT_GPIOE->PTCLR |= 0x4;} 
#define 		Set_SCL()	   { HT_GPIOE->PTSET |= 0x8;} 
#define 		Res_SCL()  	 { HT_GPIOE->PTCLR |= 0x8;} 

 

//#define         GC_ReadIO(IO_Num)   (IO_Num)


#define GPIO_SetAsInputPin(PIN_NO)  {  HT_GPIOE->PTDIR &= ~Pin_SDA;}
  

#define GPIO_SetAsOutputPin(PIN_NO) {  HT_GPIOE->PTDIR |= Pin_SDA;}
  




#define			_EE_PAGE_WaitMS_  (6)
#define 		_EE_PAGE_LENGTH_  16

enum 		ENUMI2CSTATE
{
	_bI2C_free_ =0 ,
	_bI2C_busy_	=1
};

struct 		STEEPROM_CACHE
{
		unsigned char	  	cEepromCache;						//	eeprom Cache有效标志
		unsigned short		iEeprom_Cache_Addr;					//	Cache  起始地址
		unsigned short		iEeprom_Cache_Len;					//	Cache  长度
		unsigned char 		sEeprom_Cache_Buf[_EE_PAGE_LENGTH_ ];			//	eeprom CACHE指针
};


extern struct 		STEEPROM_CACHE  steeprom_cache;
extern unsigned char		bOnLine; 		//		为真，上线，为0，掉电
extern unsigned char		bI2cErr;			//	RamData.I2C_Err
extern enum 	ENUMI2CSTATE		cI2cOper;			//	 正在写I2C
extern unsigned char 		cEepromDevice_statu;
void   EEData_Fun_Init(void) ;
#define		_IS_I2C_Busy()			( _bI2C_free_ != cI2cOper )
#define		_IS_I2C_Free()			( _bI2C_free_ == cI2cOper )

#define 	_EepromDevice_statu()			(cEepromDevice_statu)
#define 	_cEepromDevice_statu_Config(a)	{cEepromDevice_statu=a;}
#define 	_cEepromDevice_SetOk()			_cEepromDevice_statu_Config(SUCCESS)
#define 	_cEepromDevice_SetErr()			_cEepromDevice_statu_Config(FAIL)
#endif
