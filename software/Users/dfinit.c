/*
 * dfinit.c
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */
#include <string.h>
#include <stdio.h>

#include "PubSet.h"
#include "spset.h"

void	__errno(void);

extern unsigned long __END_data_lma;

/************************************************************************* */

const  strFData FData = {
	{
		"EPSON11"
#ifdef		_USE_CARD_
			"L"
#else
			"F"
#endif
		" " __DATE__ " " __TIME__,
		_AREA_NUM_,								 			//������
		_KWH_IMP_,											//�й�����
		_MN_CU_CNST_,										//��ͭ��ֵ
		_VOLT_STD_,											//��ѹ���
		{ _A_STD_1_, _A_STD_2_ },							//�������
		_G_STD_,											//�й����ȵȼ�
		_IM_TYPE_,											//�������
		_SOFT_VER_,											//����汾��
		_HARD_VER_,											//Ӳ���汾��
		_FAC_NO_,											//���ұ��
		_MAKE_TIME_,								 		//��������
		_VER_TYPE_,											//645�汾
		{
					0x77,
		#ifdef _METER_LOC_
					_bMETTER_LOCAL_,
		#else
					_bMETTER_REMOTE_,
		#endif
		#ifdef _METER_485_
					_bMETER_485_ ,
		#else
					_bMETER_ZB_ ,
		#endif
		#ifdef  _JD_MODE_OUT_
				_bMeter_JD_OUT_ ,
		#else
				_bMeter_JD_IN_ ,
		#endif
		},
		{
			0,

			_LCD_VOLT_,										//DispVolt		��ʾ��ѹ����
			_LCD_DT_,										//DispCR3		��ʾ��������
			0,
			0,

			0,

			0,
		},
		0x17800,											//CodeLth 		���볤��
		_V_TIME_,											//vTime			����ʱ��
		_SOFT_ID_,											//SoftID		���������
	},
};

#define			_DFT_YEARTBL_1_		{		\
				YTB( 1, 1, 1),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2),				\
				YTB( 6, 1, 2)	}							//��ʱ����
#define			_DFT_TMTBL_1_		{		\
				TTB( 0, 0, 1),				\
				TTB( 3, 0, 2),				\
				TTB( 6, 0, 3),				\
				TTB( 9, 0, 4),				\
				TTB(12, 0, 1),				\
				TTB(15, 0, 2),				\
				TTB(18, 0, 3),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4),				\
				TTB(21, 0, 4)  }							//��ʱ�α�
#define			_DFT_PRICETBL_1_	{		\
				PRICE(5000),				\
				PRICE(4900),				\
				PRICE(4800),				\
				PRICE(4700) }								//��۱�
#define			_DFT_LPRICETBL_1_	{		\
				PRICE(0),					\
				PRICE(0),					\
				PRICE(0),					\
				PRICE(0),					\
				PRICE(0),					\
				PRICE(0),					\
				PRICE(0)	}								//���ݵ�۱�
#define			_DFT_LUSE_			{		\
				0,							\
				0,							\
				0,							\
				0,							\
				0,							\
				0	}										//���ݵ�����
#define			_DFT_YTM_			{		\
				{ 0x99, 0x99, 0x99 },		\
				{ 0x99, 0x99, 0x99 },		\
				{ 0x99, 0x99, 0x99 },		\
				{ 0x99, 0x99, 0x99 }	}					//�������
const  u16 DftYTbl1[2][14] ={ _DFT_YEARTBL_1_, _DFT_YEARTBL_1_};
const  u16 DftTmTbl1[2][14] ={ _DFT_TMTBL_1_, _DFT_TMTBL_1_};
const  u8  DftPrice11[2][32][3] ={ _DFT_PRICETBL_1_, _DFT_PRICETBL_1_};

const  u16 DftYTbl[14] = _DFT_YEARTBL_1_;
const  u16 DftTmTbl[14] = _DFT_TMTBL_1_;
const  u8  DftPrice[32][3] = _DFT_PRICETBL_1_;
const  strLTbl DftLTbl = {
	_DFT_LUSE_,
	_DFT_LPRICETBL_1_,
	_DFT_YTM_,
	{ 0 },
};

