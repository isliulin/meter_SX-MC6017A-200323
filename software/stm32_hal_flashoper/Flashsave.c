#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "Flashsave.h"
//#include "wdgfun.h"
//#include "global_flashaddr_stm32102c8.h"

#define HAL_FLASH_Unlock()   {}
#define	 HAL_FLASH_Lock()  {}
	#define Wdg_Inc {}
struct STFLASHREFRESHDATA stflashrefreshdata;

//char sFlashTemp[_MCUFLASH_PAGE_SIZE_];
//const unsigned int lsFlashTemp = sizeof (sFlashTemp);

#define 	_Flashmemcpy(a,b,c)		memcpy ( a,b,c)
#define 	_Flashmemcmp(a,b,c)		memcmp ( a,b,c)

#define 		_cWDGINC_				2000	//20MS
#define 		_cFLASHOPERRETRY_			2	//       ÖØÊÔ´ÎÊý



enum ENUMFLASHOPER
enMcuAddr_Check (unsigned long lAddr)
{
  if (lAddr > lMcuMaxAddr)
    return _FLASHOPER_ERROR_STARTADDROVER_;
  else if (lAddr < lMcuProgDataBase)
    return _FLASHOPER_ERROR_ADDRTOOLOW_;
  return _FLASHOPER_SUCCESS_;
}


static enum ENUMFLASHOPER
McuFlashPageWrite (unsigned long Dst, char *Src1, unsigned short Lth)
{
  enum ENUMFLASHOPER enResult;
  int itemp;
  char *Src = (char *) Src1;
  unsigned long j, n, ltemp;
  char cSteoreUnitLen = sizeof (uint32_t);

  if (0 == Lth)
    return _FLASHOPER_ERROR_WRITELEN_;

  enResult = enMcuAddr_Check (Dst);
  if (_FLASHOPER_SUCCESS_ != enResult)
    return enResult;


  itemp = Dst % _MCUFLASH_PAGE_SIZE_;
  if (itemp)
    return _FLASHOPER_ERROR_PAGEAGLIEN_;

  if (Lth > _MCUFLASH_PAGE_SIZE_)
    return _FLASHOPER_ERROR_REFREHPAGETOOLONG_;

  itemp = Dst / _MCUFLASH_PAGE_SIZE_;

  if (0 == _Flashmemcmp (Src1, (char *) Dst, Lth))
    return _FLASHOPER_SUCCESS_;

  HAL_FLASH_Unlock ();
  Wdg_Inc (_cWDGINC_);
	
	/*
  for (j = Dst, n = 0; n < Lth;
       n += cSteoreUnitLen, j += cSteoreUnitLen, Src += cSteoreUnitLen)
    {
      _Flashmemcpy ((char *) &ltemp, Src, cSteoreUnitLen);
      HAL_FLASH_Program (FLASH_TYPEPROGRAM_WORD, j, ltemp);     
    }
	*/
	 HT_Flash_ByteWrite ((char *)Src1, (unsigned long) Dst,	Lth);
  HAL_FLASH_Lock ();
  return (0 ==
	  _Flashmemcmp (Src1, (char *) Dst,
			Lth)) ? _FLASHOPER_SUCCESS_ :
    _FLASHOPER_ERROR_VERITY_;

}
 
static enum ENUMFLASHOPER
McuFlashPageSafeWrite (unsigned long Dst, char *Src, unsigned short  Lth)
{
  enum ENUMFLASHOPER enResult;
  unsigned char i;
 // FLASH_EraseInitTypeDef EraseInitStruct;

  uint32_t PageError;


  for (i = 0; i < _cFLASHOPERRETRY_; i++)
    {
      enResult = McuFlashPageWrite (Dst, Src, Lth);
      if (_FLASHOPER_ERROR_VERITY_ == enResult)
	{
	  HAL_FLASH_Unlock ();
	//  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	//  EraseInitStruct.PageAddress = Dst;	//FLASH_USER_START_ADDR;
	//  EraseInitStruct.NbPages = Lth / FLASH_PAGE_SIZE;;	// (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

	  //HAL_FLASHEx_Erase (&EraseInitStruct, &PageError);
	HT_Flash_PageErase(Dst);//  	BlockEarse (&stDianLian1,Dst))
	  HAL_FLASH_Lock ();
	}
      else
	return enResult;
    }
  return enResult;
}

static enum ENUMFLASHOPER
McuFlashPageRandWrite (unsigned long Dst, char *Src, unsigned short Lth,
		       unsigned long *len)
{
  enum ENUMFLASHOPER enResult;
  unsigned long lMcuAddr;
  unsigned long lMcuEndAddr = Dst + Lth;
  unsigned long lCurrPageEnd;
  unsigned long loffset = 0;
  int itemp;

  if (0 == Lth)
    return _FLASHOPER_ERROR_WRITELEN_;

  enResult = enMcuAddr_Check (Dst);
  if (_FLASHOPER_SUCCESS_ != enResult)
    return enResult;

  enResult = enMcuAddr_Check (lMcuEndAddr);
  if (_FLASHOPER_SUCCESS_ != enResult)
    return enResult;

  if (NULL == _sFlashDataTemp)
    return _FLASHOPER_ERROR_MEMNOINIT_;
  if (_iFlashDataLen < _MCUFLASH_PAGE_SIZE_)
    return _FLASHOPER_ERROR_MEMNOENOUGH_;


  itemp = Dst / _MCUFLASH_PAGE_SIZE_;
  lMcuAddr = itemp * _MCUFLASH_PAGE_SIZE_;
  lCurrPageEnd = lMcuAddr + _MCUFLASH_PAGE_SIZE_;
  if (lMcuAddr != Dst)
    loffset = Dst - lMcuAddr;

  if (lCurrPageEnd >= lMcuEndAddr)
    *len = Lth;
  else
    *len = lCurrPageEnd - Dst;

  _Flashmemcpy (_sFlashDataTemp, (char *) lMcuAddr, _MCUFLASH_PAGE_SIZE_);
  _Flashmemcpy (_sFlashDataTemp + loffset, Src, *len);

  return McuFlashPageSafeWrite (lMcuAddr, (char *) _sFlashDataTemp,
				_MCUFLASH_PAGE_SIZE_);
}

enum ENUMFLASHOPER
McuFlashRandWrite (unsigned long Dst, char *Src, unsigned short Lth)
{
  signed int len = Lth;
  enum ENUMFLASHOPER enResult;
  while (Lth)
    {
      enResult =
	McuFlashPageRandWrite (Dst, Src, Lth, (unsigned long *) &len);
      if (_FLASHOPER_SUCCESS_ != enResult)
	return enResult;

      if ((signed) len > 0)
	{
	  Dst += len;
	  Src += len;
	  Lth -= len;
	}
      if (0 == len)
	return _FLASHOPER_SUCCESS_;
    }
  if (0 == Lth)
    return _FLASHOPER_SUCCESS_;
  return enResult;

}

enum ENUMFLASHOPER
McuFlashRandWriteStack (unsigned long Dst, char *Src, unsigned short Lth)
{
  char stemp[_McuFlashControlDataLen_];
  _McuFlashControlDataRefresh (stemp, sizeof (stemp));

  return McuFlashRandWrite (Dst, Src, Lth);
}
