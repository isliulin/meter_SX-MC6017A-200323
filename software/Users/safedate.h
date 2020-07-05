#ifndef  _SAFEDATA_HPP
#define  _SAFEDATA_HPP



#include "predef.h"


#define _cUpdate   0x55
#define	_cLog  		0x5a

enum
{
  _system_success = 0,
  _system_Errmem = 0x01,
  _system_ErrDate = 0x02,
	_system_ErrAppData =0x04,
	_system_ErrLog =0x8
};


u8 cSafeData_checkdata (void);
u8 cSafeData_restoreData (void);
void nSafeData_update2bak (void);
void nSafeData_set_upb (void);	//      更新状态标志
void nSafeData_res_upb (void);	//     清除状态标志
void nSafeData_struct_Init (u8 * datap, u8 * bak1p, u8 * bak2p, u8 clen);	// 初始化

unsigned char  cSafeData_Data_Refreah(void);


#endif
