#ifndef		_PUB_SET_
#define		_PUB_SET_	1

#include <stdlib.h>
#include "metersystem.h"
#include "systickapp.h"
#include "ioset.h"
 #include "GetTimeCnt.h" 
#include "string.h"

//-----------------------------------------------------------------------------

//#include	"options.h" // Define the IC hardware, CE code and options.
//#include	"lcd.h"
//#include	"afe.h"
#include "globlvar_debug.h"
#include	"metersystem.h"
#include	<ctype.h>
#include	 <stdio.h>
#include	"SpSet.h"
#include	"gc_def.h"
#include "pubdata.h"
 
#include "gc_def.h"
#include "spset.h"
 


#define 		_COMDATA_LEN_		 512
#define 		__code

extern unsigned  long	lFullNull;
#define 		cFullNull		*((char *)&lFullNull)
#define 		_cFullNull_Len_		(sizeof(cFullNull)/sizeof(char))
#define 		_ISBootLoad()		(0x5a5aa5a5 == lFullNull )

struct 		ST20MSPLUSE
{
	unsigned short	iLowWidth;
	unsigned short  iFullWidth;
};

enum  ENUM20MSPHASE
{
	_20mspluse_Start_ = 0,
	_20mspluse_Middle_ = 1,
	_20mspluse_End_ = 2,
};

struct   STMEMTER_STATU
{
	u8		IMType[10];					//�������(F12F)
	u8		SoftVer[32];				//����汾��(F13F)
	u8		HardVer[32];				//Ӳ���汾��(F13F)
};

struct 		STDISPARRYDATA_BAK
{

	unsigned char cDispArrayNum[2];
	strDispCode	DArryNorm[10];
	strDispCode	DArryKey[40];
};

struct  		STJDPLUSE
{
	struct 		ST20MSPLUSE		st20mspluse[3];
	unsigned 	int		iHigh;				//	 20MS�����������ߵ�ƽʱ��ʱ��ֵ
	unsigned 	int		iLow;				//	 20MS�����������͵�ƽʱ��ʱ��ֵ
	unsigned 	int		iHighOld;			//	 20MS����������ʼ�ߵ͵�ƽ��ʱ��ֵ
	unsigned 	char	cjdpluse_no;
	unsigned 	char	cjdpluseTm; 		//	 ��λ��������ʱ
	unsigned    char	cJDDeteTm;			//		�����ʱ��һ��Ϊ30*
	unsigned   	char	cst20msplusep;		//	���20MS���嶨ʱ������
	enum  ENUM20MSPHASE  cPlusePhase ;
};


enum  ENUMMETERCONFIG
{
	_bMETTER_LOCAL_ = 0x00,
	_bMETTER_REMOTE_ = 0x01

};

enum  ENUMDATACONFIG
{
		_bData_Loc_ = (1<<_bMETTER_LOCAL_),
		_bData_Far_	= (1<<_bMETTER_REMOTE_),
		_bData_All_ = ((_bData_Loc_ )|(_bData_Far_)),
};

enum ENUMMETER_COMCONFIG
{
	_bMETER_ZB_ =0x33,
	_bMETER_485_ =0x44
};

enum ENUMMETER_JDMODE
{
	_bMeter_JD_IN_ =0x55,
	_bMeter_JD_OUT_ =0x66
};

struct 	STMETER_CONFIG
{
		char 	ctemp;
		char  	cMeter_Type;    //	enum  	ENUMMETERCONFIG			bMeter_Type:1;					// Զ�̱�ͱ��ر�����
		char 	cMeter_ComType;	//	enum 	ENUMMETER_COMCONFIG		bMeter_ComType:1;				//	�ز����485������
		char	cMeter_JDMODE;	//	enum 	ENUMMETER_JDMODE		bMeter_JDMODE:1;				//	բ��������

};

union UNMETERCONFIG
{
	unsigned char cMeterConfig[4] ; 	//	��������������
	struct 	STMETER_CONFIG  stmeter_config;
};




#define 		_cBAKDATA_COPYNO_		3
typedef struct
{
	GBit Bit0:1;
	GBit Bit1:1;
	GBit Bit2:1;
	GBit Bit3:1;
	GBit Bit4:1;
	GBit Bit5:1;
	GBit Bit6:1;
	GBit Bit7:1;
    GBit Bit8:1;
	GBit Bit9:1;
	GBit Bit10:1;
	GBit Bit11:1;
	GBit Bit12:1;
	GBit Bit13:1;
	GBit Bit14:1;
	GBit Bit15:1;
	GBit Bit16:1;
	GBit Bit17:1;
	GBit Bit18:1;
	GBit Bit19:1;
	GBit Bit20:1;
	GBit Bit21:1;
	GBit Bit22:1;
	GBit Bit23:1;
	GBit Bit24:1;
	GBit Bit25:1;
	GBit Bit26:1;
	GBit Bit27:1;
    GBit Bit28:1;
	GBit Bit29:1;
	GBit Bit30:1;
	GBit Bit31:1;
}strBit32;

			/*		USE DATA SET		*/
#define		TYPE_COM_ACT_USE	0x00
#define		TYPE_ACT_USE		0x01
#define		TYPE_REV_ACT_USE	0x02
#define		TYPE_ADD_USE		0x02

			/*		PHASE SET			*/
#define		TYPE_PHASE_A		0x04
#define		TYPE_PHASE_B		0x08
#define		TYPE_PHASE_C		0x0C
			/*		FL_SET		*/
#define		TYPE_FEILV_1		0x10
#define		TYPE_FEILV_2		0x20
#define		TYPE_FEILV_3		0x30
#define		TYPE_FEILV_4		0x40
#if		_MAX_TARRIF_ > 4
	#define		TYPE_FEILV_5	0x50
	#define		TYPE_FEILV_6	0x60
	#define		TYPE_FEILV_7	0x70
	#define		TYPE_FEILV_8	0x80
#endif

#define		_USE_V_ADDR_												//ʹ�������ַ

#define		_V_RAMDATA_ADDR_	0x0000
#define		_V_EEDATA_ADDR_		0x1000
#define		_V_COMDATA_ADDR_	0x2000
#define		_V_FILESET_ADDR_	0x3000


#define		vRamData			(*((strRamData *)_V_RAMDATA_ADDR_))
#define		vFlashInfo			(*((strFlashInfo *)_V_EEDATA_ADDR_))
#define		vComData			((u8*)_V_COMDATA_ADDR_)
#define		vFileSet			(*(strFileSet *)_V_FILESET_ADDR_)		//0x3400~0X3FFF

#define		EX_ADDR				0x4000

#define		_FILL_RAM_ADDR_		(EX_ADDR)								//0x4000
#define		_SET_RAM_2_ADDR_	(EX_ADDR+2)								//0x4002~0x4003
#define		_SET_RAM_3_ADDR_	(EX_ADDR+0x200)							//0x4200~0x43FF
#define		PUSE_DATA			(EX_ADDR+0x0400)						//0x4400
#define		PBASE_ADDR			(EX_ADDR+0x0800)						//0x4800
#define		_RET_				0x8000									//Return Code
#define		_REV_				0x4000									//Reverse Code

//#define		FILL_RAM( DATA,	L)		((u32)(((u32)0x4000<<16)|(((((u32)L)-1)&0xFF)<<8)|((DATA)&0xFF)))
#define		FILL_RAM( Data, L)					{	(u16)(EX_ADDR), (((L)&_RET_)|((((L)-1)&0x7F)<<8)|((Data)&0xFF)) }		//(0x2000)
#define		FILL_RAM_PBASE( Data, L)			{	(u16)(EX_ADDR+1), (((L)&_RET_)|((((L)-1)&0x7F)<<8)|((Data)&0xFF)) }		//(0x2000)
#define		SET_RAM_2( Data, Ret)				{	(u16)((EX_ADDR+2)|(((Data)>>15)&0x1)), ((Data)&0x7FFF)|((Ret)&_RET_) }		//(0x2002~0x2003)
#define		SET_RAM_3( Data, Ret)				{	(u16)((EX_ADDR+0x200)|(((Data)>>15)&0x1FF)), ((Data)&0x7FFF)|((Ret)&_RET_) }	//(0x2200~0x23FF)
#define		READ_USEDATA( TYPE, Opt)			{	(u16)(PUSE_DATA|((TYPE)&0xFF)), Opt }									//(0x2400~0x24FF)
#define		READ_RAM(Addr, Opt)					{	(u16)((u32)(Addr)), Opt }	// Addr == (0x2800~0x2FFF)

#define		READ_FROZE( Type, Cnt, Bias, Opt)	{	(void *)((EX_ADDR+0x1000)|((Type&0x3F)<<6)|(Bias&0x3F)), (((Cnt&0xF)<<10)|Opt) }	//0x3000~0x3FFF
#define		READ_645( Code, Ret )				{	(void *)((EX_ADDR+0x2000)|((Code>>15)&0x1FFF)), (((Code)&0x7FFF)|((Ret)&_RET_)) }				//0x6000~0x6FFF



		/*	Opt		*/
#define		COPY_L(LTH)							((LTH)&0x1F)										//Opt( 0~0x1F)
#define		REV_COPY_L(LTH)						(((LTH)&0x1F)|_REV_)								//Opt( 0~0x1F)
#define		READ_BCD_TIME(TM_BCD,TM_TP)			((0x30)|(((u16)TM_BCD)&3)|((u16)((TM_TP)&0x0C)))		//Opt( 0x30 ~ 0x3F)
#define		OUT_TIME(TM_TP, TM_BCD_TYPE)		((0x40)|(((u16)TM_TP)&3)|(((u16)TM_BCD_TYPE)&0x1C))	//Opt( 0x40 ~ 0x7F)

#define		READ_PBASE(vAddr, Opt)				{(u16)((PBASE_ADDR)+((vAddr)&0x7FF)), Opt }

#define		BCD_OUT(L1, L2, DOT)	(0x100|((((L1)-1)&0x3)<<4)|((((L2)-1)&3)<<2)|((DOT)&0x3))		//Opt( 0x100~0x13f)
#define		HEX_OUT(L1, L2,	DOT)	(0x140|((((L1)-1)&0x3)<<4)|((((L2)-1)&0x3)<<2)|((DOT)&3))	//Opt( 0x180~0x1BF)
#define		BCD_IN(L1, L2,	DOT)	(0x180|((((L1)-1)&0x3)<<4)|((((L2)-1)&3)<<2)|((DOT)&0x3))		//Opt( 0x140~0x17f)
#define		HEX_IN(L1, L2, DOT)		(0x1C0|((((L1)-1)&0x3)<<4)|((((L2)-1)&0x3)<<2)|((DOT)&3))	//Opt( 0x1C0~0x1FF)

#define		NORM_COPY( Addr, LTH)		{	(u16)((u32)(Addr)), ((LTH&_RET_)|((LTH)&0x1F)) }
#define		REV_COPY( Addr, LTH)		{	(u16)((u32)(Addr)), ((LTH&_RET_)|_REV_|((LTH)&0x1F)) }
#define		SET_PCHAR(Addr, Opt)		{	(u16)((u32)(Addr)), 0x20|((Opt)&_RET_) }
#define		SET_PBASE(Addr, Opt)		{	(u16)((u32)(Addr)), 0x21|((Opt)&_RET_) }
#define		REV_LST(Addr, LTH, REPEAT)	{	(u16)((u32)(Addr)), ((0x80)|((((REPEAT)-1)&0x07)<<2)|(((LTH)-1)&3)|((REPEAT)&_RET_)) }		//Opt( 0x80 ~ 0x9F)

/*
#define		S_SEC		0
#define		S_MIN		1
#define		S_DAY		2
#define		S_RTC		3

#define		T_SEC		0x0
#define		T_MIN		0x4
#define		T_DAY		0x8

#define		T_YMDHMS	0x0		//
#define		T_HMS		0x4		//
#define		T_YMDHM		0x8		//
#define		T_YMDW		0xC		//
*/
#define		MV_DATA(Addr)			( (u32)(((u32)(Addr)) + ((u32)0x20E<<16)) )

#define		_LTH_3_		0
#define		_LTH_4_		0x40
#define		_LTH_5_		0x80
#define		_LTH_6_		0xC0

enum  ENUMPROG
{
	_prog_null_ = 0,
	_prog_set_ =1,
};


