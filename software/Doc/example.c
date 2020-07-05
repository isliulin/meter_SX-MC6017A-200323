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
*                                           本地宏/结构体
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                GPIO EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对GPIO初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void GPIO_Example()
{      
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitAFTypeDef  GPIO_InitAFStructure;
    
    /***** 以下代码用于将PC9，PC10配置为GPIO输入，并使能上拉 *****/    
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IOIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_InputStruct = GPIO_Input_Up;
    GPIO_InitStructure.GPIO_OutputStruct = GPIO_Output_PP;
    
    HT_GPIO_Init(HT_GPIOC, &GPIO_InitStructure);
    
    /***** 以下代码用于将PA0-PA4配置为SEG功能  *****/     
        
    GPIO_InitAFStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  
    GPIO_InitAFStructure.GPIO_AFMode = PA0_SEG28|PA1_SEG29|PA2_SEG30|PA3_SEG31|PA4_SEG32;
    
    GPIO_InitAFStructure.GPIO_InputStruct = GPIO_Input_Up;      /*!< 对于Seg功能，这两句配置其实无意义 */
    GPIO_InitAFStructure.GPIO_OutputStruct = GPIO_Output_PP; 
    
    HT_GPIO_AFInit(HT_GPIOA, &GPIO_InitAFStructure);    
    
} 

/*
*********************************************************************************************************
*                                CMU EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对CMU初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void CMU_Example()
{      
    
    /***** 以下代码用于将系统时钟配置为PLL时钟，CPU时钟为系统时钟2分频 *****/  
    
    CMU_InitTypeDef  CMU_InitStructure;
    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv2;

    HT_CMU_Init(&CMU_InitStructure);
} 

/*
*********************************************************************************************************
*                                PMU EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对PMU初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void PMU_Example()
{      
    
    /***** 以下代码用于配置芯片PMU模块 *****/  
    
    PMU_InitTypeDef  PMU_InitStructure;
    
    PMU_InitStructure.Lvdin0State = LVDINEnable;                 /*!< LVDIN功能使能                */
    
    PMU_InitStructure.BorLvlSet = BORLVL_2V4;                   /*!< BOR阈值2.4v                  */
    PMU_InitStructure.BorState = BORReset;                      /*!< BOR产生复位                  */
#if  defined  HT6x2x  ||  defined  HT6x1x    
    PMU_InitStructure.VccLvlSet = VccLVL_4V0;                   /*!< Vcc阈值设为4v                */
#elif defined HT501x
    PMU_InitStructure.VccLvlSet = VccLVL_2V8;                   /*!< Vcc阈值设为2.8v              */
#endif    
    PMU_InitStructure.VccBorDetPeriod = DETPRD_1072mS;          /*!< Vcc和Bor分时检测周期=1072ms  */
    PMU_InitStructure.VccBorDetTime = DETTIM_300uS;             /*!< Vcc和Bor分时检测时间=300uS   */
    
    HT_PMU_Init(&PMU_InitStructure);
} 

/*
*********************************************************************************************************
*                                UART EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对UART初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void UART_Example()
{      
    
    /***** 以下代码用于配置芯片UART0模块，以下代码不含GPIO配置 *****/  
    
    UART_InitTypeDef UART_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);        /*!< 使能UART0模块              */
    
    UART_InitStructure.UART_Logic = UartLogicPositive;          /*!< UART逻辑为正               */
    UART_InitStructure.UART_StopBits = OneStopBits;             /*!< 1位停止位                  */
    UART_InitStructure.UART_WordLength = WordLength_8Bits;      /*!< 8位数据位                  */
    UART_InitStructure.UART_Parity = UartParity_EVEN;           /*!< 偶校验                     */
    UART_InitStructure.UART_BaudRate = 2400;                    /*!< 波特率2400                 */
    UART_InitStructure.ReceiveEN = ENABLE;                      /*!< 接收使能   注：非中断使能  */
    UART_InitStructure.SendEN = ENABLE;                         /*!< 发送使能   注：非中断使能  */
        
    HT_UART_Init(HT_UART0, &UART_InitStructure);
} 

/*
*********************************************************************************************************
*                                 IR EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对IR初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void IR_Example()
{      
    
    /***** 以下代码用于配置UART0为红外模块，以下代码不含GPIO配置 *****/  
    
    IR_InitTypeDef IR_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);               /*!< 使能UART0模块         */
    
    IR_InitStructure.UARTModule.UART_Logic = UartLogicPositive;        /*!< 配置UART              */ 
    IR_InitStructure.UARTModule.UART_StopBits = OneStopBits;
    IR_InitStructure.UARTModule.UART_WordLength = WordLength_8Bits;
    IR_InitStructure.UARTModule.UART_Parity = UartParity_EVEN;
    IR_InitStructure.UARTModule.UART_BaudRate = 2400;
    IR_InitStructure.UARTModule.ReceiveEN = ENABLE;
    IR_InitStructure.UARTModule.SendEN = ENABLE;
    
    IR_InitStructure.IRPolarity = IRPolarityPositive;                  /*!< 配置红外              */      
    IR_InitStructure.IRDuty = IRDuty50;
     
    HT_IR_Init(HT_UART0, &IR_InitStructure);
}

