/*
*********************************************************************************************************
*                                 			   HT6XXX
*                            			      HT6XXX����
*
*                       			Copyright 2013, Hi-Trend Tech, Corp.
*                             			 All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : main.c
* By           : SocTeam
* Version      : CurrentVersion
* Description  : 
*********************************************************************************************************
*/

#define  __MAIN_C
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
#include "lcdio.h"
extern  const  strFileSet FileSet;
void LCD_Init();
void  UpDisp1( unsigned long  N);
 
#define _KEYDOWN()			(0 ==(HT_GPIOA-��PTDAT & GPIO_Pin_5))
#define _KEYUP()				(!_KEYDOWN())
extern enum  ENUMCHIPMODE  enChipMode  ;  
void   OnLine_Dete ( void);
const	 unsigned char		cSysTickTm = 5;
const	unsigned char			cKeyTmMax = 3;
unsigned char		cKeyOn;
unsigned char		cKeyOff;
struct STSCIBUF  USARTCOM[_CHANEL_END_];
void SysTick_fun(void);
#define 		_iComSilence_		1500				//	    
#define 		_iComSendDelay	200        //
extern 	struct 	STDATALIST  stdatalist[] ;
const	 unsigned short 		cSilenceMax =  _iComSilence_	/	cSysTickTm ;
const	 unsigned short 		iSendTmMax = _iComSendDelay /	cSysTickTm ;
void PMU_Init() ;

 const struct 	STCOMLIST  stcomlist[_CHANEL_END_]=
 {
		{HT_UART3 , 300 , 0 },
		{HT_UART0 , 300 , 0 },
	};

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            ���ر���
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/
void   UART_init();
void   UART_init();
void CMU_Init();
void SystemTick_Start (void);
void SystemTick_Interrupt_On(void);
unsigned char		cSysTickNum;
void GPIO_Init();
void stream_cSilence_fun (void);
void get_emu_var_default(void);
extern unsigned short  iTurnOnTm ;
extern unsigned short iTurnOffTm;
/*
*********************************************************************************************************
*                                              MAIN
*
*********************************************************************************************************
*/
 extern const 	struct	  STFLASHDATA   stflashdata   ;
 	 void  MeterData_Show(struct 	STDATALIST  *stp);
 unsigned char FileSystem_Init (struct STFLASHFILESYSTEM *stfilep);
 extern  struct STFLASHFILESYSTEM stDianLian;
void get_emu_var_default(void) ;
int main(void)                                   
{     
    /*  SystemInit() has been called before enter main() */ 
		char stempwww[100] ;
	unsigned int i ;
  const char stemp1[] = "one";
  const char stemp2[] = "three";
	unsigned long kill = 0x12345678;  
    /*  Add your application code here  */ 
    
    /*  Infinite loop  */
unsigned char ctemp;
	a3:
	iTurnOnTm  = iTurnOffTm= 0;
	CMU_Init();
	PMU_Init();
	bitstruct_init ();
	GPIO_Init();
	SysTick_Config(8000000/160);	
	//get_emu_var_default();
	do{
	} while (  !(_OnLine()) && !(_OffLine()));
	
 
	emu_init();

	
 
	
	
	// 	fprintf((FILE *)7, "%c%x%s%s%d\n\r",(char)10,13,stemp1,(char *)"123456" ,0x123456);
	 
	//==============

	UART_init();

	//SystemTick_Start ();
	
  //NVIC_EnableIRQ(SysTick_IRQn  );                                  /*!< ʹ��RTC���ж�        */
     LCD_Init();
  RamData.LVSave = &(RamData.stdianlian[0].LVSave);
	for (i = 0; i < _MAX_TARRIF_; i++)
		RamData.pFLUse[i] = (RamData.stdianlian[0].FLUse + i); //�����õ�ָ��
   
	 i = 0;
    while (1)
    {	
			timeclock_get ();
			 // ��ʱ��״̬����
      mstimerun();	
			halfsectimerun();
			Do_Task  ( bInt_HalfSec , UpDisp());							
			Do_Task (  bInt_HalfSec ,		emu_proc());	
			Do_Task ( bInt_HalfSec  ,   Do_Use());				//�������õ���
			Do_Task ( bInt_HalfSec && _IsXBMode(enChipMode),	Chip_Trim())
			stBitnewtoold_fun;
      timeclock_clear ();
			
    } 
}


 /*
*********************************************************************************************************
*                                             ASSERT
*
*********************************************************************************************************
*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	
  while (1)
  {

  }
}
#endif

void CMU_Init()
{      
    
    /***** ���´������ڽ�ϵͳʱ������ΪPLLʱ�ӣ�CPUʱ��Ϊϵͳʱ��2��Ƶ *****/  
    
    CMU_InitTypeDef  CMU_InitStructure;
    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv2;

    HT_CMU_Init(&CMU_InitStructure);
		HT_CMU->WPREG = 0xA55A;
		HT_CMU->CLKCTRL0 |=	CMU_CLKCTRL0_EMUEN ;
	 
	delay(0x5fff);
	
} 

