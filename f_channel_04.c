/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_04 (void)
{

  f_channel[4].value = ain[4];
  f_channel[4].tag = "(none)";
  f_channel[4].unit = "Volts";
  f_channel[4].status = 1;
  f_channel[4].comment = "Analog input AIN4";

  return 1;
}
