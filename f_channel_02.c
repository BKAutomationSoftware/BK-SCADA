/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_02 (void)
{

  f_channel[2].value = ain[2];
  f_channel[2].tag = "(none)";
  f_channel[2].unit = "Volts";
  f_channel[2].status = 1;
  f_channel[2].comment = "Analog input AIN2";

  return 1;
}
