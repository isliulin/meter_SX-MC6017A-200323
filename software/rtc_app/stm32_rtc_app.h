#ifndef _stm32_rtcrtc_app_hpp_
#define _rtc_app_hpp_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"
#include "stm32f1xx_hal.h"


void APP_RTC_SetAlarmCounter_IT (uint32_t AlarmCounter);
static void RTC_AlarmConfig (void);
void HAL_RTC_AlarmAEventCallback (RTC_HandleTypeDef * hrtc);
//#define               _haltdebug_

//    __HAL_RTC_ALARM_DISABLE_IT(&hrtc, RTC_IT_ALRA);     关闭中断用的宏    
#endif
