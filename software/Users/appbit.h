#ifndef _APP_BIT_HPP
#define _APP_BIT_HPP


enum appbit
{

  _cn_start_timer_ms,			// 	MS��ʱ������
  msTimer_R485Sendelay,		//  485������ʱ
	mstime_bmagnetism,					//	�Ÿ�Ӧ��ʱ��
	msTime_ComShow  ,				//  ͨѶ����˸��ʱ��
	msTime_Key, 
  msTime_offLineKeep,		//	ͣ�óּ�ʱ�䣬����ͣ����ڻ���
  msTime_OnLine,		//  �ϵ綨ʱ��
  msTime_OffLine,		//�����ʱ��,���ϵ�ʱ�����պ�ʱ׼���� �����״̬
  msTimer_LineSendelay,		//        �ز���ʱ
  msTimer_IRSendelay,		// ���ⷢ�ʹ�ʱ
  _cn_end_timer_ms,
  _cn_start_timer_half = _cn_end_timer_ms,
  hsecTime_OverVol,		//      ��ѹ��ʱ��
	hsecTime_reset,		//	 		��λ��ʱ��
	hsecTime_BeepTm, 	//	 		��ʾ�������ķ���ʱ��
	HscTime_FirstShow,
  _cn_end_timer_half,
  _cn_start_timer_sec = _cn_end_timer_half,
  secTime_DianLianSave = _cn_start_timer_sec,	//       �������涨ʱ��
  secTime_Keylong,		// ��������ʱ�������Ӽ��պ�
  secTime_Refresh,		//      ����8025T
  secTime_IRtm,			//  ������ʱ��������ʾ
  secTime_Rs485tm,		//  485��ʱ��������ʾ
  secTime_Linetm,		//  �ز���ʱ��������ʾ
  _cn_end_timer_sec,

  _cn_start_timer_2sec = _cn_end_timer_sec,
  _cn_end_timer_2sec,

  _cn_start_timer_rtcsec = _cn_end_timer_2sec,
  rsecTimer_ErrShow,		//       ��������ʾ
  _cn_end_timer_rtcsec,


  _cn_start_timer_min = _cn_end_timer_rtcsec,
  minTime_Impdelay = _cn_start_timer_min,	//  �������嶨ʱ������ֹ������������֮��ļ��̫��
  minTime_bitsave,
  _cn_end_timer_min,
  _cn_end_total_timer = _cn_end_timer_min,	// ��ʱ�����������

  _cn_start_meter_state = _cn_end_total_timer,	// �������״̬��
  bOverVol_statu,
  bOVerPow_statu,
  bOVerRev_Statu,
  _cn_end_meter_state,

  _cn_start_appbit = _cn_end_meter_state,	// λ״̬
  Event_Firstrun,		//      ��һ�����У���һ��Ϊ�����أ�������պ�
  Event_Rev,				//  		�����־
	Event_JBIb,				//	 		IB��У���־
	Event_JB5l,				//   		0.5L��У���־
	Event_JBsmall,		//   		С������У���־
	Event_bpsChange,		// 			�����ʸı��־  



  _cn_end_appbit,
  _cn_start_s_bit = _cn_end_appbit,	// ���籣��λ
  sEvent_open = _cn_start_s_bit,	//  Ϊ1���ǣ�Ϊ0�ո�
  sEvent_MDVT,			//  ���� 

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
  sEvent_systeminit,			//	 ϵͳ��ʼ��
  _cn_end_s_bit,
  _cn_end_total_bit = _cn_end_s_bit	// ����λ����ʱ����Ԫ����
};

#define			_cn_spmode_				14	//�л�������

#define		_KeyShow          	(!cGetB(Event_AutoShow))	// ����
#define		_AutoShow 					(cGetB(Event_AutoShow))	// ѭ��

#endif
