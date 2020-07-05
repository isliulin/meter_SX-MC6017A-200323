
#include "ATT7053CAL1.h"

#ifdef USE_8209
	#include "RN8209CAL1.c"
#endif

#ifdef USE_7053
//-----------------------------------------------------------------------------

#define	CmdLth	(*(u16 *)ComData)
#define	Code645	(*(u16 *)(ComData+2))
#define	Data645	((u8 *)(ComData+4))


#ifdef USE_7053B
	#define		_ECHIP_FREQ_	6000000
	#define		_DFT_EMUCFG_	0x00
#else
	#define		_ECHIP_FREQ_	5529600
	#define		_DFT_EMUCFG_	0x10
#endif

#define		_V_BEILV_		1000

#define		_HF_CONST_		(((u32)(_ECHIP_FREQ_/6)*_MN_CU_CNST_)/((u32)_V_BEILV_*_KWH_IMP_))

#define		vBeilv			((u16*)(ComData+100))
#define 	vHFConst (*(u16*)(&FlashInfo.ChipSet.DftSet[18]))


union{
	u8  b8s[16];
	u32 b32;
}rnd;

typedef struct{
	u8  addr;
	u8  len;
}strReg7053;

struct {			//待处理
	u8 rd7053Cnt;
	u32 chkSum7053;
	u8 debug;
}mp;

static const u16 DftRms[3] = { 0x2710, 0x2710, 0x2710 };

//-----------------------------------------------------------------------------

u8 setErr(u8 a){
	pSendMsg->cmd = (pRecvMsg->cmd | 0xc0);
	pSendMsg->ln = 1;
	pSendMsg->data[0] = a;
	return SUCCESS;
}

u8 setln0(void){
	pSendMsg->cmd = pRecvMsg->cmd|0x80;
	pSendMsg ->ln = 0;
	return SUCCESS;
}
//--------------------------地址定义---------------------------------------------------

#define Spi_I1      0
#define Spi_I2      1
#define Spi_U       2
#define Spi_P       3
#define Spi_Q       4
#define Spi_S       5
#define I1RMS       0x06
#define I2RMS       0x07
#define U1RMS        0x08
#define Freq_U      0x09
#define PowerP1     0x0a
#define PowerQ1     0x0b
#define Power_S     0x0c
#define Energy_P    0x0d
#define Energy_Q    0x0e
#define Energy_S    0x0f
#define PowerP2     0x10
#define PowerQ2     0x11
#define I1Angle     0x12
#define I2Angle     0x13
#define TempData    0x14
#define VDCINData   0x15
#define BackupData  0x16
#define COMChecksum 0x17
#define SUMChecksum 0x18
#define EMUSR       0x19

#define EMUIE       0x30
#define EMUIF	    0x31
#define WPREG       0x32
#define SRSTREG     0x33
#define PWMPeriodDR 0x34
#define PWMDutyDR   0x35

#define EMUCFG      0x40
#define FreqCFG     0x41
#define ModuleEN    0x42
#define ANAEN       0x43
#define STATUSCFG   0x44
#define IOCFG       0x45

#define GP1	    0x50
#define GQ1	    0x51
#define GS1	    0x52
#define Phase1      0x53
#define GP2	    0x54
#define GQ2	    0x55
#define GS2	    0x56
#define Phase2	    0x57
#define QPhsCal1    0x58
#define ADCCON	    0x59
#define ALLGain     0x5a
#define I2Gain	    0x5b
#define I1Off       0x5c
#define I2Off	    0x5d
#define UOff	    0x5e
#define PQStart     0x5f
#define RMSStart    0x60
#define HFConst1     0x61
#define ICHK	    0x62
#define ITAMP	    0x63
#define UCONST      0x64
#define P1OFFSET    0x65
#define P2OFFSET    0x66
#define Q1OFFSET    0x67
#define Q2OFFSET    0x68
#define I1RMSOFFSET 0x69
#define I2RMSOFFSET 0x6a
#define U1RMSOFFSET  0x6b
#define ZCrossCurre 0x6c
#define GPhs1	    0x6d
#define GPhs2       0x6e
#define PFCnt1       0x6f
#define QFCnt1       0x70
#define SFCnt       0x71

