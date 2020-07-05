#ifndef _formatcmd_HPP
#define _formatcmd_HPP

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


char *search_first (char *s, u8);
char *search_end (char *s, u8);

u8 format_cmd (char *ssrc, u8 clen); // 	对输入参数进行分析，将其格式化成char **argc这种命令行参数方式


extern char *sincmd[10];	//  用于描述命令码
extern u8 cargc;		//	用于描述命令码的个数
// main ( int argc , char **argc)

#define 	_incmdid      sincmd[0]
#define 	_incmdtext1 		sincmd[1]
#define   _incmdtext2 		sincmd[2]
#endif
