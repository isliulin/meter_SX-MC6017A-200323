#ifndef __ATT7053CAL1_H__
#define __ATT7053CAL1_H__

#include "stm8l15x_it.h"
#include "PublicSet1.h"
#include "IOSet1.h"
#include "extern1.h"

//------------------------------------------------------------------------
#ifdef USE_7053

typedef struct {
	u8 cmd;
	u8 ln;
	u8 data[50];
}strRecvMsg;

#define pRecvMsg ((strRecvMsg*)(ComData))
#define pSendMsg ((strRecvMsg*)(ComData))

u8 ReceiveSpi7053(void);

u8 Read_7053(u8* Base1, u8 no);

void DoSinglePhase_7053(void);

u8 findReg7053(u8 addr);

u8 Init_7053(void);

u8 up7053ToEEP(void);

u8 Write_7053(u8* Base2, u8 no);

u8 start7053(void);

void  sendSpi7053(u8 a);

u8 rst7053(void);

u8 recv7053(void);

u8 setErr(u8 a);
u8 setln0(void);
void spiCs_0(void);

#endif

#endif