//-----------------------------------------------------------------------------
const strReg7053 reg7053[] = { //寄存器地址, 及其有效数据的长度
	Spi_I1,      	3,
	Spi_I2,         3,
	Spi_U,          3,
	Spi_P,          3,
	Spi_Q,          3,
	Spi_S,          3,
	I1RMS,          3,
	I2RMS,          3,
	U1RMS,           3,
	Freq_U,         2,
	PowerP1,        3,
	PowerQ1,        3,
	Power_S,        3,
	Energy_P,       3,
	Energy_Q,       3,
	Energy_S,       3,
	PowerP2,        3,
	PowerQ2,        3,
	I1Angle,        3,
	I2Angle,        3,
	TempData,       1,
	VDCINData,      1,
	BackupData,     3,
	COMChecksum,    3,
	SUMChecksum,    3,
	EMUSR,          1,

	EMUIE,          2,
	EMUIF,          2,
	WPREG,          1,
	SRSTREG,        1,
	PWMPeriodDR,    2,
	PWMDutyDR,      2,

	EMUCFG,         2,
	FreqCFG,        2,
	ModuleEN,       2,
	ANAEN,          1,
	STATUSCFG,      2,
	IOCFG,          2,

	GP1,            2,
	GQ1,            2,
	GS1,            2,
	Phase1,         1,
	GP2,            2,
	GQ2,            2,
	GS2,            2,
	Phase2,         1,
	QPhsCal1,        2,
	ADCCON,         2,
	ALLGain,        2,
	I2Gain,         2,
	I1Off,          2,
	I2Off,          2,
	UOff,           2,
	PQStart,        2,
	RMSStart,       2,
	HFConst1,       2,
	ICHK,           1,
	ITAMP,          2,
	UCONST,         2,
	P1OFFSET,       1,
	P2OFFSET,       1,
	Q1OFFSET,       1,
	Q2OFFSET,       1,
	I1RMSOFFSET,    1,
	I2RMSOFFSET,    1,
	U1RMSOFFSET,     1,
	ZCrossCurre,    2,
	GPhs1,          2,
	GPhs2,          2,
	PFCnt1,          2,
	QFCnt1,          2,
	SFCnt,          2,
};

enum  reg7053_no{ //reg7053[]
	METER_Spi_I1,
	METER_Spi_I2,
	METER_Spi_U,
	METER_Spi_P,
	METER_Spi_Q,
	METER_Spi_S,
	METER_I1RMS,
	METER_I2RMS,
	METER_U1RMS,
	METER_Freq_U,
	METER_PowerP1,
	METER_PowerQ1,
	METER_Power_S,
	METER_Energy_P,
	METER_Energy_Q,
	METER_Energy_S,
	METER_PowerP2,
	METER_PowerQ2,
	METER_I1Angle,
	METER_I2Angle,
	METER_TempData,
	METER_VDCINData,
	METER_BackupData,
	METER_COMChecksum,
	METER_SUMChecksum,
	METER_EMUSR,

	METER_EMUIE,
	METER_EMUIF,
	METER_WPREG,
	METER_SRSTREG,
	METER_PWMPeriodDR,
	METER_PWMDutyDR,

	METER_EMUCFG,
	METER_FreqCFG,
	METER_ModuleEN,
	METER_ANAEN,
	METER_STATUSCFG,
	METER_IOCFG,

	METER_GP1,
	METER_GQ1,
	METER_GS1,
	METER_Phase1,
	METER_GP2,
	METER_GQ2,
	METER_GS2,
	METER_Phase2,
	METER_QPhsCal1,
	METER_ADCCON,
	METER_ALLGain,
	METER_I2Gain,
	METER_I1Off,
	METER_I2Off,
	METER_UOff,
	METER_PQStart,
	METER_RMSStart,
	METER_HFConst1,
	METER_ICHK,
	METER_ITAMP,
	METER_UCONST,
	METER_P1OFFSET,
	METER_P2OFFSET,
	METER_Q1OFFSET,
	METER_Q2OFFSET,
	METER_I1RMSOFFSET,
	METER_I2RMSOFFSET,
	METER_U1RMSOFFSET,
	METER_ZCrossCurre,
	METER_GPhs1,
	METER_GPhs2,
	METER_PFCnt1,
	METER_QFCnt1,
	METER_SFCnt
};

const u8 init7053Id[] = {
	METER_EMUCFG,
	METER_FreqCFG,
	METER_ModuleEN,
	METER_ANAEN,
	METER_GP1,		//改写保护
	METER_GQ1,
	METER_GS1,
	METER_ADCCON,		//定期检验
	METER_PQStart,
	METER_HFConst1,
	METER_P1OFFSET,
	METER_I1RMSOFFSET,
	METER_I2RMSOFFSET,
	METER_GPhs1
};


