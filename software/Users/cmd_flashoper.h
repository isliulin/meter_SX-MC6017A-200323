#ifndef _cmd_flashoper_hpp
#define _cmd_flashoper_hpp

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


char cmd_rflash (char *s);
char cmd_wflash(char *s);
unsigned char   flash_wrio (  unsigned long   laddr ,  unsigned char cvalue); 
unsigned char   intereeprom_wrio (  unsigned long   laddr ,  unsigned char cvalue);

#endif
