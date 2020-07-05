#include "pubset.h"
#include "safedate.h"
#include <string.h>

struct stSafeDataBlock
{
	u8 *p;
	u16 cs;
	u8 log;

};
 // pData[0],pData[1]数据相同，pData[2]为0xff-pData[0]
struct stSAFEDATEIO
{
  struct stSafeDataBlock pData[3];
	u8 cLen;  // 用于指示stSafeDataBlock中P数据的长度
	u16 cs;
 	u8 cUp;
};

struct stSAFEDATEIO stsafedateio;
const unsigned char cstSAFEDATEIO_SumLen = (int )&(((struct stSAFEDATEIO  *)0)->cs)-1;

u16 iGetsum ( u8 *p , u16 len )
{
	u16 itemp ;
	u16 i;

  for (i = 0, itemp = 0; i < len ;   i++, p++)

		itemp += *p;
   return  itemp;

}

unsigned char cvote(unsigned char s1,unsigned char s2 ,unsigned char s3,char *des)
{
		unsigned char j,k,m;
		j = s1- s2;
		k = s1 - s3 ;
		m = s2-  s3;

   if ((j != 0) && (k != 0) && ( m !=0))
	return _system_Errmem;

    if ( 0 == j)
				*des = s1;
		else
			  if ( 0 == k )
						*des = s2 ;
				else
						if ( 0 == m )
							*des = s3;
		return  _system_success;

}

unsigned char iVote(unsigned short s1,unsigned short s2 ,unsigned short s3,char *des)
{
		unsigned short j,k,m,itemp;
		j = s1- s2;
		k = s1 - s3 ;
		m = s2-  s3;

   if ((j != 0) && (k != 0) && ( m !=0))
	return _system_Errmem;

    if ( 0 == j)
				itemp = s1;
		else
			  if ( 0 == k )
						itemp = s2 ;
				else
						if ( 0 == m )
							itemp = s3;

		*des =*(( unsigned char *)&itemp);
		*des =*(( unsigned char *)&itemp+1);
		return  _system_success;

}

//	检查数据,出错后并且恢复
u8
cSafeData_checkdata (void)
{
	unsigned char cError = 0;
  u8 i, j, k,m ;
  u16 itemp;
  u8 *p = (u8 *) & stsafedateio;
  u8 *datap = stsafedateio.pData[0].p;
  u8 *bak1p = stsafedateio.pData[1].p;
  u8 *bak2p = stsafedateio.pData[2].p;

	if (( stsafedateio.pData[0].log != stsafedateio.pData[1].log ) || (stsafedateio.pData[0].log !=0xff - stsafedateio.pData[2].log ))
			cError  |= _system_ErrLog ;

  itemp  =  iGetsum  ( (u8 *) & stsafedateio , cstSAFEDATEIO_SumLen) ;
  if (itemp != stsafedateio.cs)
   cError  |= _system_Errmem;
	if ( _cLog != stsafedateio.pData[0].log  )
    cError  |= _system_ErrDate;
	itemp = iGetsum(  stsafedateio.pData[0].p , stsafedateio.cLen   );
	if (itemp != stsafedateio.pData[0].cs)
    cError  |=_system_ErrDate;

	  for (i = 0; i < stsafedateio.cLen; i++, datap++, bak1p++, bak2p++)
    {
			  j = *datap - *bak1p;
				k = 0xff -   *bak2p;
				k = *datap - k ;
				m = *bak1p-  k;
      if ((j != 0) && (k != 0) && ( m !=0))
			{	cError  |= _system_Errmem; break;}
		}
  return cError;
}



