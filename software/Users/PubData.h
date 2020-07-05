#ifndef		_PUB_DATA_
#define		_PUB_DATA_

 #define _RAM_ADDR_START_  0x20000000
 #define  _RAM_ZERO_END_	 0x20000100
     #define _RAM_ADDR_END_    0x20001800
		 
#define		_CODE_ADDR_			0x4000		//常量数据存储地址
#define		_FDATA_ADDR_		0x8000		//数据存储区地址
#define		RCODE(A)				(A)  //((uc8 *)(((u8*)(A))+ _CODE_ADDR_))

#define		_FLASH_CODE_SIZE_			0x20000	 				//程序空间(256k)

#define		_FLASH_DATA_MASK_			0x10000	//0x20000
//#define		_FLASH_DATA_START_			0x19000	//0x29000				//共24k数据区(4k*6)
//#define		_FLASH_PAGE_SIZE_			0x20					//数据存储块长度(512Byte)
//#define		_FLASH_PAGE_DWORDS_			0x80					//存储区双字数
//#define		_FLASH_BLOCK_SIZE_			0x1000					//数据块长度4k
//#define		_FLASH_DATA_BLOCKS_			7						//数据块数

#define		FLASH_INFO_START			0x60					//参数数据区
#define		FLASH_SAVE_START			0x64					//电量存储区
#define		FLASH_TBL_START				0x66					//时段费率表区
#define		FLASH_HLD_START				0x68					//假日表区
#define		FLASH_DISP_START			0x6A					//显示项区

#define		YTBL_START					0x00					//0x00第一套年时区表
																	//d
																	//0x01第二套年时区表
#define		TTBL_START					0x02					//0x02-0x09第一套时段表
																	//0x0A-0x11第二套时段表
#define		PTBL_START					0x12					//0x12第一套费率电价表
																	//0x13第二套费率电价表
#define		LTBL_START					0x14					//0x14第一套阶梯表
	/*																//0x15第二套阶梯表
typedef	struct {
	u16		FlashInfoNext;											//FlashInfo存储地址
	u16		pFLUse[_MAX_TARRIF_];									//费率电量存储地址
	u16		pFLTmp;													//当前费率Flash存储地址
	u16		LVSaveNext;												//LVSave存储地址
//	u16		pYTbl[2];												//年时区表起始地址
//	u16		pTmTbl[2][8];											//日时段表起始地址
	u16		pPriceTbl[2];											//电价表起始地址
	u16		pLTbl[2];												//阶梯表起始地址
	u16		TTblNext;												//时段费率电价表结束地址
	//u16 	pHld[2];					   //	u16 pHld[254];		//假日表地址
	//u16		pHldNext;
	//u16		pDisp[2][9];   				// u16	pDisp[2][99];		//显示项地址
	//u16		pDispNext;
	u16		pDispCardOver[4];										//插卡显示地址
} strGAddr;
extern strGAddr	GAddr;

typedef	struct {
	u32	StartAddr;
	u8	Data[_FLASH_PAGE_SIZE_];
	u16	Lth;
	u16	Lth_2;
	u32	Sum;
} strWFBuf;								//
extern	strWFBuf WFBuf;
*/
typedef struct {
	u8		StartCode;
	u8		SNum;
	u16		Rev;
	strSectUse	FLUse[_MAX_TARRIF_];
	strLVSave	LVSave;
} strLVSaveBlock;

typedef struct {
	u32	LUse[_MAX_LADDER_];				//阶梯电量表(24B)
	u8	LPrice[_MAX_LADDER_+1][3];		//阶梯电价表(21B)
	u8	YStepTm[4][3];					//年结算日(12B)
	u8	Rev[3];							//4字节对齐
} strLTbl;

#define		PRICE(a)		{ (a)&0xFF, (((a)>>8)&0xFF), ((((u32)(a))>>16)&0xFF) }	//电价存储格式
#define		YTB(a,b,c)		((((a)&0x0F)<<9)|((b&0x1F)<<4)|(c&0x0F))				//年时区表存储格式
#define		TTB(a,b,c)		(((((a)*60)+(b))<<5)|(c&0x1F))							//日时段表存储格式

typedef struct {
	u8		StartCode;
	u8		SNum;
	u16		Rev;						//4字节对齐
	u16		YearTbl[2][14];				//年时区表(2*28)dd
	u16		TmTbl[2][8][14];			//日时段表(2*8*28B)
	u8		Price[2][32][3];			//电价表(2*96B)
	strLTbl	LTBl[2];					//阶梯表(2*60B
	u32		Sum;						//校验和
} strTTBlBlock;

extern	const  u16 DftYTbl[14];
extern	const  u16 DftTmTbl[14];
extern	const  u8  DftPrice[32][3];
extern	const  strLTbl DftLTbl;

typedef struct {
	u8	HDay[2];
	u8	TblNum;
} strHolidayTbl;						//假日表结构





#endif		//_PUB_DATA_

