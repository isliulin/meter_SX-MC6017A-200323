/*
 * ioset.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef IOSET_H_
#define IOSET_H_

 
#include "lcdio.h"
#include "gc_def.h"

#define INTERRUPT_DISABLE()     __disable_irq()
#define INTERRUPT_ENABLE()      __enable_irq()
#define 	_disableInterrupts()  __disable_irq()
#define   _enableInterrupts()	 __enable_irq()



enum JDSTAU
{
	_TURNOFF_ =0,//_OFFLINE_=0,
	_TURNON_ =1 ,//_ONLINE_=1,

};

enum ENUMMETERSTATU
{
	_OffLine_ =0,
	_OnLine_ =1,
};
/*
enum ENUMINTERRUPTSTATE
{
	_off_  =0 ,
	_on_  =1 ,
};

*/


#define 		Disp_LedOn()		{ HT_GPIOA->PTSET |= 0x20;} 
#define 		Disp_LedOff()	  { HT_GPIOA->PTCLR |= 0x20;} 


				/*		ͨѶ��		*/


#define		P1_30_D			0					//�ز�ͨѶ
#define		P1_30_O			0
#define		P1_30_D_OFF		0
#define		P1_30_O_OFF		0
#define		Pin_RX_2				GPIO_P1_30
#define		SCON_ZB			SCON2
#define		SBUF_ZB			SBUF2
#define		SMD_ZB			SMD2
#define		BAUD_ZB			PR2
#define		ACK_RI_ZB		ACK_RI2
#define		ACK_TI_ZB		ACK_TI2

#define		P1_31_D			0
#define		P1_31_O			0
#define		P1_31_D_OFF		0
#define		P1_31_O_OFF		0
#define		Pin_ZB_TXD		GPIO_P1_31

//����ͨѶ
#define 		IR_Pluse_In				GC_ReadIO(  IO_IR_RX3    )


#define		P0_13_D			0					//�ز��¼����
#define		P0_13_O			0
#define		P0_13_D_OFF		0
#define		P0_13_O_OFF		0
//#define		Pin_ZB_EVT		GPIO_P0_13

#define		P0_14_D			0					//�ز�״̬����
#define		P0_14_O			0
#define		P0_14_D_OFF		0
#define		P0_14_O_OFF		0
//#define		Pin_STA			GPIO_P0_14

#define		P0_25_D			0					//�ز���λ���

//#define		Pin_ZB_Rst			GPIO_P0_25

#define		P1_28_D			1					//�ز���Դ����
#define		P1_28_O			0
#define		P1_28_D_OFF		1
#define		P1_28_O_OFF		0
//#define		Pin_ZB_Pwr			GPIO_P1_28

				/*		��������			*/
#define		Pin_EOpen			IO_OPEN					            //���Ǽ�����룬�ϸ�״̬1
#define		Pin_Eopen_INT		P1INTCTL_P15EDGE
#define   _BioOpen		        (GC_ReadIO(Pin_EOpen)?0:1)			//  �����¼�Ϊ��


#define		Pin_Key		        IO_UP					            //���԰�������
#define     _BioKey		        (GC_ReadIO(Pin_Key)?0:1)


#define		Pin_ICKK 				GC_ReadIO(IO_ICKK)     		//IO_ICKK			                    //�̵����������
//#define   Pin_Ickk_Edge	    P0INTCTL_P04EDGE
//#define		Pin_Ickk_Int_On()	{P0INTCTL_P04IE =1;}
//#define		Pin_Ickk_Int_Off()	{do { P0INTCTL_P04IE =0;} while(P0INTCTL_P04IE );}

enum   ENUM_JDPLuseWIDTH
{
	_iJDPLuseWidth_130V_	= 3563,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_140V_	= 4187,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_150V_	= 4742,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_160V_	= 5325,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_170V_	= 5547,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_180V_	= 5800,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_190V_	= 6064,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_200V_	= 6269,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_210V_	= 6473,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_
	_iJDPLuseWidth_220V_	= 6640,			//150V�͵�ƽ��ȵ�ֵ����ѹ����150V����ȴ���_iJDPLuseWidth_150V_


};

#define		_cJDPLUSETM_ 		8			//	 ����������ʱ

				/*		��Դ���			*/
