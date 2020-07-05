#ifndef _APP_BIT_HPP
#define _APP_BIT_HPP


enum appbit
{

  _cn_start_timer_ms,			// 	MS计时器定义
  msTimer_R485Sendelay,		//  485发送延时
	mstime_bmagnetism,					//	磁感应定时器
	msTime_ComShow  ,				//  通讯灯闪烁定时器
	msTime_Key, 
  msTime_offLineKeep,		//	停用持继时间，用于停电初期唤醒
  msTime_OnLine,		//  上电定时器
  msTime_OffLine,		//掉电掉时器,在上电时处理，闭合时准备进 入掉电状态
  msTimer_LineSendelay,		//        载波延时
  msTimer_IRSendelay,		// 红外发送处时
  _cn_end_timer_ms,
  _cn_start_timer_half = _cn_end_timer_ms,
  hsecTime_OverVol,		//      过压定时器
	hsecTime_reset,		//	 		复位定时器
	hsecTime_BeepTm, 	//	 		提示峰鸣器的发声时间
	HscTime_FirstShow,
  _cn_end_timer_half,
  _cn_start_timer_sec = _cn_end_timer_half,
  secTime_DianLianSave = _cn_start_timer_sec,	//       电量保存定时器
  secTime_Keylong,		// 常按键定时器，常接键闭合
  secTime_Refresh,		//      更新8025T
  secTime_IRtm,			//  红外延时，用于显示
  secTime_Rs485tm,		//  485延时，用于显示
  secTime_Linetm,		//  载波延时，用于显示
  _cn_end_timer_sec,

  _cn_start_timer_2sec = _cn_end_timer_sec,
  _cn_end_timer_2sec,

  _cn_start_timer_rtcsec = _cn_end_timer_2sec,
  rsecTimer_ErrShow,		//       错误码显示
  _cn_end_timer_rtcsec,


  _cn_start_timer_min = _cn_end_timer_rtcsec,
  minTime_Impdelay = _cn_start_timer_min,	//  输入脉冲定时器，防止两个输入脉冲之间的间隔太长
  minTime_bitsave,
  _cn_end_timer_min,
  _cn_end_total_timer = _cn_end_timer_min,	// 定时器定义结束、

  _cn_start_meter_state = _cn_end_total_timer,	// 电表运行状态字
  bOverVol_statu,
  bOVerPow_statu,
  bOVerRev_Statu,
  _cn_end_meter_state,

  _cn_start_appbit = _cn_end_meter_state,	// 位状态
  Event_Firstrun,		//      第一次运行，第一次为上升沿，其余均闭合
  Event_Rev,				//  		反向标志
	Event_JBIb,				//	 		IB点校表标志
	Event_JB5l,				//   		0.5L点校表标志
	Event_JBsmall,		//   		小电流点校表标志
	Event_bpsChange,		// 			波特率改变标志  



  _cn_end_appbit,
  _cn_start_s_bit = _cn_end_appbit,	// 掉电保存位
  sEvent_open = _cn_start_s_bit,	//  为1开盖，为0闭盖
  sEvent_MDVT,			//  报警 

  sEvent_11,
  sEvent_12,
  sEvent_13,
  sEvent_14,
  sEvent_15,
  sEvent_16,
  sEvent_17,
  sEvent_18,
  sEvent_19,
  sEvent_20,
  sEvent_21,
  sEvent_22,
  sEvent_23,
  sEvent_24,
  sEvent_25,
  sEvent_26,
  sEvent_27,
  sEvent_28,
  sEvent_29,
  sEvent_30,
  sEvent_systeminit,			//	 系统初始化
  _cn_end_s_bit,
  _cn_end_total_bit = _cn_end_s_bit	// 整个位及定时器单元结束
};

#define			_cn_spmode_				14	//切换脉冲宽度

#define		_KeyShow          	(!cGetB(Event_AutoShow))	// 键显
#define		_AutoShow 					(cGetB(Event_AutoShow))	// 循显

#endif
