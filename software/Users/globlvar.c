#ifndef _GLOBAL_VAR_
#define _GLOBAL_VAR_

#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
#include "flash_filesystem.h"
#include "math.h"
extern const strFData FData;

const unsigned char cKeyTmMax = 3;
const unsigned char cncDianLianSave = 5;	//       电量保存定时器延时设置
const unsigned char cncOverVolMax = 5;	//       电量保存定时器延时设置
const unsigned char cnmagnetismc = 5;	//       电量保存定时器延时设置
unsigned char cBpsset;
//unsigned char	cResetSecTm =0;
 
//unsigned char cXorCheck = 0;
//unsigned char cXorEnable = 0;
unsigned short iTurnOnTm;
unsigned short iTurnOffTm;
unsigned short iVolDete;	//       交流电压
strFlashInfo FlashInfo;
const unsigned char cCWDTMAx = 50;
unsigned char ComData[_COMDATA_LEN_]__attribute__ ((aligned (4), used,at(0x20000100))) ;
strEx645 Ex645;
strRamData RamData;
 volatile struct STSCIBUF USARTCOM[_Com_End_];


EMUC emu_ecr_reg;
enum ENUMCHIPMODE enChipMode = _cChipRun_Mode_;


volatile unsigned int iDelayOut;
unsigned long lPowerKtemp = 10728836;
extern const unsigned short iVolCon;
volatile unsigned short iTime_Isr_no;
  u8* tpChar;
  u8* tpBase;

const unsigned short iVolCon = 16; 
const char sAsk[] = "\/?!\r\n";
const char sAnswer[] = "\/AME5AMS B1B-OA3TCI  \r\n";
 
const char sAsk1[] = { 06, 0x30, 0x35, 0x30, 0x0d, 0x0a, 0 };	//"\0x6050\r\n";



extern const strFlashInfo DftFlashInfo;
const struct STDATALIST stdatalist[] = {
  //{ 0x180,  (unsigned long)&RamData.stdianlian[0].LVSave.SumUse.Use , 0 , 0  },
  {0xC10, (unsigned long) &(DftFlashInfo.SafeInfo.E_Num[2]), _lAddr_, 0},
  {0x180, (unsigned long) &((RamData.lDianL180)), _lAddr_, 0},	// (0000000.122*kWh)


  {0x280, (unsigned long) &(RamData.lDianL280), _lAddr_, 0},	// (0000000.010*kWh) 0 , 0  },// 当前反向有功总电量         
  {0xC70, (unsigned long) &(RamData.lDianL280), _Func_, 0},	//
  {0x110, (long) &(RamData.Iph.sVI), _lAddr_, 0},	//       (000.0*A)
  {0x120, (long) &(RamData.Vph.sVI), _lAddr_, 0},	// (221.7*V)
};

#define			_def_chip_   	 			\
		{			   VAR_CHNLCR, 0x0607},\
    {    VAR_ADCCON, 0x000C},\
    {    VAR_EMUCTRL, 0x03F2},\
    {    VAR_ANA_control, 0xA7A6},\
    {    VAR_ADCCFG, 0x2003},\
    {    VAR_RosiCtrl, 0x4000},\
    {    VAR_HFconst, 0x38},\
    {    VAR_PStart, 0x20},\
    {    VAR_GP1, 0},\
    {    VAR_GP2, 0},\
    {    VAR_P1OFFSET, 0},\
		{    VAR_P2OFFSET, 0},\
    {    VAR_GPhs1, 0},\
    {    VAR_GPhs2, 0}

const struct STFLASHDATA stflashdata __attribute__ ((aligned (1024), used,at(0xD400))) =
{
  {  _cInit_Char_1_, _cInit_Char_2_},
  {  0x55, 0xaa},
	{  0x55, 0xaa},
  {0x55, 0xaa},
};

const unsigned short iBpsSet[7] = { 300, 600, 1200, 2400, 4800, 9600, 19200 }; 

const struct STFLASHFILESYSTEM stDianLian = {
  (unsigned long) &(stflashdata.sDIAN),
  _iBlcokLen_,
  (sizeof (stflashdata.sDIAN) / _iBlcokLen_),
  sizeof (struct STDIANLIAN) + _Receord_iNo_Len_ + _Receord_iCheckData_Len_,
  60000,
};

const struct STCHIPSET stChipdef =
{
  {
   _def_chip_

  },
  {
    {
    VAR_Krms, 1001},		//电压系数
    {
    VAR_Kims, 1002},		//电流通道系数
    {
    VAR_Kp, 1003},		//有功功率系数
  }
};




#endif
