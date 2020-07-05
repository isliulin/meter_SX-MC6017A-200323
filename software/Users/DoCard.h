#ifndef	_DOCARD_H_
#define	_DOCARD_H_

#define		SAFE_INFO_ID	0x82				//参数信息文件(长度0x28)
#define		FL_TBL_1_ID		0x83				//第一套费率文件(长度0x102)
#define		FL_TBL_2_ID		0x84				//第二套费率文件(长度0x102)
#define		KEY_INFO_1_ID	0x85				//密钥信息文件1(长度0x04)
#define		KEY_INFO_2_ID	0x86				//密钥信息文件2(长度0x04)

#define		ESAM_WBACK_ID	0x87				//运行信息文件(长度0x66)
#define		CTRL_1_ID		0x88				//控制命令文件(长度????)
#define 	DECODE_1_ID		0x89				//参数更新文件1
#define 	DECODE_2_ID		0x90				//参数更新文件1
#define 	DECODE_3_ID		0x91				//参数更新文件1
#define 	DECODE_4_ID		0x92				//参数更新文件1
#define 	DECODE_5_ID		0x93				//参数更新文件1

#define		CMD_USERCARD	0x01				//用户卡文件
#define		CMD_KEYCARD		0x02				//修改密钥卡


typedef	struct {								//保护参数(ESAM02文件)
	u8		Start;								//起始码
	u8		Cmd;								//命令码(预置卡06, 用户卡01
	u8		Length[2];							//长度
	u8		UserType;							//用户类型(01-单费率,02-分时费率,03-阶梯电价,04-分时阶梯电价
	u8		Chg;								//参数修改标志(用户卡使用)
	u8		TmTblVer[4];						//现场参数卡版本号
	u8		StartTm[5];							//分时费率切换时间
	u8		FLNum;								//费率数
	u8		Alm1[4];							//报警电量1
	u8		Alm2[4];							//报警电量2
	u8		I_Mul[3];							//电流互感器变比
	u8		V_Mul[3];							//电压互感器变比
	u8		E_Num[6];							//表号
	u8		H_Num[6];							//客户编号
	u8		CardType;							//电卡类型
	u8		ID_Tm[2];							//身份认证时效
	u8		Sum;
	u8		End;
} strESAMInfo;

#define  		 _strESAMInfo_Len_		((int )&(((strESAMInfo *)0)->End)+(sizeof(((strESAMInfo *)0)->End)/sizeof(char)))

typedef struct {								//费率表(ESAM03文件)
	u8	Start;									//起始码
	u8	Cmd;
	u8	Length[2];								//长度
	u8	Price[32][4];							//电价表
	u8	LStep[6][4];							//阶梯电量表
	u8	LPrice[7][4];							//阶梯电价表
	u8	YStep[4][3];							//年结算日
	u8	StartTm_4[5];							//阶梯电价切换时间
	u8	Sum;
	u8	End;
} strESAMTmTbl;

typedef struct	{								//用户卡返写文件(ESAM06文件)
	u8	Start;									//0x68
	u8	Cmd;									//0x11
	u8	Length[2];								//长度
	u8	UserType;								//用户类型(01-单费率,02-分时费率,03-阶梯电价,04-分时阶梯电价
	u8	I_Mul[3];								//电流互感器变比
	u8	V_Mul[3];								//电压互感器变比
	u8	E_Num[6];								//表号
	u8	H_Num[6];								//客户编号
	u8	Remain[4];								//剩余电量
	u8	ChargeCnt[4];							//购电次数
	u8	OverDraft[4];							//透支金额(电量)
	u8	KeyInfo[4];								//密钥信息
	u8	ErrCardCnt[3];							//非法插卡次数
	u8	WriteTime[5];							//返写日期
	u8	Sum;
	u8	End;
} strUserBack;

		/***********	用户卡		**********/
typedef	struct {								//保护参数(ESAM02文件)
	u8	Start;									//起始码
	u8	Cmd;									//命令码(预置卡06, 用户卡01
	u8	Length[2];								//长度
	u8	UserType;								//用户类型(01-单费率,02-分时费率,03-阶梯电价,04-分时阶梯电价
	u8	Chg;									//参数修改标志(用户卡使用)
	u8	TmTblVer[4];							//现场参数卡版本号
	u8	StartTm[5];								//分时费率切换时间
	u8	FLNum;									//费率数
	u8	Alm1[4];								//报警电量1
	u8	Alm2[4];								//报警电量2
	u8	I_Mul[3];								//电流互感器变比
	u8	V_Mul[3];								//电压互感器变比
	u8	E_Num[6];								//表号
	u8	H_Num[6];								//客户编号
	u8	CardType;								//电卡类型
	u8	Sum;
	u8	End;
} strUserCard;

		/***********	预置卡文件	********/
typedef struct									//预置卡数据结构
{
	u8	Start;
	u8	Cmd;									//0x06
	u8	Length[2];
	u8	UserType;								//用户类型
	u8	Chg;									//参数更新标志
	u8	TmTblVer[4];							//现场参数卡版本号
	u8	StartTm[5];								//费率电价切换时间
	u8	FLNum;									//费率数
	u8	Alm1[4];								//报警电量1
	u8	Alm2[4];								//报警电量2
	u8	I_Mul[3];								//电流互感器变比
	u8	V_Mul[3];								//电压互感器变比
	u8	Sum;
	u8	End;
} strSetupCard;

#endif			//	_DOCARD_H_

