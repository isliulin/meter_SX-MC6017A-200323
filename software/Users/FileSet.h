#ifdef		_FILE_SET_DATA_
#define		_FILE_SET_(Name, Data)	Data,
#else
#define		_FILE_SET_(Name, Data)	Name;
#endif
#ifndef  FILESET_PRE_SET
#define  FILESET_PRE_SET
#define u8 		unsigned char 
#define 	u16  unsigned short
	#define u32  unsigned long
		#endif
#ifndef		_FILE_SET_DATA_
u8 FacKey[8];
#else
{	(_TIME_DAY_((_FACKEYNUM_+9), 3, 27)%199),
	(_TIME_DAY_((_FACKEYNUM_+3), 1, 5)%137),
	(_TIME_DAY_((_FACKEYNUM_+5), 6, 9)%179),
	(_TIME_DAY_((_FACKEYNUM_+4), 5, 12)%127),
	(_TIME_DAY_((_FACKEYNUM_+2), 11, 3)%151),
	(_TIME_DAY_((_FACKEYNUM_+6), 4, 30)%173),
	(_TIME_DAY_((_FACKEYNUM_+8), 12, 18)%109),
	(_TIME_DAY_((_FACKEYNUM_+7), 8, 5)%157)},
#endif

#ifndef		_FILE_SET_DATA_
u16 AlmMask; //AlmMask
u8 EvtCntMask[8]; //状态字不记次数掩码
#else
0x400C, //	0b0100000000001100,
{	0xBE, 0x03, 0x40, 0, 0, 0, 0, 0},
#endif

#ifndef		_FILE_SET_DATA_
u8 DCardOverLth;
#else
2,
#endif

#ifndef		_FILE_SET_DATA_
strDispCode DArryCardOver[4];
#else
{ //	DArryCardOver
	{	_CARD_OVER_D1_, 0},
	{	_CARD_OVER_D2_, 0},
},
#endif
/*
 #ifndef		_FILE_SET_DATA_
 strDispCode	DArryNorm[2][10];
 strDispCode	DArryKey[2][40];
 #else
 {
 {													//	Auto disp
 { 0x00900200, 0 },
 { 0x00000000, 0 },
 { 0x00000100, 0 },
 { 0x00000200, 0 },
 { 0x00000300, 0 },
 { 0x00000400, 0 },
 { 0x02800020, 0 },
 },
 {													//	Auto Disp
 { 0x00000000, 0 },
 { 0x00000100, 0 },
 { 0x00000200, 0 },
 { 0x00000300, 0 },
 { 0x00000400, 0 },
 },
 },
 {
 {												//	Key Disp
 //		{ 0x00000000, 0 },							//临时测试
 //		{ 0x00000000, 0 },							//临时测试
 { 0x00900200, 0 },
 { 0x00000000, 0 },
 { 0x00000100, 0 },
 { 0x00000200, 0 },
 { 0x00000300, 0 },
 { 0x00000400, 0 },
 { 0x00000001, 0 },
 { 0x00000101, 0 },
 { 0x00000201, 0 },
 { 0x00000301, 0 },
 { 0x00000401, 0 },
 { 0x00000002, 0 },
 { 0x00000102, 0 },
 { 0x00000202, 0 },
 { 0x00000302, 0 },
 { 0x00000402, 0 },
 { 0x02800020, 0 },
 { 0x0400040E, 1 },
 { 0x0400040E, 0 },
 { 0x04000401, 1 },
 { 0x04000401, 0 },
 { 0x04000101, 0 },
 { 0x04000102, 0 },
 { 0x02010100, 0 },
 { 0x02020100, 0 },
 { 0x02030100, 0 },
 { 0x02060100, 0 },
 },
 {												// Key Disp
 { 0x00000000, 0 },
 { 0x00000100, 0 },
 { 0x00000200, 0 },
 { 0x00000300, 0 },
 { 0x00000400, 0 },
 { 0x00000001, 0 },
 { 0x00000101, 0 },
 { 0x00000201, 0 },
 { 0x00000301, 0 },
 { 0x00000401, 0 },
 { 0x00000002, 0 },
 { 0x00000102, 0 },
 { 0x00000202, 0 },
 { 0x00000302, 0 },
 { 0x00000402, 0 },
 //	{ 0x0400040E, 0 },
 //	{ 0x0400040E, 1 },
 { 0x04000401, 1 },
 { 0x04000401, 0 },
 { 0x04000101, 0 },
 { 0x04000102, 0 },
 { 0x02010100, 0 },
 { 0x02020100, 0 },
 //	{ 0x02800001, 0 },
 { 0x02030100, 0 },
 { 0x02060100, 0 },
 }
 },

 #endif
 */

