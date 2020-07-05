#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "emu.h"
#include "bit_edge.h"
#include "pubset.h"
#include "flash_filesystem.h"
#include "math.h"
#include "hal_delay_us.h"

 #define 	read_eeprom(a,b,c)		{}  //(uint8_t*)&emu_ecr_reg,EE_EMU_VAR_225K,150);
		#define  write_eeprom(a,b,c)	{}

void Emu_StructData_To_Chip (void);
// EMU软件复位后延时
const unsigned short _iEmuSoftResetDelay = 1000;
// EMU校表数据复位后延时
const unsigned short _iEmuDataResetDelay = 1000;

extern const struct STFLASHDATA stflashdata;
extern enum ENUMCHIPMODE enChipMode;
extern EMUC emu_ecr_reg;

uint8_t emu_cal_mode;
uint8_t flag_emu_cal_mode;
uint32_t emu_var_checksum;

float kims_fast_rmsi1;
uint32_t fast_rmsi1;
uint8_t hold_flag;

uint8_t flag_emu_mode;
uint8_t flag_fast_rmsi1;

uint32_t energyP;

uint32_t EP_old;
uint32_t ep_disp, ep_save;
uint8_t flag_ee_save;

static void emu_var_cal (void);

static void load_emu_var (void);
static void load_emu_var_225k (void);
static void load_emu_var_32k (void);
static void load_emu_var_const (void);

void GetAVR (strVI * pVI, u32 Data);
//extern struct         STCHIPSET  stChipSet;


void
delay (uint32_t t)
{
  while (--t);
}

void
emu_init (void)
{
  HT_CMU->WPREG = 0xA55A;
// HT_CMU->CLKCTRL0 |= 0x8000; //enable EMU

  HT_GPIOF->IOCFG |= 0x0001;	//PF0 as PFOUT
  HT_EMUECA->EMUIF = 0x0000;

  emu_cal_mode = 0;
  flag_emu_cal_mode = 0;
  hold_flag = 0;
  flag_fast_rmsi1 = 0;
// read_eeprom((uint8_t*)&kims_fast_rmsi1,EE_EMU_KIMS_FAST,4);

//opt   read_eeprom((uint8_t*)&energyP,EE_SAVE_ENERGY_START,4);
  if (energyP == 0xFFFFFFFF)
    {
      energyP = 0;
      //opt   write_eeprom((uint8_t*)&energyP,EE_SAVE_ENERGY_START,4);
      delay (3333);
    }
  EP_old = HT_EMUEPA->ENERGYP;
  ep_disp = 0;
  ep_save = 0;
  flag_ee_save = 1;

  flag_emu_mode = 0;
  if ((HT_PMU->PMUSTA & 0x01) != 0)	//normal
    {
      flag_ee_save = 0;

      HT_CMU->CLKCTRL0 |= 0x8000;	//enable EMU
      //load_emu_var();
      emu_var_cal ();
      emu_ecr_reg.ECR.CheckSum = Read_EPADR (EPR_Checksum);
    }

// if(((HT_PMU->PMUSTA&0x04) != 0)&&((HT_PMU->PMUSTA&0x01) != 0)) //normal
// {
// load_emu_var();
// flag_emu_mode=0;
// }
// else if(((HT_PMU->PMUSTA&0x04) == 0)&&((HT_PMU->PMUSTA&0x01) != 0)) //lf
// {
// #if (EMU_LF_MODE == LF_225K_MODE)
// load_emu_var_225k();
// flag_emu_mode=1;
//
// run_osc();
//
// PageNum=1;
//
// #else
// load_emu_var_32k();
// flag_emu_mode=2;
//
// run_osc();
// close_pll();
//
// PageNum=1;
// #endif
// }
// else if((HT_PMU->PMUSTA&0x01) == 0)                 //const
// {
// run_hrc();
// open_pll();
// close_hrc();
//
// load_emu_var_const();
// flag_emu_mode=3;

// PageNum=1;
//
// write_power_data();
// emu_var_checksum = Read_EPADR(EPR_Checksum);
//
// hold_flag=1;
// flag_fast_rmsi1=1;
//
// TaskDisplay();
// enter_hold_init();
// EnterHold();     //enter HOLD
//
// hold_wake_init();
// }
//
// emu_var_checksum = Read_EPADR(EPR_Checksum);
}

void
load_emu_var (void)
{
  read_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR, 150);
  if (check_checksum (&emu_ecr_reg) == TRUE)
    {
      emu_var_cal ();
    }
  else
    {
      read_eeprom ((uint8_t *) & emu_ecr_reg, EE_RESAVE_EMU_VAR, 150);
      if (check_checksum (&emu_ecr_reg) == TRUE)
	{
	  emu_var_cal ();
	  write_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR, 150);
	}
      else			//第一次上电  存默认值
	{
	  Emu_Chip_to_StructData ();

	  get_checksum ((uint8_t *) & emu_ecr_reg, 150);
	  write_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR, 150);
	  write_eeprom ((uint8_t *) & emu_ecr_reg, EE_RESAVE_EMU_VAR, 150);
	}
    }
}

void
load_emu_var_225k (void)
{
  read_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_225K, 150);
  if (check_checksum (&emu_ecr_reg) == TRUE)
    {
      emu_var_cal ();
    }
  else				//第一次上电  存默认值
    {
      Emu_Chip_to_StructData ();
      get_checksum ((uint8_t *) & emu_ecr_reg, 150);
      write_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_225K, 150);
    }
}

void
load_emu_var_32k (void)
{
  read_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_32K, 150);
  if (check_checksum (&emu_ecr_reg) == TRUE)
    {
      emu_var_cal ();
    }
  else				//第一次上电  存默认值
    {
      Emu_Chip_to_StructData ();
      get_checksum ((uint8_t *) & emu_ecr_reg, 150);
      write_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_32K, 150);
    }
}

void
load_emu_var_const (void)
{
  read_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_CONST, 150);
  if (check_checksum (&emu_ecr_reg) == TRUE)
    {
      emu_var_cal ();
    }
  else				//第一次上电  存默认值
    {
      Emu_Chip_to_StructData ();
      get_checksum ((uint8_t *) & emu_ecr_reg, 150);
      write_eeprom ((uint8_t *) & emu_ecr_reg, EE_EMU_VAR_CONST, 150);
    }
}