const  strFlashInfo DftFlashInfo __attribute__  ((aligned(1024),used)) = {
//	{ 0},
	FLASH_INFO_START,										//	StartCode
	0,														//	SNum
//	{0},
//	{0},
//	{0},
												//	ChipSet

//	{0,},   // strSectUse pFLUse[_MAX_TARRIF_];
//	{ 0, 0, 0, 650, -9101, -990 },							//	RTCSet(71315��һ��)
	{ 0, 0, 114, 433, -6067, -660 },						//	RTCSet(71315�ڶ���)
	{
			0x77,
#ifdef _METER_LOC_
			_bMETTER_LOCAL_,
#else
			_bMETTER_REMOTE_,
#endif
#ifdef _METER_485_
			_bMETER_485_ ,
#else
			_bMETER_ZB_ ,
#endif
#ifdef  _JD_MODE_OUT_
		_bMeter_JD_OUT_ ,
#else
		_bMeter_JD_IN_ ,
#endif
			},														//�������ò���
	{														//	SafeInfo
		0, 0,												//	Cmd, UserType
		{ 0 },												//	H_Num
		{ 0, 0, 0, 0, 0, 1 },								//	E_Num
		{ 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 },				//	TRx_Num
		500, 100,											//	Alm1, Alm2
		1, 1,												//	I_Mul, V_Mul
		20000, 0,											//	SumCharge, ChargeCnt
		0,													//	DisCharge
		0, 0, 0, 0										//	StartTm_1/2/3/4
	},
	{														//	RunInfo
		{ 0, 0, 0x80, 0x11, 0, 0, 0, 0x1f },				//	IC_Num
	//	0,													//	NKeyState
	//	{ 0 },												//	LastEvtState
	//	{ 0 },												//	EvtCnt
			FLASH_INFO_START,										//	StartCode
		0,													//	LastTmDay
		{ 0 },												//	Rev1
		{ 0 },												//	KeyErrCnt
		0,													//	MacErrCnt
		0,													//	BatUseTime
		0, 0,												//	LastUse, LastYUse
		_TIME_DAY_(20, 1, 1)*((u32)(24*60)),				//	LastMFrozeTm
		_TIME_DAY_(20, 1, 1)*((u32)(24*60)), 				//	LastYFrozeTm
		_TIME_DAY_(20, 1, 1)*((u32)(24*60)),				//	LastDFrozeTm
		{ 0 },												//	ComUseDiff

		0,													//	ProgTime;
		0,													//	ProgWrite
		{ 0 },													//	ProgCode[10];
		0,													//	ProgCodeCnt;

		0,													//	ErrCardCnt
		1,													//	YTbl_Num
		0,													//	ActTblNum
		0,													//	FarJDPreTm
		{ 0 },												//	Rev2
		0,													//	LStep
		0,													//	StepPrice
		0,													//	FLPrice
		0,													//	ePrice
		0,		 											//	RevUseTime,
		0, 0, 0,											//	OverPfphTime, OverIphTime, OverPwrTime
		0,													//	LastJDWriter
		{ 0, 0, 0, 0, 0, 0, 0,0 },							//	RcdTm
		_ESTATE_JD_STATE_|_ESTATE_LAST_STATE_,				//	EState
			0x16,													//	End
	},
	{														//	SetInfo
		0,													//	EKeyState
		1000, 99999999, 200, 								//	OverDraftLimit, MaxPurse, JDOnMinPurse
		_OVERPWR_LMT_,										//	OverPwrLimit
		_REV_LMT_,											//	RevPwrLimit
		30000,												//	JDWaitAM
		_HFROZE_START_,										//	HourFrozeStartTm
		_FHFROZE_START,										//	FuheStartTm
		{ {0x01,00}, {0x99, 0x99}, {0x99, 0x99} },			//	FrozeDT
		0,													//	FrozeHM
		0,													//	JDOffDelayTm
		1440,													//	AMOffDelayTm

		0,													//	OverVoltLimit
		0,													//	OverPhLimit
		_A_STD_2_,											//	OverIphLimit
		_BAT_LO_VOLT_,										//	BatLoLmt
		{ {0}, {2,0x56,0x34,0x12}, { 0x04, 0x78, 0x56, 0x34 } },		//	TRxKey
		20,													//	KeyNum
		0,													//	RunType
		{ 0x0, 0x04, 0x10, 0, 0, 0, 0x02, 0 },					//	EvtMode
		{ 0x04, 0x08, 0x08 },								//	Baud
	#ifdef		_USE_CARD_
		{ 7, 27 },//27 },									//	DispLength
	#else
		 { 5, 23 },											//	DispLength
	#endif
		"",													//	F_Num
		{ 0 },												//	GPSCode
		60,													//	OverVoltTm
		60,													//	OverPwrTm
		60,													//	OverPhTm
		60,													//	OverIphLimit
		10,													//	RevPwrTm
		60,													//	HourFrozeStep
		30,													//	IRAccessTmSet;
		30,													//	EvtClrTmSet;
		{0x99, 0x99, 0x12, 0},								//	FrozeCode
		0x0F,												//	FHMode
		{ 0x83, 0x83, 0x83, 0x03, 0x83 },					//	FrozeMode
		{15, 15, 15, 15, },									//	FHTmStep
		0x5,												//	����й�������(ActUseMode)
		{ 2,												//	FLMode	YTbl Num
		  2,												//			DTbl Num
		  8,												//			TTbl Num
		  4,												//			FL Num
		  0,												//			Hld Num
		  0,												//			Reverse
		  0,												//			LStep Num
		},
		0x7F,												//	WeekMode
		1,													//	WeekTblNum
		2,													//	DispEnergyDots
		4,													//	DispPowerDots
		5,													//	AutoDispTm
		5,													//	DispAllTm
		{ 0 },												//	Rev
	},

	{ 0,0,0,0,0 },													//	FrozeCnt

	0x16,													//	End

};
 

#define _FSET_BIAS_(F) ((u16)(((u8*)(&vFileSet.F))-((u8*)&vFileSet.FacKey)))

const  strFileSet FileSet = {
	#define	_FILE_SET_DATA_
    #include "FileSet.h"
	#undef	_FILE_SET_DATA_
};



