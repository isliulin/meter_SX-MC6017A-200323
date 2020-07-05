#ifndef _comthread_HPP
#define _comthread_HPP

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

enum STMESSAGE
{
	_Message_,
	_Message_SUCCESS_,
	_Message_ERROR_
};


struct STCMDLIST
{
  char scmd[10];
  char (*DoProg) (char *);
};

void com_shell (void);
char hex2char (char i);
extern const struct STCMDLIST stcmdlist[];
extern const char _stcmdlist_len;
void  prog_stop_dealy(unsigned char  cnum ) ;

  



unsigned char   getladdr (  char *s  , char  clen  , u32  *laddr )  ; //
unsigned char  get_value (  char *s , unsigned char *cvalue);
unsigned char   getstr (  char *s  , char  clen  , unsigned char  *sp )   ;
char hex_str ( u32 lvalue , char *s  );
unsigned char checkaddr( unsigned long  laddr )		;	
				

#endif
