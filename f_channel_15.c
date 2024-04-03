/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_15 (void)
{

  f_channel[15].value = 0;
  f_channel[15].tag = "(none)";
  f_channel[15].unit = "(none)";
  f_channel[15].status = 1;
  f_channel[15].comment = "(none)";

  return 1;
}
