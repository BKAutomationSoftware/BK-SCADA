/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_01 (void)
{

  f_channel[1].value = ain[1];
  f_channel[1].tag = "(none)";
  f_channel[1].unit = "Volts";
  f_channel[1].status = 1;
  f_channel[1].comment = "Analog input AIN1";

  return 1;
}
