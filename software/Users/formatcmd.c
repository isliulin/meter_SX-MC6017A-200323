#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "streamio.h"
#include "bit_edge.h"
#include "emu.h"
#include "pubset.h"
 
#include "flash_filesystem.h"
 
 char *sincmd[10] ;		//  ÓÃÓÚÃèÊöÃüÁîÂë
 u8 cargc ; 

#define	 isascii(a)   (( isalpha(a) || isdigit(a) ||( a== '[') || ( a==']')|| ( a==',') || ( a== '=') || ( a=='(') || ( a== ')')))

char * search_first( char *s , u8 clen )
{
	u8 i ; 
	u8 j = clen ; 

	for ( i = 0 ; ((i <j)  &&  !isascii(*s)  )  ; i++,s++);

	if ( i >=j  ) 	return NULL;
	if ( !isascii(*s)  )  return NULL;
	return s ; 
}

char 	*search_end ( char *s , u8 clen )
{
	u8 i ;u8 j = clen  ; 

	for ( i = 0 ; ((i <j)  && isascii(*s) ) ; i++,s++);

	if ( i >j  ) 	return NULL;
	if (  isascii(*s) )  return NULL;
	return s ; 
}
		

 
u8   format_cmd ( char  *ssrc,  u8 clen   ) 
{
	u8 i ,j;
	char *s = ssrc;
	char 	*send = s  + clen  ;
	memset ( ( u8 *)sincmd , 0 ,clen) ; 
 
 
for ( i = 0 ; i < clen ; i++)
{
		if  ( isupper(ssrc[i] ))
				ssrc[i] = tolower( ssrc[i]);
		 if ( !isascii(ssrc[i]))
				ssrc[i] =  0;
}

for ( cargc  = 0 , s = ssrc ; (( cargc <  sizeof ( sincmd ) / sizeof ( char *)) && ( s <= send  ))  ; cargc++ )
{
	 
		s  = search_first(  s , send - s );
		if ( s )
				 sincmd[cargc] = s ;
    else				
			break;
		s = search_end ( s , send  - s );	 
}			
		  
}

