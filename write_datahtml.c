/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

Function: write_datahtml
The purpose of this program is to write all channel data to an HTML
file which may be read by any web browser or other application.

*************************************************************************/


#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters
#include "cascada.h"		// Contains all the declarations specific to caSCADA


int
write_datahtml (void)
{
  int n;
  FILE *fp;			// Pointer to a FILE type


  // opening the data file "data.html" write-only
  if ((fp = fopen ("data.html", "w")) == NULL)
    {
      fprintf (stderr, "Can't open `data.html' HTML file \n");	// Do this if the file doesn't exist
      return 2;			// Return value of "2" means we cannot open the file
    }


  // Prints "preamble" HTML code
  fprintf (fp,
	   " <!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"  \"http://www.w3.org/TR/html4/strict.dtd\">  \n");
  fprintf (fp,
	   " <html lang=\"en\"> <head> <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">  <title>Channel Data</title> </head> <body>    \n");


  // Prints heading text for the HTML page
  fprintf (fp, " <h1>caSCADA channel data</h1> \n");


  // Prints beginning of HTML "table" structure
  fprintf (fp, " <table border=\"5\" bordercolor=\"blue\"> \n");


  // Prints a "header" line to label each column of data
  fprintf (fp,
	   "<tr> <td> <b> Channel number </b> </td> <td> <b> Tag </b> </td> <td> <b> Value </b> </td> <td> <b> Unit </b> </td> <td> <b> Status </b> </td> <td> <b> Comment </b> </td> </tr> \n");



  // Prints 32 channels' worth of data starting with f_channel[0]
  for (n = 0; n < 32; ++n)
    {
      fprintf (fp,
	       "<tr> <td> %i </td> <td> %s </td> <td> %2.4f </td> <td> %s </td> <td> %i </td> <td> %s </td> </tr> \n",
	       n, f_channel[n].tag, f_channel[n].value, f_channel[n].unit,
	       f_channel[n].status, f_channel[n].comment);
    }



  // Prints ending of HTML "table" structure
  fprintf (fp, " </table>  \n");

  // Prints "closing" HTML code
  fprintf (fp, " </body> </html>  \n");

  // Close the "data.html" HTML file
  fclose (fp);

  return 1;

}