const u8 defInit7053[] = {
	CHIP_7053,
	0x00,_DFT_EMUCFG_,
	0x00,0xC8,
	0x00,0x7E,
	0x3F,
	0x00,0x00,
	0x00,0x00,
	0x00,0x00,
	0x00,0x0C,
	0x00,0x40,
	((_HF_CONST_)>>8),(_HF_CONST_&0xFF),
	0x00,
	0x00,
	0x00,
	0x00,0x00
};

void spiCs_0(void){

	RES_SPI_SCK;
	SetTBOC();
	WaitTBOC(200);
	gcResIO(SPI_CS);
	WaitTBOC(100);
}

u8 up7053ToEEP(void){
	u8 n;
	u8* Base1;
	u8 s[50];

	Base1 = s;
	for(n=0; n<sizeof(init7053Id); n++){
		if(!Read_7053(Base1, init7053Id[n])){
			return 0;
		}
		Base1 += reg7053[init7053Id[n]].len;
	}
	Copy_FlashInfo((u8*)&FlashInfo.ChipSet.DftSet+1, s, sizeof(defInit7053)-1);
	return 1;
}

void sendSpi7053(u8 a){
	u8 n = 0;

	Check_MS();
	SetTBOC();
	WaitTBOC(50);
	while(n < 8){
		if(a&0x80){
			SET_SPI_DO;
		}else{
			RES_SPI_DO;
		}
		a <<= 1;
		SetTBOC();
		SET_SPI_SCK;
		WaitTBOC(130);
		RES_SPI_SCK;
		WaitTBOC(180);
		n++;
	}
}

u8 ReceiveSpi7053(){
	u8 i;
	u8 ddata = 0;

	Check_MS();
	for(i=0;i<8;i++)
	{
		SET_SPI_SCK;
		SetTBOC();
		WaitTBOC(130);
		ddata <<=1;
		if(gcReadIO(pin_SPI_DI)){
			ddata |=0x01;
		}
		RES_SPI_SCK;
		WaitTBOC(180);
	}
	return ddata;
}

u8	Read_7053(u8* Base1, u8 no){
	u8 a;
	u8 b;
	u8 n;
	u8 s[8];

	spiCs_0();
	a = reg7053[no].addr & 0x7f;
	sendSpi7053(a);			//发送命令字

	a = 0;
	b = 0;
	do {
		n = ReceiveSpi7053();	//接收数据
		if((3-a) <= reg7053[no].len){
			Base1[b] = n;
			s[a] = n;
			b++;
		}
	} while(++a < 3);
	gcSetIO(SPI_CS);
	DelayMS(1);
	b = reg7053[no].len;
	n = METER_BackupData;		//校验数据
	spiCs_0();
	n = reg7053[n].addr & 0x7f;
	sendSpi7053(n);
	a = 0;
	do {
		n = ReceiveSpi7053();
		s[3+a] = n;
	} while(++a < 3);
	if(strCmp((u8*)(s+3-b),(u8*)(s+3+(3-b)), b) != 0){
		gcSetIO(SPI_CS);
		return 0;
	}
	gcSetIO(SPI_CS);
	DelayMS(1);
	return 1;
}

u8	Write_7053(u8* Base2, u8 no){
	u8 a;
	u8 b;
	u8 n;
	u8 s[8];

	spiCs_0();
	sendSpi7053(0x32|0x80);		//写使能
	sendSpi7053(0);
	sendSpi7053(0);
	if((reg7053[no].addr <= 0x45)&&(reg7053[no].addr >= 0x40)){
		sendSpi7053(0xbc);
	}else if(reg7053[no].addr <= 0x71){
		sendSpi7053(0xa6);
	}
	gcSetIO(SPI_CS);
	DelayMS(1);
	spiCs_0();
	a = (reg7053[no].addr & 0x7f)|0x80;
	sendSpi7053(a);			//发送命令字

	a = 0;
	b = 0;
	do{
		if((3-a) <= reg7053[no].len){
			sendSpi7053(Base2[b]);
			s[a] = Base2[b];
			b++;
		}else{
			sendSpi7053(0);
		}
	}while( ++a < 3);
	gcSetIO(SPI_CS);
	DelayMS(1);
	b = reg7053[no].len;
	n = METER_BackupData;		//校验数据
	spiCs_0();
	n = reg7053[n].addr & 0x7f;
	sendSpi7053(n);
	a = 0;
	do {
		n = ReceiveSpi7053();
		s[3+a] = n;
	} while(++a < 3);
	gcSetIO(SPI_CS);
	DelayMS(1);
	spiCs_0();
	sendSpi7053(0x32|0x80);		//写使能
	sendSpi7053(0);
	sendSpi7053(0);
	sendSpi7053(0);
	mp.debug = 1; mp.rd7053Cnt = 1;
	gcSetIO(SPI_CS);
	DelayMS(1);
	if(strCmp((u8*)(s+3-b),(u8*)(s+3+(3-b)), b) != 0){
		return 0;
	}else {
		return 1;
	}
}

