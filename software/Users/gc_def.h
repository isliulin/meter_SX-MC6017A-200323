/*
 * gc_def.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef GC_DEF_H_
#define GC_DEF_H_


typedef long	s32;
typedef short s16;
typedef char	s8;

typedef unsigned long 	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;

typedef unsigned long	const uc32;  /* Read Only */
typedef unsigned short	const uc16;  /* Read Only */
typedef unsigned char	const uc8;   /* Read Only */
typedef const unsigned char ucode;	//Code Data

typedef volatile unsigned long 	vu32;
typedef volatile unsigned short	vu16;
typedef volatile unsigned char	vu8;

typedef volatile unsigned long	const vuc32;  /* Read Only */
typedef volatile unsigned short	const vuc16;  /* Read Only */
typedef volatile unsigned char	const vuc8;   /* Read Only */

typedef	unsigned int GBit;		// bit variable



#define		REV32(A)		((((u32)((A)&0xFF))<<24)|(((u32)((A)&0xFF00))<<8)|(((A)>>8)&0xFF00)|((A)>>24))		//32位高低字节交换/
#define		REV16(A)		((((A)&0xFF)<<8)|(((A)&0xFF00)>>8))													//16位高低字节交换/
#define		NUMOF(A)		(sizeof(A)/sizeof(A[0]))
#define		LABS(x)			(((s32)(x)) > 0 ? (x) : (-(x)))														//绝对值处理/

#ifdef		_BIG_ENDIAN_
	#define			X16(A)		REV16(A)
	#define			X32(A)		REV32(A)
	#define			XCopy(a, b)		RevCopy(a, b)
	#define			B16(A)		(A)
	#define			B32(A)		(A)
	#define			EDIR		0
#else
	#define			X16(A)		(A)
	#define			X32(A)		(A)
	#define			XCopy(a, b)		NormCopy(a, b)
	#define			B16(A)		REV16(A)
	#define			B32(A)		REV32(A)
	#define			EDIR		1
#endif

typedef enum	{
	GPIO_P0_0,			//0
	GPIO_P0_1,			//1
	GPIO_P0_2,			//2
	GPIO_P0_3,			//3
	GPIO_P0_4,			//4
	GPIO_P0_5,			//5
	GPIO_P0_6,			//6
	GPIO_P0_7,			//7
	GPIO_P0_8,			//8
	GPIO_P0_9,			//9
	GPIO_P0_10,			//10
	GPIO_P0_11,			//11
	GPIO_P0_12,			//12
	GPIO_P0_13,			//13
	GPIO_P0_14,			//14
	GPIO_P0_15,			//15
	GPIO_P0_16,			//16
	GPIO_P0_17,			//17
	GPIO_P0_18,			//18
	GPIO_P0_19,			//19
	GPIO_P0_20,			//20
	GPIO_P0_21,			//21
	GPIO_P0_22,			//22
	GPIO_P0_23,			//23
	GPIO_P0_24,			//24
	GPIO_P0_25,			//25
	GPIO_P0_26,			//26
	GPIO_P0_27,			//27
	GPIO_P0_28,			//28
	GPIO_P0_29,			//29
	GPIO_P0_30,			//30
	GPIO_P0_31,			//31
	GPIO_P1_0,			//32
	GPIO_P1_1,			//33
	GPIO_P1_2,			//34
	GPIO_P1_3,			//35
	GPIO_P1_4,			//36
	GPIO_P1_5,			//37
	GPIO_P1_6,			//38
	GPIO_P1_7,			//39
	GPIO_P1_8,			//40
	GPIO_P1_9,			//41
	GPIO_P1_10,			//42
	GPIO_P1_11,			//43
	GPIO_P1_12,			//44
	GPIO_P1_13,			//45
	GPIO_P1_14,			//46
	GPIO_P1_15,			//47
	GPIO_P1_16,			//48
	GPIO_P1_17,			//49
	GPIO_P1_18,			//50
	GPIO_P1_19,			//51
	GPIO_P1_20,			//52
	GPIO_P1_21,			//53
	GPIO_P1_22,			//54
	GPIO_P1_23,			//55
	GPIO_P1_24,			//56
	GPIO_P1_25,			//57
	GPIO_P1_26,			//58
	GPIO_P1_27,			//59
	GPIO_P1_28,			//60d
	GPIO_P1_29,			//61
	GPIO_P1_30,			//62
	GPIO_P1_31			//63
} GPIO_NUM;

