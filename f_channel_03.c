/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_03 (void)
{

  f_channel[3].value = ain[3];
  f_channel[3].tag = "(none)";
  f_channel[3].unit = "Volts";
  f_channel[3].status = 1;
  f_channel[3].comment = "Analog input AIN3";

  return 1;
}
