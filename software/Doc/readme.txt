

   ʱ��      |    ִ����     | �޸ļ�¼
2014/3/10    |     ywfu      | ��ɳ���V0.1
             |               |
2014/3/20    |     ywfu      | �汾��V0.2
             |               | �޸ļ�¼��1. ����Flash �ֽ�/����/�ֶ�  
             |               |           2. ��������жϱ�־����
             |               |           3. ����uartдbuf����
             |               |           4. ��������uart/timer֮ǰ����clkctrl0ע��˵��
             |               |           5. ����HT_RTC_LoadInfoData()��������ht60xx.hͷ�ļ�������HT_INFO�ṹ��
             |               |
2014/3/24    |     ywfu      | �汾��V0.3
             |               | �޸ļ�¼��1. HT_GPIO_Init()��������IO�����֮ǰ�д�
             |               |              �������£� GPIOx->IOCFG-------> GPIOx->PTDIR
             |               |           2. HT_GPIO_Init()��������Ϊ�ڶ����ù��� ��֮ǰ�д�
             |               |              �������£� GPIOx->IOCFG-------> GPIOx->AFCFG 
             |               |           3. HT_Flash_HalfWordWrite(), HT_Flash_HalfWordRead(),HT_Flash_WordWrite(),HT_Flash_WordRead(),HT_Flash_PageErase()
             |               |              ��Ϊ�ڶ����ַ�����ϴ������⣬�����޸� 
             |               |           4. ����ht60xx_reset.c
             |               |           5. CMU_InitTypeDef �ṹ��ı䣬ɾ��HRCDiv����
             |               |           6. HT_TMR_Init() �ı�Ĵ�������˳�������üĴ��������ʹ�ܶ�ʱ��
             |               |           7. example.c����LowPowerConfigure_Example()
             |               |               
2014/5/23    |     ywfu      | �汾��V0.4
             |               | �޸ļ�¼��1. TBS��Ƶ�����ں궨���޸�(ht60xx.h)        
             |               |           2. TBS�����趨�����޸�
             |               |           3. ɾ��TBS��VbatCMP����λ            
             |               |           4. SPI��д�����޸ģ�����ͳһ��Ϊ HT_SPI_SendByte()����
             |               |           5. �⺯��ͷ�ļ�ͳһ������"ht60xx_lib.h"
             |               |           6. CMU_IRQHandler��ΪDES_IRQHandler
             |               |           7. ����JTAGSTA״̬�Ĵ�����������Ӧ��״̬����HT_CMU_JTAGStatusGet()
             |               |           8. ���Ӷ�ͣ��״ָ̬ʾ��HT_CMU_StopFlagGet()
             |               |                      
2014/5/26    |     ywfu      | �汾��V0.5
             |               | �޸ļ�¼��1. ht60xx.h ͷ�ļ�enum IRQn��ע���޸�    
             |               |     
2014/6/11    |     ywfu      | �汾��V1.0~1.2
             |               | �޸ļ�¼��1. ����1301/1401/1409�����궨������       
             |               |     
2014/10/30   |     hxyu      | �汾��V1.3
             |               | �޸ļ�¼��1. �޸����Ϻ�Ĵ��� 




 