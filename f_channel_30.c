/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_30 (void)
{

  f_channel[30].value = 0;
  f_channel[30].tag = "(none)";
  f_channel[30].unit = "(none)";
  f_channel[30].status = 1;
  f_channel[30].comment = "(none)";

  return 1;
}
