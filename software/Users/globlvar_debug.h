#ifndef  _globalvar_debug_hpp_
#define  _globalvar_debug_hpp_

extern volatile unsigned long lDebugIntState;

#ifdef _DEBUG_
#define 		_INTSTATE_MASK(a)   	{ lDebugIntState |= ( (unsigned long )1<< a);}
#define 		_INTSTATE_REMASK(a)   { lDebugIntState &= ~( (unsigned long )1<< a);} 
#define 		_ISINTSTATE()   ( lDebugIntState )
#else
#define 		_INTSTATE_MASK(a)   	{}
#define 		_INTSTATE_REMASK(a)   {}
#define 		_ISINTSTATE()   ( 0 )
#endif

enum ENUMINTSTAT
{
  _DMA1_Channel4_IRQHandler_ = 0,	//; DMA1 Channel 4
  _USART1_IRQHandler_,		//; USART1

  _NMI_Handler_,		//  ; NMI Handler
  _HardFault_Handler_,		//; Hard Fault Handler
  _MemManage_Handler_,		//; MPU Fault Handler
  _BusFault_Handler_,		// ; Bus Fault Handler
  _UsageFault_Handler_,		//; Usage Fault Handler
  _SVC_Handler_,		//  ; SVCall Handler
  _DebugMon_Handler_,		// ; Debug Monitor Handler
  _PendSV_Handler_,		//   ; PendSV Handler
  _SysTick_Handler_,		//  ; SysTick Handler
  _WWDG_IRQHandler_,		//  ; Window Watchdog
  _PVD_IRQHandler_,		//   ; PVD through EXTI Line detect
  _TAMPER_IRQHandler_,		//; Tamper
  _RTC_IRQHandler_,		//   ; RTC
  _FLASH_IRQHandler_,		// ; Flash
  _RCC_IRQHandler_,		//   ; RCC
  _EXTI0_IRQHandler_,		// ; EXTI Line 0
  _EXTI1_IRQHandler_,		// ; EXTI Line 1
  _EXTI2_IRQHandler_,		// ; EXTI Line 2
  _EXTI3_IRQHandler_,		// ; EXTI Line 3
  _EXTI4_IRQHandler_,		// ; EXTI Line 4
  _DMA1_Channel1_IRQHandler_,	//; DMA1 Channel 1
  _DMA1_Channel2_IRQHandler_,	//; DMA1 Channel 2
  _DMA1_Channel3_IRQHandler_,	//; DMA1 Channel 3
//_DMA1_Channel4_IRQHandler_,//; DMA1 Channel 4
  _DMA1_Channel5_IRQHandler_,	//; DMA1 Channel 5
  _DMA1_Channel6_IRQHandler_,	//; DMA1 Channel 6
  _DMA1_Channel7_IRQHandler_,	//; DMA1 Channel 7
  _ADC1_IRQHandler_,		//  ; ADC1
  _USB_HP_IRQHandler_,		//; USB High Priority
  _USB_LP_IRQHandler_,		//; USB Low  Priority
  _EXTI9_5_IRQHandler_,		//; EXTI Line 9..5
  _TIM2_IRQHandler_,		//  ; TIM2
  _TIM3_IRQHandler_,		//  ; TIM3
  _TIM4_IRQHandler_,		//  ; TIM4
  _I2C1_EV_IRQHandler_,		//; I2C1 Event
  _I2C1_ER_IRQHandler_,		//; I2C1 Error
  _I2C2_EV_IRQHandler_,		//; I2C2 Event
  _I2C2_ER_IRQHandler_,		//; I2C2 Error
  _SPI1_IRQHandler_,		//  ; SPI1
  _SPI2_IRQHandler_,		//  ; SPI2
 // _USART1_IRQHandler_,//; USART1
  _USART2_IRQHandler_,		//; USART2
  _USART3_IRQHandler_,		//; USART3
  _EXTI15_10_IRQHandler_,	//    ; EXTI Line 15..10
  _RTC_Alarm_IRQHandler_,	//     ; RTC Alarm through EXTI Line
  _USBWakeUp_IRQHandler_,	//    ; USB Wakeup from suspend
  _Vectors_End_,
};


enum ENUMERRNO
{
  _debug_errno_null_,
  _debug_errno_resetvector_,	//debug_break(_debug_errno_resetvector_);
  _debug_errno_wdgout_,		//debug_break(_debug_errno_wdgout_);
  _debug_errno_wdginterrupt_,
  _debug_errno_flashdatarestore_,
  _debug_errno_prog_reset_,
  _debug_errno_harfault_,	//debug_break(_debug_errno_harfault_);
  _debug_errno_NMI_Handler_,
  _debug_errno_SVC_Handler_,
  _debug_errno_PendSV_Handler_,
  _debug_errno_DebugMon_Handler_,
  _debug_errno_MemManage_Handler_,
  _debug_errno_BusFault_Handler_,
  _debug_errno_UsageFault_Handler_,

