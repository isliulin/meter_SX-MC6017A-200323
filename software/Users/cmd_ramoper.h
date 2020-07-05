#ifndef _cmd_ramoper_hpp
#define _cmd_ramoper_hpp

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


#define	cmd_rram    cmd_rflash   
 
char cmd_wram (char *s);

#endif
