#ifndef	_DOCARD_H_
#define	_DOCARD_H_

#define		SAFE_INFO_ID	0x82				//������Ϣ�ļ�(����0x28)
#define		FL_TBL_1_ID		0x83				//��һ�׷����ļ�(����0x102)
#define		FL_TBL_2_ID		0x84				//�ڶ��׷����ļ�(����0x102)
#define		KEY_INFO_1_ID	0x85				//��Կ��Ϣ�ļ�1(����0x04)
#define		KEY_INFO_2_ID	0x86				//��Կ��Ϣ�ļ�2(����0x04)

#define		ESAM_WBACK_ID	0x87				//������Ϣ�ļ�(����0x66)
#define		CTRL_1_ID		0x88				//���������ļ�(����????)
#define 	DECODE_1_ID		0x89				//���������ļ�1
#define 	DECODE_2_ID		0x90				//���������ļ�1
#define 	DECODE_3_ID		0x91				//���������ļ�1
#define 	DECODE_4_ID		0x92				//���������ļ�1
#define 	DECODE_5_ID		0x93				//���������ļ�1

#define		CMD_USERCARD	0x01				//�û����ļ�
#define		CMD_KEYCARD		0x02				//�޸���Կ��


typedef	struct {								//��������(ESAM02�ļ�)
	u8		Start;								//��ʼ��
	u8		Cmd;								//������(Ԥ�ÿ�06, �û���01
	u8		Length[2];							//����
	u8		UserType;							//�û�����(01-������,02-��ʱ����,03-���ݵ��,04-��ʱ���ݵ��
	u8		Chg;								//�����޸ı�־(�û���ʹ��)
	u8		TmTblVer[4];						//�ֳ��������汾��
	u8		StartTm[5];							//��ʱ�����л�ʱ��
	u8		FLNum;								//������
	u8		Alm1[4];							//��������1
	u8		Alm2[4];							//��������2
	u8		I_Mul[3];							//�������������
	u8		V_Mul[3];							//��ѹ���������
	u8		E_Num[6];							//���
	u8		H_Num[6];							//�ͻ����
	u8		CardType;							//�翨����
	u8		ID_Tm[2];							//�����֤ʱЧ
	u8		Sum;
	u8		End;
} strESAMInfo;

#define  		 _strESAMInfo_Len_		((int )&(((strESAMInfo *)0)->End)+(sizeof(((strESAMInfo *)0)->End)/sizeof(char)))

typedef struct {								//���ʱ�(ESAM03�ļ�)
	u8	Start;									//��ʼ��
	u8	Cmd;
	u8	Length[2];								//����
	u8	Price[32][4];							//��۱�
	u8	LStep[6][4];							//���ݵ�����
	u8	LPrice[7][4];							//���ݵ�۱�
	u8	YStep[4][3];							//�������
	u8	StartTm_4[5];							//���ݵ���л�ʱ��
	u8	Sum;
	u8	End;
} strESAMTmTbl;

typedef struct	{								//�û�����д�ļ�(ESAM06�ļ�)
	u8	Start;									//0x68
	u8	Cmd;									//0x11
	u8	Length[2];								//����
	u8	UserType;								//�û�����(01-������,02-��ʱ����,03-���ݵ��,04-��ʱ���ݵ��
	u8	I_Mul[3];								//�������������
	u8	V_Mul[3];								//��ѹ���������
	u8	E_Num[6];								//���
	u8	H_Num[6];								//�ͻ����
	u8	Remain[4];								//ʣ�����
	u8	ChargeCnt[4];							//�������
	u8	OverDraft[4];							//͸֧���(����)
	u8	KeyInfo[4];								//��Կ��Ϣ
	u8	ErrCardCnt[3];							//�Ƿ��忨����
	u8	WriteTime[5];							//��д����
	u8	Sum;
	u8	End;
} strUserBack;

		/***********	�û���		**********/
typedef	struct {								//��������(ESAM02�ļ�)
	u8	Start;									//��ʼ��
	u8	Cmd;									//������(Ԥ�ÿ�06, �û���01
	u8	Length[2];								//����
	u8	UserType;								//�û�����(01-������,02-��ʱ����,03-���ݵ��,04-��ʱ���ݵ��
	u8	Chg;									//�����޸ı�־(�û���ʹ��)
	u8	TmTblVer[4];							//�ֳ��������汾��
	u8	StartTm[5];								//��ʱ�����л�ʱ��
	u8	FLNum;									//������
	u8	Alm1[4];								//��������1
	u8	Alm2[4];								//��������2
	u8	I_Mul[3];								//�������������
	u8	V_Mul[3];								//��ѹ���������
	u8	E_Num[6];								//���
	u8	H_Num[6];								//�ͻ����
	u8	CardType;								//�翨����
	u8	Sum;
	u8	End;
} strUserCard;

		/***********	Ԥ�ÿ��ļ�	********/
typedef struct									//Ԥ�ÿ����ݽṹ
{
	u8	Start;
	u8	Cmd;									//0x06
	u8	Length[2];
	u8	UserType;								//�û�����
	u8	Chg;									//�������±�־
	u8	TmTblVer[4];							//�ֳ��������汾��
	u8	StartTm[5];								//���ʵ���л�ʱ��
	u8	FLNum;									//������
	u8	Alm1[4];								//��������1
	u8	Alm2[4];								//��������2
	u8	I_Mul[3];								//�������������
	u8	V_Mul[3];								//��ѹ���������
	u8	Sum;
	u8	End;
} strSetupCard;

#endif			//	_DOCARD_H_