typedef	struct {
	u16	Code;
	u8	DI0_Max;
	u8	Safe;			//Safe Level
	struct {
		unsigned int	Lth;		//645 Length
		unsigned int	Dir:1;		//Data Direct
		unsigned int	HexBCD:2;	//Hex/BCD	(0-HEX, 1-BCDתHEX, 2 - ��HEX, 3-��BCD(ֻ��鲻ת��))
		unsigned int	Rol:2;		//Rol Left Bits
		unsigned int	ProgRcd:1;	//Add Program Recode
		unsigned int	FeilvChg:1;	//
		enum  ENUMPROG	ReadProg:1;	//Read Data have a subroutine
		enum  ENUMPROG	WriteProg:1;//Write Data have a subroutine
	} BitSet;
	u16	DispSet;
	u8 *pAddr;
	u32  MinMax ;//u8 (*MinMax)(strDispCode const *pCode, u8* pAddr);
	ucode	*pBitLst;
	enum  ENUMDATACONFIG  cMod;			//	Զ�̡����س���
} strWriteCode;							//645 RW Code Lth 40 Bytes

typedef struct {
	u16	Code;
	u8	DI0_Max;
	u8	FType;
	u8	ZeroLth;						//Zero Lth if No Record
	void (*DoProg)(strDispCode const *pCode, u8 FType);
	enum  ENUMDATACONFIG  cMod;			//	Զ�̡����س���
} strRcdLst;							//645 Froze Code

typedef struct {
	u16		TRxTm;						//
	u16		CSTime;						//
	GBit	Act:2;						//��ǰ״̬(0-�ȴ�����,1-����,2-�ȴ�����,3-����
	GBit	AddrMode:3;					//��ַ״̬(0-������ַ,1-AAƥ���ַ,2-�㲥��ַ,3-У���ַ
#define		_ADD_LOC_		0
#define		_ADDR_99_		2
#define		_ADDR_6162_		3
#define   _broadcast99Addr     ( _ADDR_99_ == Ex645.SCIx->AddrMode)	// ȫ9��ַ
#define   _localAddr           ( _ADD_LOC_ == Ex645.SCIx->AddrMode)	// ���ص�ַ
	GBit	AddrAct:1;						//��ַƥ��(0-δƥ�� 1-ƥ��)
	GBit	TRxEvt:1;						//�����¼�����
	GBit	NoRet:1;						//�޷���
	u8		Head;						//
	u8		SendLth;					//
	u8		Rev;						//4�ֽڶ���
	u8		*DataBuf;//[220]; 						//���֧��220�ֽڳ�������
}strSCIBuf;


#define		_HAVE_NEXT_			0
#define		_LOCAL_CARD_		0
#define		_DISP_NUM_			0
#define		_USE_DOTS_			0
#define		_DISP_TARIFF_		0
#define		_DISP_STEP_			0

typedef	struct {
	u32	DispData[(_DISP_OVER_/sizeof(u32)+1)*sizeof(u32)];								//DispData(256BIT)
	u8	BitLst[32];									//
	strDispCode	DispCode;							//
	u16	DispSet;									//
	u8	NextTm;
	u8	OffMode;									//���紦��ģʽ
	u8	DispStart;									//��ʾ����ʼ���
//	strDispCode*	pDispArry;
	u8	DispArryLength;
	u8	DispArryNum;
//	u8	DispArrySubNum;
	u8	SubItem;
	u8	SubDispTm;
	u8 	cDispArry;				//		��ʾ�������ָ��
} strDisp;

/*
#ifdef	_USE_CARD_
	#define		_FROZE_NUM_			37		//
#else
	#define		_FROZE_NUM_			(37-7)
#endif
*/
enum {
	//_FROZE_FUHE_,								//���ɼ�¼(ʱ��4/��ѹ2/����4/Ƶ��2)
	//_FROZE_KEYDATA_,							//�޸���Կ����
	_FROZE_MONTH_,								//����������
//	_FROZE_MSTEP_,								//�����ڽ�������
//	_FROZE_YSTEP_,								//�����ڽ�������
//	_FROZE_YTBL_CHG_,							//��ʱ�����л���¼
//	_FROZE_TMTBL_CHG_,							//��ʱ�α��л���¼
#ifdef	_USE_CARD_
	_FROZE_FL_PRICE_CHG_,						//���ʵ���л���¼
	_FROZE_STEP_PRICE_CHG_,						//���ݱ��л���¼
	_FROZE_CHARGE_,								//�����¼
	_FROZE_OFF_MONEY_,							//�˷Ѽ�¼
	_FROZE_IC_ERR_,								//�쳣�忨��¼
	_FROZE_TPRICE_SET_,							//���ʵ���޸ļ�¼
	_FROZE_LPRICE_SET_,							//���ݱ��޸ļ�¼
#endif
	_FROZE_ACT_,								//˲ʱ�����¼
	_FROZE_TM_,									//��ʱ/Լ������
	_FROZE_DAY_,								//�ն���
	_FROZE_HOUR_,								//���㶳��
//	_FROZE_KEYCHG_,								//��Կ���¼�¼
//	_FROZE_EK_1_,								//���1
	//_FROZE_JD_,									//����բ��¼
	//_FROZE_PROG_,								//��̼�¼
	_FROZE_CLEAR_,								//�����¼
	//_FROZE_EVENT_CLEAR_,						//�¼������¼
	_FROZE_CHG_TM_,
	//_FROZE_CHG_TM_,								//ʱ���޸ļ�¼
	//_FROZE_TMTBL_SET_,							//��ʱ�α��޸ļ�¼
	//_FROZE_YTBL_SET_,							//��ʱ�����޸ļ�¼
	//_FROZE_WEEK_SET_,							//�������޸ļ�¼
	//_FROZE_HLD_SET_,							//���ձ��޸ļ�¼
	//_FROZE_LSTEP_SET_,							//���ݱ��޸ļ�¼
	//_FROZE_COM_SET_,							//�й�����������޸ļ�¼
	//_FROZE_FDT_SET_,							//������ʱ�޸ļ�¼
	//_FROZE_OVERIM_,								//������¼
	//_FROZE_PERR_,								//��Դ�쳣��¼
	//_FROZE_JDERR_,								//���ɿ����쳣��¼
	_FROZE_PWR_,								//�����¼
	_FROZE_N_1_	,								//����
	_FROZE_NUM_,  //=====================
	_FROZE_FUHE_,								//���ɼ�¼(ʱ��4/��ѹ2/����4/Ƶ��2)
	_FROZE_KEYDATA_,							//�޸���Կ����
	//_FROZE_MONTH_,								//����������
	_FROZE_MSTEP_,								//�����ڽ�������
	_FROZE_YSTEP_,								//�����ڽ�������
	_FROZE_YTBL_CHG_,							//��ʱ�����л���¼
	_FROZE_TMTBL_CHG_,							//��ʱ�α��л���¼
		_FROZE_KEYCHG_,								//��Կ���¼�¼
	_FROZE_EK_1_,								//���1
	_FROZE_JD_,									//����բ��¼
	_FROZE_PROG_,								//��̼�¼
	//_FROZE_CLEAR_,								//�����¼
	_FROZE_EVENT_CLEAR_,						//�¼������¼
	//_FROZE_CHG_TM_,
	//_FROZE_CHG_TM_,								//ʱ���޸ļ�¼
	_FROZE_TMTBL_SET_,							//��ʱ�α��޸ļ�¼
	_FROZE_YTBL_SET_,							//��ʱ�����޸ļ�¼
	_FROZE_WEEK_SET_,							//�������޸ļ�¼
	_FROZE_HLD_SET_,							//���ձ��޸ļ�¼
	_FROZE_LSTEP_SET_,							//���ݱ��޸ļ�¼
	_FROZE_COM_SET_,							//�й�����������޸ļ�¼
	_FROZE_FDT_SET_,							//������ʱ�޸ļ�¼
	_FROZE_OVERIM_,								//������¼
	_FROZE_PERR_,								//��Դ�쳣��¼
	_FROZE_JDERR_,								//���ɿ����쳣��¼

};
#define		_FROZE_EOPEN_1_			(0x40|_FROZE_EK_1_)
#define		_FROZE_JD_OFF_			(0x40|_FROZE_JD_)
#define		_FROZE_PWR_OFF_			(0x40|_FROZE_PWR_)
#define		_FROZE_PERR_START_		(0x40|_FROZE_PERR_)
#define		_FROZE_JDERR_START_		(0x40|_FROZE_JDERR_)
#define		_FROZE_OVERIM_START_	(0x40|_FROZE_OVERIM_)

#define		_FROZE_ECLOSE_1_		(0x80|_FROZE_EK_1_)
#define		_FROZE_JD_ON_			(0x80|_FROZE_JD_)
#define		_FROZE_PWR_ON_			(0x80|_FROZE_PWR_)
#define		_FROZE_PERR_END_		(0x80|_FROZE_PERR_)
#define		_FROZE_JDERR_END_		(0x80|_FROZE_JDERR_)
#define		_FROZE_OVERIM_END_		(0x80|_FROZE_OVERIM_)

#define		_FH_BLOCKS_		  16
#define		_FH_BLTH_		512
#define     _cEvnCnt_Len_			1 //64					//�����ϱ����� 64
typedef struct {
	u32	FHTime;
	u16	FHAddr;
} strFH;

typedef struct {
	u32	FHStartTm[_FH_BLOCKS_+1];
	strFH	LastFH[5];
	u16	NextFH;
} strFHSet;

typedef	struct {
	//u8	FzFuhe[_FH_BLOCKS_][_FH_BLTH_];			//_FROZE_FUHE_				//���ɼ�¼(ʱ��4/��ѹ2/����4/Ƶ��2)
	//u8	FKData[32][36];							//_FROZE_KEYDATA_           //�޸���Կ����
	u8	FMouth[3][16+(_MAX_TARRIF_*12)];		//_FROZE_MONTH_             //����������
	//u8	FMStep[2][4];							//_FROZE_MSTEP_             //�����ڽ�������
	//u8	FYStep[4][8];							//_FROZE_YSTEP_             //�����ڽ�������
	//u8	FYTabl[2][16+(_MAX_TARRIF_*8)];			//_FROZE_YTBL_CHG_          //��ʱ�����л���¼
	//u8	FTmTbl[2][16+(_MAX_TARRIF_*8)];			//_FROZE_TMTBL_CHG_         //��ʱ�α��л���¼
#ifdef	_USE_CARD_
	u8	FPrice[2][16+(_MAX_TARRIF_*8)];			//_FROZE_FL_PRICE_CHG_      //���ʵ���л���¼
	u8	FLStep[2][16+(_MAX_TARRIF_*8)];			//_FROZE_STEP_PRICE_CHG_    //���ݱ��л���¼
	u8	FCharg[20][24];							//_FROZE_CHARGE_            //�����¼
	u8	FDsChg[10][20];							//_FROZE_OFF_MONEY_         //�˷Ѽ�¼
	u8	FICErr[10][34];							//_FROZE_IC_ERR_            //�쳣�忨��¼
	u8	FPSet1[10][104];						//_FROZE_TPRICE_SET_        //���ʵ���޸ļ�¼(��¼16���)
	u8	FLPSet[10][124];//[124];				//_FROZE_LPRICE_SET_        //���ݱ��޸ļ�¼
#endif
	u8	FFzAct[3][16+(_MAX_TARRIF_*8)];			//_FROZE_ACT_               //˲ʱ�����¼
	u8	FFTime[2][16+(_MAX_TARRIF_*8)];		//_FROZE_TM_                //��ʱ/Լ������
	u8	FFzDay[10][8+16+(_MAX_TARRIF_*8)];		//_FROZE_DAY_               //�ն���
	u8	FFHour[8][12];						//_FROZE_HOUR_              //���㶳��
	//u8	FKyChg[10][16];							//_FROZE_KEYCHG_            //��Կ���¼�¼
	//u8	FEOpn1[20][12];							//_FROZE_EK_1_,             //���1
	//u8	FJDOff[20][16];							//_FROZE_JD_,               //����բ��¼
	//u8	FFProg[10][48];							//_FROZE_PROG_,             //��̼�¼
	u8	FClear[3][16];							//_FROZE_CLEAR_,            //�����¼
	//u8	FEvClr[10][12];							//_FROZE_EVENT_CLEAR_,      //�¼������¼
	u8	FChgTm[10][12];							//_FROZE_CHG_TM_,           //ʱ���޸ļ�¼
	//u8	FTTbCg[10][40];							//_FROZE_TMTBL_SET_,        //��ʱ�α��޸ļ�¼
	//u8	FYTbCg[10][40];							//_FROZE_YTBL_SET_,         //��ʱ�����޸ļ�¼
	//u8	FWkSet[10][12];							//_FROZE_WEEK_SET_,         //�������޸ļ�¼
	//u8	FHldSt[10][12];							//_FROZE_HLD_SET_,          //���ձ��޸ļ�¼
	//u8	FLSSet[10][24];							//_FROZE_LSTEP_SET_,        //���ݱ��޸ļ�¼
	//u8	FCmSet[10][12];							//_FROZE_COM_SET_,          //�й�����������޸ļ�¼
	//u8	FFrzDT[10][12];							//_FROZE_FDT_SET_,          //������ʱ�޸ļ�¼
	//u8	FOvAmp[20][24];							//_FROZE_OVERIM_,		    //������¼
	//u8	FPrErr[20][12];							//_FROZE_PERR_,             //��Դ�쳣��¼
	//u8	FJDErr[20][12];							//_FROZE_JDERR_,            //���ɿ����쳣��¼
	u8	FPwOff[20][4];							//_FROZE_PWR_,              //�����¼
	u8	FNUse1[2][4];							//_FROZE_N_1_,              //����
} strExFrozeData;				//

