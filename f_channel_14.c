/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_14 (void)
{

  f_channel[14].value = 0;
  f_channel[14].tag = "(none)";
  f_channel[14].unit = "Volts";
  f_channel[14].status = 1;
  f_channel[14].comment = "(none)";

  return 1;
}