u8 rst7053(void){

	u8	Retry;
	ClrRam((u8*)&mp, sizeof(mp));
	gcResIO(pin_Chip_Rst);
	DelayMS(100);
	gcSetIO(pin_Chip_Rst);
	DelayMS(100);
	Retry = 0;
	while(1){
		if(Read_7053(rnd.b8s, METER_EMUIF)){										//读系统状态寄存器，检测复位是否结束，
			if((rnd.b8s[0]&0x80)){
				DelayMS(100);
				break;
			}
		}
		if(++Retry > 3){
			return 1;
		}
	}
	return 0;
}

u8 start7053(void){//0 ok
	DelayMS(50);//50ms updata data
	rnd.b8s[0] = 0;
	if(!Read_7053(&rnd.b8s[1], METER_SUMChecksum)){
		return 0x84;
	}
	//校验和计算状态完成。可用
	mp.chkSum7053 = *(u32*)rnd.b8s;		//待验证数据
	return 0;
}

#define I_RmsBeilv (*(u16*)(FlashInfo.ChipSet.DftSet+50))	//A通道电流有效值系数
#define I_RmsBeilv2 (*(u16*)(FlashInfo.ChipSet.DftSet+52))	//B通道电流有效值系数
#define Vol_RmsBeilv (*(u16*)(FlashInfo.ChipSet.DftSet+54))	//C通道电压有效值系数

void	InitHFConst(u16 NewConst){

	if(NewConst == 0){
		NewConst = (((u32)(_ECHIP_FREQ_/6)*FData.DftSet.MnCuRes)/((u32)_V_BEILV_*FData.DftSet.kWhImp));
	}
	Copy_FlashInfo(	(u8*)&vHFConst,	(u8*)&NewConst, 2 );
	Write_7053((u8*)&NewConst, METER_HFConst1);
}

u8	Init_7053(void){ //1 SUCCESS, 0 FAIL
	u8* Base2;
	u8 n;

	if(((*(u32*)(FlashInfo.ChipSet.DftSet+1))==0)||((*(u32*)(FlashInfo.ChipSet.DftSet+1))==0XFFFFFFFF)){
		Copy_FlashInfo(FlashInfo.ChipSet.DftSet,(void*)defInit7053, sizeof(defInit7053));
		Copy_FlashInfo((u8*)(&I_RmsBeilv), (u8*)&DftRms, 6);
		InitHFConst(0);
	}
	if(rst7053()){
		return 0;
	}
	Base2 = FlashInfo.ChipSet.DftSet+1;
	for(n=0; n<sizeof(init7053Id); n++){
		if(!Write_7053(Base2, init7053Id[n])){
			return 0;
		}
		Base2 += reg7053[init7053Id[n]].len;
	}

	Base2 = FlashInfo.ChipSet.DftSet+1;
	for(n=0; n<sizeof(init7053Id); n++){
		if(!Read_7053(rnd.b8s, init7053Id[n])){
			return 0;
		}
		if(strCmp(Base2, rnd.b8s, reg7053[init7053Id[n]].len) != 0){
			return 0;
		}
		Base2 += reg7053[init7053Id[n]].len;
	}
	if(start7053()){
		return 0;
	}
	return 1;
}

u32	MulDivX( u32 Mul_1, u32 Mul_2, u32 Div){

/*	double x64;
	x64 = Mul_1;
	x64 *= Mul_2;
	x64 /= Div;
	return (u32)x64;	*/
	u32	DivX;
	u32	MulX;

	DivX = (u32)0xFFFFFFFF/Mul_1;
	MulX = 1;
	while(DivX < Mul_2){
		if((Mul_1&1) == 0){
			Mul_1>>= 1;
			DivX = (u32)0xFFFFFFFF/Mul_1;
		} else if(((Mul_2&1) == 0)||(Mul_1 < Mul_2)){
			Mul_2 >>= 1;
		} else {
			Mul_1 >>= 1;
			DivX = (u32)0xFFFFFFFF/Mul_1;
		}
		if((Div&1)==0){
			Div >>= 1;
		} else {
			MulX <<= 1;
		}
	}
	Mul_1 *= Mul_2;
	DivX = Mul_1%Div;
	Mul_1 = Mul_1/Div;
	return (Mul_1*MulX) + ((DivX*MulX)/Div);
}