//#define		Pin_BAT_V			P2DAT_P20OUT    	//��طŵ�

//#define 	Pin_BAT_V_OEN		P3IOEN_P30OEN
//#define 	Pin_BAT_V_IEN		P3IOEN_P30IEN
//#define		ADC_BAT			0
//#define		ADC_LVD			7					//LVD���ͨ��
//#define		ADC_LVD_2		1					

#define     ADC_LVD_2       0   // VIN1         //�ز���Դ���ͨ��
#define     ADC_BAT         2   // VIN3
#define     ADC_VREF        6   // VIN7
#define   	ADC_LVD         11  // VIN12
#define     ADC_RTCT        12  // VIN13
#define     ADC_VCC_MCU     13  // VIN14


#define 		cFullNull		*((char *)&lFullNull) // �ղ���



	//���������
#define		Pin_Spk_start()				{TIMER_Start(TIMER3);}	//{P4MODSEL_P42SEL  =1;}
#define		Pin_Spk_end()					{ TIMER_Stop(TIMER3); } //P4MODSEL_P42SEL  =0;}

#define		Pin_SPK			GPIO_P0_24

#define		P1_29_D			1					//�������


				/*		������·			*/
 
#define 		_Pin_Cf_in_Out_Set()			 GPIO_Init_Pin(IO_CF_IN, GPIO_Mode_OUT, GPIO_Pu_WEAK, Bit_SET);
#define 		_Pin_Cf_in_Out_ReSet()	 	 GPIO_Init_Pin(IO_CF_IN, GPIO_Mode_OUT, GPIO_Pu_NOPULL, Bit_RESET);
#define 		_Pin_Cf_in_In_Set()			 	GPIO_Init_Pin(IO_CF_IN, GPIO_Mode_IN, GPIO_Pu_WEAK, Bit_RESET);
				/*		ESAM IC		*/
 void	Init_MCU(void);											//InOut.c
void	Do_PwrOff(void);										//InOut.c

void	Init_EChip(void);										//CE_Chip.c
void	Close_EChip(void);

u8		TRx_EChip(u8 *pData);

void	Init_System(void);										//Drv_Sys.c
//void	Wait_Rst(u8	RstType);									//Drv_Sys.c

void	Do_ChkVolt(void);										//Drv_ADCJD.c
void	ADC_On(void);
void	ADC_Off(void);
//u16		Get_LVDVolt(void);
void	Chk_ZBVolt(void);
//void	SetJD(u8 NewState);
#define SetJD( NewState)  {}
void	Do_JDTask(void);
void	ReDrvJD(void);

 void	Init_Disp(void);										//Drv_DispKey.c

#define  Close_Disp()		offLcd8aDisplay()												//Drv_DispKey.c
void	Chk_KeyValue(void);										//Drv_DispKey.c
void	Init_PwrOffKey(void);									//Drv_DispKey.c
u8		Get_PwrOffKey(void);									//Drv_DispKey.c

#ifdef _DEBUG_
	#define		_SMC0_DEBUG_PIN_	() //P1DAT_P14OUT 		// 5MS�ж�
	#define		_SMC1_DEBUG_PIN_	()//P0DAT_P02OUT 		//  ʾ���������ź�
	#define		_SMC2_DEBUG_PIN_	()//P1DAT_P13OUT    	//	�̵�������
	#define		_SMC3_DEBUG_PIN_	()//P1DAT_P12OUT	  	//	�̵�������
	#define		Bklt_On()			{} //{P1DAT_P14OUT=1;}	 	//asm("nop")  //GC_ResIO(Pin_BKLT)
	#define		Bklt_Off()			{} //{P1DAT_P14OUT=0;}		//asm("nop")  //GC_SetIO(Pin_BKLT)
	#define		JDLED_On()			NOP_ASM() //asm("nop")  //GC_ResIO(Pin_JD_LED)
	#define		JDLED_Off()			NOP_ASM() //asm("nop")  //GC_SetIO(Pin_JD_LED)
	#define		Pin_JDOn			cFullNull    	//�̵�������
	#define		Pin_JDOff			cFullNull	  	//�̵�������
