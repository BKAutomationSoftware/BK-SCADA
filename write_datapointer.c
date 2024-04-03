/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

Function: write_datapointer
The purpose of this program is to write pointer addresses to a data file
which may be read by some other application.  Any application given this
pointer and the data structures defined in cascada.h will be able to 
access all the data stored in those structures.

NOTE: THIS FUNCTION IS NOT WORKING PROPERLY (YET).  I HAVE NOT YET FIGURED
OUT HOW TO SUCCESSFULLY SAVE AND RETRIEVE MEMORY POINTERS USING FILES!

*************************************************************************/


#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters
#include "cascada.h"		// Contains all the declarations specific to caSCADA


int
write_datapointer (void)
{
  FILE *fp;			// Pointer to a FILE type

  // opening the data file "data.dat" write-only
  if ((fp = fopen ("data.dat", "w")) == NULL)
    {
      fprintf (stderr, "Can't open `data.dat' data file \n");	// Do this if the file doesn't exist
      return 2;			// Return value of "2" means we cannot open the file
    }

  // If we can open the "data.dat" file then we write pointer addresses to it.
  // Note: the name of a structure is a pointer to its first element, therefore
  // "f_channel" serves as a pointer for the first element of the f_channel[]
  // array of data structures.
  else
    {
      fwrite (f_channel, sizeof (f_channel), 1, fp);
      fclose (fp);
    }

  return 1;

}