enum {
	_USE_ECHIP_NULL_ =0,
	_USE_8209_ = 1,
	_USE_7053_ = 2,
	_USE_7053B_ = 3,
	_USE_7780_ = 4,
	_USE_GC8209_ = 5,
	_USE_8209G_ = 6,
	_USE_90E21_ = 7,
	_USE_MSP430_ = 8,
	_USE_71071_ = 9
};

				/*	当前时间的年/月/日/时/(时分(时*10+分/6))		*/
#define		TIME_YEAR	(((__DATE__[9]-'0')*16)+(__DATE__[10]-'0'))
#define		TIME_MONTH	("ihgcsaerjbdqf"[((__DATE__[2]-'a') + 14 - ((__DATE__[1]-'a')&0xFC))/3]-'a')
#define		TIME_DATE	((((__DATE__[4]-'0')&0x0F)<<4)+(__DATE__[5]-'0'))
#define		TIME_HOUR	((((__TIME__[0]-'0')&0x0F)<<4)+(__TIME__[1]-'0'))
#define		TIME_MIN	((((__TIME__[3]-'0')&0x0F)<<4)+(__TIME__[4]-'0'))
#define		TIME_HM		(((__TIME__[0]-'0')*100)+((__TIME__[1]-'0')*10)+((((__TIME__[3]-'0')*10)+(__TIME__[4]-'0'))/6))

//#define		_TIME_DAY_(a,b,c)		((((u16)(a))*365)+(((b)-1)*30)+(c)+(("bcabbccdeeff"[(b)-1])-'a')-1+((((a)*12)+(b)+45)/48))
#define		_TIME_DAY_(a,b,c)		((((u16)(a))*365)+(((b)-1)*30)+(c)+((b-1)/4)+((0x6116AF>>((12-b)*2))&3)-1+((((a)*12)+(b)+45)/48))

				/*	以下为MvData宏定义
					strMvData结构在 void	MoveDataLst(strMvData	  *pMv) 中使用并解释执行
					 	*/
typedef	struct {
	u16 pData;
	u16	Opt;
} strMvData;

typedef struct  __attribute__ ((__packed__))
{
	u32	Code;
	u8	SubCode;
} strDispCode;							//DispCode

				/*		Card File ID		*/
#define 	CMD_USERCARD		0x01							//UserCard ID
#define		CMD_SETUPCARD		0x06                            //SetupCard ID

enum {
    KEY_DN = 1,
    KEY_PROG =	2,
    KEY_UP = 3
};

enum {
	NO_ERR,
	EEP_ERR,
	RAM_ADDR_ERR,
	FLASH_ERR,
	ESAM_ERR,
	ESAM_OK,
	CARD_KEY_ERR,
	CARD_SHORT_TM,
	IDWG_ERR,
	DISP_ITEM_ERR,
	LO_PWR,
	HSE_ERR,
	LSE_ERR,
	UP_PWR,
	DATA_CHG
};

enum {
	CARD_OK_BEEP,
	CARD_ERR_BEEP,
	IR_OK_BEEP,
	ALM_BEEP
};

enum {											//Used In SetDispSubItem(u8 Item,	u8 Secs)
	CARD_IN = 0,								//
	CARD_OK = 0x100,							//
	CARD_FAIL = 0x200,							//
	TRX_OK = 0x300, 							//
	WTRX_OK = 0x400,							//
	MAX_OVER = 0x500,							//
	CHARGE_OK = 0x600							//
};

