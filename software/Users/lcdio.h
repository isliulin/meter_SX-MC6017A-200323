/*
 * lcd_1.h
 *
 *  Created on: 2015-6-5
 *      Author: Pangxiao
 */

#ifndef LCD_1_H_
#define LCD_1_H_

enum
{


  _DISP_K_ = 0,
  _DISP_IMP_ = 1,
  _DISP_DOT_4_2_ = 2,
  _DISP_4_E_ = 3,
  _DISP_4_G_ = 4,
  _DISP_4_F_ = 5,

  _DISP_BAT_ = 8,		//??1
  _DISP_TIAOZHA_ = 9,		//??
  _DISP_5_D_ = 10,
  _DISP_5_C_ = 11,
  _DISP_5_B_ = 12,
  _DISP_5_A_ = 13,

  _DISP_TRX_ = 17,		//??485????
  _DISP_YUAN_ = 18,		//'?'
  _DISP_5_E_ = 19,
  _DISP_5_G_ = 20,
  _DISP_5_F_ = 21,

  _DISP_ST_4_ = 24,		//   
  _DISP_ST_2_ = 25,		// 
  _DISP_6_D_ = 26,
  _DISP_6_C_ = 27,
  _DISP_6_B_ = 28,
  _DISP_6_A_ = 29,
  _DISP_6_E_ = 35,
  _DISP_6_G_ = 36,
  _DISP_6_F_ = 37,

  _DISP_ST_3_ = 40,		//
  _DISP_ST_1_ = 41,		//
  _DISP_7_D_ = 42,
  _DISP_7_C_ = 43,
  _DISP_7_B_ = 44,
  _DISP_7_A_ = 45,
  _DISP_DIANLIAN_ = 48,		//?
  _DISP_GOU_ = 49,		//???
  _DISP_SHENGYU_ = 50,		//??
  _DISP_7_E_ = 51,
  _DISP_7_G_ = 52,
  _DISP_7_F_ = 53,
  _DISP_1_B_ = 56,
  _DISP_1_F_ = 57,
  _DISP_DOT_2_ = 58,
  _DISP_2_E_ = 59,
  _DISP_2_G_ = 60,
  _DISP_2_F_ = 61,
  _DISP_1_C_ = 64,
  _DISP_1_G_ = 65,
  _DISP_3_D_ = 66,
  _DISP_3_C_ = 67,
  _DISP_3_B_ = 68,
  _DISP_3_A_ = 69,

  _DISP_H_ = 72,
  _DISP_W_ = 73,
  _DISP_4_D_ = 74,
  _DISP_4_C_ = 75,
  _DISP_4_B_ = 76,
  _DISP_4_A_ = 77,
  _DISP_1_D_ = 80,
  _DISP_1_E_ = 81,
  _DISP_DOT_3_ = 82,
  _DISP_3_E_ = 83,
  _DISP_3_G_ = 84,
  _DISP_3_F_ = 85,
  _DISP_SHANGYUE_ = 88,
  _DISP_LO_2_ = 89,
  _DISP_8_D_ = 90,
  _DISP_8_C_ = 91,
  _DISP_8_B_ = 92,
  _DISP_8_A_ = 93,
  _DISP_1_A_ = 96,
  _DISP_DOT_1_ = 97,
  _DISP_2_D_ = 98,
  _DISP_2_C_ = 99,
  _DISP_2_B_ = 100,
  _DISP_2_A_ = 101,
  _DISP_DOT_2_2_ = 104,
  _DISP_LO_1_ = 105,
  _DISP_REVERSE_ = 106,		//      功率反向 
  _DISP_8_E_ = 107,
  _DISP_8_G_ = 108,
  _DISP_8_F_ = 109,
  _DISP_OVER_ = 110,

  // 
  _DISP_JIETI_2_ = _DISP_LO_2_,
  _DISP_JIETI_1_ = _DISP_LO_1_,
  _DISP_BAT1_ = _DISP_BAT_,

  //       显示描述符
  _SP_ = 0xFF,
  _BLINK_BIT_ = 0xFE,
  _DISP_DIGIT_ = 0xFD,

  _DISP_PUB_ = 6,		// 实际为空
  _DISP_PROG_ = 6,		// 实际为空
  _DISP_GOUDIAN_ = 6,		// 实际为空
  _DISP_LOCK_ = 6,		// 实际为空
  _DISP_NEG_ = 6,		// 实际为空
  _DISP_TOUZHI_ = 6,		// 实际为空
  _DISP_DOT_4_ = 6,		// 实际为空
  _DISP_ZONG_ = 6,		// 实际为空
  _DISP_JIAN_ = 6,		// 实际为空
  _DISP_FENG_ = 6,		// 实际为空
  _DISP_PING_ = 6,		// 实际为空
  _DISP_GU_ = 6,		// 实际为空
  _DISP_AT_JIAN_ = 6,		// 实际为空
  _DISP_AT_FENG_ = 6,		// 实际为空
  _DISP_AT_PING_ = 6,		// 实际为空
  _DISP_AT_GU_ = 6,		// 实际为空
  _DISP_KW_ = 6,		// 实际为空 
  _DISP_V_ = 6,			// 实际为空
  _DISP_A_ = 6,			// 实际为空
  _DISP_PULSE_ = 6,		// 实际为空
  _DISP_TUNJI_ = 6,		// 实际为空
  _DISP_DUKA_ = 6,		// 实际为空
  _DISP_SHIBAI_ = 6,		// 实际为空
  _DISP_LIANG_ = 6,		// 实际为空
  _DISP_ZHONG_ = 6,		// 实际为空
  _DISP_CHENGGONG_ = 6,		// 实际为空
  _DISP_DIAN_ = 6,		// 实际为空
  _DISP_N_ = 6,			// 实际为空
  _DISP_DANGQIAN_ = 6,		// 实际为空
  _DISP_TM_SHI_ = 6,		// 实际为空
  _DISP_HAO_ = 6,		// 实际为空
  _DISP_HU_ = 6,		// 实际为空
  _DISP_FAN_ = 6,		// 实际为空
  _DISP_JIETI_ = 6,		// 实际为空
  _DISP_YUE_ = 6,		// 实际为空
  _DISP_CHANGSHU_ = 6,		// 实际为空
  _DISP_ZHENG_ = 6,		// 实际为空
  _DISP_SHANG_ = 6,		// 实际为空
  _DISP_XIANG_ = 6,		// 实际为空
  _DISP_TM_JIAN_ = 6,		// 实际为空
  _DISP_BIAO_ = 6,		// 实际为空
  _DISP_JIA_ = 6,		// 实际为空
  _DISP_10_1_ = 6,		// 实际为空
  _DISP_YONG_ = 6,		// 实际为空
  _DISP_L_ = 6,			// 实际为空
  _DISP_JIN_E_ = 6,		// 实际为空
  _DISP_TOUZHI_H_ = 6,		// 实际为空
  _DISP_COS_ = 6,		// 实际为空
};
void LCD_Init (void);
void UpdateDisp (char *sp);
#endif /* LCD_1_H_ */