/*
*********************************************************************************************************
*                                 ISO7816 EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对ISO7816初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void ISO7816_Example()
{      
    
    /***** 以下代码用于配置ISO7816_1模块，以下代码不含GPIO配置 *****/  
    ISO7816_InitTypeDef  ISO7816_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);             /*!< 使能7816_1            */
        
    ISO7816_InitStructure.ISO7816_Parity = ISO7816Parity_EVEN;              /*!< 偶校验                */
    ISO7816_InitStructure.ISO7816_RepeatTime = Repeat_Zero;                 /*!< 自动重发重收次数设为0 */
    ISO7816_InitStructure.AutoRxEN = DISABLE;                               /*!< 自动重收功能关闭      */
    ISO7816_InitStructure.AutoTxEN = DISABLE;                               /*!< 自动重发功能关闭      */
    ISO7816_InitStructure.ISO7816_AckLen = ACKLen_1P5Bit;                   /*!< ACK长度1.5Bit         */
    ISO7816_InitStructure.ISO7816_BaudRate = 9600;                          /*!< 波特率9600            */
    
    HT_ISO7816_Init(HT_ISO7816_1, &ISO7816_InitStructure);
}

/*
*********************************************************************************************************
*                                  LCD EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对LCD初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void LCD_Example()
{      
    
    /***** 以下代码用于配置液晶模块，以下代码不含GPIO配置 *****/  
    
    LCD_InitTypeDef  LCD_InitStructure;
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_LCDEN, ENABLE);            /*!< 使能LCD模块                */
    
    LCD_InitStructure.LCD_Bias = LCDBias_OneThird;                /*!< 1/3 bias 偏压驱动          */
    LCD_InitStructure.LCD_Com  = LCD_6COM;                        /*!< 6Com                       */
    LCD_InitStructure.LCD_CLK  = LCDCLK_Div1;                     /*!< 扫描频率最高               */
    LCD_InitStructure.LCD_ChargeMode  = LCDCharge_HighCurrent;    /*!< 大电流充电模式             */
    LCD_InitStructure.LCD_Vrsel = 0x02;                           /*!< 对比度配置                 */
    
    HT_LCD_Init(&LCD_InitStructure);      
}

/*
*********************************************************************************************************
*                                  SPI EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对SPI初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void SPI_Example()
{      
    
    /***** 以下代码用于配置SPI，以下代码不含GPIO配置 *****/  
    
    SPI_InitTypeDef  SPI_InitStructure;
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_SPI0EN, ENABLE);            /*!< 使能SPI0模块                */
    
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
* 函数说明: 利用库函数对Timer初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void TMR_Example()
{      
    
    /***** 以下代码用于配置定时器0，以下代码不含GPIO配置 *****/  
    
    TMR_InitTypeDef  TMR_InitStructure;
    
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_TMR0EN, ENABLE);            /*!< 使能TIMER0模块              */
    
    TMR_InitStructure.TimerMode = TIMCycleTiming;                  /*!< Timer设定为定时器功能       */
    TMR_InitStructure.TimerPreDiv = 0x00;                          /*!< Timer时钟等于系统时钟       */
    TMR_InitStructure.TimerPeriod = 0x1000;                        /*!< 定时周期设定                */
    TMR_InitStructure.TimerCompare = 0x00;                         /*!< 比较寄存器设定              */
    
    HT_TMR_Init(HT_TMR0, &TMR_InitStructure);  
}

/*
*********************************************************************************************************
*                                   RTC EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对RTC初始化示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void RTC_Example()
{      
    
    /***** 以下代码用于配置RTC模块，以下代码不含GPIO配置 *****/  
    
    RTC_InitTypeDef  RTC_InitStructure;

    RTC_InitStructure.RTC_TMR1.NewState = ENABLE;               /*!< 使能RTC定时器1              */   
    RTC_InitStructure.RTC_TMR1.Count = 5;                       /*!< 定时周期6s                  */
    RTC_InitStructure.RTC_TMR2.NewState = DISABLE;              /*!< 关闭RTC定时器2              */   
    RTC_InitStructure.RTC_TMR1.Count = 0;
    
    RTC_InitStructure.RTC_Alarm.Muster.hour = 0x99;             /*!< 不产生闹钟                  */
    RTC_InitStructure.RTC_Alarm.Muster.minute = 0x99;
    
    RTC_InitStructure.RTC_TOUT = Tout1Hz;                       /*!< Tout输出1Hz                 */
    
    HT_RTC_Init(&RTC_InitStructure); 
}

