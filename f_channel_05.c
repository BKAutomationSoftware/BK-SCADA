/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_05 (void)
{

  f_channel[5].value = ain[5];
  f_channel[5].tag = "(none)";
  f_channel[5].unit = "Volts";
  f_channel[5].status = 1;
  f_channel[5].comment = "Analog input AIN5";

  return 1;
}
