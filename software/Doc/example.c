/*
*********************************************************************************************************
*                                              HT60XX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : example.c
* By           : SocTeam
* Version      : CurrentVersion
* Description  : 
*********************************************************************************************************
*/

#define  __EXAMPLE_C
//#undef   __EXAMPLE_C

#ifdef   __EXAMPLE_C

#include "example.h"
#include "ht6xxx_lib.h"

/*
*********************************************************************************************************
*                                           ���غ�/�ṹ��
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             ���ر���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           ���غ�������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                GPIO EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����GPIO��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void GPIO_Example()
{      
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitAFTypeDef  GPIO_InitAFStructure;
    
    /***** ���´������ڽ�PC9��PC10����ΪGPIO���룬��ʹ������ *****/    
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    
    HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);
    
    /***** ���´������ڽ�PA0-PA4����ΪSEG����  *****/     
        
    GPIO_InitAFStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  
    GPIO_InitAFStructure.GPIO_AFMode = PA0_SEG28|PA1_SEG29|PA2_SEG30|PA3_SEG31|PA4_SEG32;
    
    GPIO_InitAFStructure.GPIO_InputStruct = GPIO_Input_Up;      /*!< ����Seg���ܣ�������������ʵ������ */
    GPIO_InitAFStructure.GPIO_OutputStruct = GPIO_Output_PP; 
    
    HT_GPIO_AFInit(HT_GPIOA, &GPIO_InitAFStructure);    
    
} 

/*
*********************************************************************************************************
*                                CMU EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����CMU��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void CMU_Example()
{      
    
    /***** ���´������ڽ�ϵͳʱ������ΪPLLʱ�ӣ�CPUʱ��Ϊϵͳʱ��2��Ƶ *****/  
    
    CMU_InitTypeDef  CMU_InitStructure;
    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv2;

    HT_CMU_Init(&CMU_InitStructure);
} 

/*
*********************************************************************************************************
*                                PMU EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����PMU��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void PMU_Example()
{      
    
    /***** ���´�����������оƬPMUģ�� *****/  
    
    PMU_InitTypeDef  PMU_InitStructure;
    
    PMU_InitStructure.Lvdin0State = LVDINEnable;                 /*!< LVDIN����ʹ��                */
    
    PMU_InitStructure.BorLvlSet = BORLVL_2V4;                   /*!< BOR��ֵ2.4v                  */
    PMU_InitStructure.BorState = BORReset;                      /*!< BOR������λ                  */
#if  defined  HT6x2x  ||  defined  HT6x1x    
    PMU_InitStructure.VccLvlSet = VccLVL_4V0;                   /*!< Vcc��ֵ��Ϊ4v                */
#elif defined HT501x
    PMU_InitStructure.VccLvlSet = VccLVL_2V8;                   /*!< Vcc��ֵ��Ϊ2.8v              */
#endif    
    PMU_InitStructure.VccBorDetPeriod = DETPRD_1072mS;          /*!< Vcc��Bor��ʱ�������=1072ms  */
    PMU_InitStructure.VccBorDetTime = DETTIM_300uS;             /*!< Vcc��Bor��ʱ���ʱ��=300uS   */
    
    HT_PMU_Init(&PMU_InitStructure);
} 

/*
*********************************************************************************************************
*                                UART EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����UART��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void UART_Example()
{      
    
    /***** ���´�����������оƬUART0ģ�飬���´��벻��GPIO���� *****/  
    
    UART_InitTypeDef UART_InitStructure;
    
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

/*
*********************************************************************************************************
*                                 IR EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����IR��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void IR_Example()
{      
    
    /***** ���´�����������UART0Ϊ����ģ�飬���´��벻��GPIO���� *****/  
    
    IR_InitTypeDef IR_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);               /*!< ʹ��UART0ģ��         */
    
    IR_InitStructure.UARTModule.UART_Logic = UartLogicPositive;        /*!< ����UART              */ 
    IR_InitStructure.UARTModule.UART_StopBits = OneStopBits;
    IR_InitStructure.UARTModule.UART_WordLength = WordLength_8Bits;
    IR_InitStructure.UARTModule.UART_Parity = UartParity_EVEN;
    IR_InitStructure.UARTModule.UART_BaudRate = 2400;
    IR_InitStructure.UARTModule.ReceiveEN = ENABLE;
    IR_InitStructure.UARTModule.SendEN = ENABLE;
    
    IR_InitStructure.IRPolarity = IRPolarityPositive;                  /*!< ���ú���              */      
    IR_InitStructure.IRDuty = IRDuty50;
     
    HT_IR_Init(HT_UART0, &IR_InitStructure);
}

