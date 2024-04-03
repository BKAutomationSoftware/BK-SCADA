/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_07 (void)
{

  f_channel[7].value = ain[7];
  f_channel[7].tag = "(none)";
  f_channel[7].unit = "Volts";
  f_channel[7].status = 1;
  f_channel[7].comment = "Analog input AIN7";

  return 1;
}