typedef	enum {
	_ERR_OTHER_ = 1,
	_ERR_NODATA_ = 2,
	_ERR_SAFE_ = 4,
	_ERR_BAND_ = 8,
	_ERR_OVER_YTB_ = 0x10,
	_ERR_OVER_TMTB_ = 0x20,
	_ERR_OVER_FL_ = 0x40
} eErr;

				/*		客户列表		*/
#define		_FAC_GC_					0x00
#define		_FAC_FUGEN_					0x01
#define		_FAC_BAIFU_					0x02
#define		_FAC_JIANGJI_				0x03
#define		_FAC_JINLING_				0x04
#define		_FAC_DIANKEYUAN_			0x05
#define		_FAC_YUBANG_				0x06
#define		_FAC_XI_OU_					0x07
#define		_FAC_XILI_					0x08
#define		_FAC_QIANNENG_				0x09
#define		_FAC_KAILI_					0x10
#define		_FAC_SHENKEJI_				0x11
#define		_FAC_JIANNAN_				0x12
#define		_FAC_RONGLIANG_				0x13
#define		_FAC_KELIN_					0x14
#define		_FAC_JINYI_					0x15
#define		_FAC_SONGXIA_				0x16
#define		_FAC_YINHE_					0x17
#define		_FAC_JINGQI_				0x18
#define		_FAC_SHANGHAIDIANBIAO_		0x19
#define		_FAC_GUANGDA_				0x20
#define		_FAC_SUYUANJIERUI_			0x21
#define		_FAC_HUIXIN_				0x22
#define		_FAC_HENGTAI_				0x23
#define		_FAC_WANTAI_				0x24
#define		_FAC_LONGDIAN_				0x25
#define		_FAC_HUIYI_					0x26
#define		_FAC_WEIJI_					0x27
#define		_FAC_SHUDA_					0x28
#define		_FAC_YIXINDA_				0x29
#define		_FAC_ZHONGNAN_				0x30
#define		_FAC_ZHONGTIAN_				0x31
#define		_FAC_HUICHUAN_				0x32
#define		_FAC_GERUIDE_				0x33
#define		_FAC_LUNENG_				0x34
#define		_FAC_HAIWAN_				0x35
#define		_FAC_OTHER_					0x99

				/*		用的报警状态定义	*/
enum {
	ALM_NORM,
	ALM_ALM1,
	ALM_ALM2,
	ALM_OVERDRAFT,				//
	ALM_OVEROFF					//
};

				/*		显示单位列表		*/
enum {
	_KWH_			= 1,
	_KVAH_			= 2,
	_KW_			= 3,
	_KVA_			= 4,
	_V_				= 5,
	_A_				= 6,
	_YUAN_			= 7
};

#define		_DISP_USE_DOTS_		0x08
#define		_DISP_NO_TARIFF_	0x10
#define		_DISP_NO_STEP_		0x20
#define		_DISP_MIN_BITS_		0x40
#define		_DISP_NO_DISP_		0x80
#define		_DISP_LTH_2_		0x100
#define		_DISP_LTH_3_		0x200
#define		_DISP_LTH_4_		0x300
#define		_DISP_LTH_5_		0x400

#ifdef		_BIG_ENDIAN_
	#define		_XBYTE_(a)		(((a)>>8)&0xFF), ((a)&0xFF)
#else
	#define		_XBYTE_(a)		((a)&0xFF), (((a)>>8)&0xFF)
#endif


#define		_MAX_TARRIF_	 4
#define		_MAX_LADDER_		6
//#define		_CHIP_SET_LEN_		60

#define 	FLASH_INFO_START	 0x60


//#pragma	pack(1)

typedef	struct {						//Get_Code_Lst Code Struct
	u16	Code;
	u8	DI0_Max;
} strChkCode;