void
emu_calibrate (void)
{
#ifdef _DEL
  if (emu_cal_mode == 0)
    return;

  flag_emu_mode = 4;
  if (emu_cal_mode == 1)
    {
      if (flag_emu_cal_mode != 1)
	{
	  load_emu_var_225k ();
	  flag_emu_cal_mode = 1;
	  PageNum = 1;
	}
      flag_fast_rmsi1 = 0;
    }
  else if (emu_cal_mode == 2)
    {
      if (flag_emu_cal_mode != 2)
	{
	  load_emu_var_32k ();
	  flag_emu_cal_mode = 2;
	  PageNum = 1;
	}
      flag_fast_rmsi1 = 0;
    }
  else if (emu_cal_mode == 3)
    {
      if (flag_emu_cal_mode != 3)
	{
	  load_emu_var_const ();
	  flag_emu_cal_mode = 3;
	  write_power_data ();
	  PageNum = 1;
	}

      if (Second_hold)
	{
	  Second_hold = 0;
	  write_power_data ();
	}

      flag_fast_rmsi1 = 1;
    }
  else if (emu_cal_mode == 4)
    {
      PageNum = 1;
      flag_fast_rmsi1 = 1;
    }
#endif
}

void
vol_read (void)
{
  extern unsigned short iVolDete;
  GetAVR (&RamData.Vph, HT_EMUEPA->FASTRMSU & 0xffffff);	//电压(1位小数)
  RamData.Vph.sVI = RamData.Vph.VI / (emu_ecr_reg.ECR.Emu_Krms);
  __disable_irq ();		//
  iVolDete = RamData.Vph.sVI;
  __enable_irq ();		//
}

