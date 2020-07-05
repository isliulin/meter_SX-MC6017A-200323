/*
 * gc_5_60_1200.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef GC_5_60_1200_H_
#define GC_5_60_1200_H_

#define		_FAC_				0
#define		_FACKEYNUM_			0

#define		_USE_ECHIP_		_USE_ECHIP_NULL_  //	_USE_71071_

#define		_USE_SET_BAUD_

#define		_USE_EEPROM_
#define		_USE_I2C_

#define		_TEL_NUM_			0
//#define _DEBUG_
//#define _METER_LOC_


#ifdef		_METER_LOC_
	#define		_USE_CARD_
	#define		_USE_UINT_	_YUAN_
	#define		_USE_ZB_					//ʹ���ز�ģ��
#endif

#define		_USE_RTC_
//#define		_USE_32768_

#define		_KEY_DISP_TM_		60
#define		_OFF_DISP_TM_		30

#define		_HFROZE_START_		(_TIME_DAY_(10,1,1)*86400)
#define		_FHFROZE_START		0x01010000

#define		_BAT_RECHG_VOLT_	300
#define		_BAT_LO_VOLT_		260			//��ص͵�ѹ����(2.8V)

#define		_CARD_OVER_DLTH_	2
#define		_CARD_OVER_D1_		0x009002FA
#define		_CARD_OVER_D2_		0x00900200
#define		_CARD_OVER_D3_		0
#define		_CARD_OVER_D4_		0

#define  	_iLVD_Lo_ 				(_LVD_131V_-300)  //  ������й����У���λ��ѹ
#define		_LVD_RST_			_LVD_131V_			//ESAM ����������ѹ  LVD���縴λ��ѹ
#define		_LVD_START_			(_iLVD_Lo_+200)			//LVD������ѹ
//#define		_LVD_SAVE_			56			//LVD�����ѹ

//#define		_VDD_LCD_OFF_		220			//ͣ���ֹ��ʾ��ѹ
//#define		_VDD_PWR_OFF_		310			//VDD�������ѹ
//#define		_VDD_RST_			250			//VDD��λ��ѹ

#define		_LO_VOLT_LV_		1500		//150V������Ϊ�͵�ѹ

#define		_AREA_NUM_		(((u32)_M_MODE_<<24)|(((u32)_FAC_)<<16)|((_USE_ECHIP_&0x0F)<<12)|_TEL_NUM_)			//������


#define		_KWH_IMP_			1600
#define		_MN_CU_CNST_		300			//��ͭ��ֵ
#define		_V_BEILV_			1100		//��ѹ˥������

#define		_VOLT_STD_			2200		//��ѹ���
#define		_A_STD_1_			50
#define		_A_STD_2_			600			//�������
#define		_G_STD_				20			//���ȵȼ�
#define		_REV_LMT_			44			//��������(4.4W)
#define		_OVERPWR_LMT_		0

#define		_LCD_VOLT_			4			//LCD��ʾ��ѹ
#define		_LCD_VOLT_OFF_		1			//ͣ��LCD��ʾ��ѹ
#define		_LCD_DT_			1			//��ʾ��������

#define		_TRX_TYPE_			1			//�ز���
#define		_JD_REV_STATE_		0			//�̵������Ʒ���
#define		_DB_DIR_			0			//DoubleDir		˫�����


#define		_USE_CARD_BEEP_					//��������

//�����ز���DDZYXXXC-M
#define		_IM_TYPE_CARRY_			"DDZYXXXC-M"	//�������
#define		_SOFT_VER_CARRY_		"DDZYXXXC-M"	//����汾��
#define		_HARD_VER_CARRY_		"DDZYXXXC-M"	//Ӳ���汾��

//����485��DDZYXXXC
#define		_IM_TYPE_485_		"DDZYXXXC"	//�������
#define		_SOFT_VER_485_		"DDZYXXXC"	//����汾��
#define		_HARD_VER_485_		"DDZYXXXC"	//Ӳ���汾��


//Զ���ز���DDZYXXX-M
#define		_IM_TYPE_CARRY_FAR_		"DDZYXXX-M"	//�������
#define		_SOFT_VER_CARRY_FAR_	"DDZYXXX-M"	//����汾��
#define		_HARD_VER_CARRY_FAR_	"DDZYXXX-M"	//Ӳ���汾��

//Զ��485��DDZYXXX
#define		_IM_TYPE_485_FAR_		"DDZYXXX"	//�������
#define		_SOFT_VER_485_FAR_		"DDZYXXX"	//����汾��
#define		_HARD_VER_485_FAR_		"DDZYXXX"	//Ӳ���汾��


#ifdef _METER_LOC_

	#ifdef _METER_485_
		#define		_IM_TYPE_		_IM_TYPE_485_		//	"DDZYXXXC-M"	//�������
		#define		_SOFT_VER_		_SOFT_VER_485_	//	"DDZYXXXC-M"	//����汾��
		#define		_HARD_VER_		_HARD_VER_485_	//	"DDZYXXXC-M"	//Ӳ���汾��
	#else
		#define		_IM_TYPE_		_IM_TYPE_CARRY_		//	"DDZYXXXC-M"	//�������
		#define		_SOFT_VER_		_SOFT_VER_CARRY_	//	"DDZYXXXC-M"	//����汾��
		#define		_HARD_VER_		_HARD_VER_CARRY_	//	"DDZYXXXC-M"	//Ӳ���汾��
	#endif
#else
	#ifdef _METER_485_
		#define		_IM_TYPE_		_IM_TYPE_485_FAR_		//	"DDZYXXXC-M"	//�������
		#define		_SOFT_VER_		_SOFT_VER_485_FAR_		//	"DDZYXXXC-M"	//����汾��
		#define		_HARD_VER_		_HARD_VER_485_FAR_		//	"DDZYXXXC-M"	//Ӳ���汾��
	#else
		#define		_IM_TYPE_		_IM_TYPE_CARRY_FAR_		//	"DDZYXXXC-M"	//�������
		#define		_SOFT_VER_		_SOFT_VER_CARRY_FAR_	//	"DDZYXXXC-M"	//����汾��
		#define		_HARD_VER_		_HARD_VER_CARRY_FAR_	//	"DDZYXXXC-M"	//Ӳ���汾��
	#endif
#endif




#define		_FAC_NO_		"XXXX"			//���ұ��
#define		_VER_TYPE_		"DL/T645-2007-14"
/*#define		_MAKE_TIME_		{	(DateYear/10%10)+'0',			\
								(DateYear%10)+'0',				\
								(DateMonth/10%10)+'0',			\
								(DateMonth%10)+'0',				\
								(DateDay/10%10)+'0',			\
								(DateDay%10)+'0' }			 			//��������
*/
#define		_MAKE_TIME_		"2014-XX-XX"
#define		_V_TIME_		{ 0x14, 0x12, 0x25, 1 }									//����ʱ��

#define		_SOFT_ID_		{ 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA }		//���������

#define		_DFT_FL_			1			//	Ĭ�Ϸ��ʼ�
#define		_SPK_4K_						// 4K���Ʒ�����

#define		_REV_DELAY_	1
#define		_IMP_DELAY_	4

#define _YUAN_			 7


#endif /* GC_5_60_1200_H_ */
