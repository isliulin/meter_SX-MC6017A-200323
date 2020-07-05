#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "string.h"
#include <string.h>
#include "bit_edge.h"
#include "unit_instance.h"
#include "flash_filesystem.h"

extern const struct STFLASHDATA stflashdata;
extern struct STFLASHFILESYSTEM stDianLian;

const unsigned char cFlash_Eease_Retry = 2;

unsigned char
Full_FF_Check (unsigned char *sp, unsigned short ilen)
{
  unsigned short i;
  unsigned char ctemp;

  for (i = 0, ctemp = 0xff; i < ilen; i++)
    ctemp &= *(sp + i);
  if (0xff == ctemp)
    return TRUE;
  return FALSE;
}

unsigned short
CRC_Calu (struct STRECORD *stp)
{
  unsigned short i, j = _Receord_CRC_Len (stp);
  unsigned short itemp;

  for (i = 0, itemp = 0; i < j; i++)
    itemp += *(stp->sData + i);
  return itemp;
}

//              检查数据是否有效
unsigned char
Recode_CheckState (struct STRECORD *stp)
{
  unsigned short iCheckData;

  unsigned char ctemp;
  ctemp = Full_FF_Check (stp->sData, _Receord_CheckZero_Len (stp));
  stp->cFlag = _Record_Null_;
  if (TRUE == ctemp)
    return stp->cFlag;

  iCheckData = CRC_Calu (stp) + 1;
  stp->cFlag = _Record_Fail_;
  if (iCheckData != stp->iCheckData)
    return stp->cFlag;;

  stp->cFlag = _Record_Ok_;
  return stp->cFlag;
}

short
Record_Comm (unsigned short iPrev, unsigned short iSucc, unsigned short iMax)
{
  //若iPrev=iSucc，则返回零；
  //若iPrev>iSucc，则返回正数；
  //若iPrev<iSucc，则返回负数。

  if (iPrev == iSucc)
    return 0;
  else if (iPrev > iSucc)
    {
      if ((iPrev == iMax - 1) && (0 == iSucc))
	return 0;
      return 1;
    }
  else
    return 0;
}

//      将外部数据格式化成存储结构
void
Record_Format (struct STRECORD *stp)
{
  unsigned short i, j, itemp;

  j =
    ((((struct STRECORD *) stp)->cLen)) - _Receord_iCheckData_Len_ -
    _Receord_iNo_Len_;

  for (i = 0, itemp = 0; i < j; i++)
    itemp += *(stp->sData + i);
  for (i = 0; i < _Receord_iNo_Len_; i++)
    itemp += *((unsigned char *) &(stp->iNo) + i);
  stp->iCheckData = itemp + 1;

}

//      存储结构恢复至工作结构
void
Record_restore (unsigned char *sp, unsigned short ilen, struct STRECORD *stp)
{
  char *stemp;
  stp->sData = sp;
  stp->cLen = (unsigned char) ilen;
  stemp = (char *) sp + ilen - _Receord_iNo_Len_ - _Receord_iCheckData_Len_;
  memcpy ((char *) &stp->iNo, stemp, sizeof (stp->iNo));
  stemp = (char *) sp + ilen - _Receord_iCheckData_Len_;
  memcpy ((char *) &stp->iCheckData, stemp, sizeof (stp->iCheckData));
}

unsigned char
Record_Get_First (struct STFLASHFILESYSTEM *stfilep, struct STRECORD *stCurr)
{
  unsigned long lAddr;
  unsigned short i, j, iFind;
  enum RECORDSTATU enRecordstatu;
  struct STRECORD *stPrev = &(stCurr[1]);
  struct STRECORD sttemp;
  unsigned short iBlockRecMax = _GetBlockRecordMax (stfilep);

  memset ((char *) &sttemp, 0xff, sizeof (struct STRECORD));
  memset ((char *) stCurr, 0xff, sizeof (struct STRECORD) * 2);

  for (i = 0, iFind = FALSE; i < stfilep->iBlockMax; i++)
    {
      lAddr = stfilep->lStartAddr + i * stfilep->iBlockByte;
      for (j = 0; j < iBlockRecMax; j++, lAddr += stfilep->iRecordLen)
	{
	  Record_restore ((unsigned char *) lAddr, stfilep->iRecordLen,
			  &sttemp);
	  enRecordstatu = Recode_CheckState (&sttemp);
	  switch (enRecordstatu)
	    {
	    case _Record_Ok_:
	      if (FALSE == iFind)
		iFind = TRUE;
	      memcpy ((char *) stPrev, (char *) stCurr,
		      sizeof (struct STRECORD));
	      memcpy ((char *) stCurr, (char *) &sttemp,
		      sizeof (struct STRECORD));
	      continue;

	    case _Record_Fail_:
	      continue;

	    case _Record_Null_:
	      if (TRUE == iFind)
		{
		  memcpy ((char *) stPrev, (char *) stCurr,
			  sizeof (struct STRECORD));
		  memcpy ((char *) stCurr, (char *) &sttemp,
			  sizeof (struct STRECORD));
		  return TRUE;
		}
	      else
		{

		  if (lAddr == stfilep->lStartAddr + i * stfilep->iBlockByte)
		    goto a1;

		}
	    }
	}
    a1:
    }
  if (FALSE == iFind)
    {
      Record_restore ((unsigned char *) stfilep->lStartAddr,
		      stfilep->iRecordLen, stCurr);
      Recode_CheckState (stCurr);
      return TRUE;
    }
  else
    {
      if ((i >= stfilep->iBlockMax) && (j >= _GetBlockRecordMax (stfilep)))
	{
	  memcpy ((char *) stPrev, (char *) stCurr, sizeof (struct STRECORD));
	  Record_restore ((unsigned char *) stfilep->lStartAddr,
			  stfilep->iRecordLen, stCurr);
	  Recode_CheckState (stCurr);
	  return TRUE;
	}

    }
  return FALSE;
}