#define	vExFrozeData	(*((strExFrozeData *)0))
#define	_FRZ_LTH_(a)	(sizeof(vExFrozeData.a[0]))

#define			DATA_FLASH_PAGE_SIZE			128

#define	_F_FRZ_LTH_(a)	((u16)(((u16)(sizeof(vExFrozeData.a))+(DATA_FLASH_PAGE_SIZE*2)-1)&(~(DATA_FLASH_PAGE_SIZE-1))))
typedef	struct {				//
	//u8	FzFuhe[_F_FRZ_LTH_(FzFuhe)];			//_FROZE_FUHE_
	//u8	FKData[_F_FRZ_LTH_(FKData)];			//_FROZE_KEYDATA_
	u8	FMouth[_F_FRZ_LTH_(FMouth)];			//_FROZE_MONTH_
	//u8	FMStep[_F_FRZ_LTH_(FMStep)];			//_FROZE_MSTEP_
	//u8	FYStep[_F_FRZ_LTH_(FYStep)];			//_FROZE_YSTEP_
	//u8	FYTabl[_F_FRZ_LTH_(FYTabl)];			//_FROZE_YTBL_CHG_
	//u8	FTmTbl[_F_FRZ_LTH_(FTmTbl)];			//_FROZE_TMTBL_CHG_
#ifdef	_USE_CARD_
	u8	FPrice[_F_FRZ_LTH_(FPrice)];			//_FROZE_FL_PRICE_CHG_
	u8	FLStep[_F_FRZ_LTH_(FLStep)];			//_FROZE_STEP_PRICE_CHG_
	u8	FCharg[_F_FRZ_LTH_(FCharg)];			//_FROZE_CHARGE_
	u8	FDsChg[_F_FRZ_LTH_(FDsChg)];			//_FROZE_OFF_MONEY_
	u8	FICErr[_F_FRZ_LTH_(FICErr)];			//_FROZE_IC_ERR_
	u8	FPSet1[_F_FRZ_LTH_(FPSet1)];			//_FROZE_TPRICE_SET_1_,
	u8	FLPSet[_F_FRZ_LTH_(FLPSet)];			//_FROZE_LPRICE_SET_,
#endif
	u8	FFzAct[_F_FRZ_LTH_(FFzAct)];			//_FROZE_ACT_               //˲ʱ�����¼
	u8	FFTime[_F_FRZ_LTH_(FFTime)];			//_FROZE_TM_                //��ʱ/Լ������
	u8	FFzDay[_F_FRZ_LTH_(FFzDay)];			//_FROZE_DAY_               //�ն���
	u8	FFHour[_F_FRZ_LTH_(FFHour)];			//_FROZE_HOUR_              //���㶳��
	//u8	FKyChg[_F_FRZ_LTH_(FKyChg)];			//_FROZE_KEYCHG_            //��Կ���¼�¼
	//u8	FEOpn1[_F_FRZ_LTH_(FEOpn1)];			//_FROZE_EK_1_,             //���1
//	u8	FJDOff[_F_FRZ_LTH_(FJDOff)];			//_FROZE_JD_,               //����բ��¼
	//u8	FFProg[_F_FRZ_LTH_(FFProg)];			//_FROZE_PROG_,             //��̼�¼
	u8	FClear[_F_FRZ_LTH_(FClear)];			//_FROZE_CLEAR_,            //�����¼
	//u8	FEvClr[_F_FRZ_LTH_(FEvClr)];			//_FROZE_EVENT_CLEAR_,      //�¼������¼
	//u8	FChgTm[_F_FRZ_LTH_(FChgTm)];			//_FROZE_CHG_TM_,           //ʱ���޸ļ�¼
	//u8	FTTbCg[_F_FRZ_LTH_(FTTbCg)];			//_FROZE_TMTBL_SET_,        //��ʱ�α��޸ļ�¼
	//u8	FYTbCg[_F_FRZ_LTH_(FYTbCg)];			//_FROZE_YTBL_SET_,         //��ʱ�����޸ļ�¼
	//u8	FWkSet[_F_FRZ_LTH_(FWkSet)];			//_FROZE_WEEK_SET_,         //�������޸ļ�¼
	//u8	FHldSt[_F_FRZ_LTH_(FHldSt)];			//_FROZE_HLD_SET_,          //���ձ��޸ļ�¼
	//u8	FLSSet[_F_FRZ_LTH_(FLSSet)];			//_FROZE_LSTEP_SET_,        //���ݱ��޸ļ�¼
	//u8	FCmSet[_F_FRZ_LTH_(FCmSet)];			//_FROZE_COM_SET_,          //�й�����������޸ļ�¼
	//u8	FFrzDT[_F_FRZ_LTH_(FFrzDT)];			//_FROZE_FDT_SET_,          //������ʱ�޸ļ�¼
	//u8	FOvAmp[_F_FRZ_LTH_(FOvAmp)];			//_FROZE_OVERIM_,		    //������¼
	//u8	FPrErr[_F_FRZ_LTH_(FPrErr)];			//_FROZE_PERR_,             //��Դ�쳣��¼
	//u8	FJDErr[_F_FRZ_LTH_(FJDErr)];			//_FROZE_JDERR_,            //���ɿ����쳣��¼
	u8	FPwOff[_F_FRZ_LTH_(FPwOff)];			//_FROZE_PWR_,              //�����¼
	u8	FNUse1[_F_FRZ_LTH_(FNUse1)];			//_FROZE_N_1_,              //����
} strFlashFrozeData;				//
#define	vFlashFrozeData	(*((strFlashFrozeData *)0))


typedef	struct {
	GBit	Bias:15;					//
	GBit	Length:7;					//
	GBit	MinCnt:10;					//
	u16	pFrozeData;						//
} strFrozeSet;

typedef struct {
    #include "FileSet.h"
} strFileSet;

/*		Use Flash Struct				*/
//	#define	_FRZ_BIAS_(a)	((u16)(((u8*)&vFlashFrozeData.a) - ((u8*)&vFlashFrozeData)))
//	#define	_FRZ_MINCNT_(a)	(sizeof(vFlashFrozeData.a)/sizeof(vExFrozeData.a[0]))
//	#define	_ALL_FROZE_LENGTH_	(sizeof(vFlashFrozeData))		//All FrozeData Length
/*		Use EEPROM struct				*/
	#define	_FRZ_BIAS_(a)	((u16)((u32)&vExFrozeData.a - (u32)&vExFrozeData))
	#define	_FRZ_MINCNT_(a)	(sizeof(vExFrozeData.a)/sizeof(vExFrozeData.a[0]))
	#define	_ALL_FROZE_LENGTH_	(sizeof(vExFrozeData))			//All FrozeData Length

struct   STDIANLIAN
{
	strLVSave	LVSave;				 		//��ǰ
	strSectUse	FLUse[_MAX_TARRIF_];		//�ܹ�36Byte
};
#define 		_cElectric_Buflen_		16

typedef struct {
	u32		VI_Lst[_cElectric_Buflen_];
	u8		VI_Cnt;
	u32		VI;								//ƽ������
	s32		sVI;							//ʵʱ����
} strVI;

typedef	struct {
	u32		FastData;
	strSCIBuf *SCIx;						//��ǰ����SCIָ��
	u8		Frm645;							//645֡��
	u8		Frm645Read;						//645����֡
	u8		ExLth;							//��չ����(����645���ݷ������)
	strDispCode	Code645;
    u8		WBit;							//645д��־
} strEx645;