void
emu_proc (void)
{
  long ltemp;
	unsigned long X32;
  static short iInit = FALSE;
  extern const unsigned short iVolCon;

// if ( TRUE!=iInit )
// {
//
// Emu_Chip_to_StructData();
// load_emu_var();
// emu_var_checksum = Read_EPADR(EPR_Checksum);
// iInit = TRUE;
// }else
  {

    if ((check_checksum (&emu_ecr_reg) != TRUE)
	&& (enChipMode == _cChipRun_Mode_))
      {
	emu_init ();
	return;
      }

    // lVar = ReadUrms();                                  //电压 3位小数位
    // lCurrent=ReadI1rms(); //电流 3位小数位
    // lPower =ReadPowerP(); //功率 3位小数位

    GetAVR (&RamData.Vph, HT_EMUEPA->FASTRMSU);	//电压(1位小数)
    RamData.Vph.sVI = RamData.Vph.VI / (emu_ecr_reg.ECR.Emu_Krms);
    GetAVR (&RamData.Iph, HT_EMUEPA->FASTRMSI1);	//电流(3位小数)
    RamData.Iph.sVI = RamData.Iph.VI / (emu_ecr_reg.ECR.Emu_Kims);
    ltemp = HT_EMUEPA->FASTPOWERP1;
    if (ltemp < 0)
      ltemp = -ltemp;
    GetAVR (&RamData.Pph, ltemp);	//功率(3位小数)
    RamData.Pph.sVI =
      (unsigned long) ((double) RamData.Pph.VI * (emu_ecr_reg.ECR.Emu_Kp) /
		       10000000);
    ltemp = HT_EMUEPA->FASTPOWERQ1;
    if (ltemp < 0)
      ltemp = -ltemp;
    GetAVR (&RamData.wPph, ltemp);	//电流(3位小数)
  }

  FreeDog ();
	
	if((RamData.Pph.VI >= RamData.StartPwr)){ // ||(RamData.ImpBfr == 0)){
		 
		}
		X32 = (RamData.Vph.sVI* RamData.Iph.sVI/1000);
	 
		if(RamData.Pph.sVI <= 2){
			RamData.Pfph = 1000;
		} else {
			RamData.Pfph = (RamData.Pph.sVI*100/X32);
			if(RamData.Pfph	> 1000){
				RamData.Pfph = 1000;
			}
		}
		if((RamData.Pph.sVI < RamData.StartPwr)){ //&&(RamData.ImpBfr == 0)){
			if(labs(RamData.Iph.sVI) < (FData.DftSet.A_Std[0])){
				RamData.Pph.sVI = 0;
				RamData.Iph.sVI = 0;
				RamData.Pfph = 1000;
				RamData.Use_Dir = 0;
				if(RamData.RevTmCnt < 0){
					RamData.RevTmCnt = 0;
				}
				AUse_Dir = 0;
				AUse_Dir_A = 0;
			}
		}
		if(RamData.Use_Dir){
			RamData.Pfph = -RamData.Pfph;
			RamData.Pph.sVI = -RamData.Pph.sVI;
		}
	//	if(labs(RamData.I_z.VI) < (FData.DftSet.A_Std[0]*3/10)){			//电流隐藏处理
	//		RamData.I_z.sVI = 0;
	//	}
	
	
	
  return;
  if (emu_cal_mode != 0)
    return;

  flag_emu_cal_mode = 0;
  if ((HT_PMU->PMUSTA & 0x01) != 0)	//normal
    {
      delay (100);
      if ((HT_PMU->PMUSTA & 0x01) != 0)
	{
	  if ((HT_PMU->PMUSTA & 0x08) != 0)
	    {
	      flag_ee_save = 0;
	    }
	  else
	    {
	      if (flag_ee_save == 0)
		{
		  write_eeprom ((uint8_t *) & energyP, EE_SAVE_ENERGY_START,
				4);
		  flag_ee_save = 1;
		}
	    }

	  if ((flag_emu_mode != 1)
	      || (Read_EPADR (EPR_Checksum) != emu_var_checksum))
	    {
#ifdef _DEL
	      run_hrc ();
	      open_pll ();
	      close_hrc ();

	      HT_PMU->PMUCON = 0x000B;
	      HT_RTC->RTCCON &= 0xFFDF;	//RTC定时器1关闭
	      HT_CMU->CLKCTRL0 |= 0x8002;	//enable EMU,LCD
	      HT_RTC->RTCCON &= 0xFFF7;	//TOUT output 1Hz
	      HT_RTC->RTCCON |= 0x0006;
	      HT_TBS->TBSCON = 0x01;

	      Secondx = 0;
	      Second_hold = 0;
	      flag_rtctmr1 = 0;
#endif
	      load_emu_var ();
	      flag_emu_mode = 1;
	      emu_var_checksum = Read_EPADR (EPR_Checksum);
	    }
	}
    }
  else if ((HT_PMU->PMUSTA & 0x01) == 0)	//vbat
    {
      delay (10);
      if ((HT_PMU->PMUSTA & 0x01) == 0)
	{
	  /*
	     if(flag_rtctmr1 == 0)
	     {
	     run_osc();
	     close_pll();

	     Write_ECADR(VAR_EMUCTRL, 0x0382);
	     Write_ECADR(VAR_ADCCFG, 0x0003);
	     Write_ECADR(VAR_ANA_control, 0x27A6);

	     if((RstsrTmp&0x0004) != 0) //WDT reset
	     {
	     HT_CMU->WPREG = 0xA55A;
	     HT_CMU->CLKCTRL0 = 0x0400; //close LCD...
	     HT_RTC->RTCCON &= 0xFFDF; //RTC定时器1关闭
	     Second_hold=1;
	     }
	     else
	     {
	     HT_CMU->WPREG = 0xA55A;
	     HT_CMU->CLKCTRL0 = 0x0402; //close EMU...
	     HT_RTC->RTCCON |= 0x20; //RTC定时器1使能
	     Second_hold=0;
	     }
	     HT_PMU->PMUCON = 0x0002;
	     HT_RTC->RTCCON &= 0xFFF1; //TOUT output 0
	     HT_TBS->TBSCON = 0x00;

	     flag_rtctmr1=1;
	     }
	   */
#ifdef _DEL
	  if (Second_hold)
	    {
	      Second_hold = 0;
	      Delay_FreeDog ();
	      HT_CMU->CLKCTRL0 = 0x0400;	//close LCD...
	      HT_RTC->RTCIE &= 0xFFFE;	//RTC秒中断关闭
	      HT_RTC->RTCCON &= 0xFFDF;	//RTC定时器1关闭

	      EnterSleep ();	//enter SLEEP

	    }
#endif
	}

    }

// if(((HT_PMU->PMUSTA&0x04) != 0)&&((HT_PMU->PMUSTA&0x01) != 0)) //normal
// {
// delay(100);
// if(((HT_PMU->PMUSTA&0x04) != 0)&&((HT_PMU->PMUSTA&0x01) != 0))
// {
// if((flag_emu_mode!=0)||(Read_EPADR(EPR_Checksum)!=emu_var_checksum))
// {
// run_hrc();
// open_pll();
// close_hrc();
//
// load_emu_var();
// flag_emu_mode=0;
// emu_var_checksum = Read_EPADR(EPR_Checksum);
// }
//
// flag_fast_rmsi1=0;
// }
// }
// else if(((HT_PMU->PMUSTA&0x04) == 0)&&((HT_PMU->PMUSTA&0x01) != 0)) //lf
// {
// delay(100);
// if(((HT_PMU->PMUSTA&0x04) == 0)&&((HT_PMU->PMUSTA&0x01) != 0))
// {
// #if (EMU_LF_MODE == LF_225K_MODE)
// if((flag_emu_mode!=1)||(Read_EPADR(EPR_Checksum)!=emu_var_checksum))
// {
// run_hrc();
// open_pll();
// close_hrc();
//
// load_emu_var_225k();
// flag_emu_mode=1;
// emu_var_checksum = Read_EPADR(EPR_Checksum);
//
// run_osc();
//
// PageNum=1;
// }
//
// flag_fast_rmsi1=0;
//
// #else
// if((flag_emu_mode!=2)||(Read_EPADR(EPR_Checksum)!=emu_var_checksum))
// {
// run_hrc();
// open_pll();
// close_hrc();
//
// load_emu_var_32k();
// flag_emu_mode=2;
// emu_var_checksum = Read_EPADR(EPR_Checksum);
//
// run_osc();
// close_pll();
//
// PageNum=1;
// }
//
// flag_fast_rmsi1=0;
//
// #endif
// }
// }
// else if((HT_PMU->PMUSTA&0x01) == 0)                 //const
// {
// delay(10);
// if((HT_PMU->PMUSTA&0x01) == 0)
// {
// if((flag_emu_mode!=3)||(Read_EPADR(EPR_Checksum)!=emu_var_checksum))
// {
// run_hrc();
// open_pll();
// close_hrc();
//
// load_emu_var_const();
// flag_emu_mode=3;
//
// PageNum=1;
//
// write_power_data();
// emu_var_checksum = Read_EPADR(EPR_Checksum);
// 
// hold_flag=1;
// flag_fast_rmsi1=1;
//
// TaskDisplay();
// enter_hold_init();
// EnterHold();     //enter HOLD
//
// hold_wake_init();
// }
//
// flag_fast_rmsi1=1;
//
// if(Second_hold)
// {
// Second_hold=0;
// write_power_data();
// emu_var_checksum = Read_EPADR(EPR_Checksum);
//
// hold_flag=1;
//
// TaskDisplay();
// enter_hold_init();
// EnterHold();     //enter HOLD
//
// hold_wake_init();
// }
// }
// }
}

uint32_t
Read_EPADR (uint16_t address)
{
  uint32_t *ptr;
  FreeDog ();
  ptr = (uint32_t *) (HT_EMU_EPA_BASE + address);
  return ((*ptr) & 0xffffffff);
}

uint16_t
Read_ECADR (uint16_t address)
{
  uint16_t *ptr;
  FreeDog ();
  ptr = (uint16_t *) (HT_EMU_ECA_BASE + address);
  return ((*ptr) & 0xffff);
}

void
Write_ECADR (uint16_t address, uint16_t data)
{
  uint16_t *ptr;
  FreeDog ();
  ptr = (uint16_t *) (HT_EMU_ECA_BASE + address);
  *ptr = data;
}

uint32_t
ReadUrms (void)			//电压 3位小数位
{
  uint32_t temp_urms;
  float urms;
  temp_urms = Read_EPADR (EPR_FastRMSU);
  urms = temp_urms / (emu_ecr_reg.ECR.Emu_Krms);
  return (urms);
}

uint32_t
ReadI1rms (void)		//电流 3位小数位
{
  uint32_t temp_i1rms;
  float i1rms;
  temp_i1rms = Read_EPADR (EPR_FastRMSI1);
  i1rms = temp_i1rms / (emu_ecr_reg.ECR.Emu_Kims);
  return (i1rms);
}