/*
*********************************************************************************************************
*                                   TBS EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对TBS配置示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void TBS_Example()
{      
    
    /***** 以下代码用于配置TBS模块，使能温度和电池电压测量，并配置周期 *****/  
    
//    HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC1S_MEM4S);    /*!< 温度测量1s打开一次              */
//    HT_TBS_PeriodSet(TBS_VBAT, TBS_TBSPRD_VBATPRD_8S);           /*!< 电池电压测量8s打开一次          */
    
    HT_TBSConfig(TBS_TBSCON_TMPEn|TBS_TBSCON_VBATEn, ENABLE);    /*!< 使能温度和电池电压测量          */
    
}

/*
*********************************************************************************************************
*                                   FLASH EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数对Flash操作示例
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void Flash_Example()
{      
    
    /***** 以下代码用于Flash操作 *****/  
    const uint8_t WriteCode[]={0,1,2,3,4,5,6,7,8,9};
    
    HT_Flash_PageErase(0x1FC00);                 /*!< 擦除地址0x1FC00所在页，对HT60xx，即最后一页   */
    
    HT_Flash_ByteWrite(WriteCode, 0x1FC00, sizeof(WriteCode));                 
                                                 /*!< 向地址0x1FC00写10字节                         */   
}

/*
*********************************************************************************************************
*                                 Interrupt EXAMPLES FOR HT60XX
*
* 函数说明: 利用库函数使能中断(以RTC中断为例)
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/
void Interrupt_Example()
{      
    
    /***** 以下代码用于使能RTC分钟和小时中断 *****/  
    
    HT_RTC_ITConfig(RTC_RTCIE_MINIE|RTC_RTCIE_HRIE, ENABLE);
    
    NVIC_EnableIRQ(RTC_IRQn);                                  /*!< 使能RTC总中断        */
    
    /***** 以下代码用于关闭RTC分钟和小时中断 *****/ 
    
    HT_RTC_ITConfig(RTC_RTCIE_MINIE|RTC_RTCIE_HRIE, DISABLE);
    
    /***** 以下代码用于关闭RTC总中断 *****/
    
    NVIC_DisableIRQ(RTC_IRQn);                                 /*!< 使能RTC总中断        */
}

/*
*********************************************************************************************************
*                                  CHIP LOW POWER CONFIGURE 
*
* 函数说明: 芯片低功耗配置，正常芯片在进入Hold或者Sleep模式前需要做此配置
*
* 入口参数: 无                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 1)此函数默认不参与工程编译
*           2)此函数配置不含GPIO，GPIO配置需根据具体工程
*           3)除了TBS中温度测量模块，此函数将关闭其他所有模块，用户可以根据自己方案的实际需求再打开其他模块
*********************************************************************************************************
*/
void LowPowerConfigure_Example()
{    
    CMU_InitTypeDef  CMU_InitStructure; 
    PMU_InitTypeDef  PMU_InitStructure; 
        
    HT_FreeDog();
    /**********  配置CMU 模块  **********/    
    CMU_InitStructure.SysClkSel = SysLF;
    CMU_InitStructure.CPUDiv = CPUDiv1;   
    HT_CMU_Init(&CMU_InitStructure);                    /*!< 配置系统时钟Fsys = Flf                   */
    
    HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0, DISABLE);       /*!< 关闭CLKCTRL0寄存器                       */
    HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1, DISABLE);       /*!< 关闭CLKCTRL1寄存器                       */
    
    HT_SHModeDogConfigure(DISABLE);                     /*!< Sleep/Hold模式下关闭WatchDog             */

    /**********  配置PMU 模块  **********/ 
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
    
    /**********  配置TBS 模块  **********/   
    HT_TBSConfig(TBS_TBSCON_VBATEn|TBS_TBSCON_ADC0En|TBS_TBSCON_ADC1En
                 |TBS_TBSCON_VCCEn, DISABLE);
                                                        /*!< 关闭除温度测量外其他所有模块              */
    HT_TBSConfig(TBS_TBSCON_TMPEn, ENABLE);             /*!< 使能温度测量                              */
    
//  HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS);  /*!< 对HT60xx，这里设置为64s打开一次           */
 // HT_TBS_PeriodSet(TBS_TMP, TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S);  /*!< 对HT61xx，这里应设置为0，即8s打开一次     */
}




#endif  /* __EXAMPLE_C */