/*
*********************************************************************************************************
*                                 ISO7816 EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����ISO7816��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void ISO7816_Example()
{      
    
    /***** ���´�����������ISO7816_1ģ�飬���´��벻��GPIO���� *****/  
    ISO7816_InitTypeDef  ISO7816_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);             /*!< ʹ��7816_1            */
        
    ISO7816_InitStructure.ISO7816_Parity = ISO7816Parity_EVEN;              /*!< żУ��                */
    ISO7816_InitStructure.ISO7816_RepeatTime = Repeat_Zero;                 /*!< �Զ��ط����մ�����Ϊ0 */
    ISO7816_InitStructure.AutoRxEN = DISABLE;                               /*!< �Զ����չ��ܹر�      */
    ISO7816_InitStructure.AutoTxEN = DISABLE;                               /*!< �Զ��ط����ܹر�      */
    ISO7816_InitStructure.ISO7816_AckLen = ACKLen_1P5Bit;                   /*!< ACK����1.5Bit         */
    ISO7816_InitStructure.ISO7816_BaudRate = 9600;                          /*!< ������9600            */
    
    HT_ISO7816_Init(HT_ISO7816_1, &ISO7816_InitStructure);
}

/*
*********************************************************************************************************
*                                  LCD EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����LCD��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void LCD_Example()
{      
    
    /***** ���´�����������Һ��ģ�飬���´��벻��GPIO���� *****/  
    
    LCD_InitTypeDef  LCD_InitStructure;
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_LCDEN, ENABLE);            /*!< ʹ��LCDģ��                */
    
    LCD_InitStructure.LCD_Bias = LCDBias_OneThird;                /*!< 1/3 bias ƫѹ����          */
    LCD_InitStructure.LCD_Com  = LCD_6COM;                        /*!< 6Com                       */
    LCD_InitStructure.LCD_CLK  = LCDCLK_Div1;                     /*!< ɨ��Ƶ�����               */
    LCD_InitStructure.LCD_ChargeMode  = LCDCharge_HighCurrent;    /*!< ��������ģʽ             */
    LCD_InitStructure.LCD_Vrsel = 0x02;                           /*!< �Աȶ�����                 */
    
    HT_LCD_Init(&LCD_InitStructure);      
}

/*
*********************************************************************************************************
*                                  SPI EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����SPI��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void SPI_Example()
{      
    
    /***** ���´�����������SPI�����´��벻��GPIO���� *****/  
    
    SPI_InitTypeDef  SPI_InitStructure;
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_SPI0EN, ENABLE);            /*!< ʹ��SPI0ģ��                */
    
    SPI_InitStructure.SPI_CSInCtrol = ENABLE;
    SPI_InitStructure.SPI_Baudrate = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_high;
    SPI_InitStructure.SPI_Mode = SPI_Master;             
    
    HT_SPI_Init(HT_SPI0, &SPI_InitStructure);      
}

/*
*********************************************************************************************************
*                                  TIMER EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����Timer��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void TMR_Example()
{      
    
    /***** ���´����������ö�ʱ��0�����´��벻��GPIO���� *****/  
    
    TMR_InitTypeDef  TMR_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR0EN, ENABLE);            /*!< ʹ��TIMER0ģ��              */
    
    TMR_InitStructure.TimerMode = TIMCycleTiming;                  /*!< Timer�趨Ϊ��ʱ������       */
    TMR_InitStructure.TimerPreDiv = 0x00;                          /*!< Timerʱ�ӵ���ϵͳʱ��       */
    TMR_InitStructure.TimerPeriod = 0x1000;                        /*!< ��ʱ�����趨                */
    TMR_InitStructure.TimerCompare = 0x00;                         /*!< �ȽϼĴ����趨              */
    
    HT_TMR_Init(HT_TMR0, &TMR_InitStructure);  
}

/*
*********************************************************************************************************
*                                   RTC EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����RTC��ʼ��ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void RTC_Example()
{      
    
    /***** ���´�����������RTCģ�飬���´��벻��GPIO���� *****/  
    
    RTC_InitTypeDef  RTC_InitStructure;

    RTC_InitStructure.RTC_TMR1.NewState = ENABLE;               /*!< ʹ��RTC��ʱ��1              */   
    RTC_InitStructure.RTC_TMR1.Count = 5;                       /*!< ��ʱ����6s                  */
    RTC_InitStructure.RTC_TMR2.NewState = DISABLE;              /*!< �ر�RTC��ʱ��2              */   
    RTC_InitStructure.RTC_TMR1.Count = 0;
    
    RTC_InitStructure.RTC_Alarm.Muster.hour = 0x99;             /*!< ����������                  */
    RTC_InitStructure.RTC_Alarm.Muster.minute = 0x99;
    
    RTC_InitStructure.RTC_TOUT = Tout1Hz;                       /*!< Tout���1Hz                 */
    
    HT_RTC_Init(&RTC_InitStructure); 
}

/*
*********************************************************************************************************
*                                   TBS EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����TBS����ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void TBS_Example()
{      
    
    /***** ���´�����������TBSģ�飬ʹ���¶Ⱥ͵�ص�ѹ���������������� *****/  
    