uint32_t
ReadPowerP (void)		//功率 3位小数位
{
  uint32_t temp_powerp;
  float powerp;

  temp_powerp = Read_EPADR (EPR_FastPowerP1);
  powerp = temp_powerp * (emu_ecr_reg.ECR.Emu_Kp);
  return (powerp);
}

void
get_checksum (uint8_t * start, uint8_t len)
{
  uint8_t i, j;
  uint16_t checksum, *p16;

  j = len - 2;
  checksum = 0;
  for (i = 0; i < j; i++)
    {
      checksum += start[i];
    }
  p16 = (uint16_t *) (start + j);
  *p16 = checksum;
}

uint8_t
check_checksum (EMUC * stp)
{
  uint8_t i, j, len;
  uint16_t checksum, *p16;

  len = _cSumCheck_Len_;
  j = len - 2;
  for (checksum = 0, i = 0, p16 = (uint16_t *) (stp); i < len; i++, p16++)
    checksum += *p16;

  if (stp->ECR.CheckSum == checksum)
    return TRUE;
  else
    return FALSE;
}

uint32_t
fast_get_rmsi1 (void)
{
  uint16_t temp[8];

  temp[0] = HT_CMU->CLKCTRL0;
  temp[1] = HT_CMU->SYSCLKCFG;
  temp[2] = HT_CMU->SYSCLKDIV;

  open_pll_tmp ();

  temp[3] = Read_ECADR (VAR_ADCCON);
  temp[4] = Read_ECADR (VAR_EMUCTRL);
  temp[5] = Read_ECADR (VAR_FilterCtrl);
  temp[6] = Read_ECADR (VAR_CHNLCR);
  temp[7] = Read_ECADR (VAR_ANA_control);

  Write_ECADR (VAR_ADCCON, 0x000C);
  Write_ECADR (VAR_ANA_control, 0xE7A7);	//Lvref:5uA
  Write_ECADR (VAR_FilterCtrl, 0x0023);	//HPF:8, LPF:7
  Write_ECADR (VAR_EMUCTRL, 0x03C0);	//EMUClk 892K, 有效值更新速度20Hz
  Write_ECADR (VAR_CHNLCR, 0x8607);
// delay(0x2fff);       //Fcpu:300KHz, 200ms
  delay (0x5fff);		//Fcpu:600KHz, 200ms
// delay(0x5fff);       //Fcpu:1.2MHz, 200ms

  fast_rmsi1 = Read_EPADR (EPR_FastRMSI1);

// HT_GPIOF->PTDAT = ~HT_GPIOF->PTDAT; //test time

  Write_ECADR (VAR_ADCCON, temp[3]);
  Write_ECADR (VAR_EMUCTRL, temp[4]);
  Write_ECADR (VAR_FilterCtrl, temp[5]);
  Write_ECADR (VAR_CHNLCR, temp[6]);
  Write_ECADR (VAR_ANA_control, temp[7]);
  HT_CMU->WPREG = 0xA55A;
  HT_CMU->SYSCLKCFG = temp[1] | 0x80;
  HT_CMU->SYSCLKDIV = temp[2];
  HT_CMU->CLKCTRL0 = temp[0];
  return (fast_rmsi1);
}

void
write_power_data (void)
{
  float temp_power;

  temp_power = kims_fast_rmsi1 * (fast_get_rmsi1 ());
  temp_power = (temp_power / 5000) * PowerP1_5A * (1 - 0.2186);
  Write_ECADR (VAR_PDataCpH, (uint32_t) temp_power >> 16);
  Write_ECADR (VAR_PDataCpL, (uint32_t) temp_power & 0xFFFF);
  HT_EMUECA->LOADDATACP = 0x00BC;
}

void
Save_Energy (void)
{
  uint32_t EP_new, EP_delta, Wh_delta;

  EP_new = HT_EMUEPA->ENERGYP;
  if (EP_new < EP_old)
    {
      EP_delta = EP_new + (0x00FFFFFF - EP_old);
    }
  else
    {
      EP_delta = EP_new - EP_old;
    }
  EP_old = EP_new;

  ep_disp += EP_delta;
  Wh_delta = ep_disp * 1000 / EC;
  if (Wh_delta >= 1)
    {
      ep_disp = 0;
      energyP += Wh_delta;
    }
  ep_save += EP_delta;
  Wh_delta = ep_save * 1000 / EC;
  if (Wh_delta >= 100)
    {
      ep_save = 0;
      write_eeprom ((uint8_t *) & energyP, EE_SAVE_ENERGY_START, 4);
    }
}

unsigned char
Inc_Use (strSectUse * pSectUse, unsigned short Imp)
{				//用电量处理(脉冲数转为电量/Imp最高为为1时表示反向电量)

  unsigned long k;
  unsigned long t;
  unsigned long Dir;
  unsigned long *pUse;
  unsigned short *pImp;
  if (Imp & 0x8000)
    {
      pUse = &pSectUse->RevUse;
      pImp = &pSectUse->RevImp;
      Dir = 1;
    }
  else
    {
      pUse = &pSectUse->Use;
      pImp = &pSectUse->Imp;
      Dir = 0;
    }
  Imp &= 0xFF;
  Imp += *pImp;
  k = (FData.DftSet.kWhImp / 100);
  if (Imp >= k)
    {
      t = Imp / k;
      *pUse += t;
      *pImp = Imp % k;
      if (Dir)
	{
	  pSectUse->Use += t;
	}
      return (unsigned char) t;
    }
  *pImp = Imp;
  return 0;
}

void
GetAVR (strVI * pVI, u32 Data)
{
  u8 Cnt;
  u32 Sum;
  pVI->sVI = Data;
  if (++(pVI->VI_Cnt) >= _cElectric_Buflen_)
    {
      pVI->VI_Cnt = 0;
    }
  pVI->VI_Lst[pVI->VI_Cnt] = Data;
  Sum = 0;
  for (Cnt = 0; Cnt < _cElectric_Buflen_; Cnt++)
    {
      Sum += pVI->VI_Lst[Cnt];
    }
  Sum /= _cElectric_Buflen_;
  pVI->VI = Sum;
}

