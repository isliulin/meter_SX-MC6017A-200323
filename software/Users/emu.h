#ifndef __HT60xx_EMU_H
#define __HT60xx_EMU_H

#include "ht6xxx.h"

/********************* 全局宏定义 ***********************************/
#define			EPR_SPLI1 				      0x00
#define			EPR_SPLI2 				      0x04
#define			EPR_SPLU  				      0x08
#define			EPR_SPLP  				      0x0C
#define			EPR_SPLQ 				        0x10
#define			EPR_FastRMSI1 				  0x14
#define			EPR_FastRMSI2  				  0x18
#define			EPR_FastRMSU  				  0x1C
#define			EPR_FreqU 				      0x20
#define			EPR_FastPowerP1 				0x24
#define			EPR_FastPowerQ1  				0x28
#define			EPR_FastPowerP2  				0x2C
#define			EPR_FastPowerQ2 				0x30
#define			EPR_FastPowerS1 				0x34
#define			EPR_FastPowerS2  				0x38
#define			EPR_RMSI1  				      0x3C
#define			EPR_RMSI2 				      0x40
#define			EPR_RMSU 				        0x44
#define			EPR_PowerP1  				    0x48
#define			EPR_PowerQ1  				    0x4C
#define			EPR_PowerP2 			    	0x50
#define			EPR_PowerQ2 			    	0x54
#define			EPR_PowerS1  				    0x58
#define			EPR_PowerS2  				    0x5C
#define			EPR_EnergyP 				    0x60
#define			EPR_EnergyQ 				    0x64
#define			EPR_EnergyS  				    0x68
#define			EPR_EnergyPC  				  0x6C
#define			EPR_EnergyQC 				    0x70
#define			EPR_EnergySC 				    0x74
#define			EPR_DC_UAverage  				0x78
#define			EPR_DC_I1Average  		  0x7C
#define			EPR_DC_I2Average 				0x80
#define			EPR_Checksum 				    0x84
#define			EPR_Upeak  				      0x88
#define			EPR_I1peak  				    0x8C
#define			EPR_I2peak 				      0x90
#define			EPR_PFCNT_Photo 				0x94
#define			EPR_QFCNT_Photo  				0x98
#define			EPR_SFCNT_Photo  				0x9C
#define			EPR_AutoUgain 				  0xA0
#define			EPR_AutoI1gain 				  0xA4
#define			EPR_AutoI2gain  				0xA8

#define     VAR_EMUSR               0x00
#define     VAR_EMUIE               0x04
#define     VAR_EMUIF               0x08
#define     VAR_GP1                 0x0C
#define     VAR_GQ1                 0x10
#define     VAR_GS1                 0x14
#define     VAR_GPhs1               0x18
#define     VAR_GP2                 0x1C
#define     VAR_GQ2                 0x20
#define     VAR_GS2                 0x24
#define     VAR_GPhs2               0x28
#define     VAR_QPhsCal             0x2C
#define     VAR_I2Gain              0x30
#define     VAR_I1Off               0x34
#define     VAR_I2Off               0x38
#define     VAR_UOff                0x3C
#define     VAR_PStart              0x40
#define     VAR_QStart              0x44
#define     VAR_SStart              0x48
#define     VAR_HFconst             0x4C
#define     VAR_ADCCFG              0x50
#define     VAR_CHNLCR              0x54
#define     VAR_EMCON               0x58
#define     VAR_PFCnt               0x5C
#define     VAR_QFCnt               0x60
#define     VAR_SFCnt               0x64
#define     VAR_ADCCON              0x68
#define     VAR_IPTAMP              0x6C
#define     VAR_ICHK                0x70
#define     VAR_EMUCTRL             0x74
#define     VAR_P1OFFSET            0x78
#define     VAR_P2OFFSET            0x7C
#define     VAR_Q1OFFSET            0x80
#define     VAR_Q2OFFSET            0x84
#define     VAR_I1RMSOFFSET         0x88
#define     VAR_I2RMSOFFSET         0x8C
#define     VAR_URMSOFFSET          0x90
#define     VAR_RosiCtrl            0x94
#define     VAR_ANA_control         0x98
#define     VAR_UCONST              0x9C
#define     VAR_LpIdleTime          0xA0
#define     VAR_USAGLVL             0xA4
#define     VAR_USagCyc             0xA8
#define     VAR_UOVLVL              0xAC
#define     VAR_OvCyc               0xB0
#define     VAR_IOVLVL              0xB4
#define     VAR_ZXILVL              0xB8
#define     VAR_PDataCpH            0xBC
#define     VAR_PDataCpL            0xC0
#define     VAR_QDataCpH            0xC4
#define     VAR_QDataCpL            0xC8
#define     VAR_SDataCpH            0xCC
#define     VAR_SDataCpL            0xD0
#define     VAR_FilterCtrl          0xD4
#define     VAR_TUgain              0xD8
#define     VAR_TI1gain             0xDC
#define     VAR_TI2gain             0xE0
#define     VAR_UTCcoffA            0xE4
#define     VAR_UTCcoffB            0xE8
#define     VAR_UTCcoffC            0xEC
#define     VAR_I1TCcoffA           0xF0
#define     VAR_I1TCcoffB           0xF4
#define     VAR_I1TCcoffC           0xF8
#define     VAR_I2TCcoffA           0xFC
#define     VAR_I2TCcoffB           0x100
#define     VAR_I2TCcoffC           0x104
#define     VAR_LoadDataCp          0x108
#define     VAR_SRSTREG             0x114