void	DoSinglePhase_7053(void){

	u32	X32;
	switch(mp.rd7053Cnt){
		case 1://检查寄存器
			if(Read_7053(rnd.b8s, METER_ADCCON)){
				if(FData.DftSet.MSet.UseHugan){
					if(rnd.b8s[1] != 0x00) {
						Init_7053();
						return ;
					}
				}else{
					if(rnd.b8s[1] != 0x0c) {
						Init_7053();
						return ;
					}
				}
			}
			if(Read_7053(rnd.b8s, METER_EMUCFG)){
				if(rnd.b8s[1] != _DFT_EMUCFG_){
					Init_7053();
					return ;
				}
			}

			if(Read_7053(&rnd.b8s[2], METER_Freq_U)){
				RamData.Freq = (_ECHIP_FREQ_*100/12)/(*(u16*)(rnd.b8s+2));
			}
			break;
		case 2://读电压 电流，
			if(Read_7053(&rnd.b8s[1], METER_U1RMS)){
				if((rnd.b8s[1]&0x80)==0){		//数据有效		(10^6*10)/((10^3*1.1*2^23)*10^3) = 1/1191563
					rnd.b8s[0] = 0;
					RamData.Vph[0] =  MulDivX(rnd.b32, Vol_RmsBeilv, 9311355);//((u32)8680*(1000000/_V_BEILV_)));
				}
			}
			if(Read_7053(&rnd.b8s[1], METER_I1RMS)){	//a通道脉冲输出通道电流//先接收 发来的高字节
				if((rnd.b8s[1]&0x80)==0){		//数据有效
					rnd.b8s[0] = 0;
					RamData.Iph[0] = MulDivX(rnd.b32, I_RmsBeilv, 369090);
				}
			}
			if(Read_7053(&rnd.b8s[1], METER_I2RMS)){	//b通道零线电流
				if((rnd.b8s[1]&0x80)==0){//数据有效
					rnd.b8s[0] = 0;
					RamData.Iph[1] = MulDivX(rnd.b32, (u32)I_RmsBeilv2*200, 92274688);
					if(Read_7053(&rnd.b8s[1], METER_I2Angle)){
						if((rnd.b8s[1]^(rnd.b8s[1]<<1))&0x10){
							RamData.Iph[1] = -RamData.Iph[1];
						}
					}
					RamData.I_z = RamData.Iph[1];
				}
			}
			//读有功功率 及方向
			if( Read_7053(&rnd.b8s[1], METER_PowerP1) ){
				rnd.b8s[0] = 0;
				if(rnd.b8s[1] &0x80){
					AUse_Dir = AUse_Dir_A = 1;
					rnd.b32 = 0x1000000 - rnd.b32;
				}else{
					AUse_Dir = AUse_Dir_A = 0;
				}
				RamData.Pph[0] =  MulDivX(rnd.b32, ((u32)_ECHIP_FREQ_*625/55924), (u32)vHFConst*FData.DftSet.kWhImp);
			}

			X32 = RamData.Vph[0]*RamData.Iph[0]/1000;
			if(RamData.Pph[0] <= 2){
				RamData.Pfph[0] = 0;
			} else {
				RamData.Pfph[0] = (RamData.Pph[0]*1000)/X32;
				if(RamData.Pfph[0] > 1000){
					RamData.Pfph[0] = 1000;
				}
			}

			if(AUse_Dir){
				RamData.Pfph[0] =  -RamData.Pfph[0];
				RamData.Pph[0] = -RamData.Pph[0];
				RamData.Iph[0] = -RamData.Iph[0];
			}
			break;
		case 3://读参数校验和
			if(!Read_7053(&rnd.b8s[1], METER_SUMChecksum)){
				return ;
			}
			rnd.b8s[0] = 0;
			if(mp.debug){
				mp.chkSum7053 = *(u32*)(rnd.b8s);
				mp.debug = 0;
			}else{
				if( (mp.chkSum7053) !=  *(u32*)rnd.b8s){
					Init_7053();
				}
			}
			break;
	}
	if( Read_7053(&rnd.b8s[1], METER_PowerP1) ){
		if(rnd.b8s[1] &0x80){
			AUse_Dir = AUse_Dir_A = 1;
		}else{
			AUse_Dir = AUse_Dir_A = 0;
		}
	}
	mp.rd7053Cnt++;
	if(mp.rd7053Cnt>=4){
		mp.rd7053Cnt = 1;
		RamData.Use_Act = 1;
	}

	{
		u8 a;
		a = FData.DftSet.A_Std[0]/50;
		if(a){
			a *= 22; //基本电流的2/1000,  *220V
		}else{
			a =6;
		}
		if((abs32(RamData.Pph[0]) < a)&&(abs32(RamData.Iph[0]) < 100)){
			RamData.Pph[0] = 0;
			RamData.Iph[0] = 0;
			RamData.Pfph[0] = 1000;
			AUse_Dir = AUse_Dir_A = 0;
			if(RamData.RevTmCnt < 0){
				RamData.RevTmCnt = 0;
			}
		}
		if( abs32(RamData.I_z) < (FData.DftSet.A_Std[0]*2)){ //千分之2
			RamData.I_z = RamData.Iph[1] = 0;
		}
	}

}