void
Chip_Trim_IB (void)
{
  extern unsigned long lPowerKtemp;
  extern const unsigned short iVolCon;
  unsigned short itemp;
  unsigned long ltemp;
  unsigned long ltemp1 =
    (unsigned long) FData.DftSet.kWhImp * emu_ecr_reg.ECR.HFconst;
  unsigned long lStandpwr =
    ((unsigned long) FData.DftSet.V_Std * FData.DftSet.A_Std[0]);
  long ltemp2;
  double ftemp;

  //电压系数
  ltemp = RamData.Vph.VI * iVolCon / FData.DftSet.V_Std;
  emu_ecr_reg.ECR.Emu_Krms = ltemp / iVolCon;
  RamData.Vph.sVI = RamData.Vph.VI / emu_ecr_reg.ECR.Emu_Krms;

  //电流通道系数
  ltemp = RamData.Iph.VI * iVolCon * iVolCon/ (FData.DftSet.A_Std[0] * 100);
  emu_ecr_reg.ECR.Emu_Kims = ltemp / (iVolCon * iVolCon);
  RamData.Iph.sVI = RamData.Iph.VI / emu_ecr_reg.ECR.Emu_Kims;

  //有功功率系数
  //ftemp = (double)RamData.Pph.VI;
  ftemp = (double) lPowerKtemp *1000;
  ftemp /= ltemp1;
  //ltemp = RamData.Pph.VI * iVolCon * 8 / ((unsigned long)FData.DftSet.V_Std * FData.DftSet.A_Std[0]);
  emu_ecr_reg.ECR.Emu_Kp = (unsigned long) ftemp;	// ltemp /(iVolCon *8);
  if (RamData.Pph.VI & 0x800000)
    RamData.Pph.VI = 0xffffff - RamData.Pph.VI;
  RamData.Pph.sVI =
    (unsigned long) ((double) RamData.Pph.VI * (emu_ecr_reg.ECR.Emu_Kp) /
		     10000000);
 
		return  ;
  ltemp2 = lStandpwr - RamData.Pph.sVI;
  if (ltemp2 < 0)
    ltemp = -ltemp;
  if (ltemp < lStandpwr / 2)
    {
      ltemp2 *= 32768;
      ltemp2 /= RamData.Pph.sVI;
      emu_ecr_reg.ECR.GP1 = ltemp2;
      Emu_StructData_To_Chip ();
    }
  // 小电流点校表标志
}

void
Chip_Trim_IB_1 (double ftemp1)
{
  extern unsigned long lPowerKtemp;
  extern const unsigned short iVolCon;
  unsigned short itemp;
  unsigned long ltemp;
  unsigned long ltemp1 =
    (unsigned long) FData.DftSet.kWhImp * emu_ecr_reg.ECR.HFconst;
  unsigned long lStandpwr =
    ((unsigned long) FData.DftSet.V_Std * FData.DftSet.A_Std[0]);
  long ltemp2;
  double ftemp;

  //电压系数
  ltemp = RamData.Vph.VI * iVolCon / FData.DftSet.V_Std;
  emu_ecr_reg.ECR.Emu_Krms = ltemp / iVolCon;
  RamData.Vph.sVI = RamData.Vph.VI / emu_ecr_reg.ECR.Emu_Krms;

  //电流通道系数
  ltemp = RamData.Iph.VI * iVolCon / (FData.DftSet.A_Std[0] * 100);
  emu_ecr_reg.ECR.Emu_Kims = ltemp / iVolCon;
  RamData.Iph.sVI = RamData.Iph.VI / emu_ecr_reg.ECR.Emu_Kims;

  //有功功率系数
  //ftemp = (double)RamData.Pph.VI;
  ftemp = (double) lPowerKtemp *1000;
  ftemp /= ltemp1;
  //ltemp = RamData.Pph.VI * iVolCon * 8 / ((unsigned long)FData.DftSet.V_Std * FData.DftSet.A_Std[0]);
  emu_ecr_reg.ECR.Emu_Kp = (unsigned long) ftemp;	// ltemp /(iVolCon *8);
  if (RamData.Pph.VI & 0x80000000)
    RamData.Pph.VI = 0xffffffff - RamData.Pph.VI;
  RamData.Pph.sVI =
    (unsigned long) ((double) RamData.Pph.VI * (emu_ecr_reg.ECR.Emu_Kp) /
		     10000000);
  RamData.wPph.sVI =
    (unsigned long) ((double) RamData.wPph.VI * (emu_ecr_reg.ECR.Emu_Kp) /
		     10000000);
  RamData.Pph.sVI += ftemp1 * RamData.wPph.sVI;
  ltemp2 = lStandpwr / 2 - RamData.Pph.sVI;
  if (ltemp2 < 0)
    ltemp = -ltemp;
  if (ltemp < lStandpwr / 4)
    {
      ltemp2 *= 32768;
      ltemp2 /= RamData.Pph.sVI;
      emu_ecr_reg.ECR.GP1 = ltemp2;
      Emu_StructData_To_Chip ();
    }
  // 小电流点校表标志
}

void
Chip_Trim_5l (void)
{
  unsigned short itemp;		// 0.5L点校表标志
  unsigned long ltemp;
// unsigned long           ltemp1 =(unsigned long ) FData.DftSet.kWhImp * emu_ecr_reg.ECR.HFconst;
  unsigned long lStandpwr =
    ((unsigned long) FData.DftSet.V_Std * FData.DftSet.A_Std[0]) / 2;
  unsigned long lwStandpwr =
    ((unsigned long) FData.DftSet.V_Std * FData.DftSet.A_Std[0]) * 866 / 1000;
  long ltemp2;
  double ftemp1, ftemp3;
  double ftemp2, ftemp4;
  if (RamData.wPph.VI & 0x80000000)
    {
      // RamData.wPph.VI =0xff000000 | RamData.wPph.VI;
      RamData.wPph.VI = 0xffffffff - RamData.wPph.VI;
    }
  if (RamData.Pph.VI & 0x80000000)
    {
      // RamData.wPph.VI =0xff000000 | RamData.wPph.VI;
      RamData.Pph.VI = 0xffffffff - RamData.Pph.VI;
    }
  ftemp1 = (double) lStandpwr *(signed) RamData.wPph.VI / 10000;
  ftemp2 = (double) lwStandpwr *RamData.Pph.VI / 10000;

  ftemp3 = (double) lStandpwr *RamData.Pph.VI / 10000;
  ftemp4 = (double) lwStandpwr *(signed) RamData.wPph.VI / 10000;

  ftemp1 -= ftemp2;
  ftemp3 += ftemp4;

  ftemp1 /= ftemp3;
		Chip_Trim_IB_1 (ftemp1);
  ftemp1 *= 32768;
  ltemp2 = (long) ftemp1;
  emu_ecr_reg.ECR.GPhs1 = ltemp2;
  Emu_StructData_To_Chip ();
}