unsigned char
BlockEarse (struct STFLASHFILESYSTEM *stfilep, unsigned long lAddr)
{
  unsigned short j;
  unsigned char ctemp;

  for (ctemp = FALSE, j = 0; ((ctemp == FALSE) && (j < cFlash_Eease_Retry));
       j++)
    {
      HT_Flash_PageErase (lAddr);	/*!< 擦除地址0x1FC00所在页，对HT60xx，即最后一页   */
      ctemp = Full_FF_Check ((unsigned char *) (lAddr), stfilep->iBlockByte);
    }
  if (j >= cFlash_Eease_Retry)
    return FALSE;
  return TRUE;
}

unsigned char
Record_Put (struct STFLASHFILESYSTEM *stfilep, unsigned char *stdata)
{
  unsigned char ctemp;
  struct STRECORD stRecordBuf[2];
  struct STRECORD *stCurr = &(stRecordBuf[0]);
  struct STRECORD *stPrev = &(stRecordBuf[1]);
  unsigned long lAddr, lAddr1, lBlockEnd;
  unsigned short i;
  ctemp = Record_Get_First (stfilep, stCurr);
  /*if (TRUE != ctemp)
     return FALSE;
   */

  lAddr = (unsigned long) stCurr->sData;

  //  检查地址是否在地址范围内
  for (i = 0, lBlockEnd = stfilep->lStartAddr; i < stfilep->iBlockMax;
       i++, lBlockEnd += stfilep->iBlockByte)
    {
      if ((lAddr >= lBlockEnd) && (lAddr < (lBlockEnd + stfilep->iBlockByte)))
	break;
    }
  if (i >= stfilep->iBlockMax)
    return FALSE;

  //  以此块为基础，是否发生块跨越
  if (1)			//if (  _Record_Null_!=  stCurr->cFlag )
    {
      lBlockEnd += stfilep->iBlockByte;
      lAddr1 = lAddr + stfilep->iRecordLen;
      if (stfilep->iRecordLen > (lBlockEnd - lAddr1))
	{
	  i++;
	  i = i % stfilep->iBlockMax;
	  lBlockEnd = stfilep->lStartAddr + i * stfilep->iBlockByte;
	  ctemp = BlockEarse (stfilep, lBlockEnd);
	  if (TRUE != ctemp)
	    return FALSE;
	  //lAddr = lBlockEnd;
	}
    }
  //   将外部数据格式化成存储结构
  stCurr->sData = stdata;
  stCurr->cLen = stfilep->iRecordLen;
  stCurr->iNo = stPrev->iNo;
  stCurr->iNo++;
  stCurr->iNo = stCurr->iNo % stfilep->iRecordMax;
  Record_Format (stCurr);


  HT_Flash_ByteWrite (stCurr->sData, lAddr,
		      stCurr->cLen - _Receord_iCheckData_Len_ -
		      _Receord_iNo_Len_);
  HT_Flash_ByteWrite ((const uint8_t *) &(stCurr->iNo),
		      lAddr + stCurr->cLen - _Receord_iCheckData_Len_ -
		      _Receord_iNo_Len_,
		      _Receord_iCheckData_Len_ + _Receord_iNo_Len_);
  return SUCCESS;
}

unsigned char
FileSystem_Init (struct STFLASHFILESYSTEM *stfilep)
{
  unsigned short i;
  unsigned long lAddr;
  unsigned char ctemp;

  for (i = 0; i < stfilep->iBlockMax; i++)
    {
      lAddr = stfilep->lStartAddr + i * stfilep->iBlockByte;
      ctemp = BlockEarse (stfilep, lAddr);
      if (TRUE != ctemp)
	return FALSE;
    }
  return TRUE;
}

char 		sEmuc_ResotreDef(void)
{
	 extern const struct STCHIPSET stChipdef;
		McuFlashRandWriteStack ( (unsigned long) (stflashdata.sEmuc), (char  *) &stChipdef,sizeof (stChipdef));
	 return TRUE;
}

char 		sFlashInfo(void)
{
	  extern const strFlashInfo DftFlashInfo;
	McuFlashRandWriteStack ( (unsigned long) (stflashdata.sFlashInfo), (char  *)&DftFlashInfo,sizeof (DftFlashInfo));
	 return TRUE;
}

void
System_Init (void)
{
 

  struct STFLASHFILESYSTEM *stfilep = &stDianLian;
  if (_IsFirstInit ())
    {
      FileSystem_Init (stfilep);
    //  BlockEarse (stfilep, (unsigned long) (stflashdata.sEmuc));
    //  BlockEarse (stfilep, (unsigned long) (stflashdata.sFlashInfo));
			
      //HT_Flash_ByteWrite ((const uint8_t *) &stChipdef,
			//  (unsigned long) (stflashdata.sEmuc),
			 // sizeof (stChipdef));
			sEmuc_ResotreDef();
     // HT_Flash_ByteWrite ((const uint8_t *) &DftFlashInfo,
		//	  (unsigned long) (stflashdata.sFlashInfo),
		//	  sizeof (DftFlashInfo));
		 	sFlashInfo();
      BlockEarse (stfilep, (unsigned long) (stflashdata.sInit));
    }
}


void
System_Init_app (void)
{

  struct STFLASHFILESYSTEM *stfilep = &stDianLian;

  {
    FileSystem_Init (stfilep);
    BlockEarse (stfilep, (unsigned long) (stflashdata.sInit));
  }

}
