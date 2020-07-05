//#include "system.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"

#include "uartio.h"
 
#include "eeprom.h"
extern strRamData RamData;
extern strEx645 Ex645;
extern strFlashInfo FlashInfo;
unsigned char cIRChanel_Init(unsigned char cPoweron);
#ifdef _DEBUG_
extern unsigned char cWDTEN;
#endif
 
const unsigned char _cPERRJD_DETE_TM = 60; // 检测延时 ，总时间等于_cPERRJD_DETE_TM *5

void MCU_OffLIne_Init(void);
void Do_PwrOff(void) {
	#ifdef _DEL
	extern struct STJDPLUSE stjdpluse;
	extern volatile unsigned short iTime_Isr_no;

	unsigned char cMaxPluse = ((unsigned short) _cPERRJD_DETE_TM * _cMain_sequentialTime_)/ (2 * 20); // 超过检测时间应用脉冲个数一半的个数
	unsigned short iOnLineDete = (cMaxPluse * (20 -10)) / _cMain_sequentialTime_; //符合上电条件检测次数

	unsigned char cOver150V;

	unsigned char cSVDDlow = 0;
	unsigned char csVDDhigh = 0;
	unsigned char csLVDLow =0;
	unsigned char cReadp = 0, cst20msplusep, cjdpluseTm, cJDDeteTm,	ipluseintno  ;

	unsigned int itemp, iTime_Isr_no_work;
	unsigned int iTime_Isr_no_bak = iTime_Isr_no;

	//initSvd(_SVD_VDD_, _iSVD_On_Line_WorkVol);
	//startSvd(_disable_); /// Start SVD.
	Close_Disp();
	cOver150V = 0;

	do {
		RamData.WDGTm = 0xff;
		main_sequential_off();
		iTime_Isr_no_work = iTime_Isr_no;
		cjdpluseTm = stjdpluse.cjdpluseTm; 	//	 有符合宽度脉冲则加，每隔4MS减一
		cJDDeteTm = stjdpluse.cJDDeteTm; 	//	电源异常检测时间，在4MS中断中递减
		ipluseintno = stjdpluse.cjdpluse_no;
		cst20msplusep = stjdpluse.cst20msplusep; //	 接收缓冲区计数
		main_sequential_on();
		if (iTime_Isr_no_work == iTime_Isr_no_bak)
			continue;
		iTime_Isr_no_bak = iTime_Isr_no_work;
		
		itemp = Get_LVDVolt();
				if (itemp < _iLVD_Lo_)
				{
					 if ( csLVDLow <  iOnLineDete)
						 csLVDLow++;
				}
				else
				{
						if (csLVDLow  )
							csLVDLow--;
				}
				

/*		为去除epson文件优化 if (SVDINTF_SVDDT) {
				if ( csVDDhigh)
					csVDDhigh--;
			if (cSVDDlow < iOnLineDete)
					cSVDDlow++;
			else
				{} //break;

		} else {
			if ( cSVDDlow)
					cSVDDlow--;
			if (csVDDhigh < iOnLineDete)
				csVDDhigh++;
			else
				{};//break;

		}
	
		*Get_LVDVolt();
		 if (RamData.LVD_Volt >= _LVD_150V_)
		 {
		 if (cOver150V < (cMaxTm + 1))
		 cOver150V++;
		 else
		 clvd_off = 0;
		 } else if (RamData.LVD_Volt < (_LVD_150V_ - 200))
		 {
		 if (clvd_off < (cMaxTm + 1))
		 clvd_off++;
		 else
		 cOver150V = 0;
		 }
		 if (clvd_off >= cMaxTm)
		 break;
	*/	
		if (cjdpluseTm >= cMaxPluse) //if (0 == cjdpluseTm)
		{
			if (cSVDDlow >= iOnLineDete)
					break;
		}	
		if (0 == cJDDeteTm)
			break;
		if (cReadp == cst20msplusep)
			continue;
		else {
			itemp = stjdpluse.st20mspluse[cReadp].iLowWidth;
			if (++cReadp >= sizeof(stjdpluse.st20mspluse)
					/ sizeof(stjdpluse.st20mspluse[0]))
				cReadp = 0;
			if (itemp >= _iJDPLuseWidth_150V_)
				stjdpluse.cjdpluseTm++; //脉冲个数加1
		}
	} while (1);
Stop_count_ICKK();
		if ((cjdpluseTm >= cMaxPluse) &&((csLVDLow >=  iOnLineDete) || (cSVDDlow >= iOnLineDete)))// if((RamData.ICKK_OnTm > 200)&&(RamData.Vph.sVI > 1500)	)

	{ //电源电压超过150V并且有继电器继电器检测信号连续超过秒时认为电源异常

		Up_NormFroze(_FROZE_PERR_START_); //电源异常记录
		SetEvtState(_EVT_EPWR_);
	} else {
		Up_PwrOffFroze();
	}
	//Init_GPIO(1);
	Wait_Rst(LO_PWR);
	#endif
}
void Delay_SoftRun(unsigned short i);
unsigned char cSys_Statu_Detc(void) 
	{

	unsigned short iOnLineDete = 25; //符合上电条件检测次数
	unsigned short iDetecMax = iOnLineDete * 2; //	检测次数
	unsigned short iDelayMax = 225; //	检测延时
unsigned long	lLvd= 0;
unsigned char cSVDDlow = 0;
	unsigned char i = 0;


	GC_GPIO_Init(_OnLine_);
  SystemTick_Init();
   SystemTick_Start();
    
   
	RamData.WDGTm =0x90;
	GC_ADC_Init();	_cLVD_Filter_Tm = 0;
	INTERRUPT_ENABLE();//asm("ei");


	for (i = 0; i < (iDetecMax); i++) 
	{
		resetWdt();
		Delay_SoftRun(iDelayMax); //2MS左右
		Delay_SoftRun(iDelayMax); //2MS左右
		Get_LVDVolt();
		lLvd+= RamData.LVD_Volt;
	}
	lLvd /=iDetecMax ;
	
	if ( lLvd > _LVD_133V_)
			return 1;
	return 0;
	
  
}

