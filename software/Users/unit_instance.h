#ifndef  _unit_instance_hpp
#define  _unit_instance_hpp

#include "bit_edge.h"
#include "soft_timer_filter.h"
#include "appbit.h"
#include "datastruct.h"

extern union unSTBITSTATU bbitstatu[_cn_end_total_bit];
extern unsigned char 	cmainnum ;   //  ����ָʾʱ�ӽ��ı�־��������ÿ ��һ�Σ���������ͼ�1
extern unsigned long lESTATE;
// BIT����ʱ�����ݴ洢�ռ�ָ��
#define  stbitp  		 bbitstatu
#define  ldbbit 	 lESTATE



#endif
