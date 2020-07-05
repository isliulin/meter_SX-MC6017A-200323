#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "stm32f1xx_hal.h"

#include "stm32_rtc_app.h"
#include "mainapp.h"

extern RTC_HandleTypeDef hrtc;





void
APP_RTC_SetAlarmCounter_IT (uint32_t AlarmCounter)
{
  RTC_AlarmTypeDef sAlarm;
  uint32_t counter_alarm = 0;
  uint32_t counter_time = 0;
  RTC_TimeTypeDef stime = { 0 };
  HAL_RTC_GetTime (&hrtc, &stime, RTC_FORMAT_BIN);

  // sprintf((char*)ShowTime1," GetTime %02d:%02d:%02d",stime.Hours, stime.Minutes, stime.Seconds);

  /* Convert time in seconds */
  counter_time =
    (uint32_t) (((uint32_t) stime.Hours * 3600) +
		((uint32_t) stime.Minutes * 60) + ((uint32_t) stime.Seconds));

  counter_alarm = counter_time + AlarmCounter;
  sAlarm.Alarm = RTC_ALARM_A;
  sAlarm.AlarmTime.Hours = (uint32_t) ((counter_alarm / 3600) % 24);
  sAlarm.AlarmTime.Minutes = (uint32_t) ((counter_alarm % 3600) / 60);
  sAlarm.AlarmTime.Seconds = (uint32_t) ((counter_alarm % 3600) % 60);
  // sprintf((char*)ShowTime2,"SetAlarm %02d:%02d:%02d", sAlarm.AlarmTime.Hours,  sAlarm.AlarmTime.Minutes,  sAlarm.AlarmTime.Seconds);
  if (HAL_RTC_SetAlarm_IT (&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler ();
    }
}

void
RTC_AlarmConfig (void)
{
  RTC_AlarmTypeDef salarmstructure;

  /*##-1- Configure the RTC Alarm peripheral ################################# */
  /* Set Alarm to 01:17:30
     RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmTime.Hours = 0x1;
  salarmstructure.AlarmTime.Minutes = 0x24;
  salarmstructure.AlarmTime.Seconds = 0x00;

  if (HAL_RTC_SetAlarm_IT (&hrtc, &salarmstructure, RTC_FORMAT_BCD) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler ();
    }
}

void
HAL_RTC_AlarmAEventCallback (RTC_HandleTypeDef * hrtc)
{
  _Mcu_Wake_RtcSet ();

}