#else
	#define		_SMC0_DEBUG_PIN_	() //P1DAT_P14OUT	//  ʾ���������ź�
	#define		_SMC1_DEBUG_PIN_	()					//  ʾ���������ź�
	#define		_SMC2_DEBUG_PIN_	()  	//�̵�������
	#define		_SMC3_DEBUG_PIN_	()	  	//�̵�������
	#define		Bklt_On()				{} //GC_SetIO(IO_BKLT)   //{P1DAT_P14OUT=1;}	 	//asm("nop")  //GC_ResIO(Pin_BKLT)
	#define		Bklt_Off()			{} //GC_ResIO(IO_BKLT)  //{P1DAT_P14OUT=0;}		//asm("nop")  //GC_SetIO(Pin_BKLT)
	#define		JDLED_On()			{} //GC_SetIO(IO_TZ_LED) //{P0DAT_P03OUT=1;}		////��բLED asm("nop")  //GC_ResIO(Pin_JD_LED)
	#define		JDLED_Off()			{} //GC_ResIO(IO_TZ_LED) //{P0DAT_P03OUT=0;}		////��բLEDasm("nop")  //GC_SetIO(Pin_JD_LED)
	//#define		Pin_JDOn			P1DAT_P13OUT    	//�̵�������
	#define     Pin_JDOn            IO_JDQ1A          
    //#define		Pin_JDOff			P1DAT_P12OUT	  	//�̵�������
    #define     Pin_JDOff           IO_JDQ1B
#endif
 

//LVD�������־ֵ
#define 		_iADC_VOLREF_		 1240  //	 VDDֵȡ��
#define 		_LVD_220V_			1620 //3119 //2827 //2731	//2089
#define 		_LVD_150V_			1021 //2234//1720//1790   	// 2800			//	150V
#define 		_LVD_133V_			866 //ǰ��ֵ�ǲ� ��ֵ2000//1525	//	2350
#define 		_LVD_131V_			845  //1900//1480	//	130v
 
#define GC_ResIO(IO_Num)    {  GPIO_ClearPin(IO_Num);   }
 
#define GC_SetIO(IO_Num)    {  GPIO_SetPin(IO_Num);   }
 

void	Init_GPIO(u8 Mode);										//Drv_IOI2C.c
 

void	IDle(void);
void	Init_TIMER(void);
void	Chk_ICKK(void);
void	Init_TB0(u8 Mode);										//TB0��ʼ��	Mode: 0-����38k	1-����
void	SetTBOC(void);
void	WaitTBOC(u16 Tm);
void	DelayTBOC(u16 Tm);

	 
u8		Samic( strRWSamic const *pCmd);							//Drv_Tm7816.c
#define	cSamic(pCmd)	Samic((strRWSamic *)((pCmd)))
void	Card_Over(void);

//pxopt bool	RTC_Busy_Wait(u8 Mode);

//void	Init_RTC(void);											//Drv_RTCSPK.c
void	SetRTCOut(void);										//Drv_RTCSPK.c
void	ChkRTCCal(void);
void	GetTempVBat(void);
#define		GetTemperature()	GetTempVBat()
//u32		GetRTCCnt(void);
#define GetRTCCnt()    GetTimeCnt((char *)NULL, S_RTC | T_SEC);
u8		SetRTCTrim(s16	Diff, u8 Mode);
void	Check_WakeMode(void);
void	EnterLowPwrMode(u8	WakeTm);
 
void	ChangeTime(u32 NewCnt);
void	CheckTime(void);
void	StartBeep(u8 Mode);
void	Do_BeepTask(void);

void	SetEChipCal(void);										//CE_Chip.c

void	ZB_STA(void);

void	SCIProg(void);
void	MCU_Rst(void);

struct   STSVD_DATASTRU
{
	unsigned int iSVD_On_Line_Down ;		//  �������
	unsigned int iSVD_On_Line_Up ;			//  �������

};

enum ENUMZBSTATE
{
		_zb_free_ =0,
		_zb_busy_=	1

};

  
   
