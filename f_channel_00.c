/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_00 (void)
{

  f_channel[0].value = ain[0];
  f_channel[0].tag = "(none)";
  f_channel[0].unit = "Volts";
  f_channel[0].status = 1;
  f_channel[0].comment = "Analog input AIN0";

  return 1;
}