#ifndef		_FILE_SET_DATA_
struct STDISPARRYDATA_BAK stdisparrydatabak[2];
#else
{
	{ //#ifdef		_USE_CARD_

		{	7 ,27},
		{ //	Auto disp
			{	0x00900200, 0},
			{	0x00000000, 0},
			{	0x00000100, 0},
			{	0x00000200, 0},
			{	0x00000300, 0},
			{	0x00000400, 0},
			{	0x02800020, 0},
		},
		{ //	Key Disp
			//		{ 0x00000000, 0 },							//临时测试
			//		{ 0x00000000, 0 },							//临时测试
			{	0x00900200, 0},
			{	0x00000000, 0},
			{	0x00000100, 0},
			{	0x00000200, 0},
			{	0x00000300, 0},
			{	0x00000400, 0},
			{	0x00000001, 0},
			{	0x00000101, 0},
			{	0x00000201, 0},
			{	0x00000301, 0},
			{	0x00000401, 0},
			{	0x00000002, 0},
			{	0x00000102, 0},
			{	0x00000202, 0},
			{	0x00000302, 0},
			{	0x00000402, 0},
			{	0x02800020, 0},
			{	0x0400040E, 1},
			{	0x0400040E, 0},
			{	0x04000401, 1},
			{	0x04000401, 0},
			{	0x04000101, 0},
			{	0x04000102, 0},
			{	0x02010100, 0},
			{	0x02020100, 0},
			{	0x02030100, 0},
			{	0x02060100, 0},
		},
	},//#else
	{

		{	5 ,23},
		{ //	Auto Disp
			{	0x00000000, 0},
			{	0x00000100, 0},
			{	0x00000200, 0},
			{	0x00000300, 0},
			{	0x00000400, 0},
		},
		{ // Key Disp
			{	0x00000000, 0},
			{	0x00000100, 0},
			{	0x00000200, 0},
			{	0x00000300, 0},
			{	0x00000400, 0},
			{	0x00000001, 0},
			{	0x00000101, 0},
			{	0x00000201, 0},
			{	0x00000301, 0},
			{	0x00000401, 0},
			{	0x00000002, 0},
			{	0x00000102, 0},
			{	0x00000202, 0},
			{	0x00000302, 0},
			{	0x00000402, 0},
			//	{ 0x0400040E, 0 },
			//	{ 0x0400040E, 1 },
			{	0x04000401, 1},
			{	0x04000401, 0},
			{	0x04000101, 0},
			{	0x04000102, 0},
			{	0x02010100, 0},
			{	0x02020100, 0},
			//	{ 0x02800001, 0 },
			{	0x02030100, 0},
			{	0x02060100, 0},
		},
	},
},
//#endif
//#endif

#endif

#ifndef		_FILE_SET_DATA_
struct STMEMTER_STATU stmemter_statu[4];
#else
{
	{
		_IM_TYPE_CARRY_, //		"DDZYXXXC-M"	//电表类型
		_SOFT_VER_CARRY_, //	"DDZYXXXC-M"	//软件版本号
		_HARD_VER_CARRY_, //	"DDZYXXXC-M"	//硬件版本号
	}, //硬件版本号0x1F, 0xF1, 0x00, 0x04 },
	{
		//本地485表：DDZYXXXC
		_IM_TYPE_485_, //			"DDZYXXXC"	//电表类型
		_SOFT_VER_485_, //			"DDZYXXXC"	//软件版本号
		_HARD_VER_485_, //			"DDZYXXXC"	//硬件版本号
	},

	{
		_IM_TYPE_CARRY_FAR_, //			"DDZYXXXC-M"	//电表类型
		_SOFT_VER_CARRY_FAR_, //		"DDZYXXXC-M"	//软件版本号
		_HARD_VER_CARRY_FAR_, //		"DDZYXXXC-M"	//硬件版本号
	}, //硬件版本号0x1F, 0xF1, 0x00, 0x04 },
	{
		_IM_TYPE_485_FAR_, //			"DDZYXXX"	//电表类型
		_SOFT_VER_485_FAR_, //			"DDZYXXX"	//软件版本号
		_HARD_VER_485_FAR_, //			"DDZYXXX"	//硬件版本号
	},
},
#endif

