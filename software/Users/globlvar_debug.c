#ifndef _GLOBAL_DEBUG_VAR_
#define _GLOBAL_DEBUG_VAR_


#include "predef.h"
#include "globlvar_debug.h"
#define _DEBUG_
#ifdef _DEBUG_
volatile enum ENUMERRNO idebugno;
volatile unsigned long lDebugIntState;
//volatile unsigned long _usart2cr3
//  __attribute__ ((section (".ARM.__at_0x40004814")));
 extern volatile 		int 		lllltemp ;
void
debug_break (enum ENUMERRNO ierrno)
{
  int i;
  if (ierrno >= _debug_app_nobreak_)
    return;
	
//	if ( lllltemp == 0)
//return;
		

		
  if (ierrno == _debug_errno_in_int_)
    idebugno = ierrno + 1;

  for (i = 10; i < idebugno; i++)
    {

    }
  return;
}
#else
void
debug_break (enum ENUMERRNO ierrno)
{
}
#endif


#ifdef _DEBUG_

 

#else

 
#endif


#endif
