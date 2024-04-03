/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_09 (void)
{

  f_channel[9].value = ain[9];
  f_channel[9].tag = "(none)";
  f_channel[9].unit = "Volts";
  f_channel[9].status = 1;
  f_channel[9].comment = "Analog input AIN9";

  return 1;
}
