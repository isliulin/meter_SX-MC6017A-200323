

   时间      |    执行人     | 修改记录
2014/3/10    |     ywfu      | 完成初版V0.1
             |               |
2014/3/20    |     ywfu      | 版本号V0.2
             |               | 修改记录：1. 增加Flash 字节/半字/字读  
             |               |           2. 增加清除中断标志函数
             |               |           3. 增加uart写buf函数
             |               |           4. 增加配置uart/timer之前先配clkctrl0注释说明
             |               |           5. 增加HT_RTC_LoadInfoData()函数，在ht60xx.h头文件中增加HT_INFO结构体
             |               |
2014/3/24    |     ywfu      | 版本号V0.3
             |               | 修改记录：1. HT_GPIO_Init()函数配置IO输出，之前有错。
             |               |              修正如下： GPIOx->IOCFG-------> GPIOx->PTDIR
             |               |           2. HT_GPIO_Init()函数配置为第二复用功能 ，之前有错。
             |               |              修正如下： GPIOx->IOCFG-------> GPIOx->AFCFG 
             |               |           3. HT_Flash_HalfWordWrite(), HT_Flash_HalfWordRead(),HT_Flash_WordWrite(),HT_Flash_WordRead(),HT_Flash_PageErase()
             |               |              因为在对齐地址访问上存在问题，已做修改 
             |               |           4. 增加ht60xx_reset.c
             |               |           5. CMU_InitTypeDef 结构体改变，删除HRCDiv变量
             |               |           6. HT_TMR_Init() 改变寄存器配置顺序，先配置寄存器，最后使能定时器
             |               |           7. example.c增加LowPowerConfigure_Example()
             |               |               
2014/5/23    |     ywfu      | 版本号V0.4
             |               | 修改记录：1. TBS打开频率周期宏定义修改(ht60xx.h)        
             |               |           2. TBS周期设定函数修改
             |               |           3. 删除TBS中VbatCMP控制位            
             |               |           4. SPI读写数据修改，现在统一改为 HT_SPI_SendByte()函数
             |               |           5. 库函数头文件统一包含到"ht60xx_lib.h"
             |               |           6. CMU_IRQHandler改为DES_IRQHandler
             |               |           7. 增加JTAGSTA状态寄存器，增加相应读状态函数HT_CMU_JTAGStatusGet()
             |               |           8. 增加读停振状态指示函HT_CMU_StopFlagGet()
             |               |                      
2014/5/26    |     ywfu      | 版本号V0.5
             |               | 修改记录：1. ht60xx.h 头文件enum IRQn处注释修改    
             |               |     
2014/6/11    |     ywfu      | 版本号V1.0~1.2
             |               | 修改记录：1. 整合1301/1401/1409，并宏定义区分       
             |               |     
2014/10/30   |     hxyu      | 版本号V1.3
             |               | 修改记录：1. 修改整合后的错误 




 