  _debug_errno_Call_Null_,

	

  _debug_errno_timezero_,
  

  _debug_app_uart0_ErrorCallback_int_,	// debug_break(_debug_app_uart0_ErrorCallback_int_);
  
   _debug_app_into_haltmode_,// debug_break(__debug_app_into_haltmode_);
  _debug_errno_in_int_,

  _debug_app_nobreak_,		//==================
	  _debug_RTC_Second_,		//debug_break(_debug_RTC_Second_);
  _debug_RTC_Alarm_,		//debug_break(_debug_RTC_Alarm_);

_debug_app_uart3_ErrorCallback_int_,	// debug_break(_debug_app_uart3_ErrorCallback_int_);
  _debug_HAL_UART_TxCpltCallback_,

  _debug_HAL_UART_TxCpltCallback1_,
  _debug_DMA1_Channel4_IRQHandler1,
  _debug_USART1_IRQHandler_1_,
  _debug_USART1_IRQHandler_2_,
  _debug_USART1_IRQHandler_3_,
  _debug_USART1_IRQHandler_4_,
  _debug_USART1_IRQHandler_5_,

  _debug_app_uart0_int_,
  _debug_app_uart1_int_,
  _debug_app_uart3_int_,
  _debug_app_uart4_int_,
  _debug_app_uart5_int_,

  _debug_UART_DMATransmitCplt_,


  _debug_errno_SysTick_Handler_,
  _debug_Usart_Dma_Rece_Handler_,
  _debug_HAL_UART_RxCpltCallback_,	
	_debug_app_gprsfroze_push_,
	
	_debug_app_GprsolineFroze_Get_,
		 _debug_errno_EXT0_Handler_,
	 _debug_errno_EXT4_Handler_,
	_debug_errno_DMA1_Channel1_Handler_,
	_debug_errno_DMA1_Channel2_Handler_,
	
	_debug_errno_DMA1_Channel3_Handler_,
	_debug_errno_DMA1_Channel4_Handler_,
	
	_debug_errno_DMA1_Channel5_Handler_,
	_debug_errno_DMA1_Channel6_Handler_,
	
	_debug_errno_DMA1_Channel7_Handler_,
	
	_debug_errno_TIM2_IRQHandler_,
	_debug_errno_TIM3_IRQHandler_,
	
	_debug_app_timechangefroze_push_,
	_debug_app_onlinefroze_push_,
	_debug_app_systemrunfroze_Push_,
	_debug_app_OnlineunitFroze_Get_,
	_debug_app_TimechangeFroze_Get_,
	_debug_app_SystemrunFroze_Get_,
		_debug_app_ModifyTime_,
	_debug_app_TimeUpdate_,
	_debug_Gprs_qgnssts_answer_,
	_debug_modbusfun_,		//debug_break(_debug_modbusfun_);
_debug_app_time3_int_,	// debug_break(_debug_app_time3_int_);
  _debug_app_uart2_int_,	// debug_break(_debug_app_time2_int_);
	
  _debug_aroundmode_slave_run_rececheckt_,	// debug_break(_debug_aroundmode_slave_run_rececheckt_);
  _debug_gprs_changeto_master_,	// debug_break(_debug_gprs_changeto_master_),
  _debug_gprs_changeto_master645_,	// debug_break(_debug_gprs_changeto_master645_),
  _debug_gprs_changeto_ftp_,	// debug_break(_debug_gprs_changeto_ftp_),
  _debug_gprs_changeto_slave_,	// debug_break(_debug_gprs_changeto_slave_),
  _debug_gprs_location_success_,	// debug_break(_debug_gprs_changeto_slave_),
  _debug_gprs_distance_over_,	// debug_break(_debug_gprs_distance_over_);
  _debug_gprs_nolink_too_long_,	// debug_break(_debug_gprs_nolink_too_long_)
  _debug_gprs_ftp_too_long_,	// debug_break(_debug_gprs_ftp_too_long_)
  _debug_gprs_nocomm_too_long_,	// debug_break(_debug_gprs_nocomm_too_long_)
  _debug_gprs_rece_answer_,	//debug_break(_debug_gprs_rece_answer_);
  _debug_gprs_changeto_mainremporttime_,	// debug_break(_debug_gprs_changeto_mainremporttime_),



  _debug_Modbus_Timeout_,


  _debug_errno_end_,




};
void
debug_break (enum ENUMERRNO ierrno);
#endif
