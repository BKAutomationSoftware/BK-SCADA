/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_16 (void)
{

  f_channel[16].value = 0;
  f_channel[16].tag = "(none)";
  f_channel[16].unit = "(none)";
  f_channel[16].status = 1;
  f_channel[16].comment = "(none)";

  return 1;
}
