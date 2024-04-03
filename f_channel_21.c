/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_21 (void)
{

  f_channel[21].value = 0;
  f_channel[21].tag = "(none)";
  f_channel[21].unit = "(none)";
  f_channel[21].status = 1;
  f_channel[21].comment = "(none)";

  return 1;
}
