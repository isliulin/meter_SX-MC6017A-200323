#ifndef  _flash_filesystem_HPP
#define  _flash_filesystem_HPP

#include "ht6xxx.h"
//#include "datastruct.h"
//#include "unit_instance.h"
#include "system_ht6xxx.h"
#include "ht6xxx_lib.h"

#define		_iBlcokLen_	1024
struct STFLASHDATA
{
  char sInit[_iBlcokLen_];
  char sEmuc[_iBlcokLen_];
  char sFlashInfo[_iBlcokLen_];
  char sDIAN[_iBlcokLen_ * 6];
  char sFRoze1[_iBlcokLen_ * 2];
};

struct STFLASHFILESYSTEM
{
  unsigned long lStartAddr;	//              起始地址
  unsigned short iBlockByte;	//              每块长度
  unsigned short iBlockMax;	//              块数                    
  unsigned short iRecordLen;	//              每条记录长度
  unsigned short iRecordMax;	//              最大记录id
};

enum RECORDSTATU
{
  _Record_Null_ = 0,
  _Record_Fail_,
  _Record_Ok_
};

struct STRECORD
{
  unsigned char *sData;		//        实际数据
  unsigned char cLen;		// 实际有用数据,不包括iCheckData,iNo
  unsigned char cFlag;		//       记录状态        
  unsigned long lp;		//       存储地址
  unsigned short iNo;
  unsigned short iCheckData;
};


#define 		_Receord_cLen_Len_					sizeof(((struct		STRECORD *)0)->cLen)
#define 		_Receord_iCheckData_Len_		sizeof(((struct		STRECORD *)0)->iCheckData)
#define 		_Receord_iNo_Len_					sizeof(((struct		STRECORD *)0)->iNo)

//  工作结构中检查空长度
#define 		_Receord_CheckZero_Len(a)			((((struct		STRECORD *)a)->cLen))
#define 		_Receord_iNo_Len_							sizeof(((struct		STRECORD *)0)->iNo)


//  工作结构中计算校验和长度
#define 		_Receord_CRC_Len(a)					((((struct		STRECORD *)a)->cLen)-_Receord_iCheckData_Len_)

struct STRECORD_STORE
{
  unsigned char sData[1];
  //unsigned char                 cLen;           // 实际有有用数据
  //unsigned char                 cFlag;   
  unsigned short iNo;
  unsigned short iCheckData;
};

#define 		_ReceordStore_cLen_Len_					sizeof(((strucrt		STRECORD_STORE *)0)->cLen)
#define 		_ReceordStore_iCheckData_Len_		sizeof(((strucrt		STRECORD_STORE *)0)->iCheckData)
#define 		_ReceordStore_iNo_Len_					sizeof(((strucrt		STRECORD_STORE *)0)->iNo)

extern const struct STFLASHDATA stflashdata;
//       得到一块数据区存储记录条数
#define 		_GetBlockRecordMax(a)			(((struct 		STFLASHFILESYSTEM *)a)->iBlockByte / ((struct 		STFLASHFILESYSTEM *)a)->iRecordLen)
//      得到整个数据区存储记录的条数
#define 		_GetkRecordMax(a)					(_GetBlockRecordMax(a) * (struct 		STFLASHFILESYSTEM *)a->iBlockMax)
#define     _IsRecordiNo_Empty(a)     (0xffff ==( (struct  	STRECORD*)a)->iNo )

#define 	_cInit_Char_1_			0x55
#define 	_cInit_Char_2_			0xaa

#define 	_IsFirstInit()		((_cInit_Char_1_ == stflashdata.sInit[0]) && (_cInit_Char_2_ == stflashdata.sInit[1]))
void System_Init (void);
unsigned char Record_Put (struct STFLASHFILESYSTEM *stfilep,
			  unsigned char *stdata);

char 		sEmuc_ResotreDef(void);
char 		sFlashInfo(void);

#endif
