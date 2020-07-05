
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "ht6xxx_lib.h"
#include "aes_example.h"
#include "formatstring.h"
#include "hal_delay_us.h"


void
HAL_Delay_us (unsigned long  delay_us1)
{
  uint32_t first_value = 0;
  uint32_t current_value = 0;
  uint32_t reload = SysTick->LOAD;
	unsigned long  delay_us = delay_us1 /5 ;

  uint32_t nus_number = delay_us * ((reload + 1) / 1000);
  uint32_t change_number = 0;

  first_value = SysTick->VAL;
  while (1)
    {
      current_value = SysTick->VAL;
      if (current_value != first_value)
	{

	  if (current_value < first_value)
	    {
	      change_number += first_value - current_value;
	      //change_number = first_value - current_value + change_number;  
	    }

	  else
	    {
	      change_number += reload - current_value + first_value;
	    }
	  first_value = current_value;
	  if (change_number >= nus_number)
	    {
	      break;
	    }
	}
    }
}