u8 recv7053(void){
	u8 n;
	u16 RevData;

#ifndef USE_7053_MCU_CAL

	if((pRecvMsg->cmd==0x01)&&(pRecvMsg->ln==0x02)){
		if(Code645 == REV16(0xC030)){											//读脉冲常数
			pSendMsg->cmd = 0x81;
			pSendMsg->ln = 0x04;
			pSendMsg->data[0] = 0x30;
			pSendMsg->data[1] = 0xc0;
			pSendMsg->data[2] = BCD((u8)(FData.DftSet.kWhImp%100));
			pSendMsg->data[3] = BCD((u8)(FData.DftSet.kWhImp/100));
			return SUCCESS;
		}else if((pRecvMsg ->data[1] == 0xd5)){
			if(pRecvMsg ->data[0] > 71 ){
				return setErr(0x02);
			}
			n = pRecvMsg ->data[0];
			if( !Read_7053(rnd.b8s, n) ){
				return setErr(0x02);
			}
			pSendMsg->cmd = pRecvMsg->cmd | 0x80;
			pSendMsg->ln = 2 + reg7053[n].len;
			CopyRam(pSendMsg->data+2, rnd.b8s, reg7053[n].len);
			return SUCCESS;
		}else if((pRecvMsg ->data[1] == 0xd6)){
			if(pRecvMsg ->data[0] > 3 ){
				return setErr(0x02);
			}
			n = pRecvMsg ->data[0];
			if(n == 1){
				RevData = REV16(I_RmsBeilv);
			}else if(n == 2){
				RevData = REV16(I_RmsBeilv2);
			}else if(n == 3){
				RevData = REV16(Vol_RmsBeilv);
			}
			CopyRam(pSendMsg->data+2, (u8*)&RevData, 2);
			pSendMsg->cmd = pRecvMsg->cmd | 0x80;
			pSendMsg->ln = 4;
			return SUCCESS;
		}
	}else if (pRecvMsg->cmd==0x04){
		if(((pRecvMsg->data[1]&0xF0) == 0xD0)){
			if(!getProgSecurity()){
					return setErr(0x02);
			}
			if((u16p(pRecvMsg->data) == 0x99D9)&&(pRecvMsg ->data[2] == 0x01)){//保存校表数据
				if(up7053ToEEP()){
					return setln0();
				}else{
					return setErr(0x01);
				}
			}else if((u16p(pRecvMsg ->data) == 0x99d9)&&(pRecvMsg ->data[2] == 0x02)){//保存a(低字节在前),b,c 通道有效值系数6个
				if(pRecvMsg->ln != 9){
					return setErr(0x01);
				}
				*((u16*)(pRecvMsg ->data+9))  = REV16(*(u16*)(pRecvMsg ->data+3));
				*((u16*)(pRecvMsg ->data+11)) = REV16(*((u16*)(pRecvMsg ->data+5)));
				*((u16*)(pRecvMsg ->data+13)) = REV16(*((u16*)(pRecvMsg ->data+7)));
				Copy_FlashInfo( (u8*)&I_RmsBeilv, (u8*)pRecvMsg ->data+9, 6 );
				return setln0();
			}else if(pRecvMsg ->data[1] == 0xd5){
				if(pRecvMsg ->data[0] > 71 ){
					return setErr(0x02);
				}
				n = pRecvMsg ->data[0];
				CopyRam(rnd.b8s, pRecvMsg ->data+2, reg7053[n].len);    //高字节在前
				if(!Write_7053(rnd.b8s, n)){
					return setErr(0x02);
				}
				if(up7053ToEEP()){
					return setln0();
				}else{
					return setErr(0x01);
				}
			}
		}
	}

#endif

#ifdef USE_7053_MCU_CAL

	if((CmdLth == 0x0102)&&(Code645==0x21d5)){
			if( !Read_7053(rnd.b8s, 0x21) ){
				return setErr(0x02);
			}
			pSendMsg->cmd = pRecvMsg->cmd | 0x80;
			pSendMsg->ln = 2 + reg7053[0x21].len;
			CopyRam(pSendMsg->data+2, rnd.b8s, reg7053[0x21].len);
			return SUCCESS;
	}

	if(getProgSecurity()){
	if((CmdLth == 0x1107)&&(Code645 == REV16(0xFC01))){
//		 (pRecvMsg ->data[0]==0x01)&&(pRecvMsg ->data[1]==((u8)(0x2f-0x33)))){
		//初始化
		Write_7053(zeroP, METER_GP1); //功率增益
		Write_7053(zeroP, METER_GQ1);
		Write_7053(zeroP, METER_GS1);
		Write_7053(zeroP, METER_GPhs1);//相位
		Write_7053(zeroP, METER_P1OFFSET);//功率偏置
		InitHFConst(REV16(*(u16*)(ComData+5)));
		RamData.State.No_Ret = 1;
		RamData.cal8209.event2 = 1;
		return SUCCESS;
	} else if((CmdLth == 0x1111)&&(ComData[3] == 0x1D)){
		//基本电压 电流 1.0
		if(ComData[2] == 0x11){											//基本电流
		} else if(ComData[2] == 0x15){											//基本电流0.5
		} else if(ComData[2] == 0x31){											//功率偏值5%Ib
		} else if(ComData[2] == 0x51){											//零线电流
		} else {
			return FAIL;
		}
		RamData.cal8209.event2 = ComData[2];
		RamData.cal8209.voltageRef = BCD_Hex(REV16(*(u16*)(ComData+5)));
		RamData.cal8209.powerRef = BCD_Hex(rev32((u32*)(ComData+11)));
		ComData[11] = 0;
		RamData.cal8209.current2Ref = RamData.cal8209.current1Ref =
		BCD_Hex(rev32((u32*)(pRecvMsg ->data+6)));
		RamData.cal8209.cnt = 0;
		RamData.cal8209.voltage = 0;
		RamData.cal8209.current1 = 0;
		RamData.cal8209.current2 = 0;
		RamData.cal8209.power = 0;
		RamData.cal8209.event1 = 0xaa;
		RamData.State.No_Ret = 1;
		return SUCCESS;
	}
	}

#endif

	return FAIL;
}