typedef	struct {
	u32			PwrOnCode;					//PwrOn Key Code 52707000 or 70005270
	u8			Buf645[64];					//645ͨѶ��ʱ����
	u8			FrozeBuf[140];				//�������ݶ�ȡ������
	//u8			FlashBuf[256];				//Flash���ݴ洢ʹ��
	//u8			MacBuf[280];				//Mac��֤��ʱ����
	u32			TestData;					//����������
	strSectUse*	pFLUse[_MAX_TARRIF_];		//�����õ�ָ��
	//strLVSave	LVSave[_cBAKDATA_COPYNO_];					//
	strLVSave	 *LVSave;
	struct   STDIANLIAN 	stdianlian[_cBAKDATA_COPYNO_];
	u16			EvtClrTm;					//�������/�¼������־
	_tyTIMECNT			RTCTime;					//RTC Data;
	u32			LastMin;					//RTC LastMin
	u32			LastRTC;					//Last RTC
	u32			xLastRTC;					//Last RevRTC
	u32			LastKeySec;					//
	u8			RTCSec;						//RTC Sec
	//u8			JD_Out;						//JD OutState;
	strDisp		Disp;						//Disp_Data
	vu16		msTime;						//MsCnt	Increase Per 5ms, No Zero State

				/*		�������ݸ�λ������			*/
	u8			HSecCnt;					//
	u8			Ms_Cnt;						//
	vu8			Tm_ms;						//
	vu8			WDGTm;						//
	u8			Tm_5ms;						//
	u8			Tm_10;
	//vu32		IntFlagEvent;				//IntFlagEvent can be changed in Interrupt
	unsigned char	sFlagEvent[4];
#define		Event_Ms			 RamData.sFlagEvent[0]  //(*((vu8*)&RamData.IntFlagEvent))
#define		Event_HalfSec		 bInt_HalfSec	 //RamData.sFlagEvent[1]  //(*(((vu8*)&RamData.IntFlagEvent)+1))
//#define		Event_USART			RamData.sFlagEvent[2]  //(*(((vu8*)&RamData.IntFlagEvent)+2))
#define		Event_Sec			bInt_SecBit   //RamData.sFlagEvent[3]  //(*(((vu8*)&RamData.IntFlagEvent)+3))
	GBit		Event_LoPwr:1;				//��ѹ�¼�
	GBit		Event_PwrOff:1;				//�����¼�����
	GBit		Event_Min:1;				//�����¼�
	GBit		Event_Feilv:1;				//�����л��¼�
	GBit		Event_LVFull:1;				//����洢�����¼�
	GBit		Event_Key:1;				//�����¼�
	GBit		Event_IRKey:1;				//���ⰴ���¼�
	GBit		Event_EOpen:1;				//���Ǽ���¼�
	GBit		Event_CardIn:1;				//�忨�¼�
	GBit		Event_Season:1;				//�����¼�
	GBit		Event_TmDelay:1;			//��ʱ��־
	GBit		Event_ICKK:1;				//�̵�������ж�
	GBit		Event_KeyChg:1;				//�޸���Կ�¼�


	GBit		PwrUpFrozeOver:1;			//�ϵ粹��������־
	GBit		initEnd:1;
    GBit		InMsTask:1;
	GBit		Pwr_On:1;					//Pow On State is 1
	GBit		SP_Mode:1;					//Sec Out Mode
	GBit		Is_Card:1;					//Set to 1 when RW Card
	GBit		Use_Neg:1;					//Neg Flag
	GBit		PwrOn_Disp:1;				//PwrOn Disp Flag
	GBit		Disp_State:2;				//Disp State
	GBit		Disp_Blink:1;				//
	GBit		Disp_BinkState:1;			//
	GBit		Next_JD:1;					//
	GBit		JD_TestState:1;				//
	GBit		Evt_JD:1;					//
	GBit		Evt_JDClr:1;				//�����̵�����������բ�¼�
	GBit		EOpen_PwrOff:1;				//
	GBit		FrozeTmChg:1;				//
	GBit		MToYFroze:1;				//
	GBit		ChipImpChange:1;
    GBit		InDisp:1;
    GBit		exti_h:1;
    GBit		oldKey:1;
    GBit		keySt:1;
    GBit		Evt_Rev:1;					//
	GBit		Alm_State:3;				//(0-Norm 1-Alm, 2-AlmOff,	3-OverDraft, 4-OverDraftOff
	GBit		Tm_Err:1;						//ʱ�Ӵ���


	GBit		I2C_Err:1;					//I2C����
	GBit		I2C_No_Err:1;				//����I2C����(������ʾ)
	GBit		TRx_Wait:1;					//ͨѶ�ȴ�


 	GBit		Use_Dir:1;					//��ǰ��������
	GBit		Use_Act:1;					//������Ч��־
	GBit		Use_Ch:1;					//��ǰ����ͨ��
	GBit		Volt_Err:1;					//
	GBit		Alm_Chg:1;					//
	GBit		ZB_Busy:1;					//
	GBit		YTm_Chg:1;					//
	GBit		ForceCal:1;					//
	GBit		LoBatRechg:1;				//
	GBit		ZBBit:1;					//�ز�����״̬
	GBit		Read_FH:1;					//
	GBit		Use_Step:1;					//
	GBit		FacMode:1;					//
	GBit		VoltUp:1;					//��ѹ������־
	GBit		LoVolt:1;					//�͵�ѹ��־
	GBit		Key_State:1;				//
	GBit		Last_Key:1;					//
	GBit		EOpen_State:1;

	GBit		CF_State:1;					//��������״̬
	GBit		Lo_Pwr:1;					//�͵�ѹ״̬
	GBit		CF_Rev:1;					//����״̬

	GBit		Use_Chg:1;					//
	GBit		Card_In:1;					//
	GBit		Wr_Card:1;					//
	GBit		Mac_99:1;					//
	GBit		Mac_98:1;					//
	GBit		Read_RP:1;					//
	GBit		Imp_Busy:1;					//
	GBit		Read_F0:1;					//
	GBit		Read_F1:1;					//
	GBit		noUse:1;					//
/*
	u8			mSec;						//ʵʱ1/256��
	u8			JDTm;						//�̵���������ʱ
	u8			JDCmdTm;					//�̵�����������ʱ
	u8			JDErrTm;					//�̵���������ʱ
	u8			JDOKTm;						//�̵�����ȷ��ʱ
	u8			JD_TestCnt;
	s8			TBatErrCnt;					//�¶ȵ�ص�ѹ���������
	u8			BatChkCnt;
	u8			BeepMode;

	
	
	u8			PwrOffDelay;
	
	u8			BatChkMode;

	
	u8			Key_Cnt;

	

	u8			ICKK_OffTm;					//�ź�ֹͣ��ʱ
	u8			ICKK_OnTm;					//�źŷ�����ʱ
*/
u8			ErrImpTm;
	u8			UseChgTm;
u8			LastTblAct;
u8			FarSubTm;
	u8			PulseTm;
u8			LastDayYTbl;				//
	u8			NotChg;
		u8			EOpen_Cnt;
	unsigned long		lDianL180;
	unsigned long		lDianL280;
	u8			Card_InCnt;
	u8			ESAMErrCnt;
	u16			KeyDnCnt;
	u8			KeyDispCnt;					//
	u8			BkltTm;						//Back Light Time
	u8			TDispTm;					//

//appopt	u8			IRKeyTm;
//appopt	u8			IRKeyCnt;
 		u8			IRDispCnt;					//
	//appopt	u8			MDBusyTm;					//
 	u8			TRxNumChgTm;
	u8			FacProgTime;				//

	u8			CF_Buf;						//�洢����״̬
	u8			ImpBfr;						//�й�������
	u8			OldImpBfr;

	//appopt	u16			ImpDelay;
	//appopt	u16			LastJDVolt;
	//appopt	u16			BeepTm;
	 	u16			ErrImp;
 	u16			LastDay;					//

	 	u32			PwrOnTime;					//�ϵ�ʱ��
 	u32			LastFrozeTime;				//
	u32			FrozeTime;					//

	u8			PwrUpCnt;
	u8			TmErrCnt;
	u8			RTCErrTm;					//ʱ�Ӵ������
	u8			DelayRst;
	u8			DelayTmChg;
	u8			DispErr;
	u8			LastFType;					//LastFrozeType+1
	u8			LastFCnt;					//LastFrozeCnt



	u8			RevTmCnt;					//������ʱ
	u8			RevPwrCnt;					//���������¼��ʱ
#ifdef _OPT

	u8			OverVoltTm;					//��ѹ��ʱʱ��
	u8			OverPwrTm;					//���ش�����ʱʱ��
	u8			OverPhTm;					//�������������ж�ʱ��
	u8			OverIphTm;					//���������ж�ʱ��
	u8			LoHVPwrTm;					//�͵�ѹ�����¼��ʱ

#endif
	u32			RunState[4];
	u16			CardInState;

	s32		s_Remain;
	u32		u_Remain;
	u32		u_OverDraft;
	s32		Last_Remain;				//


			/*		��ͨѶ����			*/
#define			_ERR_CARD_				0x80		//�Ƿ��忨
#define			_ERR_NCMD_				0x40		//������ͷ
	u8		ICECode;					//�忨������Ϣ��
	u8		ICECmd[5];					//�忨��������ͷ
	u16		Return_7816;				//������������

	strFHSet	FHSet;					//��������
	u32		RdFHTime;					//���ɶ�ȡʱ��
	u8		RdFHNum;					//���ɶ�ȡ��ʼ����
	u8		RdFHBlks;					//���ɶ�ȡ����

/*			Meter Run Data			*/
	s16		Pfph;						//
	u32		Varh;						//���ڹ���(4λС��)
	strVI	Pph;						//����(4λС��)
	strVI	Vph;						//��ѹ(1λС��)
	strVI	Iph;						//����(3λС��)
	//strVI	I_z;						//���ߵ���(3λС��)
	strVI  wPph;				//	 �޹�
	u16		StartPwr;					//��������(4λС��)
	u16		Freq;						//
	u16		VDD_Volt;					//
	u16		LVD_Volt;					//
 	u16			IRACCessTm;					//

	u16		VBat[2];					//��ص�ѹ(3λС��)
	s16		OldT[4];					//�¶�ֵ����
	s16		Temperature;				//�¶�(1λС��)
		u32			IDACCess;					//
u16			FErrWord;					//Զ���������״̬��
#ifdef _OPT
/*			IC Card	Data		*/
	u32			ePurse;						//
	u8			Rand_Num[8];				//

	u8			At_Back[16];				//
	u8			ESAM_Sn[8];					//
	

/*          Far Trx Data       */


	u8			Rand_Num_Far[8];			//
	u8			IC_Num_Far[8];				//

	
	#endif
	u8			IC_Num[8];					//
	u32			Write ;
#define		_FEW_OTHER_			(1<<0)		//��������
#define		_FEW_REPEATCHARGE_	(1<<1)		//�ظ���ֵ
#define		_FEW_ESAMERR_		(1<<2)		//ESAM��֤ʧ��
#define		_FEW_IDERR_			(1<<3)		//�����֤ʧ��
#define		_FEW_HNUMERR_		(1<<4)		//�û��Ŵ���
#define		_FEW_CHARGECNTERR_	(1<<5)		//�����������
#define		_FEW_OVERCHARGE_	(1<<6)		//���ڻ�
#define		_FEW_ADDRERR_		(1<<7)		//��ַ�쳣
#define		_FEW_HANGUP_		(1<<8)		//����
#define		_FEW_MACERR_		(1<<15)		//MAC����

	u16			FarCtrlState;					//Զ�̿��Ʒ���״̬
#define		_FCS_JD_ON_			(1<<0)		//ֱ�Ӻ�բ
#define		_FCS_JD_ON_ALLOW_	(1<<1)		//�����բ
#define		_FCS_JD_OFF_		(1<<2)		//ֱ����բ
#define		_FCS_JD_OFF_DELAY_	(1<<3)		//��ʱ��բ
#define		_FCS_JD_OFF_PRE_	(1<<4)		//Ԥ��բ
#define		_FCS_JD_OFF_IPH_	(1<<5)		//��ʱ��բ(�����)

#define		_FCS_PRT_			(1<<8)		//����
#define		_FCS_PRT_OFF_		(1<<9)		//������
#define		_FCS_ALM_			(1<<10)		//����
#define		_FCS_ALM_OFF_		(1<<11)		//�������

#define		_FCS_HANG_			(1<<0)		//������
#define		_FCS_MACERR_		(1<<1)		//MAC��
#define		_FCS_KEYERR_		(1<<2)		//�����δ��Ȩ
#define		_FCS_IDTIME_		(1<<3)		//�����֤��ʱ
#define		_FCS_TIMEERR_		(1<<4)		//ִ��ʱ����Ч
#define		_FCS_JDOFFERR_		(1<<5)		//��բʧ��(����)
#define		_FCS_JDOFFPREERR_	(1<<6)		//Ԥ��բʧ��(����)
#define		_FCS_JDOFFPRETMERR_	(1<<7)		//Ԥ��բ����ʱ����Ч

#define		_FCS_JDOFFPREOFF_	(1<<15)		//��բ״̬��Ԥ��բ
#define		_FCS_SAFE_			(_ERR_SAFE_<<8)

//	strFarICErr	FErrWord;					//
	//u8			EChipCnt;					//����оƬ����
	//u8			EChipChkCnt;				//����оƬ���ݼ���
	//u8			EChipVoltCnt;				//��ѹ���У������
	//u8			EChipVP;					//��ѹ���У����
//	//u8			EChipState;					//����оƬ״̬
	//=====================pxadd start
	//++unsigned char bKeyint_io:1;
	//++unsigned char bEopenint_io:1;
	//++unsigned char bUpDateTime:1;
	unsigned char	bKeyLongPress:1;		//	 ������Ϊ1
//	unsigned char b8025Terr:1;
	unsigned char bGetLvd:1;	//	unsigned char bFlashMod:1;
	unsigned char bVDD_Down:1;	//	VDD ����
	unsigned char bDianLianMod:1;		//	  �����޸ĺ���1��
	unsigned char bClr_Comm:1;		//	ͨѶ��0
	unsigned char bUp_LVSave:1;
	unsigned char cLVD_Filter_Tm;
	unsigned char cShowStatr;
	unsigned char cOffDispTm;
	unsigned char 	cKeyOnPress ;
	unsigned char 	cKeyOffPress ;
	unsigned char	cOpenTm;
	unsigned char 	cCloseTm;
	unsigned int		iIntPowkey;
	unsigned long lOffLineTime	;
	unsigned short   iVDDCONFIG;
	unsigned short		iXiaoBiaoNo;


	//u8 			cFlashSaveTm_Sec;
	u8 			btemp ;

	//====================pxadd  end
	u8			End;						//
} strRamData;

#define		Bat1_State		(((strBit32 *)(&RamData.RunState[0]))->Bit2)
#define		Bat2_State		(((strBit32 *)(&RamData.RunState[0]))->Bit3)
#define		AUse_Dir		(((strBit32 *)(&RamData.RunState[0]))->Bit4)
#define		RUse_Dir		(((strBit32 *)(&RamData.RunState[0]))->Bit5)
#define		JD_Err			(((strBit32 *)(&RamData.RunState[0]))->Bit8)
#define		RunState_ESAM	(((strBit32 *)(&RamData.RunState[0]))->Bit9)
#define		RunState_Prog	(((strBit32 *)(&RamData.RunState[0]))->Bit12)		//�ڲ������
#define		RunState_EEP	(((strBit32 *)(&RamData.RunState[0]))->Bit13)		//�洢������
#define		RunState_OverDraft	(((strBit32 *)(&RamData.RunState[0]))->Bit14)	//͸֧״̬
#define		RunState_TmErr	(((strBit32 *)(&RamData.RunState[0]))->Bit15)		//ʱ�ӹ���

//#define		STA_EEP			(((strBit32 *)(&RamData.RunState[0]))->Bit11)
//#define		LocalKey_State	(((strBit32 *)(&RamData.RunState[0]))->Bit15)
//#define		PROG_State		(((strBit32 *)(&RamData.RunState[2]))->Bit3)

#define		AUse_Dir_A		(((strBit32 *)(&RamData.RunState[0]))->Bit16)

#define		TMTBL_State		(((strBit32 *)(&RamData.RunState[1]))->Bit0)
#define		RunState_IR		(((strBit32 *)(&RamData.RunState[1]))->Bit3)		//������֤��Ч

//#if		_JD_MODE_ == 1
	
	#define		JD_TState	(((strBit32 *)(&RamData.RunState[1]))->Bit4)		//�̵������״̬���
	#define		JD_State	JD_TState //	RamData.JD_Out
//	#else
//	#define		JD_State		(((strBit32 *)(&RamData.RunState[2]))->Bit4)
//#endif