#ifndef		_FILE_SET_DATA_
u8 DVe[4];
u8 DTmTbl[4];
u8 DRemain[4];
#define		DSumUse		ZeroP
#define		DAll		Const5FF
#else
{	0x1F, 0xF1, 0x00, 0x04},
{	0x07, 0x06, 0x06, 0x06},
{	0x00, 0x02, 0x90, 0x00},
#endif

#ifndef		_FILE_SET_DATA_
u8 DigitDots[4]; //Dots Set
u8 DispFeilvSet[5]; //
u8 DispFeilvDots[4]; //
u8 DispStepDots[4]; //
u16 DispBias_kWh; //
u16 DispBias_kVah; //
u16 DispBias_kW; //
u16 DispBias_kVa; //
u16 DispBias_V; //
u16 DispBias_A; //
u16 DispBias_Yuan; //
#else
{	_DISP_DOT_1_, _DISP_DOT_2_, _DISP_DOT_3_, _DISP_DOT_4_},
{	_DISP_ZONG_, _DISP_JIAN_, _DISP_FENG_, _DISP_PING_, _DISP_GU_},
{	_DISP_AT_JIAN_, _DISP_AT_FENG_, _DISP_AT_PING_, _DISP_AT_GU_},
{	_DISP_ST_1_, _DISP_ST_2_, _DISP_ST_3_, _DISP_ST_4_},
_FSET_BIAS_(D_kWh),
0,
(_FSET_BIAS_(D_kWh)+1),
0,
_FSET_BIAS_(D_V),
_FSET_BIAS_(D_A),
_FSET_BIAS_(D_Yuan),
#endif

#ifndef	_FILE_SET_DATA_
u8 D_kWh[4];
u8 D_V[2];
u8 D_A[2];
u8 D_Yuan[2];
#else
{	_DISP_H_, _DISP_K_ ,_DISP_W_ ,_DISP_W_},
{	_DISP_V_, _DISP_V_},
{	_DISP_A_, _DISP_A_},
{	_DISP_YUAN_, _DISP_YUAN_},
#endif

#ifndef	_FILE_SET_DATA_
u8 DC_RD_CARD_IN[3];
u8 DC_RD_CARD_OK[3];
u8 DC_RD_CARD_FAIL[3];
u8 DC_TRX_OK[2];
u8 DC_WTRX_OK[2];
u8 DC_MAXOVER[3];
u16 DispSubItemBias[7]; //
#else
{	_DISP_DUKA_, _DISP_ZHONG_, _DISP_ZHONG_},
{	_DISP_DUKA_, _DISP_CHENGGONG_, _DISP_CHENGGONG_},
{	_DISP_DUKA_, _DISP_SHIBAI_, _DISP_SHIBAI_},
{	_DISP_TRX_, _DISP_TRX_},
{	_DISP_PULSE_, _DISP_PULSE_},
{	_BLINK_BIT_, _DISP_TUNJI_, _DISP_TUNJI_},
{
	_FSET_BIAS_(DC_RD_CARD_IN), //(CARD_IN)
	_FSET_BIAS_(DC_RD_CARD_OK), //(CARD_OK)
	_FSET_BIAS_(DC_RD_CARD_FAIL), //(CARD_FAIL)
	_FSET_BIAS_(DC_TRX_OK), //(TRX_OK)
	_FSET_BIAS_(DC_WTRX_OK), //(WTRX_OK)
	_FSET_BIAS_(DC_MAXOVER), //(MAX_OVER)
	_FSET_BIAS_(DC_RD_CARD_OK)+1, //(CHARGE_OK)
},
#endif