void
Chip_Trim_5 (void)		// 小电流点校表标志
{
//RamData.Pph.sVI =  (unsigned long )((double)RamData.Pph.VI  *    (emu_ecr_reg.ECR.Emu_Kp  ) /1000000);
  extern unsigned long lPowerKtemp;
  unsigned short itemp;
  unsigned long ltemp;
  unsigned long ltemp1 =
    (unsigned long) FData.DftSet.kWhImp * emu_ecr_reg.ECR.HFconst;
  unsigned long lStandpwr =
    ((unsigned long) FData.DftSet.V_Std * FData.DftSet.A_Std[0] * 5 / 100);
  long ltemp2;
  double ftemp;

  ftemp = (double) lStandpwr *10000000 / (emu_ecr_reg.ECR.Emu_Kp);
  ltemp2 = (long) ftemp;
  ltemp2 -= RamData.Pph.VI;
  if (ltemp2 >= 0)
    {
      if (ltemp2 > 255)
	return;
      emu_ecr_reg.ECR.P1OFFSET = ltemp2;
    }
  if (ltemp2 < 0)
    {
      ltemp2 += 256;
      if (ltemp > 255)
	return;
      emu_ecr_reg.ECR.P1OFFSET = ltemp2;
    }
  Emu_StructData_To_Chip ();
}

char
Emu_Var_Write_io (struct STCHIPSET * stp)
{
  struct STFLASHFILESYSTEM stDianLian1 = {
    0,
    _iBlcokLen_,
    0,
    0,
    0,
  };
 
  if (TRUE == BlockEarse (&stDianLian1, (unsigned long) &stChipSet))
    {
      HT_Flash_ByteWrite ((char *)stp, (unsigned long) &stChipSet,
			  sizeof (struct STCHIPSET));
      return SUCCESS;
    }
  return ERROR;
}


char
Emu_Var_Write (void)
{
  unsigned char ctemp;
  unsigned short i;
  struct STCHIPSET stchiptemp;
  

  memcpy ((char *) &stchiptemp, (char *) &stChipSet, sizeof (stchiptemp));
  for (i = 0;
       i < sizeof (stchiptemp.stChipreg) / sizeof (stchiptemp.stChipreg[0]);
       i++)
    stchiptemp.stChipreg[i].iConfig =
      Read_ECADR (stchiptemp.stChipreg[i].iNo);

  // 电压通道系数
  stchiptemp.stKset[0].iConfig = emu_ecr_reg.ECR.Emu_Krms;
  //电流通道系数
  stchiptemp.stKset[1].iConfig = emu_ecr_reg.ECR.Emu_Kims;
  //有功功率系数
  stchiptemp.stKset[2].iConfig = emu_ecr_reg.ECR.Emu_Kp;
	return Emu_Var_Write_io (&stchiptemp);
  
}