#ifdef USE_7053_MCU_CAL

/*
u8 doCalib7053(void){
	u16 a[3];
	s32 g;
	u32 x;
	s16 phase1;
	double ang;

	RamData.cal8209.cnt++;
	if(RamData.cal8209.cnt >10){
		RamData.cal8209.voltage += RamData.Vph[0];
		RamData.cal8209.current1 += RamData.Iph[0];
		RamData.cal8209.current2 += RamData.Iph[1];
		RamData.cal8209.power += RamData.Pph[0];
		RamData.cal8209.powerReg += RamData.powerReg;
	}
	if(RamData.cal8209.cnt == 20){
		RamData.cal8209.voltage /= 10;
			RamData.cal8209.current1 /= 10;
			RamData.cal8209.current2 /= 10;
			RamData.cal8209.power /= 10;
			RamData.cal8209.powerReg /= 10;
		if(RamData.cal8209.event2 == 0x11){//基本电压电流 1.0
			a[0] = (((u32)RamData.cal8209.current1Ref*I_RmsBeilv)/RamData.cal8209.current1);
			a[1] = I_RmsBeilv2;
			a[2] = (((u32)RamData.cal8209.voltageRef*Vol_RmsBeilv)/RamData.cal8209.voltage);
			Copy_FlashInfo( (u8*)&I_RmsBeilv, (u8*)a, 6 );
			// 		  -err
			//g = --------
			//     1 + err
			ang = (RamData.cal8209.powerRef - RamData.cal8209.power*100);
			ang *= 32768.0;
			ang = ang / (double)(RamData.cal8209.power*100);
			g = (s32)ang;
			g = g + 65536;
			Write_7053(((u8*)&g)+2, METER_GP1); //功率增益
			Write_7053(((u8*)&g)+2, METER_GQ1); //功率增益
			Write_7053(((u8*)&g)+2, METER_GS1); //功率增益
			up7053ToEEP();
		}else if(RamData.cal8209.event2 == 0x15){//基本电压电流 0.5L
			//				-err
			// @ = ------------
			//			1.73205
			//
			//					p1-p0
			// err = -----------
			//            p0
			// p0 = 11000 ,0.1w

			//g = RamData.cal8209.power - 11000;
			g = RamData.cal8209.powerRef - RamData.cal8209.power*100;
			ang = ((double)(RamData.cal8209.powerRef))*1.73205;
			ang = ((double)g)/ang;
			ang *= 32768.0;
			g = (s32)ang;
 			g += 65536;
			Write_7053(((u8*)&g)+2, METER_GPhs1); //相位
			up7053ToEEP();
		}else if(RamData.cal8209.event2 == 0x31){ //小功率偏移
			//     2^23*HFCONST*1200      Preal
			//x = --------------------- *------- * (-err)
			//      5.75*10^10             	1
			//x 的补码 ,Preal的单位是瓦
			ang = (double)(RamData.cal8209.powerRef - RamData.cal8209.power*100);
			ang *= 0.0252;
			g = (s32)ang;
			g = g + 256;
			Write_7053(((u8*)(&g))+3, METER_P1OFFSET);
			up7053ToEEP();
		}else if(RamData.cal8209.event2 == 0x51){ //零线电流
			a[0] = I_RmsBeilv;
			a[1] = (((u32)RamData.cal8209.current2Ref*I_RmsBeilv2)/RamData.cal8209.current2);
			a[2] = Vol_RmsBeilv;
			Copy_FlashInfo( (u8*)&I_RmsBeilv, (u8*)a, 6 );
		}
		RamData.cal8209.event1 = 0;
		RamData.cal8209.event2 = 0;
	}
}
*/

