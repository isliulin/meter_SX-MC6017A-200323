#ifndef		_PREDEF_HPP
#define		_PREDEF_HPP

 /*
typedef	unsigned char			u8;
typedef	unsigned short			u16;
typedef	unsigned long			u32;
typedef	unsigned long long		u64;

typedef	signed char				s8;
typedef	signed short			s16;
typedef	signed long				s32;
typedef	signed long	long		s64;
#define uc8  u8
#define vu8  u8
#define vu16 u16
#define vu32 u32
#define uc32 u32
*/


/*
#define   u8 	INT8U
#define		u16  INT8U
*/



typedef int (*FunPoint)(void);


struct BYTEBIT {
unsigned b0: 1;
unsigned b1: 1;
unsigned b2: 1;
unsigned b3: 1;
unsigned b4: 1;
unsigned b5: 1;
unsigned b6: 1;
unsigned b7: 1;
};


union  BYTEBITDEF{
	struct BYTEBIT bbyte;
	unsigned char  cbyte;
} ;


#define		BIT0		(1<<0)
#define		BIT1		(1<<1)
#define		BIT2		(1<<2)
#define		BIT3		(1<<3)
#define		BIT4		(1<<4)
#define		BIT5		(1<<5)
#define		BIT6		(1<<6)
#define		BIT7		(1<<7)
#define		BIT8		(1<<8)
#define		BIT9		(1<<9)
#define		BIT10		(1<<10)
#define		BIT11		(1<<11)
#define		BIT12		(1<<12)
#define		BIT13		(1<<13)
#define		BIT14		(1<<14)
#define		BIT15		((u32)1<<15)
#define		BIT16		((u32)1<<16)
#define		BIT17		((u32)1<<17)
#define		BIT18		((u32)1<<18)
#define		BIT19		((u32)1<<19)
#define		BIT20		((u32)1<<20)
#define		BIT21		((u32)1<<21)

#define		REV32(A)		((((u32)((A)&0xFF))<<24)|(((u32)((A)&0xFF00))<<8)|(((A)>>8)&0xFF00)|((A)>>24))
#define		REV16(A)		((((A)&0xFF)<<8)|(((A)&0xFF00)>>8))



#define u32p(s) (*((u32*)(s)))
#define u16p(s) (*((u16*)(s)))
#define u8p(s) (*((u8*)(s)))

#define p8(a) ((u8*)(&(a)))



//-------------------------------------

typedef struct
{
	unsigned char Bit0:1;
	unsigned char Bit1:1;
	unsigned char Bit2:1;
	unsigned char Bit3:1;
	unsigned char Bit4:1;
	unsigned char Bit5:1;
	unsigned char Bit6:1;
	unsigned char Bit7:1;
}strBit8;

typedef struct
{
	unsigned short Bit0:1;
	unsigned short Bit1:1;
	unsigned short Bit2:1;
	unsigned short Bit3:1;
	unsigned short Bit4:1;
	unsigned short Bit5:1;
	unsigned short Bit6:1;
	unsigned short Bit7:1;
	unsigned short Bit8:1;
	unsigned short Bit9:1;
	unsigned short Bit10:1;
	unsigned short Bit11:1;
	unsigned short Bit12:1;
	unsigned short Bit13:1;
	unsigned short Bit14:1;
	unsigned short Bit15:1;
}strBit16;

#define BigtoLittle16(A)   (( ((unsigned short)(A) & 0xff00) >> 8)    |  (( (unsigned short)(A) & 0x00ff) << 8))
#define BigtoLittle32(A)   ((( (u32)(A) & 0xff000000) >> 24) | (( (uint32)(A) & 0x00ff0000) >> 8)   | (( (uint32)(A) & 0x0000ff00) << 8)   | (( (uint32)(A) & 0x000000ff) << 24))

#define _iB2l(a)			BigtoLittle16(a)
//  大端宏


// 小端宏
#define 	_ih8(a)		(*((char *)&a+1))
#define 	_il8(a)		(*((char *)&a))
#define 	_lh8(a)		(*((char *)&a+3))
#define 	_ll8(a)		(*(char *)&a)
//#define  _lgetp(a)	 (lgtemp = (((unsigned long)_lh8(a))<<24 | ((unsigned long)_lh8(a-1))<<16 | ((unsigned long)_lh8(a-2))<<8 | ((unsigned long)_ll8(a))))


extern unsigned short igtemp;		//	 全局16位数,用于处理边界对齐
extern unsigned long  lgtemp;		//	 全局32位数，用于处理边界对齐


#define  _lpgetp(a)	 ( (  \
	((unsigned long)*((unsigned char *)a))<<24 |  \
	((unsigned long)*((unsigned char *)(a+1)))<<16 |  \
	((unsigned long)*((unsigned char *)(a+2)))<<8 |  \
	((unsigned long)*((unsigned char *)(a+3)))))
#define  _ipgetp(a)   (((unsigned short)*((unsigned char *)a))<<8 |((unsigned short)*((unsigned char *)(a+1))) )

#define  _lpputp(a,b)  { *(char *)(a)=*((char *)(b)+3);*(((char *)(a))+1)=*((char *)b+2);*(((char *)(a))+2)=*((char *)b+1);*(((char *)(a))+3)=*((char *)b+0);}
#define  _ipputp(a,b)  { *(char *)(a)=*((char *)(b)+1);*(char *)((a)+1)=*((char *)(b));}

#define Do_Task(a,b)		{if(a){b;}}
#define  Do_Flow(a,b,c) {if(a){b;}else{c;}}
//  小端模式下取16位数
#define	 _u16get(a)		( (unsigned short )*((unsigned char *)a) + (unsigned short )*((((unsigned char *)a) +1)) * 256 )
#define  _slpgetp(a)	 ( (  \
	((unsigned long)*((unsigned char *)a)) |  \
	((unsigned long)*((unsigned char *)(a+1)))<<8 |  \
	((unsigned long)*((unsigned char *)(a+2)))<<16 |  \
	((unsigned long)*((unsigned char *)(a+3))<<24)))
//  小端模式下取32位数
#define  _l32pgetp(a)	 ( (  \
	((unsigned long)*((unsigned char *)a))<<0 |  \
	((unsigned long)*((unsigned char *)(a+1)))<<8 |  \
	((unsigned long)*((unsigned char *)(a+2)))<<14 |  \
	((unsigned long)*((unsigned char *)(a+3)))<<24))
		
	
//用于处理对齐
#define _lEvaluate(a,b)			{*((char  *)a+0)=*((char  *)b+0);*((char  *)a+1)=*((char  *)b+1);*((char  *)a+2)=*((char  *)b+2);*((char  *)a+3)=*((char  *)b+3);}

#endif		//_PUB_SET_