void
Emu_StructData_To_Chip (void)
{
  Write_ECADR (VAR_EMUSR, emu_ecr_reg.ECR.EMUSR);
  Write_ECADR (VAR_EMUIE, emu_ecr_reg.ECR.EMUIE);
  Write_ECADR (VAR_EMUIF, emu_ecr_reg.ECR.EMUIF);
  Write_ECADR (VAR_GP1, emu_ecr_reg.ECR.GP1);
  Write_ECADR (VAR_GQ1, emu_ecr_reg.ECR.GQ1);
  Write_ECADR (VAR_GS1, emu_ecr_reg.ECR.GS1);
  Write_ECADR (VAR_GPhs1, emu_ecr_reg.ECR.GPhs1);
  Write_ECADR (VAR_GP2, emu_ecr_reg.ECR.GP2);
  Write_ECADR (VAR_GQ2, emu_ecr_reg.ECR.GQ2);
  Write_ECADR (VAR_GS2, emu_ecr_reg.ECR.GS2);
  Write_ECADR (VAR_GPhs2, emu_ecr_reg.ECR.GPhs2);
  Write_ECADR (VAR_QPhsCal, emu_ecr_reg.ECR.QPhsCal);
  Write_ECADR (VAR_I2Gain, emu_ecr_reg.ECR.I2Gain);
  Write_ECADR (VAR_I1Off, emu_ecr_reg.ECR.I1Off);
  Write_ECADR (VAR_I2Off, emu_ecr_reg.ECR.I2Off);
  Write_ECADR (VAR_UOff, emu_ecr_reg.ECR.UOff);
  Write_ECADR (VAR_PStart, emu_ecr_reg.ECR.PStart);
  Write_ECADR (VAR_QStart, emu_ecr_reg.ECR.QStart);
  Write_ECADR (VAR_SStart, emu_ecr_reg.ECR.SStart);
  Write_ECADR (VAR_HFconst, emu_ecr_reg.ECR.HFconst);
  Write_ECADR (VAR_ADCCFG, emu_ecr_reg.ECR.ADCCFG);
  Write_ECADR (VAR_CHNLCR, emu_ecr_reg.ECR.CHNLCR);
  Write_ECADR (VAR_EMCON, emu_ecr_reg.ECR.EMCON);
  Write_ECADR (VAR_PFCnt, emu_ecr_reg.ECR.PFCnt);
  Write_ECADR (VAR_QFCnt, emu_ecr_reg.ECR.QFCnt);
  Write_ECADR (VAR_SFCnt, emu_ecr_reg.ECR.SFCnt);
  Write_ECADR (VAR_ADCCON, emu_ecr_reg.ECR.ADCCON);
  Write_ECADR (VAR_IPTAMP, emu_ecr_reg.ECR.IPTAMP);
  Write_ECADR (VAR_ICHK, emu_ecr_reg.ECR.ICHK);
  Write_ECADR (VAR_EMUCTRL, emu_ecr_reg.ECR.EMUCTRL);
  Write_ECADR (VAR_P1OFFSET, emu_ecr_reg.ECR.P1OFFSET);
  Write_ECADR (VAR_P2OFFSET, emu_ecr_reg.ECR.P2OFFSET);
  Write_ECADR (VAR_Q1OFFSET, emu_ecr_reg.ECR.Q1OFFSET);
  Write_ECADR (VAR_Q2OFFSET, emu_ecr_reg.ECR.Q2OFFSET);
  Write_ECADR (VAR_I1RMSOFFSET, emu_ecr_reg.ECR.I1RMSOFFSET);
  Write_ECADR (VAR_I2RMSOFFSET, emu_ecr_reg.ECR.I2RMSOFFSET);
  Write_ECADR (VAR_URMSOFFSET, emu_ecr_reg.ECR.URMSOFFSET);
  Write_ECADR (VAR_RosiCtrl, emu_ecr_reg.ECR.RosiCtrl);
  Write_ECADR (VAR_ANA_control, emu_ecr_reg.ECR.ANA_control);
  Write_ECADR (VAR_UCONST, emu_ecr_reg.ECR.UCONST);
  Write_ECADR (VAR_LpIdleTime, emu_ecr_reg.ECR.LpIdleTime);
  Write_ECADR (VAR_USAGLVL, emu_ecr_reg.ECR.USAGLVL);
  Write_ECADR (VAR_USagCyc, emu_ecr_reg.ECR.USagCyc);
  Write_ECADR (VAR_UOVLVL, emu_ecr_reg.ECR.UOVLVL);
  Write_ECADR (VAR_OvCyc, emu_ecr_reg.ECR.OvCyc);
  Write_ECADR (VAR_IOVLVL, emu_ecr_reg.ECR.IOVLVL);
  Write_ECADR (VAR_ZXILVL, emu_ecr_reg.ECR.ZXILVL);
  Write_ECADR (VAR_PDataCpH, emu_ecr_reg.ECR.PDataCpH);
  Write_ECADR (VAR_PDataCpL, emu_ecr_reg.ECR.PDataCpL);
  Write_ECADR (VAR_QDataCpH, emu_ecr_reg.ECR.QDataCpH);
  Write_ECADR (VAR_QDataCpL, emu_ecr_reg.ECR.QDataCpL);
  Write_ECADR (VAR_SDataCpH, emu_ecr_reg.ECR.SDataCpH);
  Write_ECADR (VAR_SDataCpL, emu_ecr_reg.ECR.SDataCpL);
  Write_ECADR (VAR_FilterCtrl, emu_ecr_reg.ECR.FilterCtrl);
  Write_ECADR (VAR_TUgain, emu_ecr_reg.ECR.TUgain);
  Write_ECADR (VAR_TI1gain, emu_ecr_reg.ECR.TI1gain);
  Write_ECADR (VAR_TI2gain, emu_ecr_reg.ECR.TI2gain);
  Write_ECADR (VAR_UTCcoffA, emu_ecr_reg.ECR.UTCcoffA);
  Write_ECADR (VAR_UTCcoffB, emu_ecr_reg.ECR.UTCcoffB);
  Write_ECADR (VAR_UTCcoffC, emu_ecr_reg.ECR.UTCcoffC);
  Write_ECADR (VAR_I1TCcoffA, emu_ecr_reg.ECR.I1TCcoffA);
  Write_ECADR (VAR_I1TCcoffB, emu_ecr_reg.ECR.I1TCcoffB);
  Write_ECADR (VAR_I1TCcoffC, emu_ecr_reg.ECR.I1TCcoffC);
  Write_ECADR (VAR_I2TCcoffA, emu_ecr_reg.ECR.I2TCcoffA);
  Write_ECADR (VAR_I2TCcoffB, emu_ecr_reg.ECR.I2TCcoffB);
  Write_ECADR (VAR_I2TCcoffC, emu_ecr_reg.ECR.I2TCcoffC);
  Write_ECADR (VAR_LoadDataCp, emu_ecr_reg.ECR.LoadDataCp);

	emu_ecr_reg.ECR.CheckSum = Read_EPADR(EPR_Checksum);	
}

void
Emu_StoretData_To_Chip (struct STCHIPSET *stp, unsigned short ilen)
{
  unsigned short i;
  for (i = 0; i < ilen; i++)
    Write_ECADR (stp->stChipreg[i].iNo, stp->stChipreg[i].iConfig);
}

void
emu_var_cal (void)
{
  Emu_Chip_to_StructData ();
  Emu_StoretData_To_Chip (&stChipSet, _cRegMax_);
  Emu_Chip_to_StructData ();
}

void
Emu_StoretData_Init (struct STCHIPSET *stp, unsigned short ilen)
{
  _emu_module_reset ();
  HAL_Delay_us (_iEmuSoftResetDelay);
  _emu_data_reset ();
  HAL_Delay_us (_iEmuDataResetDelay);
  Emu_StoretData_To_Chip (&stChipSet, _cRegMax_);
}