#define		YTBL_State		(((strBit32 *)(&RamData.RunState[1]))->Bit5)
#define		JD_CMD_State	(((strBit32 *)(&RamData.RunState[1]))->Bit6)
#define		JD_OFF_AlmState	(((strBit32 *)(&RamData.RunState[1]))->Bit7)		//Ԥ��բ����״̬
#define		UseCard_State	(((strBit32 *)(&RamData.RunState[1]))->Bit9)
#define		Protect_State	(((strBit32 *)(&RamData.RunState[1]))->Bit12)
#define		RunState_ID		(((strBit32 *)(&RamData.RunState[1]))->Bit13)		//�����֤״̬
#define		RunState_LOpen	(((strBit32 *)(&RamData.RunState[1]))->Bit14)		//���ؿ���״̬
#define		RunState_FOpen	(((strBit32 *)(&RamData.RunState[1]))->Bit15)		//Զ�̿���״̬
#define		VOLT_Over_State	(((strBit32 *)(&RamData.RunState[1]))->Bit18)
#define		IPH_Over_State	(((strBit32 *)(&RamData.RunState[1]))->Bit20)		//����״̬
#define		PWR_Over_State	(((strBit32 *)(&RamData.RunState[1]))->Bit21)

#define		PfErr_State		(((strBit32 *)(&RamData.RunState[3]))->Bit7)
#define		RunState_EOpen	(((strBit32 *)(&RamData.RunState[3]))->Bit9)

typedef	struct {
	u8			Cmd;
	u8			UserType;
	u8			H_Num[6];					//User Number
	u8			E_Num[6];					//Meter Number
	u8			TRx_Num[6];					//Trx Number
	u32			Alm1;						//Alm 1
	u32			Alm2;						//Alm 2
	u32			I_Mul;
	u32			V_Mul;
	u32			SumCharge;
	u32			ChargeCnt;
	u32			DisCharge;
	u32			StartTm_1;					//YTbl Change Time
	u32			StartTm_2;					//DTbl Change Time
	u32			StartTm_3;					//Price Change Time
	u32			StartTm_4;					//Step Changde Time
} strSafeInfo;


#define  		 _strSafeInfo_len_   		((int )&(((strSafeInfo *)0)->StartTm_4)+(sizeof((strSafeInfo *)0)->StartTm_4)/sizeof(char)))

typedef	struct {							//RunInfo
	u8			IC_Num[8];					//User Card Num
	u8 		StartCode ;
	#ifdef _DEL
	u32			NKeyState;					//New KeyState	
	u8			LastEvtState[8];			//�ϴζ�ȡ�¼��ϱ�״̬��
	u8			EvtCnt[_cEvnCnt_Len_];					//�����ϱ�����
	#endif
#define		_EVT_JD_			0			//���ƻ�·����
#define		_EVT_ESAM_			1			//ESAM����
#define		_EVT_N1_			2			//�ڿ���ʼ������
#define		_EVT_BAT1_			3			//ʱ�ӵ��Ƿѹ
#define		_EVT_CODE_			4			//�ڲ������
#define		_EVT_EEP_			5			//�洢������
#define		_EVT_TIME_			7			//ʱ�ӹ���
#define		_EVT_BAT2_			8			//������Ƿѹ
#define		_EVT_OVERDRAFT_		9			//͸֧״̬
#define		_EVT_EOPEN_			10			//�����
#define		_EVT_EOPEN2_		11			//��β��
#define		_EVT_MEG_			12			//�㶨�ų�����
#define		_EVT_EPWR_			13			//��Դ�쳣
#define		_EVT_JDOFF_			14			//��բ�ɹ�
#define		_EVT_JDON_			15			//��բ�ɹ�
#define		_EVT_LOSTVOLT_		16			//ʧѹ
#define		_EVT_LOWVOLT_		17			//Ƿѹ
#define		_EVT_OVERVOLT_		18			//��ѹ
#define		_EVT_OVERIM_		20			//����
#define		_EVT_OVERPWR_		21			//����
#define		_EVT_PREV_			22			//���ʷ���
#define		_EVT_PWROFF_		37			//����
#define		_EVT_OVERPFPH_		39			//������������
#define		_EVT_RAREV_			41			//�ܹ��ʷ���
#define		_EVT_PROG_			48			//����¼�
#define		_EVT_CLEAR_			49			//�������
#define		_EVT_PWRCLEAR_		50			//��������
#define		_EVT_EVTCLEAR_		51			//�¼�����
#define		_EVT_CHGTIME_		52			//Уʱ�¼�
#define		_EVT_TMTBL_			53			//ʱ�α���
#define		_EVT_YTBL_			54			//ʱ������
#define		_EVT_WEEKTBL_		55			//�����ձ��
#define		_EVT_HLD_			56			//�ڼ��ձ��
#define		_EVT_COM_SET_		57			//�й���������ֱ��
#define		_EVT_FRZDT_			60			//�����ձ��
#define		_EVT_PRICE_			61			//���ʱ���
#define		_EVT_LSTEP_			62			//���ݱ���
#define		_EVT_KEYCHG_		63			//��Կ����

	u16			LastTmDay;					//��ǰ����
	u8			Rev1[2];					//�ֽڶ���
	u8			KeyErrCnt[3];				//��������
	u8			MacErrCnt;					//MAC��֤�������
	u32			BatUseTime;					//���ʹ��ʱ��

	u32			LastUse;					//�ϴν����ۼ��õ���
	u32			LastYUse;					//�ϴ�������ۼ��õ���
	u32			LastMFrozeTm;				//�ϴν��ݶ���ʱ��
	u32			LastYFrozeTm;				//�ϴ����������
	u32			LastDFrozeTm;				//�ϴ��ս���ʱ��
	s32			ComUseDiff[_MAX_TARRIF_+1];	//����й���ֵ

	u32			ProgTime;					//��̼�¼ʱ��
	u32			ProgWrite;				//��̼�¼�����ߴ���
	u32			ProgCode[10];				//��̼�¼���ݱ�ʶ
	u16			ProgCodeCnt;				//��̸���

	u16			ErrCardCnt;					//�Ƿ��忨����
	u8			YTbl_Num;					//��ǰ��ʱ�α��
	u8			ActTblNum;					//��ǰ��ʱ����
	u8			FarJDPreTm;					//Ԥ��բ��ʱ
	u8			Rev2[8];					//�ֽڶ���
	u8			LStep;						//��ǰ������
	u32			StepPrice;					//��ǰ���ݵ��
	u32			FLPrice;					//��ǰ���ʵ��
	u32			ePrice;						//��ǰ�ܵ��
	u32			RevUseTime;					//��������ʱ��
	u32			OverPfphTime;				//������������ʱ��
	u32			OverIphTime;				//����ʱ��
	u32			OverPwrTime;				//������ʱ��
	u32			LastJDWriter;				//�̵��������ߴ���
	struct {
		u32		JDOverIphTime;				//��������բ�ӳٽ���ʱ��
		u32		FarJDOffTime;				//Զ����ʱ��բ����ʱ��
		u32		FarJDOffPreTime;			//Ԥ��բ��բʱ��
		u32		PwrOffTime;					//����ʱ��
		u32		LastRevTime;				//�ϴη���ʱ��
		u32 	LastOverPfphTime;			//�ϴγ���������ʱ
		u32		LastOverIphTime;			//�ϴι���ʱ��
		u32   KeyErrTime[3];
	} RcdTm;								//ʱ���¼
	u32			EState;						//���״̬��

		
#define			_BIT_IN_PROG_			0	//????????(?????????????
#define			_BIT_ALM_OFF_			1	//??????(??????)
#define			_BIT_OVER_OFF_			2	//???????(?????)
#define			_BIT_LOC_OPEN_			4	//??????
#define			_BIT_FAR_OPEN_			5	//??????
#define			_BIT_OVERDAFT_			6	//????
#define			_BIT_YSTEP_				7	//??????
#define			_BIT_FUHE_				8	//????????
#define			_BIT_LOBAT_				9	//???????

#define			_BIT_TIME_				3	//????
#define			_BIT_ESAM_				11	//ESAM
#define 		_BIT_METER_CLEAR_		_BIT_ESAM_		//		??????????????,????
	
#define			_BIT_EEP_				15	//?????

#define			_BIT_JD_STATE_			12	//???????(1???)
#define			_BIT_JD_CMD_			13	//??????
#define			_BIT_FAR_ALM_			14	//??????(_ALM_STATE_)
#define			_BIT_FACOFF_			16	//??????
#define			_BIT_TIME_0_			17	//?????00???
#define			_BIT_YTB_STATE			18	//?????????
#define			_BIT_TTB_STATE			19	//?????????
#define			_BIT_FL_PRICE_			20	//????????
#define			_BIT_STEP_PRICE_		21	//??????????
#define			_BIT_PROTECT_CMD_		22	//??????
#define			_BIT_DAY_CHG_			23	//??????
#define			_BIT_CHG_KEY_			24	//??????????
#define			_BIT_KEY_NEXT_			25	//??/????(0-???? 1-????)
#define			_BIT_EVT_OUT_			26	//????????
#define			_BIT_LAST_STATE_		27	//???????
#define			_BIT_OFF_PRE_			28	//???????
#define			_BIT_JD_ERR_			29	//???????????
#define			_ESTATE_IN_PROG_		(((u32)1)<<_BIT_IN_PROG_)		//????????(?????????????
#define			_ESTATE_ALM_OFF_		(((u32)1)<<_BIT_ALM_OFF_)		//??????(??????)
#define			_ESTATE_OVER_OFF_		(((u32)1)<<_BIT_OVER_OFF_)		//???????(?????)
#define			_ESTATE_LOC_OPEN_		(((u32)1)<<_BIT_LOC_OPEN_)		//??????
#define			_ESTATE_FAR_OPEN_		(((u32)1)<<_BIT_FAR_OPEN_)		//??????
#define			_ESTATE_OVERDAFT_		(((u32)1)<<_BIT_OVERDAFT_)		//????
#define			_ESTATE_YSTEP_			(((u32)1)<<_BIT_YSTEP_)			//??????
#define			_ESTATE_FUHE_			(((u32)1)<<_BIT_FUHE_)			//????????
#define			_ESTATE_LOBAT_			(((u32)1)<<_BIT_LOBAT_)			//???????


#define 		_ESTATE_TIME_			(((u32)1)<<_BIT_TIME_)			//????
#define 		_ESTATE_ESAM_			(((u32)1)<<_BIT_ESAM_)			//ESAM
#define 		_ESTATE_EEP_			(((u32)1)<<_BIT_EEP_)			//?????


#define			_ESTATE_JD_STATE_		(((u32)1)<<_BIT_JD_STATE_)		//???????(1???)
#define			_ESTATE_JD_CMD_			(((u32)1)<<_BIT_JD_CMD_)		//??????
#define			_ESTATE_FAR_ALM_		(((u32)1)<<_BIT_FAR_ALM_)		//??????(_ALM_STATE_)
#define			_ESTATE_FACOFF_			(((u32)1)<<_BIT_FACOFF_)		//??????
#define			_ESTATE_TIME_0_			(((u32)1)<<_BIT_TIME_0_)		//?????00???
#define			_ESTATA_YTB_STATE		(((u32)1)<<_BIT_YTB_STATE)		//?????????
#define			_ESTATE_TTB_STATE		(((u32)1)<<_BIT_TTB_STATE)		//?????????
#define			_ESTATE_FL_PRICE_		(((u32)1)<<_BIT_FL_PRICE_)		//????????
#define			_ESTATE_STEP_PRICE_		(((u32)1)<<_BIT_STEP_PRICE_)	//??????????
#define			_ESTATE_PROTECT_CMD_	(((u32)1)<<_BIT_PROTECT_CMD_)	//??????
#define			_ESTATE_DAY_CHG_		(((u32)1)<<_BIT_DAY_CHG_)		//??????
#define			_ESTATE_CHG_KEY_		(((u32)1)<<_BIT_CHG_KEY_)		//??????????
#define			_ESTATE_KEY_NEXT_		(((u32)1)<<_BIT_KEY_NEXT_)		//??/????(0-???? 1-????)
#define			_ESTATE_EVT_OUT_		(((u32)1)<<_BIT_EVT_OUT_)		//????????
#define			_ESTATE_LAST_STATE_		(((u32)1)<<_BIT_LAST_STATE_)	//???????
#define			_ESTATE_OFF_PRE_		(((u32)1)<<_BIT_OFF_PRE_)		//???????
#define			_ESTATE_JD_ERR_			(((u32)1)<<_BIT_JD_ERR_)		//???????????
u8 End;
} strRunInfo;



