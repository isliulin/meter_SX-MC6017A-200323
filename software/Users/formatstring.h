#ifndef _FORMATSTRING_HPP_
#define _FORMATSTRING_HPP_

struct STFORMATOUT
{
  char bRoper:1;				//       反向标志
  char bFirstzero:1;		//      首位数字为0     
	char bRight:1;				//			右对齐
  unsigned int ilen;		//      当前长度
  char *cpGlobal;		
	unsigned int ibuflen;  // 缓冲区长度
  char *cpMsgp;		
	char	 *csDevice ; 		// 	设备描述
	char	  cDeviceNo; 		//	设备号
	

};


extern struct STFORMATOUT stformatout;

#define _bRoper 				stformatout.bRoper
#define _bFirstzero			stformatout.bFirstzero
#define	_bRight			   	stformatout.bRight
#define _ilen 					stformatout.ilen
#define _cpGlobal				stformatout.cpGlobal
#define	_cpMsgp					stformatout.cpMsgp
#define	 _cDeviceNo			stformatout.cDeviceNo 
 
char vformatstring (char *, ...);

#define	_SetOperBuf(a)					stformatout.cpGlobal = ( char *)a;

#endif