void
Emu_Chip_to_StructData (void)
{
  unsigned short i;
  emu_ecr_reg.ECR.EMUSR = Read_ECADR (VAR_EMUSR);
  emu_ecr_reg.ECR.EMUIE = Read_ECADR (VAR_EMUIE);
  emu_ecr_reg.ECR.EMUIF = Read_ECADR (VAR_EMUIF);
  emu_ecr_reg.ECR.GP1 = Read_ECADR (VAR_GP1);
  emu_ecr_reg.ECR.GQ1 = Read_ECADR (VAR_GQ1);
  emu_ecr_reg.ECR.GS1 = Read_ECADR (VAR_GS1);
  emu_ecr_reg.ECR.GPhs1 = Read_ECADR (VAR_GPhs1);
  emu_ecr_reg.ECR.GP2 = Read_ECADR (VAR_GP2);
  emu_ecr_reg.ECR.GQ2 = Read_ECADR (VAR_GQ2);
  emu_ecr_reg.ECR.GS2 = Read_ECADR (VAR_GS2);
  emu_ecr_reg.ECR.GPhs2 = Read_ECADR (VAR_GPhs2);
  emu_ecr_reg.ECR.QPhsCal = Read_ECADR (VAR_QPhsCal);
  emu_ecr_reg.ECR.I2Gain = Read_ECADR (VAR_I2Gain);
  emu_ecr_reg.ECR.I1Off = Read_ECADR (VAR_I1Off);
  emu_ecr_reg.ECR.I2Off = Read_ECADR (VAR_I2Off);
  emu_ecr_reg.ECR.UOff = Read_ECADR (VAR_UOff);
  emu_ecr_reg.ECR.PStart = Read_ECADR (VAR_PStart);
  emu_ecr_reg.ECR.QStart = Read_ECADR (VAR_QStart);
  emu_ecr_reg.ECR.SStart = Read_ECADR (VAR_SStart);
  emu_ecr_reg.ECR.HFconst = Read_ECADR (VAR_HFconst);
  emu_ecr_reg.ECR.ADCCFG = Read_ECADR (VAR_ADCCFG);
  emu_ecr_reg.ECR.CHNLCR = Read_ECADR (VAR_CHNLCR);
  emu_ecr_reg.ECR.EMCON = Read_ECADR (VAR_EMCON);
  emu_ecr_reg.ECR.PFCnt = Read_ECADR (VAR_PFCnt);
  emu_ecr_reg.ECR.QFCnt = Read_ECADR (VAR_QFCnt);
  emu_ecr_reg.ECR.SFCnt = Read_ECADR (VAR_SFCnt);
  emu_ecr_reg.ECR.ADCCON = Read_ECADR (VAR_ADCCON);
  emu_ecr_reg.ECR.IPTAMP = Read_ECADR (VAR_IPTAMP);
  emu_ecr_reg.ECR.ICHK = Read_ECADR (VAR_ICHK);
  emu_ecr_reg.ECR.EMUCTRL = Read_ECADR (VAR_EMUCTRL);
  emu_ecr_reg.ECR.P1OFFSET = Read_ECADR (VAR_P1OFFSET);
  emu_ecr_reg.ECR.P2OFFSET = Read_ECADR (VAR_P2OFFSET);
  emu_ecr_reg.ECR.Q1OFFSET = Read_ECADR (VAR_Q1OFFSET);
  emu_ecr_reg.ECR.Q2OFFSET = Read_ECADR (VAR_Q2OFFSET);
  emu_ecr_reg.ECR.I1RMSOFFSET = Read_ECADR (VAR_I1RMSOFFSET);
  emu_ecr_reg.ECR.I2RMSOFFSET = Read_ECADR (VAR_I2RMSOFFSET);
  emu_ecr_reg.ECR.URMSOFFSET = Read_ECADR (VAR_URMSOFFSET);
  emu_ecr_reg.ECR.RosiCtrl = Read_ECADR (VAR_RosiCtrl);
  emu_ecr_reg.ECR.ANA_control = Read_ECADR (VAR_ANA_control);
  emu_ecr_reg.ECR.UCONST = Read_ECADR (VAR_UCONST);
  emu_ecr_reg.ECR.LpIdleTime = Read_ECADR (VAR_LpIdleTime);
  emu_ecr_reg.ECR.USAGLVL = Read_ECADR (VAR_USAGLVL);
  emu_ecr_reg.ECR.USagCyc = Read_ECADR (VAR_USagCyc);
  emu_ecr_reg.ECR.UOVLVL = Read_ECADR (VAR_UOVLVL);
  emu_ecr_reg.ECR.OvCyc = Read_ECADR (VAR_OvCyc);
  emu_ecr_reg.ECR.IOVLVL = Read_ECADR (VAR_IOVLVL);
  emu_ecr_reg.ECR.ZXILVL = Read_ECADR (VAR_ZXILVL);
  emu_ecr_reg.ECR.PDataCpH = Read_ECADR (VAR_PDataCpH);
  emu_ecr_reg.ECR.PDataCpL = Read_ECADR (VAR_PDataCpL);
  emu_ecr_reg.ECR.QDataCpH = Read_ECADR (VAR_QDataCpH);
  emu_ecr_reg.ECR.QDataCpL = Read_ECADR (VAR_QDataCpL);
  emu_ecr_reg.ECR.SDataCpH = Read_ECADR (VAR_SDataCpH);
  emu_ecr_reg.ECR.SDataCpL = Read_ECADR (VAR_SDataCpL);
  emu_ecr_reg.ECR.FilterCtrl = Read_ECADR (VAR_FilterCtrl);
  emu_ecr_reg.ECR.TUgain = Read_ECADR (VAR_TUgain);
  emu_ecr_reg.ECR.TI1gain = Read_ECADR (VAR_TI1gain);
  emu_ecr_reg.ECR.TI2gain = Read_ECADR (VAR_TI2gain);
  emu_ecr_reg.ECR.UTCcoffA = Read_ECADR (VAR_UTCcoffA);
  emu_ecr_reg.ECR.UTCcoffB = Read_ECADR (VAR_UTCcoffB);
  emu_ecr_reg.ECR.UTCcoffC = Read_ECADR (VAR_UTCcoffC);
  emu_ecr_reg.ECR.I1TCcoffA = Read_ECADR (VAR_I1TCcoffA);
  emu_ecr_reg.ECR.I1TCcoffB = Read_ECADR (VAR_I1TCcoffB);
  emu_ecr_reg.ECR.I1TCcoffC = Read_ECADR (VAR_I1TCcoffC);
  emu_ecr_reg.ECR.I2TCcoffA = Read_ECADR (VAR_I2TCcoffA);
  emu_ecr_reg.ECR.I2TCcoffB = Read_ECADR (VAR_I2TCcoffB);
  emu_ecr_reg.ECR.I2TCcoffC = Read_ECADR (VAR_I2TCcoffC);
  emu_ecr_reg.ECR.LoadDataCp = Read_ECADR (VAR_LoadDataCp);
// emu_ecr_reg.ECR.SRSTREG            =Read_ECADR(VAR_SRSTREG);

  for (i = 0; i < _cKpMax_; i++)
    {
      switch (stChipSet.stKset[i].iNo)
	{
	case VAR_Krms:		//电压系数
	  emu_ecr_reg.ECR.Emu_Krms = stChipSet.stKset[i].iConfig;
	  break;
	case VAR_Kims:		//电流通道系数
	  emu_ecr_reg.ECR.Emu_Kims = stChipSet.stKset[i].iConfig;
	  break;
	case VAR_Kp:		//有功功率系数
	  emu_ecr_reg.ECR.Emu_Kp = stChipSet.stKset[i].iConfig;
	  break;
	}
    }
	
}