#ifndef _FILE_SET_DATA_
u8 D_DianLiang[3];
#else
{	_DISP_DIAN_, _DISP_LIANG_, _DISP_LIANG_},
#endif

#ifndef _FILE_SET_DATA_
strMvData mvTmRecord[1];
#else
{
	NORM_COPY(&vRamData.RTCTime, _RET_|4),
},
#endif
 
#ifndef _FILE_SET_DATA_
struct {
	u8 Bias;
	u8 Lth;
} T_Time_Lth[4];
#else
{
	{	0, 6}, //T_YMDHMS
	{	3, 3}, //T_HMS
	{	0, 5}, //T_YMDHM
	{	0, 4}, //T_YMDW
},
#endif

#ifndef _FILE_SET_DATA_
u8 Const5FF[5];
u8 Const699[6];
u8 Const6665[6];
u8 ZeroP[10];
u8 X80;
#else
{	0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{	0x99, 0x99, 0x99, 0x99, 0x99, 0x99},
{	0x61, 0x62, 0x63, 0x64, 0x65, 0x66},
{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
0x80,
#endif

#ifndef	_FILE_SET_DATA_
u8 DispDigitSegs1[3][7];
#else
{
	{0},// {	_DISP_10_A_, _DISP_10_B_, _DISP_10_C_, _DISP_10_D_, _DISP_10_E_, _DISP_10_F_, _DISP_10_G_},
	{0},//{	_DISP_9_A_, _DISP_9_B_, _DISP_9_C_, _DISP_9_D_, _DISP_9_E_, _DISP_9_F_, _DISP_9_G_}, //D6
	{0},//{	_DISP_8_A_, _DISP_8_B_, _DISP_8_C_, _DISP_8_D_, _DISP_8_E_, _DISP_8_F_, _DISP_8_G_}, //D6
	
	 //D7
	
	
},
#endif

#ifndef	_FILE_SET_DATA_
u8 DispDigitSegs[8][7];
#else
{
	{	_DISP_8_A_, _DISP_8_B_, _DISP_8_C_, _DISP_8_D_, _DISP_8_E_, _DISP_8_F_, _DISP_8_G_}, //D7
	{	_DISP_7_A_, _DISP_7_B_, _DISP_7_C_, _DISP_7_D_, _DISP_7_E_, _DISP_7_F_, _DISP_7_G_}, //D7
	{	_DISP_6_A_, _DISP_6_B_, _DISP_6_C_, _DISP_6_D_, _DISP_6_E_, _DISP_6_F_, _DISP_6_G_}, //D6
	{	_DISP_5_A_, _DISP_5_B_, _DISP_5_C_, _DISP_5_D_, _DISP_5_E_, _DISP_5_F_, _DISP_5_G_}, //D7
	{	_DISP_4_A_, _DISP_4_B_, _DISP_4_C_, _DISP_4_D_, _DISP_4_E_, _DISP_4_F_, _DISP_4_G_}, //D6
	{	_DISP_3_A_, _DISP_3_B_, _DISP_3_C_, _DISP_3_D_, _DISP_3_E_, _DISP_3_F_, _DISP_3_G_}, //D7
	{	_DISP_2_A_, _DISP_2_B_, _DISP_2_C_, _DISP_2_D_, _DISP_2_E_, _DISP_2_F_, _DISP_2_G_}, //D6
	{	_DISP_1_A_, _DISP_1_B_, _DISP_1_C_, _DISP_1_D_, _DISP_1_E_, _DISP_1_F_, _DISP_1_G_}, //D7
	
},
#endif
 
#ifndef	_FILE_SET_DATA_
strFrozeSet FrozeSet[_FROZE_NUM_]; //冻结设置
#else
{
	//{	_FRZ_BIAS_(FzFuhe), 0, _FRZ_MINCNT_(FzFuhe), 0,}, //_FROZE_FUHE_				//负荷记录(时间4/电压2/电流4/频率2)
//	{	_FRZ_BIAS_(FKData), _FRZ_LTH_(FKData), _FRZ_MINCNT_(FKData), 0,}, //_FROZE_KEYDATA_           //修改密钥密文
	{	_FRZ_BIAS_(FMouth), _FRZ_LTH_(FMouth), _FRZ_MINCNT_(FMouth), 0,}, //_FROZE_MONTH_             //结算日数据
//	{	_FRZ_BIAS_(FMStep), _FRZ_LTH_(FMStep), _FRZ_MINCNT_(FMStep), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_MSTEP_
//	{	_FRZ_BIAS_(FYStep), _FRZ_LTH_(FYStep), _FRZ_MINCNT_(FYStep), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_YSTEP_
//	{	_FRZ_BIAS_(FYTabl), _FRZ_LTH_(FYTabl), _FRZ_MINCNT_(FYTabl), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_YTBL_CHG_
//	{	_FRZ_BIAS_(FTmTbl), _FRZ_LTH_(FTmTbl), _FRZ_MINCNT_(FTmTbl), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_TMTBL_CHG_
#ifdef		_USE_CARD_
	{	_FRZ_BIAS_(FPrice), _FRZ_LTH_(FPrice), _FRZ_MINCNT_(FPrice), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_FL_PRICE_CHG_      //费率电价切换记录
	{	_FRZ_BIAS_(FLStep), _FRZ_LTH_(FLStep), _FRZ_MINCNT_(FLStep), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_STEP_PRICE_CHG_    //阶梯表切换记录
	{	_FRZ_BIAS_(FCharg), _FRZ_LTH_(FCharg), _FRZ_MINCNT_(FCharg), 0}, //_FROZE_CHARGE_            //购电记录
	{	_FRZ_BIAS_(FDsChg), _FRZ_LTH_(FDsChg), _FRZ_MINCNT_(FDsChg), 0,}, //_FROZE_OFF_MONEY_         //退费记录
	{	_FRZ_BIAS_(FICErr), _FRZ_LTH_(FICErr), _FRZ_MINCNT_(FICErr), _FSET_BIAS_(mvICErr[0])}, //_FROZE_IC_ERR_            //异常插卡记录
	{	_FRZ_BIAS_(FPSet1), _FRZ_LTH_(FPSet1), _FRZ_MINCNT_(FPSet1), 0,}, //_FROZE_TPRICE_SET_        //费率电价修改记录
	{	_FRZ_BIAS_(FLPSet), _FRZ_LTH_(FLPSet), _FRZ_MINCNT_(FLPSet), 0,}, //_FROZE_LPRICE_SET_        //阶梯表修改记录
#endif
	{	_FRZ_BIAS_(FFzAct), _FRZ_LTH_(FFzAct), _FRZ_MINCNT_(FFzAct), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_ACT_
	{	_FRZ_BIAS_(FFTime), _FRZ_LTH_(FFTime), _FRZ_MINCNT_(FFTime), _FSET_BIAS_(mvOderFroze[0]),}, //_FROZE_TM_
	{	_FRZ_BIAS_(FFzDay), _FRZ_LTH_(FFzDay), _FRZ_MINCNT_(FFzDay), _FSET_BIAS_(mvDayFroze[0]),}, //_FROZE_DAY_
	{	_FRZ_BIAS_(FFHour), _FRZ_LTH_(FFHour), _FRZ_MINCNT_(FFHour), _FSET_BIAS_(mvHourFroze[0])}, //_FROZE_HOUR_
//	{	_FRZ_BIAS_(FKyChg), _FRZ_LTH_(FKyChg), _FRZ_MINCNT_(FKyChg), _FSET_BIAS_(mvKyChgFroze[0]),}, //_FROZE_KEYCHG_
//	{	_FRZ_BIAS_(FEOpn1), _FRZ_LTH_(FEOpn1), _FRZ_MINCNT_(FEOpn1), _FSET_BIAS_(mvHourFroze[0]),}, //_FROZE_EK_1_,
//	{	_FRZ_BIAS_(FJDOff), _FRZ_LTH_(FJDOff), _FRZ_MINCNT_(FJDOff), _FSET_BIAS_(mvWriterRecordFar[0]),},//_FROZE_JD_,
//	{	_FRZ_BIAS_(FFProg), _FRZ_LTH_(FFProg), _FRZ_MINCNT_(FFProg), 0,}, //_FROZE_PROG_,
	{	_FRZ_BIAS_(FClear), _FRZ_LTH_(FClear), _FRZ_MINCNT_(FClear), _FSET_BIAS_(mvWriterRecord[0]),}, //_FROZE_CLEAR_,
//	{	_FRZ_BIAS_(FEvClr), _FRZ_LTH_(FEvClr), _FRZ_MINCNT_(FEvClr), 0,}, //_FROZE_EVENT_CLEAR_,
	{	_FRZ_BIAS_(FChgTm), _FRZ_LTH_(FChgTm), _FRZ_MINCNT_(FChgTm), 0,}, //_FROZE_CHG_TM_,
//	{	_FRZ_BIAS_(FTTbCg), _FRZ_LTH_(FTTbCg), _FRZ_MINCNT_(FTTbCg), 0,}, //_FROZE_TMTBL_SET_,
//	{	_FRZ_BIAS_(FYTbCg), _FRZ_LTH_(FYTbCg), _FRZ_MINCNT_(FYTbCg), 0,}, //_FROZE_YTBL_SET_,
//	{	_FRZ_BIAS_(FWkSet), _FRZ_LTH_(FWkSet), _FRZ_MINCNT_(FWkSet), 0}, //_FROZE_WEEK_SET_,
//	{	_FRZ_BIAS_(FHldSt), _FRZ_LTH_(FHldSt), _FRZ_MINCNT_(FHldSt), 0,}, //_FROZE_HLD_SET_,
//	{	_FRZ_BIAS_(FLSSet), _FRZ_LTH_(FLSSet), _FRZ_MINCNT_(FLSSet), 0,}, //_FROZE_LSTEP_SET_,
//	{	_FRZ_BIAS_(FCmSet), _FRZ_LTH_(FCmSet), _FRZ_MINCNT_(FCmSet), 0,}, //_FROZE_COM_SET_,
//	{	_FRZ_BIAS_(FFrzDT), _FRZ_LTH_(FFrzDT), _FRZ_MINCNT_(FFrzDT), 0,}, //_FROZE_FDT_SET_,
//	{	_FRZ_BIAS_(FOvAmp), _FRZ_LTH_(FOvAmp), _FRZ_MINCNT_(FOvAmp), _FSET_BIAS_(mvOvAmpFroze),}, //_FROZE_OVERIM_,		    //过流记录
//	{	_FRZ_BIAS_(FPrErr), _FRZ_LTH_(FPrErr), _FRZ_MINCNT_(FPrErr), _FSET_BIAS_(mvHourFroze[0])}, //_FROZE_PERR_,             //电源异常记录
//	{	_FRZ_BIAS_(FJDErr), _FRZ_LTH_(FJDErr), _FRZ_MINCNT_(FJDErr), _FSET_BIAS_(mvFHErrFroze[0]),}, //_FROZE_JDERR_,            //负荷开关异常记录
	{	_FRZ_BIAS_(FPwOff), _FRZ_LTH_(FPwOff), _FRZ_MINCNT_(FPwOff), _FSET_BIAS_(mvTmRecord[0]),}, //_FROZE_PWR_,              //掉电记录
	{	_FRZ_BIAS_(FNUse1), _FRZ_LTH_(FNUse1), _FRZ_MINCNT_(FNUse1), 0,}, //_FROZE_N_1_,              //保留
},
#endif

#ifndef _FILE_SET_DATA_
strMvData mvICErr[9];
#else
{
	NORM_COPY(&vRamData.RTCTime, 4),
	NORM_COPY(&vRamData.IC_Num, 8),
	NORM_COPY(&vRamData.ICECode, 1),
	NORM_COPY(&vRamData.ICECmd, 5),
	REV_COPY(&vRamData.Return_7816, 2),
	NORM_COPY(((u8*)&vFlashInfo.SafeInfo.ChargeCnt), 2),
	NORM_COPY(&vRamData.u_Remain, 4),
	READ_USEDATA( TYPE_ACT_USE, 4), //正向有功
	READ_USEDATA( TYPE_REV_ACT_USE, _RET_|4), //反向有功
},
#endif

#ifndef _FILE_SET_DATA_
strMvData mvDayFroze[2];
strMvData mvOderFroze[4 + (_MAX_TARRIF_ * 2)];
#else
{
	NORM_COPY( ((u8*)&vFlashInfo.SafeInfo.ChargeCnt), 2),
	NORM_COPY( &vRamData.s_Remain, 4),
},
{
	NORM_COPY( &vRamData.FrozeTime, 4), //记录当前时间
	NORM_COPY( (u8*)&vRamData.Pph.sVI, 4), //总有功功率
	READ_USEDATA( TYPE_ACT_USE, 4), //正向有功
	READ_USEDATA( TYPE_REV_ACT_USE, 4), //反向有功
	READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_1, 4), //费率电能
	READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_1, 4), //反向费率电能
	READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_2, 4),
	READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_2, 4), //费率电能
	READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_3, 4),
	READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_3, 4),
	READ_USEDATA( TYPE_ACT_USE|TYPE_FEILV_4, 4), //费率电能
	READ_USEDATA( TYPE_REV_ACT_USE|TYPE_FEILV_4, _RET_|4),
},
#endif