u8
cSafeData_restoreData (void)
{
  u8 i, j, k,m ;
  u8 *datap = stsafedateio.pData[0].p;
  u8 *bak1p = stsafedateio.pData[1].p;
  u8 *bak2p = stsafedateio.pData[2].p;
	unsigned int 	itemp;
	unsigned char ctemp;

	 i = iVote( stsafedateio.pData[0].cs ,stsafedateio.pData[1].cs ,0xffff- stsafedateio.pData[2].cs,(unsigned char *)&itemp);
	 Do_Task (  _system_success != i  , return _system_Errmem);

  for (i = 0; i < stsafedateio.cLen; i++, datap++, bak1p++, bak2p++)
    {  //
      j = *datap - *bak1p;
      k = 0xff -   *bak2p;
      k = *datap - k ;
			m = *bak1p-  k;
      if ((j != 0) && (k != 0) && ( m !=0))
	return _system_Errmem;

      if (j | k )		//        出错
		{
	  if (j ==0 )
	    {
	      *bak2p = 0xff - *datap;
	    }
	  else if (k == 0)
	    {
	      *bak1p = *datap;
	    }
	  else
	    {
	      *datap = *bak1p;
	    }
	}
    }

	stsafedateio.pData[0].log= _cLog;
	stsafedateio.pData[0].cs = iGetsum(  stsafedateio.pData[0].p , stsafedateio.cLen );

	stsafedateio.pData[1].log = stsafedateio.pData[0].log ;
	stsafedateio.pData[2].log =0xff - stsafedateio.pData[0].log ;

	stsafedateio.pData[1].cs = stsafedateio.pData[0].cs ;
	stsafedateio.pData[2].cs=0xffff - stsafedateio.pData[0].cs ;

	Do_Task ( itemp != 	stsafedateio.pData[0].cs  , return _system_Errmem);

	return  _system_success;

}

unsigned char
cSafeData_Data_Refreah(void)
{
	const unsigned char cRetryno =2;
	unsigned char i,cError ;
	i = 0;
	do {
		cError = cSafeData_checkdata () ;
		Do_Task (  _system_success == cError , return  cError );
		cError = cSafeData_restoreData ( );i++;
	} while (i++ <cRetryno)  ;
			if ( i>=cRetryno)
						return  _system_Errmem ;

}

void
nSafeData_update2bak (void)
{
  u8 i;
  u8 *p = stsafedateio.pData[0].p;
  u8 *datap = stsafedateio.pData[0].p;
  u8 *bak1p = stsafedateio.pData[1].p;
  u8 *bak2p = stsafedateio.pData[2].p;
  u16 itemp;

  if (_cUpdate == stsafedateio.cUp)
    {

		   stsafedateio.pData[0].cs = iGetsum  ( stsafedateio.pData[0].p  ,stsafedateio.cLen  ) ;
       for (i = 0; i < stsafedateio.cLen; i++, datap++, bak1p++, bak2p++)
			{
					*bak1p = *datap;
					*bak2p = 0xff - *datap;
			}
				stsafedateio.pData[1].cs = stsafedateio.pData[0].cs ;
			stsafedateio.pData[2].cs=0xffff - stsafedateio.pData[0].cs ;

    }
		  stsafedateio.cUp = 0;
			  stsafedateio.cs =  iGetsum  ( (u8 *) & stsafedateio ,cstSAFEDATEIO_SumLen) ;
}


void
nSafeData_set_upb (void)	//       更新状态标志
{
  stsafedateio.cUp = _cUpdate;
}

void
nSafeData_res_upb (void)	// 清除状态标志
{
  stsafedateio.cUp = 0;
}


void
nSafeData_struct_Init (u8 * datap, u8 * bak1p, u8 * bak2p, u8 clen)	// 初始化
{
	//memset((u8 *) & stsafedateio, 0,sizeof( stsafedateio));
	ClrRam((u8 *) & stsafedateio, sizeof( stsafedateio));
  stsafedateio.cLen = clen;
  stsafedateio.pData[0].p = datap;
  stsafedateio.pData[1].p = bak1p;
  stsafedateio.pData[2].p = bak2p;

	stsafedateio.pData[0].log= _cLog;
	stsafedateio.pData[0].cs = iGetsum(  stsafedateio.pData[0].p , stsafedateio.cLen );

	stsafedateio.pData[1].log = stsafedateio.pData[0].log ;
	stsafedateio.pData[2].log =0xff - stsafedateio.pData[0].log ;

	stsafedateio.pData[1].cs = stsafedateio.pData[0].cs ;
	stsafedateio.pData[2].cs=0xffff - stsafedateio.pData[0].cs ;

  //stsafedateio.cs =  iGetsum  ( (u8 *) & stsafedateio ,sizeof (struct stSAFEDATEIO) - sizeof (u16) - 1) ;
	stsafedateio.cs =  iGetsum  ( (u8 *) & stsafedateio ,cstSAFEDATEIO_SumLen) ;

}
