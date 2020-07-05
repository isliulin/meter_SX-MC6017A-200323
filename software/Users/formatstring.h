#ifndef _FORMATSTRING_HPP_
#define _FORMATSTRING_HPP_

struct STFORMATOUT
{
  char bRoper:1;				//       �����־
  char bFirstzero:1;		//      ��λ����Ϊ0     
	char bRight:1;				//			�Ҷ���
  unsigned int ilen;		//      ��ǰ����
  char *cpGlobal;		
	unsigned int ibuflen;  // ����������
  char *cpMsgp;		
	char	 *csDevice ; 		// 	�豸����
	char	  cDeviceNo; 		//	�豸��
	

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