#define			VAR_Krms		          0x120	//电压系数
#define			VAR_Kims		          0x121	//电流通道系数
#define			VAR_Kp		            0x122	//有功功率系数

//校表参数寄存器
typedef union
{
  uint8_t ec_array[2 * 68 + 4 * 3 + 2];	//下面定义与EEPROM内定义必须完全一致
  struct
  {
    uint16_t GP1;		//0x0C
    uint16_t GQ1;		//0x10
    uint16_t GS1;		//0x14
    uint16_t GPhs1;		//0x18
    uint16_t GP2;		//0x1C
    uint16_t GQ2;		//0x20
    uint16_t GS2;		//0x24
    uint16_t GPhs2;		//0x28
    uint16_t QPhsCal;		//0x2C
    uint16_t I2Gain;		//0x30
    uint16_t I1Off;		//0x34
    uint16_t I2Off;		//0x38
    uint16_t UOff;		//0x3C
    uint16_t PStart;		//0x40
    uint16_t QStart;		//0x44
    uint16_t SStart;		//0x48
    uint16_t HFconst;		//0x4C
    uint16_t ADCCFG;		//0x50
    uint16_t CHNLCR;		//0x54
    uint16_t EMCON;		//0x58    
    uint16_t ADCCON;		//0x68
    uint16_t IPTAMP;		//0x6C
    uint16_t ICHK;		//0x70
    uint16_t EMUCTRL;		//0x74
    uint16_t P1OFFSET;		//0x78
    uint16_t P2OFFSET;		//0x7C
    uint16_t Q1OFFSET;		//0x80
    uint16_t Q2OFFSET;		//0x84
    uint16_t I1RMSOFFSET;	//0x88
    uint16_t I2RMSOFFSET;	//0x8C
    uint16_t URMSOFFSET;	//0x90
    uint16_t RosiCtrl;		//0x94
    uint16_t ANA_control;	//0x98
    uint16_t UCONST;		//0x9C
    uint16_t LpIdleTime;	//0xA0
    uint16_t USAGLVL;		//0xA4
    uint16_t USagCyc;		//0xA8
    uint16_t UOVLVL;		//0xAC
    uint16_t OvCyc;		//0xB0
    uint16_t IOVLVL;		//0xB4
    uint16_t ZXILVL;		//0xB8
    uint16_t PDataCpH;		//0xBC
    uint16_t PDataCpL;		//0xC0
    uint16_t QDataCpH;		//0xC4
    uint16_t QDataCpL;		//0xC8
    uint16_t SDataCpH;		//0xCC
    uint16_t SDataCpL;		//0xD0
    uint16_t FilterCtrl;	//0xD4
    uint16_t TUgain;		//0xD8
    uint16_t TI1gain;		//0xDC
    uint16_t TI2gain;		//0xE0
    uint16_t UTCcoffA;		//0xE4
    uint16_t UTCcoffB;		//0xE8
    uint16_t UTCcoffC;		//0xEC
    uint16_t I1TCcoffA;		//0xF0
    uint16_t I1TCcoffB;		//0xF4
    uint16_t I1TCcoffC;		//0xF8
    uint16_t I2TCcoffA;		//0xFC
    uint16_t I2TCcoffB;		//0x100
    uint16_t I2TCcoffC;		//0x104
    uint16_t LoadDataCp;	//0x108
    uint16_t SRSTREG;		//0x114
    uint16_t EMUSR;		//0x00
    uint16_t EMUIE;		//0x04
    uint16_t EMUIF;		//0x08
    uint16_t PFCnt;		//0x5C
    uint16_t QFCnt;		//0x60
    uint16_t SFCnt;		//0x64

    float Emu_Krms;		//电压系数
    float Emu_Kims;		//电流通道系数
    float Emu_Kp;		//有功功率系数
    uint16_t CheckSum;
  } ECR;
} EMUC;