u8 doCalib7053(void){

	u32	Cal1;
	u32	Cal2;

	mp.rd7053Cnt = 2;
	{
		RamData.cal8209.voltage += RamData.Vph[0];
		RamData.cal8209.current1 += RamData.Iph[0];
		RamData.cal8209.current2 += RamData.Iph[1];
		RamData.cal8209.power += RamData.Pph[0];
	}
	RamData.cal8209.cnt++;
	if(RamData.cal8209.cnt < 10){
		return;
	}
	RamData.cal8209.voltage /= 10;
	RamData.cal8209.current1 /= 10;
	RamData.cal8209.current2 /= 10;
	RamData.cal8209.power /= 10;
	if(RamData.cal8209.event2 == 0x11){											//基本电压电流 1.0
		vBeilv[0] = (((u32)RamData.cal8209.current1Ref*I_RmsBeilv)/RamData.cal8209.current1);
		vBeilv[1] = I_RmsBeilv2;
		vBeilv[2] = (((u32)RamData.cal8209.voltageRef*Vol_RmsBeilv)/RamData.cal8209.voltage);
		Copy_FlashInfo( (u8*)&I_RmsBeilv, (u8*)vBeilv, 6 );
		Cal1 = MulDivX(RamData.cal8209.powerRef, 32768, RamData.cal8209.power*100);
		Cal1 -= 32768;
		Write_7053(((u8*)&Cal1)+2, METER_GP1);									//功率增益
		Write_7053(((u8*)&Cal1)+2, METER_GQ1);									//功率增益
		Write_7053(((u8*)&Cal1)+2, METER_GS1);									//功率增益
		up7053ToEEP();
	} else if(RamData.cal8209.event2 == 0x15){									//基本电压电流 0.5L
		Cal1 = (RamData.cal8209.powerRef/100) - RamData.cal8209.power;
		Cal2 = abs32(Cal1)*1891961/RamData.cal8209.powerRef;						//Cal2 = labs(Cal1)*100*32768/1.73205/PowrerRef
		if(((s32)Cal1) < 0){
			Cal2 = -Cal2;
		}
		Write_7053(((u8*)&Cal2)+2, METER_GPhs1);								//相位
		up7053ToEEP();
	}else if(RamData.cal8209.event2 == 0x31){									//小功率偏移
		Cal1 = (RamData.cal8209.powerRef - RamData.cal8209.power*100);
		Cal2 = MulDivX(abs32(Cal1), ((u32)vHFConst*FData.DftSet.kWhImp)*242, (_ECHIP_FREQ_*3)*100);
		if(((s32)Cal1) < 0){
			Cal2 = -Cal2;
		}
		Write_7053(((u8*)(&Cal2))+3, METER_P1OFFSET);
		up7053ToEEP();
	}else if(RamData.cal8209.event2 == 0x51){									//零线电流
		vBeilv[0] = I_RmsBeilv;
		vBeilv[1] = (((u32)RamData.cal8209.current2Ref*I_RmsBeilv2)/RamData.cal8209.current2);
		vBeilv[2] = Vol_RmsBeilv;
		Copy_FlashInfo( (u8*)&I_RmsBeilv, (u8*)vBeilv, 6 );
	}
	RamData.cal8209.event1 = 0;

}

#endif

#endif //USE_7053