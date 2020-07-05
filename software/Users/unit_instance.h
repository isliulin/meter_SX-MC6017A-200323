#ifndef  _unit_instance_hpp
#define  _unit_instance_hpp

#include "bit_edge.h"
#include "soft_timer_filter.h"
#include "appbit.h"
#include "datastruct.h"

extern union unSTBITSTATU bbitstatu[_cn_end_total_bit];
extern unsigned char 	cmainnum ;   //  用于指示时钟节拍标志，主节拍每 进一次，这个变量就加1
extern unsigned long lESTATE;
// BIT及定时器数据存储空间指针
#define  stbitp  		 bbitstatu
#define  ldbbit 	 lESTATE



#endif