typedef	struct {							//�������
	u32			EKeyState;					//Key State
	u32			OverDraftLimit;			//
	u32			MaxPurse;				//
	u32			JDOnMinPurse;			//
	u32			OverPwrLimit;			//
	u32			RevPwrLimit;			//
	u32			JDWaitAM;					//��բ������������
	u32			HourFrozeStartTm;		//
	u32			FuheStartTm;			//
	u8			FrozeDT[3][2];			//
	u16			FrozeHM;				//
	u16			JDOffDelayTm;			//
	u16			AMOffDelayTm;			//�̵���������բ��ʱ

	u16			OverVoltLimit;			//
	s16			OverPhLimit;			//
	u16			OverIphLimit;			//��������
	u16			BatLoLmt;				//
	u8			TRxKey[3][4];			//Keys
	u8			KeyNum;					//Key Num
	u8			RunType;				//RunType 	Bit0 - Ex Relay	 Bit1 - Evt Out On Evt Line
	u8			EvtMode[8];				//Event Out Mask
	u8			Baud[3];				//SCI Baud
	u8			DispLength[2];			//
	u8			F_Num[32];				//
	u8			GPSCode[11];			//
	u8			OverVoltTm;				//
	u8			OverPwrTm;				//
	u8			OverPhTm;				//
	u8			OverIphTm;				//������ʱʱ��
	u8			RevPwrTm;				//
	u8			HourFrozeStep;			//
	u8			IRAccessTmSet;			//������֤ʱЧ
	u8			EvtClrTmSet;			//�����ϱ�״̬�ָ�λʱЧ
	u8			FrozeCode[4];			//��ʱ����ʱ�����
	u8			FHMode;					//���ɼ�¼ģʽ��
	u8			FrozeMode[5];			//����ģʽ��
	u8			FHTmStep[4];			//���ɼ�¼ʱ����

	u8			ActUseMode;				//
	u8			FLMode[7];				//
	u8			WeekMode;				//
	u8			WeekTblNum;				//
	u8			DispEnergyDots;			//
	u8			DispPowerDots;			//
	u8			AutoDispTm;				//
	u8			DispAllTm;				//
	u8			Rev[20];				//
} strSetInfo;

/*
typedef struct {						//��ʱ����ṹ
	GBit	MD:9;						//��ʼ����(Month*32+Date)
	GBit	TblNum:4;					//��ʱ�α��
} strYearTbl;
typedef struct {						//��ʱ�α�ṹ
	GBit	HM:11;						//ʱ��(Hour*60+Min)
	GBit	FLNum:5;					//���ʺ�(���֧��31����)
} strTmTbl;

typedef struct {						//���ʱ�
	strYearTbl YearTbl[14];				//��ʱ����(28B)
	strTmTbl TmTbl[8][14];				//��ʱ�α�(8*28B)
	u8	Price[32][3];					//��۱�(96B)
	u32	LUse[_MAX_LADDER_];				//���ݵ�����(24B)
	u8	LPrice[_MAX_LADDER_+1][3];		//���ݵ�۱�(21B)
	u8	YStepTm[4][3];					//�������
} strPriceTbl;	*/

typedef struct {
//	u8		ChipSet[CHIP_SET_LEN];
	u8		StartCode;					//��ʼ��
	u8		SNum;						//���
		//u16		RTCSet[10]; 	//	 u16		RTCSet[2]; 						//�¶����У
	s16		RTCSet[8];					//�¶�ʱ��У������(��׼�¶�ƫ��/�¶ȱ���ƫ��/TCABL/TCABH/TCCDL/TCCDH)
//	strSectUse pFLUse[_MAX_TARRIF_];
	union UNMETERCONFIG     meterinfo;		//	��������
	strSafeInfo	SafeInfo;
	strRunInfo	RunInfo;
	strSetInfo	SetInfo;
	u16 FrozeCnt[_FROZE_NUM_];		//
 	//strDispCode	DispArry_normal[99];
 	//strDispCode	DispArry_key[99];
	//strDispCode DArryCardOver[4];
	//strPriceTbl	PriceTbl[2];		//
	u8	End;
} strFlashInfo;

//#define   _iflashinfo_start_offset_		  	(int)&(((strFlashInfo *)0)->StartCode)


#define 		_DisArry_Num_EERom_startaddr_     (int)&(((struct   ST24256_DATACONFIG *)0)->stFlashInfo_EEprom.SetInfo.DispLength)

typedef struct {					//����(24+12+4 = 40�ֽ�)
	GBit		Rev1:8;

	GBit		DispVolt:5;				//��ʾ��ѹ����
	GBit		DispCR3:1;				//��ʾƵ������
	GBit		Rev2:1;				//RCУ��ʹ��
	GBit		Rev3:1;

	GBit		Rev4:8;

	GBit		Rev5:8;
} strMSet;

typedef struct {
	u8		InterVersion[32];			//�ڲ��汾(F12F)
	u32		AreaCode;					//�������(F12F)
	u16		kWhImp;						//���峣��(F12F)
	u16		MnCuRes;					//��ͭ��ֵ(F12F)
	u16		V_Std;						//���ѹ(F12F)
	u16		A_Std[2];					//�����/������(F12F)
	u8		Act_Grd;					//�й����ȵȼ�(F12F)
	u8		IMType[10];					//�������(F12F)
	u8		SoftVer[32];				//����汾��(F13F)
	u8		HardVer[32];				//Ӳ���汾��(F13F)
	u8		FacNum[32];					//���ұ��(F13F)
	u8		MakeTime[10];				//��������(F13F)
	u8		VerSion[16];				//Э��汾(F13F)
	union UNMETERCONFIG     meterinfo;		//	��������
	strMSet	MSet;						//��������(F15F)
	u32		CodeLth;					//���볤��
	u8		VTime[4];
	u8		SoftID[8];					//���������
	u8		ExSet[32 - sizeof(strMSet) - 4 - 4 - 8];//��������(F15F)
} strDftSet;

typedef	struct {
	strDftSet		DftSet;
} strFData;


extern	unsigned char    ComData[_COMDATA_LEN_];
extern strSCIBuf 	SCI[3];
// ==pxopt extern	strWFBuf	WFBuf;
extern strRWSamic RWSamic;
extern strFlashInfo FlashInfo;
extern	strRamData RamData;
extern strEx645		Ex645;

extern u8* tpChar;
extern u8* tpBase;
extern	const  strFData FData;
 
						/*			Drv_TRx.c			*/
void	Init_USART(void);										//ͨѶ�ӿڳ�ʼ��
void	Rx_Enable(strSCIBuf *SCIx);								//����ʹ��
void	Tx_Enable(strSCIBuf *SCIx);								//����ʹ��
void	SubSCIBuf(u8 Cnt);										//���ͨѶ����
void	Send_SCI(strSCIBuf *SCIx);								//������һ����
//void	Rst_SCI(u8 SCI_Num);									//����ͨѶ�ӿ�




void	Do_ChkVolt(void);										//Drv_ADCJD.c
void	ADC_On(void);
void	ADC_Off(void);
#define 	Get_LVDVolt()			(0)   // 	u16		Get_LVDVolt(void);

u16		ChgMOD_2(u32 *pData, u16 Num);
u16		ChgMOD_1(u32 *pData, u16 Num);
void	GetFrozeMinCnt(unsigned char  *pTm);
u32		MulDivX( u32 Mul_1, u32 Mul_2, u32 Div);
u16		GetSum(u8 *buf, u8 Cnt);
void	SetRamData(u8 *Dst, u8 Data, u16 Length);				//Public.c
#define		_ADD_33_		0x33
#define		_SUB_33_		0x34
#define	FillRam(Dst, Data, Lth)		SetRamData(Dst, Data, Lth)

strSectUse *GetFLBase(u8	Feilv);									//Public.c
void	RevStr(u8 *s, u16 Lth);
void	CopyRam(u8 *Dst, u8	const *Src, u16 Length);			//Public.c
void	cCopyRam(u8 *Dst,   u8 *Src, u16 Length);

void	RevCopyRam(char * Dst, char *Src, u16 Length);		//Public.c
#define	RevCopyCodeRam(Dst, Src, Lth)	RevCopyRam((char *)Dst, (char *)(Src), Lth)
void	Copy_4( u8 *Dst, uc8 *Src);
void	RevCopy_4( u8* Dst, uc8 *Src);
s32		strCmp(uc8 *s1, uc8* s2, u16 cnt);
void	GetDispCode(u8 DispNum);
s32		Cmp_4(uc8 *s1, uc8 *s2);
s8		Is_DispCode(u32 DCode);

void	ClrRam(u8* buf, u16 Cnt);								//Public.c
#define	ClrRam16	ClrRam
void	SetRam(u8* buf, u16 Cnt);
void	FpCharRam(u16 FillCode);
void	ClrpCharRam( u8 Cnt);
void	SetpCharRam( u8 Cnt);
#define		FillpCharRam(Data, Cnt)	FpCharRam((((u16)Data)<<8)|Cnt)
void	ClrpCharRam( u8 Cnt);
void	RevStrLst(u8 *s, u8 Lth, u8 Repeat);
void	RevStrLst(u8 *s, u8 Lth, u8 Repeat);
void	Rev_4(u8 *s);
void	Rev4Lst(u8 *s, u8 Repeat);
ucode	*Get_Code_Lst(ucode *Addr, u16 Code, u8 Bytes, u8 Length);	//Public.c

u8		HEX(u8 Data);
u8		BCD(u8 Data);
void	HEXLst(u8 *Data, u8 Lth);
void	BCDLst(u8 *Data, u8 Lth);

u8		CheckDataAct(u8 *pData);
//u8		CheckKeyChg(u32 *pData);
#define CheckKeyChg(a)    (ERROR)	

u8		CheckBCD(u8 *buf,	u8 Cnt);
u8		CheckZero(u8 *buf, u8 Cnt);
void	Hex_BCD(s32 Data, u8* Dst);
u32		BCD_Hex(u32 Data);											//Public.c
void	Up_u32Data( u32 *pDst, u32 Data);
void	Up_u16Data(u16* pData, u16 Data);
void	Up_u8Data(uc8* pData, u8 Data);
void	ClrRcdTime( u32 *pTm);
void	SetRcdTime( u32 *pTm);
void	SetRcdMin( u32 *pTm);
void	UpJDWriter(void);
void	Up_NormFroze(u8 Type);
void	Up_FrozeOverTime(u8 FType, u32* pTime, u32* pLastTime);
void	Up_ProgFroze(u8 Type, u8 Mode, u8* Src);
void	GetNewProgRcd(u8 *Buf);
void	AddProgRecord(u8 *Code);
void	CloseProgRecord(void);
void	SetEBit(u8 SetBit);
#define		ClrEBit(ClrBit)			SetEBit((ClrBit)|0x80)
void	UpEState(u32 NewState);
//u8		GetWeek(u32 *pTm);
u32		GetTimeDiff(u32 *pTime);

void	ChkEvtState(void);
void	SetEvtState(u8	BitCnt);									//Public.c




void	CheckpChar(void);
#define		CheckpCharLength(Length)	CheckpChar()
u8		GetAddrData(u32 Addr);
void	tpCharInc(u8 Inc);
void	NormCopy( uc8 *Addr, u8 Length);
void	RevCopy( uc8 *Addr, u8 Length);
void	SetpCharInc( u8 Data);
void	MvData(u8 *pData, u16 Opt);
void	MoveDataLst(const strMvData   *pMv);
void	N_MvData(void *pIn, u16	Opt, u8 Mode);
u8		Chk_FrozeType(u8 Type);
u16		GetFStartAddr(u8 Type, u16 Num);
u8*		GetFrozeStart(u8 Type, u8 Num);									//PubData.c
void	Write_Froze( uc8 *Src, u8 Type, u16 N);
void	Chk_AllData(void);
void	Up_FrozeData( uc8 *Src, u8 Type);
void	ClrFrozeCnt(u8 Type);
u16		GetFrozeCnt(u8 Type);
void	Get_RecordTime(u32 *pData, u32 *pTime, u32* pLastTime);
void	Copy_FlashInfo( u8* Dst, uc8 *Src, u8 Length);
u8		WriteFlashData(u32 Dst, uc8* Src, u16 Lth);
#define		WriteFData(D, S, L)		WriteFlashData(((D)-_CODE_ADDR_), S, L)
u8		GetFHLth(u8 Mode);
u32		GetFHTime(u8 *pData);
void	Up_FHFroze(u8 *pData);
void	Read_FHSet(void);
void	Read_FHRcd(u8 *Buf, u16 Bias, u16 Lth);
u8		Get_FHFrozeBlock(void);


