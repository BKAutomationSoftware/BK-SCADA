/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

Function: write_datatext
The purpose of this program is to write all channel data to an ASCII
text file which may be read by some other application.

*************************************************************************/


#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters
#include "cascada.h"		// Contains all the declarations specific to caSCADA


int
write_datatext (void)
{
  int n;
  FILE *fp;			// Pointer to a FILE type


  // opening the data file "data.txt" write-only
  if ((fp = fopen ("data.txt", "w")) == NULL)
    {
      fprintf (stderr, "Can't open `data.txt' text file \n");	// Do this if the file doesn't exist
      return 2;			// Return value of "2" means we cannot open the file
    }


  // Prints a "header" line to label each column of data
  fprintf (fp,
	   "F_CH # \t TAG     \t VALUE    \t UNIT  \tSTATUS \t COMMENT \n");


  // Prints 32 channels' worth of data starting with f_channel[0]
  for (n = 0; n < 32; ++n)
    {
      fprintf (fp, "%i \t %s \t %2.4f \t %s  %i   \t %s \n", n,
	       f_channel[n].tag, f_channel[n].value, f_channel[n].unit,
	       f_channel[n].status, f_channel[n].comment);
    }

  // Close the "data.txt" text file
  fclose (fp);

  return 1;

}
