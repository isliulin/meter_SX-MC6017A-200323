#ifndef  _BIT_EDGE_HPP
#define  _BIT_EDGE_HPP

 

#include "ht6xxx.h"
#include "datastruct.h"
#include "unit_instance.h"
#include "system_ht6xxx.h"
#include "ht6xxx_lib.h"
#include "emu.h"


void DTbitSave (void);
int DTbitGet (void);

struct STBITSTATU		// λ״̬����
{
  unsigned char ctemp:6;
  unsigned char bCurr:1;
  unsigned char bPrev:1;

};

struct STTIMERSATU		//  ��ʱ������
{
  unsigned char timer:5;
  unsigned char bin:1;
  unsigned char bCurr:1;
  unsigned char bPrev:1;
};

union unSTBITSTATU		// �ڲ�״̬λ����ʱ������
{
  struct STTIMERSATU sttimerstatu;
  struct STBITSTATU stbitstatu;
  unsigned char cbit;
};
 

#define SetBOld(n)					 stbitp[n].stbitstatu.bPrev = 1;
#define ClrBOld(n)					stbitp[n].stbitstatu.bPrev = 0;
#define cGetBOld(n)					( stbitp[n].stbitstatu.bPrev )
#define cparToBOld(n,y)			{ if ( y )   {SetBOld(n);} else {ClrBOld(n); }}

#define SetB(n)	stbitp[n].stbitstatu.bCurr  = 1;
#define ClrB(n)					stbitp[n].stbitstatu.bCurr = 0;
#define cGetB(n)				( stbitp[n].stbitstatu.bCurr   )
#define IsUpB(n)				(( stbitp[n].stbitstatu.bCurr  ) && (!( stbitp[n].stbitstatu.bPrev  )))
#define IsDownB(n)		 	(( stbitp[n].stbitstatu.bPrev ) && (!( stbitp[n].stbitstatu.bCurr  )))
#define IsChangeB(n)  	(( stbitp[n].stbitstatu.bPrev ) != (( stbitp[n].stbitstatu.bCurr )))

#define cparToB(n,y)		{  stbitp[n].stbitstatu.bCurr  = (_Bool)(y);}
#define  NotB(n)  			{ stbitp[n].stbitstatu.bCurr = !stbitp[n].stbitstatu.bCurr;}
#define	TureON(n)        (( stbitp[n].stbitstatu.bPrev  ==  stbitp[n].stbitstatu.bCurr) && (stbitp[n].stbitstatu.bCurr))
#define	TureOFF(n)        (( stbitp[n].stbitstatu.bPrev  ==  stbitp[n].stbitstatu.bCurr) && (0 == stbitp[n].stbitstatu.bCurr))
void stBedge (unsigned short  iNo, unsigned short  clen);
void timeclock_clear (void);	//  ���ı�־�ɼ�
void timeclock_get (void);	//      �����ж�COPY��������Ԫ
void bitstruct_init (void);	// λ��ʼ��

//#define  bitRefreash(n,y)     cparToB(n,y)

//�궨�����ڴ���λ�ṹ������֮��Ľ�λ 
#define		stBitnewtoold_fun  stBedge(0,_cn_end_total_bit );
//#define               stBitnewtoold_fun  stBedge(_cn_start_meter_state,_cn_end_total_bit-_cn_start_meter_state);

 
	 
 enum ENUMDATATYPE
	 {
		_lAddr_ = 0,
		_Func_,
 };
struct 	STDATALIST
{
		unsigned short		iDataNo;			//	 ���ݱ�ʶ
		unsigned long			lData;				//	����
		enum ENUMDATATYPE			cDataSet;			//	��������
		unsigned char			cShowType;		//	��ʾ����
};




	
enum   ENUMADDRTYPE
{
		_addr_revser_ = 0,
		_addr_flash_  ,
		_addr_emu_  ,
		_addr_memory_  
};

enum ENUMEMUFUN
{
		_Set_NoralMode_=0,
		_Set_XiaobMode_,
		_Set_XiaoSave_,
		_Set_XiaobIB_,
		_Set_Xiaob5l_,
		_Set_Xiaob5persent_,
		_Meter_Clear_,			//	 �����0 
		_Get_Electr_,  //6
		_Read_DianLian_
};
		
#define _OnLine_P()			{ HT_GPIOC->PTDAT|=0x4;}
#define _OffLine_P()			{ HT_GPIOC->PTDAT &=~(0x4);}
#define	_Change_p()			{ HT_GPIOC->PTTOG =0x4;}

#define 	_SaveEng_Start()	{ HT_GPIOC->PTDAT |=0x8;}
#define _SaveEng_End()			{ HT_GPIOC->PTDAT &=~(0x8);}

#endif
