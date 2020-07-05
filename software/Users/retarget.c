/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <stdio.h>
#include <rt_misc.h>

 

#pragma import(__use_no_semihosting_swi)


/*----------------------------------------------------------------------------
  external functions
 *----------------------------------------------------------------------------*/
 int  SendChar(int ch){};
 int  GetKey(void){};

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int ch, FILE *f) 
{
	return (SendChar(ch));
}

int fgetc(FILE *f) {
  int ch;

  do {
    ch = GetKey ();
  }
  while (ch == -1);
  SendChar(ch);
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  // wait for transmit buffer empty
	SendChar (ch);
	return;
}


void _sys_exit(int return_code) {
  while (1);    /* endless loop */
}