#define   _cSumCheck_Len_     (((int)&(((EMUC *)0)->ECR.I2TCcoffC))/sizeof(uint16_t))

#define  LF_225K_MODE   0
#define  LF_32K_MODE    1

#define  EMU_LF_MODE    LF_225K_MODE
#define  PowerP1_5A     0x005F2A1A
#define  EC             1600

/********************* 全局变量声明 ***********************************/
extern EMUC emu_ecr_reg;
extern uint8_t emu_cal_mode;
extern uint8_t flag_emu_cal_mode;
extern uint32_t emu_var_checksum;
extern float kims_fast_rmsi1;
extern uint32_t fast_rmsi1;
extern uint8_t hold_flag;
extern uint32_t energyP;

#define 	_bREVP			(HT_EMUECA->EMUSR & 0x1 )	//反向有功功率指示标识信号，当检测到负有功功率时，该信号为1。当再次检测到正有功功率时，该信号为0。在PF发脉冲时更新该值。

/********************* 全局函数声明 ***********************************/
void emu_init (void);
void emu_calibrate (void);
void emu_proc (void);
uint32_t Read_EPADR (uint16_t address);
uint16_t Read_ECADR (uint16_t address);
void Write_ECADR (uint16_t address, uint16_t data);
uint32_t ReadUrms (void);
uint32_t ReadI1rms (void);
uint32_t ReadPowerP (void);
void get_checksum (uint8_t * start, uint8_t len);
uint8_t check_checksum (EMUC *);

uint32_t fast_get_rmsi1 (void);
void write_power_data (void);
void Save_Energy (void);

enum ENUMCHIPMODE
{
  _cChipRun_Mode_ = 0,		//       普通模式
  _cChipXiaoBiao_Mode_		//      校表模式
};
#define			_IsRunMode(a)    ( _cChipRun_Mode ==  a)
#define			_IsXBMode(a)    (_cChipXiaoBiao_Mode_ == a )
#define 			_SetChipMode(a,b)	{a= b;}
void Chip_Trim (void);
struct STCHIPREG
{
  unsigned short iNo;
  unsigned long iConfig;
};

#define			 _cRegMax_	14	// 寄存器个数
#define 		_cKpMax_   3	//      系数个数

struct STCHIPSET
{
  struct STCHIPREG stChipreg[_cRegMax_];
  struct STCHIPREG stKset[_cKpMax_];
};
char Emc_Var_Write (void);
#define stChipSet   (*((struct STCHIPSET *)stflashdata.sEmuc))	
void Chip_Trim_IB (void);
#define 	_emu_data_reset()		Write_ECADR(VAR_SRSTREG,  0x55)	//        复位校表参数
#define 	_emu_module_reset() Write_ECADR(VAR_SRSTREG,  0xaa)	//              复位EMCs模块
void Emu_Chip_to_StructData (void);
void Emu_Structdata_To_Chip (void);

#endif /* __HT60xx_EMU_H */
