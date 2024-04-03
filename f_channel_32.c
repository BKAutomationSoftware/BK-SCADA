
/*************************************************************************

Consult the "README.txt" file for help editing this function!

*************************************************************************/


#include <stdio.h>
#include <math.h>		// Necessary for any advanced math functions
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
f_channel_32 (void)
{

  f_channel[32].value = 0;
  f_channel[32].tag = "(PT-666)";
  f_channel[32].unit = "Inches Hg";

  if (f_channel[32].value < 3.0)
    {
      f_channel[32].status = 0;
      f_channel[32].comment = "out of range";
    }
  else
    {
      f_channel[32].status = 1;
      f_channel[32].comment = "Range Okay";
    }
  return 1;
}
