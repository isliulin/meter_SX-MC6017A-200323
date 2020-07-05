 
#ifndef _systick_app_h_
#define _systick_app_h_

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"

#define 		_mainfreq_					8000000
#define 		_systick_step_time_ms_  		5
#define _cMain_sequentialTime_ 		 _systick_step_time_ms_ 	//	主节拍计算时间MS
#define  		_systick_config_ 			(_mainfreq_/160)

void SystemTick_Start (void);
void SystemTick_Stop (void);
void SystemTick_Interrupt_On(void);

void SystemTick_Interrupt_Off(void);
void SysTick_Handler(void);

void SysTick_fun(void);
#endif /* LCD_1_H_ */