//    HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC1S_MEM4S);    /*!< �¶Ȳ���1s��һ��              */
//    HT_TBS_PeriodSet(TBS_VBAT, TBS_TBSPRD_VBATPRD_8S);           /*!< ��ص�ѹ����8s��һ��          */
    
    HT_TBSConfig(TBS_TBSCON_TMPEn|TBS_TBSCON_VBATEn, ENABLE);    /*!< ʹ���¶Ⱥ͵�ص�ѹ����          */
    
}

/*
*********************************************************************************************************
*                                   FLASH EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯����Flash����ʾ��
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void Flash_Example()
{      
    
    /***** ���´�������Flash���� *****/  
    const uint8_t WriteCode[]={0,1,2,3,4,5,6,7,8,9};
    
    HT_Flash_PageErase(0x1FC00);                 /*!< ������ַ0x1FC00����ҳ����HT60xx�������һҳ   */
    
    HT_Flash_ByteWrite(WriteCode, 0x1FC00, sizeof(WriteCode));                 
                                                 /*!< ���ַ0x1FC00д10�ֽ�                         */   
}

/*
*********************************************************************************************************
*                                 Interrupt EXAMPLES FOR HT60XX
*
* ����˵��: ���ÿ⺯��ʹ���ж�(��RTC�ж�Ϊ��)
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: �˺���Ĭ�ϲ����빤�̱���
*********************************************************************************************************
*/
void Interrupt_Example()
{      
    
    /***** ���´�������ʹ��RTC���Ӻ�Сʱ�ж� *****/  
    
    HT_RTC_ITConfig(RTC_RTCIE_MINIE|RTC_RTCIE_HRIE, ENABLE);
    
    NVIC_EnableIRQ(RTC_IRQn);                                  /*!< ʹ��RTC���ж�        */
    
    /***** ���´������ڹر�RTC���Ӻ�Сʱ�ж� *****/ 
    
    HT_RTC_ITConfig(RTC_RTCIE_MINIE|RTC_RTCIE_HRIE, DISABLE);
    
    /***** ���´������ڹر�RTC���ж� *****/
    
    NVIC_DisableIRQ(RTC_IRQn);                                 /*!< ʹ��RTC���ж�        */
}

/*
*********************************************************************************************************
*                                  CHIP LOW POWER CONFIGURE 
*
* ����˵��: оƬ�͹������ã�����оƬ�ڽ���Hold����Sleepģʽǰ��Ҫ��������
*
* ��ڲ���: ��                            
*
* ���ز���: ��                                      
* 
* ����˵��: 1)�˺���Ĭ�ϲ����빤�̱���
*           2)�˺������ò���GPIO��GPIO��������ݾ��幤��
*           3)����TBS���¶Ȳ���ģ�飬�˺������ر���������ģ�飬�û����Ը����Լ�������ʵ�������ٴ�����ģ��
*********************************************************************************************************
*/
void LowPowerConfigure_Example()
{    
    CMU_InitTypeDef  CMU_InitStructure; 
    PMU_InitTypeDef  PMU_InitStructure; 
        
    HT_FreeDog();
    /**********  ����CMU ģ��  **********/    
    CMU_InitStructure.SysClkSel = SysLF;
    CMU_InitStructure.CPUDiv = CPUDiv1;   
    HT_CMU_Init(&CMU_InitStructure);                    /*!< ����ϵͳʱ��Fsys = Flf                   */
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0, DISABLE);       /*!< �ر�CLKCTRL0�Ĵ���                       */
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1, DISABLE);       /*!< �ر�CLKCTRL1�Ĵ���                       */
    
    HT_SHModeDogConfigure(DISABLE);                     /*!< Sleep/Holdģʽ�¹ر�WatchDog             */

    /**********  ����PMU ģ��  **********/ 
    PMU_InitStructure.Lvdin0State = LVDINDisable;
    PMU_InitStructure.BorState = BORDisable;
#if  defined  HT6x1x ||  defined  HT6x2x    
    PMU_InitStructure.VccLvlSet = VccLVL_4V0;
#elif defined HT501x
    PMU_InitStructure.VccLvlSet = VccLVL_2V8;
#endif
    PMU_InitStructure.BorLvlSet = BORLVL_2V4;
    PMU_InitStructure.VccBorDetTime = DETTIM_300uS;
    PMU_InitStructure.VccBorDetPeriod = DETPRD_536mS;
    HT_PMU_Init(&PMU_InitStructure);
    
    /**********  ����TBS ģ��  **********/   
    HT_TBSConfig(TBS_TBSCON_VBATEn|TBS_TBSCON_ADC0En|TBS_TBSCON_ADC1En
                 |TBS_TBSCON_VCCEn, DISABLE);
                                                        /*!< �رճ��¶Ȳ�������������ģ��              */
    HT_TBSConfig(TBS_TBSCON_TMPEn, ENABLE);             /*!< ʹ���¶Ȳ���                              */
    
//  HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS);  /*!< ��HT60xx����������Ϊ64s��һ��           */
 // HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S);  /*!< ��HT61xx������Ӧ����Ϊ0����8s��һ��     */
}




#endif  /* __EXAMPLE_C */