#ifndef	_FILE_SET_DATA_
strMvData mvHourFroze[3];
#else
{
	READ_RAM( 0, READ_BCD_TIME( S_RTC, T_SEC)),
	READ_USEDATA( TYPE_ACT_USE, 4),
	READ_USEDATA( TYPE_REV_ACT_USE, _RET_|4),
},
#endif

#ifndef	_FILE_SET_DATA_
strMvData mvOvAmpFroze[7];
#else
{
	NORM_COPY( &vRamData.RTCTime, 4), //记录当前时间
	READ_USEDATA( TYPE_ACT_USE, 4), //正向有功
	READ_USEDATA( TYPE_REV_ACT_USE, 4), //反向有功
	NORM_COPY(((u8*)&vRamData.Vph.sVI), 2),
	NORM_COPY(((u8*)&vRamData.Iph.sVI), 3),
	NORM_COPY(((u8*)&vRamData.Pph.sVI), 3), //总有功功率
	NORM_COPY((u8*)&vRamData.Pfph, _RET_|2),
},
#endif

#ifndef _FILE_SET_DATA_
strMvData mvKyChgFroze[4];
#else
{
	READ_RAM( &vRamData.RTCTime, READ_BCD_TIME( S_RTC, T_SEC)),
	NORM_COPY( &vFlashInfo.RunInfo.LastJDWriter, 4),
	NORM_COPY( &vFlashInfo.SetInfo.KeyNum, 1),
	REV_COPY( vRamData.IC_Num, _RET_|4),
},
#endif

#ifndef	_FILE_SET_DATA_
strMvData mvWriterRecord[4];
#else
{
	READ_RAM( 0, READ_BCD_TIME( S_RTC, T_SEC)),
	NORM_COPY( &vRamData.Write, 4),
	READ_USEDATA( TYPE_ACT_USE, 4), //正向有功
	READ_USEDATA( TYPE_REV_ACT_USE, _RET_|4), //反向有功
},
#endif

#ifndef	_FILE_SET_DATA_
strMvData mvWriterRecordFar[4];
#else
{
	READ_RAM( 0, READ_BCD_TIME( S_RTC, T_SEC)),
	NORM_COPY( &vFlashInfo.RunInfo.LastJDWriter, 4),
	READ_USEDATA( TYPE_ACT_USE, 4),
	READ_USEDATA( TYPE_REV_ACT_USE, _RET_|4),
},
#endif

#ifndef	_FILE_SET_DATA_
strMvData mvFHErrFroze[3];
#else
{
	NORM_COPY( &vRamData.FrozeTime, 4),
	READ_USEDATA( TYPE_ACT_USE, 4),
	READ_USEDATA( TYPE_REV_ACT_USE, _RET_|4),
},
#endif
 
#undef	_FILE_SET_

