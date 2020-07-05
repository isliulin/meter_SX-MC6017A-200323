
#ifndef _FlashSave_Hpp_
#define _FlashSave_Hpp_

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"


enum ENUMFLASHOPER
{
  _FLASHOPER_NULL_ = 0,
  _FLASHOPER_SUCCESS_,
  _FLASHOPER_ERROR_,
  _FLASHOPER_ERROR_MEMNOINIT_,	//       内存未初始化
  _FLASHOPER_ERROR_MEMNOENOUGH_,	//       内存不够
  _FLASHOPER_ERROR_WRITELEN_,
  _FLASHOPER_ERROR_ADDRTOOLOW_,
  _FLASHOPER_ERROR_STARTADDROVER_,
  _FLASHOPER_ERROR_ENDADDROVER_,
  _FLASHOPER_ERROR_PAGEAGLIEN_,	//        页未对齐
  _FLASHOPER_ERROR_REFREHPAGETOOLONG_,	//        页刷新中，超过页长度
  _FLASHOPER_ERROR_VERITY_,	//         校验错误
  _FLASHOPER_END_,
};

struct STFLASHREFRESHDATA
{
  char *sFlashData;
  unsigned short iLen;
};
extern struct STFLASHREFRESHDATA stflashrefreshdata;
#define _sFlashDataTemp  stflashrefreshdata.sFlashData
#define _iFlashDataLen  		stflashrefreshdata.iLen
#define 	_McuFlashControlDataRefresh(datap,len)			{stflashrefreshdata.sFlashData = (char *)datap;stflashrefreshdata.iLen=len;}
#define 	_McuFlashControlDataClr()			{stflashrefreshdata.sFlashData = NULL ;stflashrefreshdata.iLen=0;}
#define 		_McuFlashControlDataLen_ _MCUFLASH_PAGE_SIZE_

enum ENUMFLASHOPER McuFlashRandWrite (unsigned long Dst, char *Src,
				      unsigned short Lth);
#define WFlashData (Dst,Src,Lth)   McuFlashRandWrite (Dst,* Src, Lth )
enum ENUMFLASHOPER enMcuAddr_Check (unsigned long lAddr);

enum ENUMFLASHOPER
McuFlashRandWriteStack (unsigned long Dst, char *Src, unsigned short Lth);

#define _MCUFLASH_PAGE_SIZE_  1024
#define lMcuMaxAddr   			0x10000
     #define  lMcuProgDataBase   0x8000

#endif /* __STM32F10x_CONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
