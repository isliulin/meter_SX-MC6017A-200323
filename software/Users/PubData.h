#ifndef		_PUB_DATA_
#define		_PUB_DATA_

 #define _RAM_ADDR_START_  0x20000000
 #define  _RAM_ZERO_END_	 0x20000100
     #define _RAM_ADDR_END_    0x20001800
		 
#define		_CODE_ADDR_			0x4000		//�������ݴ洢��ַ
#define		_FDATA_ADDR_		0x8000		//���ݴ洢����ַ
#define		RCODE(A)				(A)  //((uc8 *)(((u8*)(A))+ _CODE_ADDR_))

#define		_FLASH_CODE_SIZE_			0x20000	 				//����ռ�(256k)

#define		_FLASH_DATA_MASK_			0x10000	//0x20000
//#define		_FLASH_DATA_START_			0x19000	//0x29000				//��24k������(4k*6)
//#define		_FLASH_PAGE_SIZE_			0x20					//���ݴ洢�鳤��(512Byte)
//#define		_FLASH_PAGE_DWORDS_			0x80					//�洢��˫����
//#define		_FLASH_BLOCK_SIZE_			0x1000					//���ݿ鳤��4k
//#define		_FLASH_DATA_BLOCKS_			7						//���ݿ���

#define		FLASH_INFO_START			0x60					//����������
#define		FLASH_SAVE_START			0x64					//�����洢��
#define		FLASH_TBL_START				0x66					//ʱ�η��ʱ���
#define		FLASH_HLD_START				0x68					//���ձ���
#define		FLASH_DISP_START			0x6A					//��ʾ����

#define		YTBL_START					0x00					//0x00��һ����ʱ����
																	//d
																	//0x01�ڶ�����ʱ����
#define		TTBL_START					0x02					//0x02-0x09��һ��ʱ�α�
																	//0x0A-0x11�ڶ���ʱ�α�
#define		PTBL_START					0x12					//0x12��һ�׷��ʵ�۱�
																	//0x13�ڶ��׷��ʵ�۱�
#define		LTBL_START					0x14					//0x14��һ�׽��ݱ�
	/*																//0x15�ڶ��׽��ݱ�
typedef	struct {
	u16		FlashInfoNext;											//FlashInfo�洢��ַ
	u16		pFLUse[_MAX_TARRIF_];									//���ʵ����洢��ַ
	u16		pFLTmp;													//��ǰ����Flash�洢��ַ
	u16		LVSaveNext;												//LVSave�洢��ַ
//	u16		pYTbl[2];												//��ʱ������ʼ��ַ
//	u16		pTmTbl[2][8];											//��ʱ�α���ʼ��ַ
	u16		pPriceTbl[2];											//��۱���ʼ��ַ
	u16		pLTbl[2];												//���ݱ���ʼ��ַ
	u16		TTblNext;												//ʱ�η��ʵ�۱������ַ
	//u16 	pHld[2];					   //	u16 pHld[254];		//���ձ��ַ
	//u16		pHldNext;
	//u16		pDisp[2][9];   				// u16	pDisp[2][99];		//��ʾ���ַ
	//u16		pDispNext;
	u16		pDispCardOver[4];										//�忨��ʾ��ַ
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
	u32	LUse[_MAX_LADDER_];				//���ݵ�����(24B)
	u8	LPrice[_MAX_LADDER_+1][3];		//���ݵ�۱�(21B)
	u8	YStepTm[4][3];					//�������(12B)
	u8	Rev[3];							//4�ֽڶ���
} strLTbl;

#define		PRICE(a)		{ (a)&0xFF, (((a)>>8)&0xFF), ((((u32)(a))>>16)&0xFF) }	//��۴洢��ʽ
#define		YTB(a,b,c)		((((a)&0x0F)<<9)|((b&0x1F)<<4)|(c&0x0F))				//��ʱ����洢��ʽ
#define		TTB(a,b,c)		(((((a)*60)+(b))<<5)|(c&0x1F))							//��ʱ�α�洢��ʽ

typedef struct {
	u8		StartCode;
	u8		SNum;
	u16		Rev;						//4�ֽڶ���
	u16		YearTbl[2][14];				//��ʱ����(2*28)dd
	u16		TmTbl[2][8][14];			//��ʱ�α�(2*8*28B)
	u8		Price[2][32][3];			//��۱�(2*96B)
	strLTbl	LTBl[2];					//���ݱ�(2*60B
	u32		Sum;						//У���
} strTTBlBlock;

extern	const  u16 DftYTbl[14];
extern	const  u16 DftTmTbl[14];
extern	const  u8  DftPrice[32][3];
extern	const  strLTbl DftLTbl;

typedef struct {
	u8	HDay[2];
	u8	TblNum;
} strHolidayTbl;						//���ձ�ṹ





#endif		//_PUB_DATA_

