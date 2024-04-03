/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_06 (void)
{

  f_channel[6].value = ain[6];
  f_channel[6].tag = "(none)";
  f_channel[6].unit = "Volts";
  f_channel[6].status = 1;
  f_channel[6].comment = "Analog input AIN6";

  return 1;
}