#define		_D_DISP_CARD_IN_	0x0400F1A1					//显示插卡符号
#define		_D_DISP_ERR_		0x06060606					//显示故障代码
#define		_D_DISP_TMTBL_		0x06060607					//显示日时段表
#define		_D_DISP_REMAIN_		0x00900200					//显示剩余金额
#define		_D_DISP_VER_		0x0400f11f					//显示版本信息
#define		_D_DISP_SUMUSE_		0x00000000					//显示总用电量
#define		_D_DISP_TIME_		0x04000102					//显示当前时间
#define		_D_DISP_LPRICE_		0x0280000B					//显示阶梯电价
#define		_D_DISP_PRICE_		0x0280000C					//显示综合电价

enum {									//
	_NORM_DISP_ARRY_,					//NORM DISP
	_KEY_DISP_ARRY_,					//KEY DISP
	_PWR_ON_DISP_ARRY_,					//PWR UP DISP
	_CARD_OVER_DISP_ARRY_				//CARD OVER DISP
};

enum {
	_SFNONE_ = 0,						//
	_SF_FAC_ = 1,						//
	_SAFE_2_ = 2,						//
	_SAFE_4_ = 3,						//
	_SAFE98_ = 0x04,					//
	_SAFE99_ = 0x08,					//
	_SF_NIR_ = 0x10,					//No IR Read
	_SFNCHG_ = 0x20,					//修改密钥后不能更改
	_SFFOFF_ = 0x40,					//Fac Mode
	_SF_ROL_ = 0x80						//Read Only
};



#define		_DLTH_(a, b)		(((a)&7)|((b)<<3))		//a-645Length, b-Store Length
#define		_MAX_0_(max)		((u32)((u8 (*)(strDispCode const *, u8*))(0x80000000|((max)&0x7FFFFFF))))
#define		_MAX_1_(max)		((u32)((u8 (*)(strDispCode const *, u8*))(0x88000000|((max)&0x7FFFFFF))))
#define		_MIN_MAX_(min, max)	((u8 (*)(strDispCode const *, u8*))((u32)(0xC0000000)|((((u32)(min))&0x3FFF)<<16)|(((u32)(max))&0xFFFF)))
#define		_CONST_N_(num)		((u8 (*)(strDispCode const *, u8*))(0x90000000|((num)&0xFFFF)))
//#define		_P_PROG_(a)			(u32)(a)
#define		_P_PROG_(a)				(u32)((ucode *)a)

typedef	struct {
	u32			Use;					//向用电量(2位小数)
	u32			RevUse;					//反向用电量(2位小数)
	u16			Imp;					//向脉冲数
	u16			RevImp;					//反向脉冲数
} strSectUse;							//总共12Byte



//typedef	struct {
//	strSectUse	SectUse;				//费率用电量
//	strSectUse	RevSectUse;				//费率反向用电量
//} strFLUse;

//typedef	struct {
//   strFLUse FLUse;
//    u32 CS;
//}strFeiLvSave;

typedef	struct {
	u8			FeilvNum;				//当前费率(为对齐使用32位)
	u8			Rev;
	u16			LVCode;					//
	strSectUse	FLUse;					//费率用电数据
	strSectUse	SumUse;					//总用电数据
	u32			SumMoney;				//累计用电金额(2位小数)
	u16			CentDots;				//累计用电金额尾数(6位小数)
	u16			CS;						//校验和
} strLVSave;							//总共36Byte

typedef	enum{
	READ_ESAM = 0x00,
	WRITE_ESAM = 0x01,
	READ_IC	= 0x02,
	WRITE_IC = 0x03,
	_7816_end_,
} enumRWCFG;

typedef	struct {
	u8		Cmd[5];					//Cmd
	u8		RWCfg;					//RW Control
	u8*		Addr;					//Data pointer
} strRWSamic;						//7816 Data Struct

//#pragma	pack()





#endif /* GC_DEF_H_ */