void	Up_BkLVSave(void);
void	Up_LVSave(void);
u8 	Up_LVSave_io(void);
void	Chg_Feilv(u8 NewFeilv);
void	Up_TTbl(u8* Buf, u8 Type);
// void	Up_Hld(strHolidayTbl *pHld, u8 Num);

void	Up_DispCode(strDispCode *pDispCode, u8 Num);
void	Reset_LVSave(void);

u8		Read_EEData( u8* Buf, u16 Addr, u16 ILth);
u8		Write_EEData( u8* Buf, u16 Addr, u16 Lth);
u8 		EEData_Safe_Read ( u8* Buf, u16 Addr, u16 iLth);
void	GetProgData(u8* Dst, u32 pCode, u32 Lth);


void	Init_Data(void);										//GC_Use.c
#define Do_MsTask()     {} //   void	Do_MsTask(void);
#define		Check_MS()		{	if((Event_Ms)&&(_SYS_ON_LINE())){	Do_MsTask(); }	}
void	Do_MsTask_100(void);
void	Do_HalfSecTask(void);
void	Do_SecTask(void);
void	Do_MinTask(void);
void	Do_EOpen(void);

#define 	Up_PwrOffFroze()   {} //     void	Up_PwrOffFroze(void);
void	Up_PwrOnFroze(void);


 
void	Up_Remain(void);
void	WaitTimeOver(u32* pTm);
u8		ChkTimeGate(u32 *pTime);
void	Do_JDOffPreOver(void);
void	Do_JDOff(void);
void	GetAlmState(void);
void	ChkUseState(void);
void	ChkAlmJD(void);
void	ClrJDOffTime(void);
void	ChkYFroze(void);
u8		ChkYMode(void);
void	ChangeFDT1Froze(void);
void	Do_FeilChg(void);										//Meter.c
void	Do_Use(void);
u8		Do_FarCtrl(u8* Ctrl);
void	CheckFroze(void);

void	SetDispItem(uc8 *pCode);
void	UpBkltLED(void);										//Disp.c
void	InitDispProg(void);
void	Do_Keys(void);
void	Do_IRKeys(void);
void	Do_Disp(void);											//Disp.c
void	SetDispBit(u8	Bit);
void	SetDigitDisp(u8 Num, u8 Bit);
void	AddDispBits( u8 * Src);
void	AddDispSetBits(ucode *Src);
void	AddUseLst( ucode* Src, u8 FL);
u8		ChkDispItem(strDispCode const *pCode);

u8		GetCompData(u8 Mode);
u8		SetErrReturn(u8 Err);
void	SetTrxDisp(void);										//TRx.c
u8		FarCtrlRet(u16 Err);
void	Get645Data(strDispCode const *pCode);					//TRx.c
void	Do_TRx(void);											//TRx.c
void	Chk_TRx(void);

//void	ClrEvtState(u8 *pEvtState);
#define 	ClrEvtState(a)  {} 

//void	cClrEvtState(  u8 *pEvtState);
#define 	cClrEvtState(a) {}
void	Chg_Mi(void);											//DoCard.c
u8		ID_IRAccess1(u8* pData);
u8		ID_IRAccess2(u8* pData);
u8		ID_Access(u8* pData);
u8		ID_AccessTimeSet(u8* pData);
u8		ID_AccessTimeClose(u8* pData);
u8		ID_AccessACK(u8* pData);
u8		ID_AccessChangeMi(u8* pData);
u8		ID_AccessDataRead(u8* pData);
u8		ID_MacWrite(u8 *pData);
u8		ID_AccessCodeCmp(u8* pData);

u8		ID_AccessRstCharge(u8* pData);
u8		ID_AccessOpenCharge(u8* pData);
u8		ID_AccessNextCharge(u8* pData);

void	Set_ESAMErr(void);

u8		Get98Data(u8 *pData, u8 FID, u8 Lth);
u8		Chk_Mac_99(void);
u8		GetSafeData(u8 *pData, u8 Lth);

void	Up_UseData(void);

u8		SetCardErr(u16 ECode);									//DoCard.c
void	Up_FPrice(u8 *pData, u8 Num);
void	Up_LData(u8 *pData, u8 Num);
void	Do_Card(void);
#define	SetUserErr(Num, Er)	SetCardErr((Num<<8)|Er)

void	RefreshESAM(void);
u8		Read_ESAMData(void);									//DoCard.c


void	SetDispBitLst(uc8* Lst);
void	NextItem(u8 Mode);
void	SetpArry(u8 Num);
void	SetDispArry(u8 Num);
void	SetDispSubItem(u16 ItemSecs);
void	UpDisp(void);
#define Wait_Rst(RstType)		{}      //void 	Wait_Rst(u8 RstType);
void	PwrOffDisp(void);										//PwrOff.c
//============

//void GPIO_Init(unsigned char cPowerOn) ;
void initLcd8a( unsigned char *s );
void	SetupRTC(void);
void	SetRTCCnt(u32 RTC_Cnt);


unsigned char Rtcunit_Read_Addr(unsigned char cAddr, unsigned char *cp,unsigned char clen);

void	GetRTCTm(u8 *TmBCD) ;
u8 Check_Datatype_Mod_Enable( enum  ENUMDATACONFIG cmod);
//===================
#define OFFSETOF(type, field) ((size_t)&(((type *)0)->field))

struct   ST24256_DATACONFIG
{
	
	strRunInfo	RunInfo;       //strFlashInfo   stFlashInfo_EEprom ;
	unsigned short FrozeCnt[_FROZE_NUM_];		//
	strExFrozeData stFlashFrozeData_EEprom ;
	strDftSet		Dftset;
	unsigned long 	ltemp; 
	struct   STDIANLIAN 	stdianlian[_cBAKDATA_COPYNO_];

	u16		YearTbl[2][14];				//��ʱ����(2*28)
	u16		TmTbl[2][8][14];			//��ʱ�α�(2*8*28B)
	u8		Price[2][32][3];			//��۱�(2*96B)
	strLTbl	LTBl[2];					//���ݱ�(2*60B
	strHolidayTbl	pHld[254];					//���ձ�ʱ��
	strDispCode	DispArry[2][99];  	//  0 == DispArry_normal ,1 ==DispArry_key
    //u16		pYTbl[2];												//��ʱ������ʼ��ַ
	//u16		pTmTbl[2][8];											//��ʱ�α���ʼ��ַ
	//u16		pPriceTbl[2];											//��۱���ʼ��ַ
	//u16		pLTbl[2];												//���ݱ���ʼ��ַ
	//u16		TTblNext;												//ʱ�η��ʵ�۱������ַ
	//u16		pHldNext;
	//u16		pDisp[2][99];											//��ʾ���ַ
	u16		pDispNext;

	u16		pDispCardOver[4];										//�忨��ʾ��ַ

};

#define	vSt24256	(*((struct   ST24256_DATACONFIG *)0))
#define	_ST24256_ARRAY_UNIT(a)	(sizeof(vSt24256.a)/sizeof(vSt24256.a[0]))

#define	   _Dftset_start_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
 
#define    _Dftset_InterVersion_			(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.InterVersion)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define    _Dftset_InterVersion_Len_		sizeof(((struct   ST24256_DATACONFIG *)0)->Dftset.InterVersion)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define    _Dftset_MakeTime_			(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.MakeTime)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define    _Dftset_MakeTime_Len_		sizeof(((struct   ST24256_DATACONFIG *)0)->Dftset.MakeTime)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_METERSTAUE_				(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.stmemter_statu)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_IMType_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.stmemter_statu.IMType)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_SoftVer_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.stmemter_statu.SoftVer)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_HardVer_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.stmemter_statu.HardVer)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define    _FlashInfo_FrozeCnt_Start_		(int)&(((struct   ST24256_DATACONFIG *)0)->stFlashInfo_EEprom.FrozeCnt)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define    _FlashInfo_FrozeCnt_Len_			sizeof(((struct   ST24256_DATACONFIG *)0)->stFlashInfo_EEprom.FrozeCnt)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define     _Froze_EERom_startaddr   		(int)&(((struct   ST24256_DATACONFIG *)0)->stFlashFrozeData_EEprom)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define     _Froze_EERom_ltempaddr   		(int)&(((struct   ST24256_DATACONFIG *)0)->ltemp)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
//#define   	_FlashInfo_eeprom_startaddr   	(int)&(((struct   ST24256_DATACONFIG *)0)->stFlashInfo_EEprom)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
  
 #define   	_RunInfo_eeprom_startaddr_   	(int)&(((struct   ST24256_DATACONFIG *)0)->RunInfo)	 
#define   	_FrozeCnt_eeprom_startaddr_   	(int)&(((struct   ST24256_DATACONFIG *)0)->FrozeCnt)	 

/*
#define	   _Dftset_IMType_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.IMType)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_SoftVer_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.SoftVer)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define	   _Dftset_HardVer_					(int)&(((struct   ST24256_DATACONFIG *)0)->Dftset.HardVer)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
*/




#define     _Froze_EERom_startaddr   		(int)&(((struct   ST24256_DATACONFIG *)0)->stFlashFrozeData_EEprom)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4
#define     _Froze_EERom_ltempaddr   		(int)&(((struct   ST24256_DATACONFIG *)0)->ltemp)	// ((( 0 + sizeof(strFlashFrozeData) )/4)+1) *4

//��	��ǰ����
#define   	_iDLDATA_START   				(int)&(((struct   ST24256_DATACONFIG *)0)->stdianlian)			//	����������ʼ��ַ
#define   	_iDLDATA_SIZE	     			sizeof(((struct   ST24256_DATACONFIG *)0)->stdianlian)/sizeof(char)			//	�������泤��

//��	�ַ��ʵ���
#define 	_FLUse_Startaddr				(int)&(((struct   ST24256_DATACONFIG *)0)->FLUse)
#define		_FLUse_Len						sizeof(((struct   ST24256_DATACONFIG *)0)->FLUse)/sizeof(char)			//	�������泤��
// �������ʵ����ĳ���
#define 	_FLUse_Unit_Len					sizeof(((struct   ST24256_DATACONFIG *)0)->FLUse[0])/sizeof(char)			//	�������泤��

//��ʱ����ַ
#define     _YearTbl_Eeprom_Startaddr    	(int)&(((struct   ST24256_DATACONFIG *)0)->YearTbl)					//
#define    	_YearTbl_Eeprom_Len    			(sizeof(((struct   ST24256_DATACONFIG *)0)->YearTbl)/sizeof(char))		//
#define 	_YearTbl_Datatype				u16					///���ձ������������
#define		_YearTbl_Unit_Len				sizeof(_YearTbl_Datatype)							//	��ʱ��Ԫ�ص�Ԫ����
//������ʱ����ƫ��
#define    	_YearTbl_Eeprom_First_Len    	(_YearTbl_Eeprom_Len/2)
//�ڶ�����ʱ����ַ
#define     _YearTbl_Eeprom_Second_Startaddr   (_YearTbl_Eeprom_Startaddr + _YearTbl_Eeprom_First_Len)
//ʱ����Ѱַ ,ctao������biaono��ʱ�α��,cno��ʱ��
#define 	_Year_Get(ctao,cno)				(int)&(((struct   ST24256_DATACONFIG *)0)->YearTbl[ctao][cno])
//ʱ����Ѱַ������
#define 	_Year_GetT(ctao)				_Year_Get(ctao,0)



//��ʱ�α��ַ
#define     _TmTbl_Eeprom_Startaddr    	((int)&(((struct   ST24256_DATACONFIG *)0)->TmTbl))					//
#define    	_TmTbl_Eeprom_Len    		(sizeof(((struct   ST24256_DATACONFIG *)0)->TmTbl)/sizeof(char))		//
#define 	_TmTbl_Datatype				u16					///��ʱ�α���������
#define		_TmTbl_Unit_Len				sizeof(_TmTbl_Datatype)			//	��ʱ�α�Ԫ�ص�Ԫ����
#define 	_TmTbl_StoreMaxNum_			((_TmTbl_Eeprom_Len)/sizeof(_TmTbl_Datatype))
//������ʱ�α��ƫ��
#define    	_TmTbl_Eeprom_First_Len    	(_TmTbl_Eeprom_Len/2)
//�ڶ�����ʱ�α��ַ
#define     _TmTbl_Eeprom_Second_Startaddr    (_TmTbl_Eeprom_Startaddr + _TmTbl_Eeprom_First_Len)
//����ʱ�α���
#define     _TmTbl_Eeprom_1_1   		 ((int)&(((struct   ST24256_DATACONFIG *)0)->TmTbl[0][1])- _TmTbl_Eeprom_Startaddr )
//ʱ�α�Ѱַ ,ctao������biaono��ʱ�α��,cno��ʱ��
#define 	_Tmtbl_Get(ctao,biaono,cno)	(int)&(((struct   ST24256_DATACONFIG *)0)->TmTbl[ctao][biaono][cno])
#define 	_Tmtbl_GetB(ctao,biaono)	_Tmtbl_Get(ctao,biaono,0)