void SystemTick_Start (void)
{
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
}


void SystemTick_Stop (void)
{
    SysTick->CTRL  &= (~SysTick_CTRL_ENABLE_Msk);	
}

void SystemTick_Interrupt_On(void)
{
    SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;		/* Turn on SysTick INT  */
}

void SystemTick_Interrupt_Off(void)
{
    SysTick->CTRL  &= (~SysTick_CTRL_TICKINT_Msk);   /* Turn off SysTick INT  */
}
//---------------------------
// SysTick_Handler
//---------------------------
//
void SysTick_Handler(void)
{
	
	uint32_t clr;
	static uint32_t s = 0;
    
	clr = SysTick->CTRL; 	/* clear COUNTFLAG */
 SysTick_fun();
}


void  UARR_Ir_Chanel_Init(unsigned  int ibps)
{
	 UART_InitTypeDef UART_InitStructure;
	
	  UART_InitStructure.UART_Logic = UartLogicPositive;          /*!< UART�߼�Ϊ��               */
    UART_InitStructure.UART_StopBits = OneStopBits;             /*!< 1λֹͣλ                  */
    UART_InitStructure.UART_WordLength = WordLength_7Bits;      /*!< 8λ����λ                  */
    UART_InitStructure.UART_Parity = UartParity_EVEN;           /*!< żУ��                     */
    UART_InitStructure.UART_BaudRate = ibps;                    /*!< ������2400                 */
    UART_InitStructure.ReceiveEN = ENABLE;                      /*!< ����ʹ��   ע�����ж�ʹ��  */
    UART_InitStructure.SendEN = ENABLE;                         /*!< ����ʹ��   ע�����ж�ʹ��  */
	
	  HT_UART_Init(HT_UART3, &UART_InitStructure);
		HT_UART3->UARTCON  |= UART_UARTCON_RXIE  ;
		HT_UART3->UARTCON  &= ~UART_UARTCON_TXIE  ;
}

void   UART_init()
{      
    
    /***** ���´�����������оƬUART0ģ�飬���´��벻��GPIO���� *****/  
    
    UART_InitTypeDef UART_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN  , ENABLE);        /*!< ʹ��UART0ģ��              */
	
    UARR_Ir_Chanel_Init(300);
        
    NVIC_SetPriority(UART3_IRQn ,3);
		NVIC_EnableIRQ(  UART3_IRQn      );                                  /*!< ʹ��RTC���ж�        */
	
	   HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);        /*!< ʹ��UART0ģ��              */
    
    UART_InitStructure.UART_Logic = UartLogicPositive;          /*!< UART�߼�Ϊ��               */
    UART_InitStructure.UART_StopBits = OneStopBits;             /*!< 1λֹͣλ                  */
    UART_InitStructure.UART_WordLength = WordLength_8Bits;      /*!< 8λ����λ                  */
    UART_InitStructure.UART_Parity = UartParity_EVEN;           /*!< żУ��                     */
    UART_InitStructure.UART_BaudRate = 2400;                    /*!< ������2400                 */
    UART_InitStructure.ReceiveEN = ENABLE;                      /*!< ����ʹ��   ע�����ж�ʹ��  */
    UART_InitStructure.SendEN = ENABLE;                         /*!< ����ʹ��   ע�����ж�ʹ��  */
        
    HT_UART_Init(HT_UART0, &UART_InitStructure);
} 


void
stream_cSilence_fun (void)
{
  unsigned short i;
  for (i = 0; i < _Com_End_; i++)
  {
	  if (USARTCOM[i].cSilence)
      {
			USARTCOM[i].cSilence--;
			Do_Task (0 == USARTCOM[i].cSilence, stream_refresh(i));
      }
		if ( USARTCOM[i].iSendTm)
				USARTCOM[i].iSendTm--;
			
	}
}
 
char	stream_line_check( unsigned short i )
{
		unsigned  char	 j,k ;
		if(  USARTCOM[i ].cHead >1 )
		{
					j  =  USARTCOM[i].DataBuf[USARTCOM[i].cHead-2]-'\r';
					k  = 	USARTCOM[i].DataBuf[USARTCOM[i].cHead-1]-'\n';
					j |=k;
					if ( 0 == j)
							return TRUE;
		}
		return FALSE;
}
			


void   stream_fun_rece( HT_UART_TypeDef   *unitp)
{
		unsigned short iChanel = 0;
	
	SystemTick_Interrupt_Off();
		USARTCOM[iChanel].cSilence = 	cSilenceMax ;
		USARTCOM[iChanel].DataBuf[USARTCOM[iChanel].cHead++] = unitp->SBUF;
		if ( TRUE ==  stream_line_check(iChanel))
		{
			USARTCOM[iChanel].bEventRec =1;	
			USARTCOM[iChanel].cSilence = 	0 ; 
			USARTCOM[iChanel].iSendTm = iSendTmMax ;
		}
		SystemTick_Interrupt_On();
}

