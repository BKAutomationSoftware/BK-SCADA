/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_11 (void)
{

  f_channel[11].value = ain[11];
  f_channel[11].tag = "(none)";
  f_channel[11].unit = "Volts";
  f_channel[11].status = 1;
  f_channel[11].comment = "Analog input AIN11";

  return 1;
}
