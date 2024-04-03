/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_13 (void)
{

  f_channel[13].value = ain[13];
  f_channel[13].tag = "(none)";
  f_channel[13].unit = "Volts";
  f_channel[13].status = 1;
  f_channel[13].comment = "Analog input AIN13";

  return 1;
}
