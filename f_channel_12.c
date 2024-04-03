/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_12 (void)
{

  f_channel[12].value = ain[12];
  f_channel[12].tag = "(none)";
  f_channel[12].unit = "Volts";
  f_channel[12].status = 1;
  f_channel[12].comment = "Analog input AIN12";

  return 1;
}