#define		Set_EVTOut()	{} //GC_ResIO(IO_PLC_EVENC)  	//�ز��¼����{GPIO_Init_Pin(IO_PLC_EVENC, GPIO_Mode_IN, GPIO_Pu_WEAK, Bit_SET);} //
#define		Clr_EVTOut()	{} //GC_SetIO(IO_PLC_EVENC) 		//�����ز��¼���� {GPIO_Init_Pin(IO_PLC_EVENC, GPIO_Mode_OUT, GPIO_Pu_NOPULL, Bit_RESET);}//
#define 	ZB_State()		(GC_ReadIO(IO_PLC_STA)?_zb_busy_:_zb_free_)   //((P0DAT_P07IN)?_zb_busy_:_zb_free_)
#define		ZB_Rst()			GC_SetIO(IO_PLC_RST )		//{	GC_ResIO(Pin_ZB_Rst); RamData.ZBBit = 0;	}		//�ز���λ
#define		ZB_RstOff()		GC_ResIO(IO_PLC_RST ) //{P3DAT_P31OUT=1;}	//{	GC_HizIO(Pin_ZB_Rst); RamData.ZBBit = 1;	}		//�����ز���λ

#define		ZB_PwrOn()		NOP_ASM()		//{	GC_SetIO(Pin_ZB_Pwr);	}
#define		ZB_PwrOff()		NOP_ASM()		//{	GC_ResIO(Pin_ZB_Pwr);	}
//#define		EvtOutTest()	{	GC_SetIO(Pin_ZB_EVT);	}		//�ز�����


#define			_V_REF_			(*(u32*)(FlashInfo.ChipSet+2))		//��ѹУ��ֵ
#define			_I_REF_			(*(u32*)(FlashInfo.ChipSet+6))		//����У��ֵ
#define			_PH_ADJ_		(*(u32*)(FlashInfo.ChipSet+10))		//��λУ��ֵ
#define			_QW_REF_		(*(u32*)(FlashInfo.ChipSet+14))		//С�ź�ƫ��
#define			_QI_REF_		(*(u32*)(FlashInfo.ChipSet+18))		//����ƫ��
#define			_IZ_REF_		(*(u32*)(FlashInfo.ChipSet+22))		//���ߵ���У��
#define			_QZ_REF_		(*(u32*)(FlashInfo.ChipSet+26))		//���ߵ���ƫ��
#define			_REF_VOLT_		((u16*)(FlashInfo.ChipSet+30))		//��ѹӰ��У��(2*4�ֽ�)



#define			_DFT_CHIP_SET_		{		\
	0x34, 0x12,								\
	0, 0x40, 0, 0,							\
	0, 0x40, 0, 0,							\
	0, 0, 0, 0,								\
	0, 0, 0, 0,								\
	0, 0, 0, 0,								\
	0, 0x40, 0, 0,							\
	0, 0, 0, 0,								\
	0, 0, 0, 0,					\
	0, 0, 0, 0,				}										//ȱʡУ������
#define		_DFT_CHPSET_LTH_		30
	
#define			_DFT_RTCTM_SET_		{		0,0,}			//	 ȱʡʱ�ӵ�У����
	

//void SP_Config(unsigned char  cSecSignOut) ;
#define  SP_Config(a) {} //unsigned char  cSecSignOut) ;

//	�����Ŀ���
#define 		main_sequential_on()		SystemTick_Interrupt_On()
#define 		main_sequential_off()		SystemTick_Interrupt_Off()


#define 		_main_sequential_start_		6
	
#define     BAT_V_SETGPIO_HIGH()	GPIO_Init_Pin(IO_BAT_V  ,GPIO_Mode_OUT, GPIO_Pu_WEAK, Bit_SET)
#define     BAT_V_SETANAOG_IN()	  GPIO_Init_Pin(IO_BAT_V  , GPIO_Mode_AN, GPIO_Pu_WEAK, Bit_SET);

#define 		GC_ADC_Init()   {}
	#define 		GC_GPIO_Init {} 
 

void delay (uint32_t t);
#define 		InitRTCTemp() {} 
#define 		Init_Disp() {} 
#define 		Reset_Option_No_Init_Ram() {} 

#define 		SetF32K_out() {} 
#define 		Stop_count_ICKK() {} 
#define 		SystemTick_Init() {} 
 
#define 		Usart_Mem_init() {} 



#define 		offLcd8aDisplay() {} 
#define 		presetT16Ch1Count(a) {} 
#define 		resetWdt() {} 
#define 		startWdt() {} 
#define 		stjdpluse() {} 
#define 		stopClgOSC3() {} 
#define 		switchClgSystemClockIOSC() {} 
#define 		switchClgSystemClockOSC3() {} 
	
#endif /* IOSET_H_ */
