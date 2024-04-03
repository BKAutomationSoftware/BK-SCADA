/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

Process: cascada_poll_simulate
The purpose of this program is to create simulated analog input values
for the first fourteen elements of the f_channel data structure, in 
order to serve as a test.

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>		// Necessary for the rand() function
#include <unistd.h>		// Necessary for the sleep() function
#include "cascada.h"		// Contains all the declarations specific to caSCADA

int
main (void)
{

  // Initialize all simulated analog inputs at 3.0 volts, which is 50% 
  // for a 1-5 volt analog range (4-20 mA dropped across a 250 ohm resistor).
  // This is done only once, before the while() loop executes.
  ain[0] = 3.0;
  ain[1] = 3.0;
  ain[2] = 3.0;
  ain[3] = 3.0;
  ain[4] = 3.0;
  ain[5] = 3.0;
  ain[6] = 3.0;
  ain[7] = 3.0;
  ain[8] = 3.0;
  ain[9] = 3.0;
  ain[10] = 3.0;
  ain[11] = 3.0;
  ain[12] = 3.0;
  ain[13] = 3.0;

// Main loop
  while (1)
    {
      int status;

      // call function to create a sawtooth pattern for the channel values
      status = read_sim_analogin (1);

      if (status != 1)
	fprintf (stderr,
		 "Bad status returned from initialization function\n");

      // Call channel functions to assign values to the floating-point structure elements
      f_channel_00 ();
      f_channel_01 ();
      f_channel_02 ();
      f_channel_03 ();
      f_channel_04 ();
      f_channel_05 ();
      f_channel_06 ();
      f_channel_07 ();
      f_channel_08 ();
      f_channel_09 ();
      f_channel_10 ();
      f_channel_11 ();
      f_channel_12 ();
      f_channel_13 ();
      f_channel_14 ();
      f_channel_15 ();
      f_channel_16 ();
      f_channel_17 ();
      f_channel_18 ();
      f_channel_19 ();
      f_channel_20 ();
      f_channel_21 ();
      f_channel_22 ();
      f_channel_23 ();
      f_channel_24 ();
      f_channel_25 ();
      f_channel_26 ();
      f_channel_27 ();
      f_channel_28 ();
      f_channel_29 ();
      f_channel_30 ();
      f_channel_31 ();

      // Write structure data to text file ("data.txt")
      write_datatext ();

      // Write structure data to HTML file ("data.html")
      write_datahtml ();

      // Wait for 1 second so we don't waste network bandwidth and processing power
      sleep (1);
    }


  return 1;
}





// This function creates a simulated voltage for analog channels 0 through 13
int
read_sim_analogin (int mode)
{
  int n;
  float x;

  if (mode == 0)		// A mode value of 0 specifies a random walk
    {
      // Randomly "walk" all analog input variables
      for (n = 0; n < 14; ++n)
	{
	  // Randomly step "x" between -0.1 and +0.1
	  x = ((float) (rand ()) * 0.2 / RAND_MAX) - 0.1;

	  ain[n] = ain[n] + x;

	  // Establishes upper bound of 5 (volts) for randomly-walking analog input simulation,
	  // which is 100% for a 1-5 volt analog range (4-20 mA dropped across a 250 ohm resistor)
	  if (ain[n] > 5)
	    ain[n] = 5;

	  // Establishes lower bound of 1 (volt) for randomly-walking analog input simulation,
	  // which is 0% for a 1-5 volt analog range (4-20 mA dropped across a 250 ohm resistor)
	  if (ain[n] < 1)
	    ain[n] = 1;

	}
    }



  else				// Any other value for mode specifies a sawtooth "step" sequence from 0 volts to 5 volts
    {
      // Steps up each analog input value by 0.5 volt, then resets to zero 
      for (n = 0; n < 14; ++n)
	{
	  ain[n] = ain[n] + 0.5;

	  // Establishes upper bound of 5 (volts) for sawtooth analog input simulation,
	  // which is 100% for a 1-5 volt analog range (4-20 mA dropped across a 250 ohm resistor)
	  if (ain[n] > 5)
	    ain[n] = 0;

	  // Establishes lower bound of 0 (volts) for sawtooth analog input simulation,
	  // which is -25% for a 1-5 volt analog range (4-20 mA dropped across a 250 ohm resistor)
	  if (ain[n] < 0)
	    ain[n] = 0;
	}

    }

  // Return a "good" (1) status value
  return 1;
}
