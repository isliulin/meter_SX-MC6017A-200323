/*
 * spset.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef SPSET_H_
#define SPSET_H_

#ifndef		_CHIP_SET_
#define		_CHIP_SET_


#define		_MAXIM_71315_
/*			以下部分应在工程中定义			*/

//#define		_METER_LOC_
//#define		_JD_MODE_		1
#define		_M_MODE_		0x11
//#define    _JD_MODE_IN_


//#define		_USE_DEMOD_											//使用红外光敏管

/*			工程中定义部分结束				*/



//#define		_MAX_TARRIF_	4
#include	"GC_5_60_1200.h"

#if		_USE_ECHIP_ == _USE_71071_
	#define		CHIP_SET_LEN		100							//
#endif

#define		TRX_NUM				TRx_Num							//通讯地址与表号分离
#define		_DFT_TIME_			(_TIME_DAY_(10,1,1)*86400)		//10年1月1日

#define		_PROG_CNT_INC_										//可读取未结束编程记录

#endif



 
#endif /* SPSET_H_ */