void Delay_SoftRun(unsigned short i) {
	
	for (; i; i--)
	{
		NOP_ASM(); //asm("nop");
		RamData.WDGTm = 10;
	}
	return;

}

//void SetF32K_out(void);
unsigned long startCalibrateTimer(void);
void MCU_OnLIne_Init(void) {

	stopClgOSC3();
	switchClgSystemClockOSC3();// startClgOSC3();
	//	switchClgSystemClockOSC3();
	//ltemp = startCalibrateTimer();
	//switchClgSystemClockIOSC( );
	//RamData.WDGTm  =0xff;
	//runClgAutoTrimmingOSC3();
	//ltemp = startCalibrateTimer();

	GC_ADC_Init();
	GC_GPIO_Init(1);
	//initT16Ch3();
    SystemTick_Init();
    
	//initT16Ch1();
	if (RamData.LastRTC != ~RamData.xLastRTC) {
		RamData.RTCTime = _DFT_TIME_;
	}
	//-ClrRam((u8*) &Ex645, sizeof(Ex645));

//--	RamData.MDBusyTm = 200;
	Init_Disp();
	if(_Is_MeterApp_OnLine()	){//(RamData.PwrOnCode != 52707000){
		FillRam((u8*)RamData.Disp.DispData, 0xFF, sizeof(RamData.Disp.DispData));
	}
//	UpdateDisp(); //InitDispProg();

	//initSmcif_Chanel0Port();//			初始化7816
	// 	initSmcif_Chanel0_Reader();
	//	cIRChanel_Init(1);
	RamData.WDGTm = 20;
	SetF32K_out(); //  	秒脉冲的输出
	//startT16Ch3(); //		startT16bCh0(   );
    SystemTick_Start();
#ifdef		_METER_LOC_
	initSnda();
#endif
#ifdef _DEBUG_
	cWDTEN = 0;
	if (cWDTEN)
#endif
	startWdt();

}