//��۱���ʼ��ַ
#define     _Price_Eeprom_Startaddr    			(int)&(((struct   ST24256_DATACONFIG *)0)->Price)					//
#define    	_Price_Eeprom_Len    				(sizeof(((struct   ST24256_DATACONFIG *)0)->Price)/sizeof(char))		//
//  ��һ�׵�۱���ʼ��ַ
#define     _Price_Eeprom_First_Startaddr   	(int)&(((struct   ST24256_DATACONFIG *)0)->Price[0][0][0])					//
#define    	_Price_Eeprom_First_Len    			(_Price_Eeprom_Len/2)
//  �ڶ��׵�۱���ʼ��ַ
#define     _Price_Eeprom_Second_Startaddr    	(int)&(((struct   ST24256_DATACONFIG *)0)->Price[1][0][0])					//
#define    	_Price_Eeprom_Second_Len    		(_Price_Eeprom_First_Len)
//   ��۱�Ԫ����
#define 	_Price_Unit_Len_					3
//  ��۱�Ѱַ
#define    	_Price_Get(ctao,biaono)				(int)&(((struct   ST24256_DATACONFIG *)0)->Price[ctao][biaono])




//���ձ��ַ
#define     _pHld_Eeprom_Startaddr    		(int)&(((struct   ST24256_DATACONFIG *)0)->pHld)					//
#define    	_PHld_Eeprom_Len    			sizeof(((struct   ST24256_DATACONFIG *)0)->pHld)/sizeof(char)		//
#define 	_pHld_Datatype					strHolidayTbl					///���ձ������������
#define		_cpHld_Unit_Len					sizeof(_pHld_Datatype)			//	���ձ�Ԫ�ص�Ԫ����
#define 	_cpHld_MaxNum					FlashInfo.SetInfo.FLMode[4]		//	���ձ�������
#define    _cpHld_StoreMaxNum				sizeof(((struct   ST24256_DATACONFIG *)0)->pHld)/sizeof(_pHld_Datatype)
//���ձ�Ѱַ
#define 	_Hld_Get(cno)					(int)&(((struct   ST24256_DATACONFIG *)0)->pHld[cno])
#define 	_Hld_Write(dmem,cno)			 Write_EEData((unsigned char *)(dmem),_Hld_Get(cno),_cpHld_Unit_Len	);


//���ݱ���ʼ��ַ
#define     _LTBl_Eeprom_Startaddr    		(int)&(((struct   ST24256_DATACONFIG *)0)->LTBl)					//
#define    	_LTBl_Eeprom_Len    			sizeof(((struct   ST24256_DATACONFIG *)0)->LTBl)/sizeof(char)		//
#define 	_LTBl_Datatype					strLTbl						//���ݱ������������
#define		_LTBl_Unit_Len					sizeof(strLTbl)				//	���ݱ�Ԫ�ص�Ԫ����
//���ݱ�Ѱַ
#define 	_LTBl_Get(cno)					(int)&(((struct   ST24256_DATACONFIG *)0)->LTBl[cno])
//���ݵ�۱�Ѱַ
#define 	_LTBl_LPrice_Get(cno,a)			(int)&(((struct   ST24256_DATACONFIG *)0)->LTBl[cno].LPrice[a])
//���ݱ��������Ѱַ
#define 	_LTBl_YStemp_Get(cno,a)			(int)&(((struct   ST24256_DATACONFIG *)0)->LTBl[cno].YStepTm[a])




//  ��ʾ���ַ
//	 ѭ���ʼ��ַ
#define     _DisArry_Normal_EERom_startaddr    	(int)&(((struct   ST24256_DATACONFIG *)0)->DispArry[0])			//
//	 �����ʼ��ַ
#define     _DispArry_key_EERom_startaddr    	(int)&(((struct   ST24256_DATACONFIG *)0)->DispArry[1])					//
//  ���ԣ�ѭ�Ե���Ĵ洢����
#define     _DispArry_Len_				    	( _DispArry_key_EERom_startaddr  -_DisArry_Normal_EERom_startaddr)				//
#define    _DispArry_Unit_MaxNum_				(_DispArry_Len_/sizeof(strDispCode))
// 	Ѱַ  dmem,Ŀ���ַ��showmenu��ʾ�˵������ָ��ѭ�Ի��Ǽ��ԣ�cno����ָʾ�˵����
#define 		GetExtShowMenu(dmem,showmenu,cno)    Read_EEData((unsigned char *)&(dmem),(int)&(((struct   ST24256_DATACONFIG *)0)->DispArry[showmenu][cno]),(sizeof(strDispCode)/sizeof(char)))
//  put showmenu��ʾ�˵������ָ��ѭ�Ի��Ǽ��ԣ�cno����ָʾ�˵����
#define 		PuttExtShowMenu(dmem,showmenu,cno)    Write_EEData((unsigned char *)(dmem),	(int)&(((struct   ST24256_DATACONFIG *)0)->DispArry[showmenu][cno])	,(sizeof(strDispCode)/sizeof(char)))

void Read_ELVSave(void) ;
//
#define  GetExtData(iFileno,sbuf,ilen )		 Read_EEData(sbuf,iFileno,ilen)
#define	 CopyExteeprom(des,iAddr,ilen)		 Read_EEData(des,iAddr,ilen)


//#define    bPinTemp      	RamData.noUse //P5DAT_P56OUT
//#define    bPinTemp1 		RamData.noUse //P5DAT_P55OUT

#define		_Is_MeterApp_Reset()		((RamData.PwrOnCode!= 70005270)&&(RamData.PwrOnCode!= 52707000))
#define		_Is_MeterApp_OnLine()		( 52707000 == RamData.PwrOnCode)
#define		_Is_MeterApp_OffLine()		( 70005270 == RamData.PwrOnCode)
#define 	_Set_MeterApp_OnLine()		{ RamData.PwrOnCode =  52707000;}
#define    _Set_MeterApp_OffLine()		{ RamData.PwrOnCode =  70005270;}
#define 	_IsChipReset_IO()				(!_Is_MeterApp_Reset())
#define  	_IsFactory_Mode()    		 (0 == (FlashInfo.RunInfo.EState&_ESTATE_FACOFF_))	 //????
#define 	_IsNoFactory_Mode()			(!_IsFactory_Mode())				 //????


void ADC_Init(void  ) ;
void ADC_OFF(void);
unsigned short    Get_ADC( unsigned char cChangel  ) ;

enum   ENUMSMARTCHANEL
{
	RW_SAM	 = 0 ,
	RW_KA   =1
};
//#define RW_IR		0x02
#define		_OFFLINE_EVENT_GO(a)		{Check_MS();if ( _ISPowerOff())return(a);}
#define 	_ISPowerOff()				(RamData.Event_PwrOff || RamData.bVDD_Down)
#define  	_cLVD_Filter_Tm   			RamData.cLVD_Filter_Tm
#define		_SYS_ON_LINE()				(RamData.Pwr_On )
#define		_SYS_OFF_LINE()				(!_SYS_ON_LINE())
#define  	_SYS_OFF_KEYPRESS()			( RamData.cKeyOnPress >= cKeyInput_Tm)  // ����״̬�°������
#define 	_SYS_OFF_OPEN()				( RamData.cOpenTm >=cOpenInput_Tm)	// ����״̬�¿��Ǽ��
#define 	_SYS_OFF_CLOSE()			( RamData.cCloseTm>=cOpenInput_Tm)	// ����״̬�ºϸǼ��
#define 	_ISCARCHANEL()				(RW_KA == RWCFG)
#define 	_ISESAMCHANEL()				(RW_SAM == RWCFG)

#define		_DIANLIANMOD_SET()			{RamData.bDianLianMod =1;}
#define		_DIANLIANMOD_CLR()			{RamData.bDianLianMod =0;}
#define		_ISDIANLIAN_MOD()			(  RamData.bDianLianMod )

#define		 _RSet_Safe(a,b)			{ do{ a=b;} while(a!=b);}
#define 	_Rset_Global_Val(var,value) {	INTERRUPT_DISABLE(); var =value ;INTERRUPT_ENABLE();}

#define 	_ISNoRunTask()				 (_IS_I2C_Free() && ( 0 ==RamData.Event_PwrOff) && (0 ==RamData.bGetLvd) 	)
#define		_IS_MCU_RESET()				((RamData.PwrOnCode!= 70005270)&&(RamData.PwrOnCode!= 52707000))
#define  	_IsFactory_Mode()    		 (0 == (FlashInfo.RunInfo.EState&_ESTATE_FACOFF_))	 //����ģʽ

unsigned char	Samic_Rst(enum   ENUMSMARTCHANEL	Mode);
extern unsigned char RWCFG ;
void RSet_WDG(unsigned char ctemp) ;
void Rset_cSm07816_tm(unsigned char );
void Set_VDDLO(void);
void  CheckBatVolt (void);



struct STTblSetLst {
	u8 Lth;
	u8* pTbl;
};

//union UNMETERCONFIG
#define 	_METERCONFIG_LEN_		4   //(sizeof(((struct   strFlashInfo *)0)->meterinfo)/sizeof(char))
//	?????
#define 		_ISJDMODE_OUT()		(_bMeter_JD_OUT_ == FlashInfo.meterinfo.stmeter_config.cMeter_JDMODE)
#define 		_ISJDMODE_IN()		(!_ISJDMODE_OUT())

//	????485???
#define 		_ISCOMMODE_ZB()		(_bMETER_ZB_ == FlashInfo.meterinfo.stmeter_config.cMeter_ComType)
#define 		_ISCOMMODE_485()	(!_ISCOMMODE_ZB())

// ?????????
#define 		_ISMETER_REMOTE()	(_bMETTER_REMOTE_ == FlashInfo.meterinfo.stmeter_config.cMeter_Type)
#define 		_ISMETER_LOCAL()	(!_ISMETER_REMOTE())


struct 		STIRREADDATALIST
{
	u32 	lData;
	enum  ENUMDATACONFIG  cMod;
};
#define Rtcunint_ReadTemp(a)  {}  //unsigned char Rtcunint_ReadTemp(short *sp);

#define _setdog()					{ RamData.WDGTm = cCWDTMAx ;}
#define FreeDog()					{}	//	
	
#define			_defaultbps_			0	
#define 		_GetBpsSet(a)			((a)>(sizeof(iBpsSet)/sizeof(iBpsSet[0]))?300:iBpsSet[(a)])			//	 
	
#define _cDeteTm_   				5
#define 	_OnLine()			( iTurnOnTm >= _cDeteTm_  )
#define 	_OffLine()		( iTurnOffTm >= _cDeteTm_  )


			
			#define  	_Uart_Handle_			1
#define 		_OverVolMax(a)				((a< (_VOLT_STD_*8/10))|| (a>_VOLT_STD_*115/100))
			
			
			struct 		STIO
			{
				unsigned 	short   bODispRev:1;		// �����
				unsigned  short 	bOLight:1;		// ��
			};
			
#define NOP_ASM()                             __nop				
#define _CODE_DEL_			 (0)
			#define CheckBatVolt()		{}
	 
					#define _IsDispGetData()		(RamData.InDisp)
					char 		IsMemZero(char *sp , unsigned short ilen);
						#define IslongZero(unit)		IsMemZero((char *)&unit , sizeof(long))
							
						struct STCOMCHANEL {
	unsigned char c645D0;
	unsigned char *sp;
};
						
//??????:?????0400010F,???461205BE,??????69053F84, ??????01?
//??????:?????0400010F,???461205BE,??????69053F84, ??????00,
#define 	_ISFACMODE()     (0 == (FlashInfo.RunInfo.EState&_ESTATE_FACOFF_))	 //????
#define  _PROGKEYSHOW()		( _ISFACMODE()  || ((RamData.FacProgTime ) && (RamData.Disp_BinkState)))
#define 	_INTOFACMODE_645_			( 0x0400010F )		
#define 		_cMaxProgTime		4*60  // ??????
void WDGTm_Refresh(unsigned char clen);

#define  ChkEvtState() {}
#define  SetEvtState(BitCnt)  {}
#endif		//_PUB_SET_



