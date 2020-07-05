#ifndef		_INDATA_OPER_HPP
#define		_INDATA_OPER_HPP


#include "pubset.h"


 #define  _Flasheck_Check() ((FlashInfo.RunInfo.StartCode != FLASH_INFO_START)||(FlashInfo.RunInfo.End != 0x16) )
unsigned char	cFlashinfo_Restore( void );
char Init_FlashData(void);
unsigned char	cFlashinfo_dianlian_backup( void );
unsigned char	cFlashinfo_backup( void );
void nSafeData_set_upb (void);	//       更新状态标志
void nSafeData_res_upb (void);	// 清除状态标志
unsigned char cSafeData_Data_Refreah(void);
unsigned char	 DispData_init( unsigned char cMode,unsigned char cComConfig);
#endif