void Init_MCU(void) {


	INTERRUPT_DISABLE();   // 关中断
	startWdt();
	//----_Pin_Cf_in_Out_Set();					
	if (_Is_MeterApp_Reset() ) {
		ClrRam16((u8*) &RamData, sizeof(RamData));
	//--	ClrRam((u8*) &Ex645, sizeof(Ex645));
		 //Close_Disp();
	} else {
		ClrRam16((u8*) &RamData.HSecCnt, (u16) ((sizeof(RamData))
				- (((u8*) &RamData.HSecCnt) - ((u8*) &RamData))));
	}
	Usart_Mem_init();resetWdt();
	//--ClrRam16( (char *)SCI,sizeof(SCI)/sizeof(char));
	if (RamData.LastRTC != ~RamData.xLastRTC) {
		RamData.RTCTime = _DFT_TIME_;
	}
	RamData.Pwr_On = (cSys_Statu_Detc()) ? 1 : 0;
	//initWdt(iWDTCMT);
	//Rtcunint_init(1);

//	initLcd8a(  RamData.Disp.DispData);
	RamData.WDGTm = 0xff;
	if (RamData.Pwr_On)
		MCU_OnLIne_Init();
	else
		MCU_OffLIne_Init();
	RamData.WDGTm = 0x99;_cLVD_Filter_Tm = 0;
    
    
  resetWdt();  //-----
    InitRTCTemp();
	Reset_Option_No_Init_Ram() ;
	INTERRUPT_ENABLE();//asm("ei");				// 开中断

}

void MCU_OffLIne_Init(void) {
	GC_GPIO_Init(0);

	//opt CLGOSC_IOSCEN = 1; ///< This bit control the clock source(IOSC) operation.
	//opt CLGOSC_OSC1EN = 1; ///< This bit control the clock source(OSC3) operation.
	//opt CLGOSC_OSC3EN = 0; ///< This bit control the clock source(OSC3) operation.
	//opt CLGOSC_EXOSCEN = 0; ///< This bit control the clock source(EXOSC) operation.

	//opt CLGOSC_IOSCSLPC = 0; ///< This bit control the clock source(IOSC) operations in SLEEP mode.
	//opt CLGOSC_OSC1SLPC = 0;
	//opt CLGOSC_OSC3SLPC = 1;
	//opt  CLGOSC_EXOSCSLPC = 1;
	switchClgSystemClockIOSC();

	//initT16Ch3();
	SystemTick_Init();
    
   // initT16Ch1_IOSC();

	//startT16Ch3(); //startT16bCh0(   );
    SystemTick_Start();

}

unsigned char Get_SYS_Statu(void) {
	return (RamData.Pwr_On) ? 0xff : 0;
}
/*
void DelayUS(unsigned int itemp) {
	extern volatile unsigned int iDelayOut;
	unsigned short itemp1 = itemp / 400 + 1;
  unsigned int itemp2;

	itemp1 += RamData.WDGTm;
	if (itemp1 > 0xff)
		itemp1 = 0xff;
	do {
		RamData.WDGTm = itemp1;
		iDelayOut = itemp;

	} while (iDelayOut != itemp);

	presetT16Ch1Count(itemp);
 //---delay_us_init(TIMER1_REG); // startT16Ch1();
	
	while ( itemp ) 
	{
		itemp1  = ( itemp > 2000)?2000 :itemp;
	//---	delay_us(TIMER1_REG,itemp1);
		itemp -=itemp1;
		if (_ISNoRunTask())Check_MS(); // 延时时检查
	} 
		 
	//stopT16Ch1();
	return;
}

void DelayMS(unsigned short iDelayLen) {
	
	
	
	unsigned short itemp = iDelayLen;
	const unsigned short iDelay_Max_Us = (65535 / 1000);

	do {
		itemp = (iDelayLen >= iDelay_Max_Us) ? iDelay_Max_Us : iDelayLen;
		DelayUS(itemp * 1000);
		iDelayLen -= itemp;
	} while (iDelayLen);
	
}
*/