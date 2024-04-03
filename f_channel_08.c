/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_08 (void)
{

  f_channel[8].value = ain[8];
  f_channel[8].tag = "(none)";
  f_channel[8].unit = "Volts";
  f_channel[8].status = 1;
  f_channel[8].comment = "Analog input AIN8";

  return 1;
}