void stream_refresh(unsigned short i )
{
		HT_UART_TypeDef  *unitp= stcomlist[i].unitp;
		UARR_Ir_Chanel_Init(USARTCOM[i].iBps);
		memset ( (char *)&USARTCOM[i], 0 , sizeof ( USARTCOM[i]));
}


void open_pll(void)
{	
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 |= 0x0010;            //enable PLL
	delay(0x5fff);
//	delay(0x5fff);
//	delay(0x5fff);
	HT_CMU->SYSCLKCFG = 0x83;              //PLL as Fsys 
//	HT_CMU->SYSCLKDIV = 0x00;              //PLL:19.66MHz, Fsys/1 as Fcpu		
	HT_CMU->SYSCLKDIV = 0x01;              //PLL:19.66MHz, Fsys/2 as Fcpu	
}


void open_pll_tmp(void)
{
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 |= 0x0010;            //enable PLL
	delay(0x5f);
	HT_CMU->SYSCLKCFG = 0x83;              //PLL as Fsys 	
//	HT_CMU->SYSCLKDIV = 0x04;             //PLL:19.66MHz, Fsys/16 as Fcpu	
	HT_CMU->SYSCLKDIV = 0x05;              //PLL:19.66MHz, Fsys/32 as Fcpu
//	HT_CMU->SYSCLKDIV = 0x06;              //PLL:19.66MHz, Fsys/64 as Fcpu
}


void close_pll(void)
{
	HT_CMU->WPREG = 0xA55A;
	HT_CMU->CLKCTRL0 &= 0xFFEF;            //close PLL
}


		
void SysTick_fun(void)
{
	static 		unsigned char   cHalfsectm = 0;
	static 	  unsigned char   cSectm =0;

extern unsigned char   c5msno ;
	
		c5msno++ ;
		  OnLine_Dete ();
	if ( !(_OnLine()))		return; 
		stream_cSilence_fun();  
		bInt_Ms_io = Event_Ms = 1;
		
		if ( ++cSectm >= 200)
					cSectm = 0 ; 
		if ( ++cHalfsectm >=100)
					cHalfsectm = 0;
			bInt_HalfSec_io= !((_Bool)  cHalfsectm ) | bInt_HalfSec_io ; 
			bInt_SecBit_io = !((_Bool) cSectm ) | bInt_SecBit_io  ; 
	 
	 /* 
		if ( cWDGTm   )
				cWDGTm-- ;
			
 
	 
		if(cWDGTm){
			IWDG->KR= 0xAA; //iwdgReloadCounter();
	    }else{
		    while(1);
	   
		 */
	 
		  
		
	}
void Up_BkLVSave() {}
	void Up_LVSave() {}
void	Do_Use(void)
	{

	u8	Imp;
	u16	sImp;
	u32	AInc;
	u32	Inc;
	u32	SInc;
	u8 cImpbuf;

	RamData.ImpBfr = Read_EPADR(EPR_EnergyPC );
		if ( 0 == RamData.ImpBfr ) return ; 
		cparToB(Event_Rev , _bREVP  )	;
		RamData.Use_Dir =  (cGetBOld(Event_Rev)	&& cGetB(Event_Rev)	)?1:0;
	if(RamData.Use_Dir)
	{
		if(AUse_Dir == 0){
			AUse_Dir = 1;
			AUse_Dir_A = 1;
			RamData.RevTmCnt = 0;
			return;
		}
	} else {
		if(AUse_Dir){
			AUse_Dir = 0;
			AUse_Dir_A = 0;
			RamData.RevTmCnt = 0;
			return;
		}
	}
	Imp =RamData.ImpBfr;
	if(RamData.RevTmCnt < _REV_DELAY_){								//������
		if((++RamData.RevTmCnt < _REV_DELAY_)&&(Imp < _IMP_DELAY_)){
			return;
		}
	}
	 
	if(RamData.Use_Dir){
		sImp = 0x8000|Imp;
	} else {
		sImp = Imp;
	}
	
	//Inc = Inc_Use( &RamData.LVSave[0].FLUse, sImp);					//��������õ���
	SInc = Inc_Use( &RamData.LVSave[0].SumUse, sImp);				//�������õ���
/*if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
	AInc = 0;
	if(Inc){
		AInc = Inc_Money(FlashInfo.RunInfo.FLPrice, Inc);			//���ӷ����õ���
	}
	if(SInc){
		AInc += Inc_Money(FlashInfo.RunInfo.StepPrice, SInc);		//���ӽ����õ���
	}
} //#endif
	*/
	Up_BkLVSave();													//�����������ݱ���
	if(SInc){
		RamData.NotChg += SInc;
//if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
//		GetPrice();
//} //#endif
		if((RamData.NotChg >= 100)&&(RamData.UseChgTm >= (15*6))){
			Up_LVSave();
			RamData.NotChg = 0;
			RamData.Use_Chg = 1;
		}
	}
//if (_ISMETER_LOCAL()) { //#ifdef		_USE_CARD_
//	if(AInc){														//�й������仯����
//		ChkUseState();
//	}
}//#endif		//_USE_CARD_
 
 
	 
	
	