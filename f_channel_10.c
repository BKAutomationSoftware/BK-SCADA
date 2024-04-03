/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_10 (void)
{

  f_channel[10].value = ain[10];
  f_channel[10].tag = "(none)";
  f_channel[10].unit = "Volts";
  f_channel[10].status = 1;
  f_channel[10].comment = "Analog input AIN10";

  return 1;
}
