/*************************************************************************
caSCADA -- PID loop simulation program 
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This program is a PID loop tuning simulator.  An "ncurses" interface 
provides the user the means to run and tune the controller.

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>		// Necessary for the random() function
#include <math.h>		// Necessary for the fabs() function
#include <ncurses.h>		// Necessary for the "ncurses" library functions for the operator display
#include <time.h>		// Necessary for the "time" library functions to work
#include "controller.h"		// Contains all the declarations specific to caSCADA's PID controller code
#include "looptune.h"		// Contains all the declarations specific to caSCADA's PID loop simulation functionality

// Declaring a time value structure for the nanosleep() function.
// The "timespec" structure is already defined in time.h, and 
// we're invoking an instance of it called "loopdelay" for our 
// purposes:
struct timespec loopdelay;

int
main (void)
{

  // Initialize all PID controller variables
  set_defaults ();


  // This function prints an introductory message to the
  // user's screen, prior to establishing a network
  // connection or executing the PID algorithm.  It returns
  // a value for "looprun" which (if not equal to 1) causes
  // the program to cleanly exit.
  looprun = splash_screen ();

  ///////////////////////////////////////////////////////
  //
  // These instructions set things up in preparation for
  // looping the PID algorithm.  We need to set default
  // control parameter values and open up the "ncurses" 
  // display mode so we have a usable operator interface.
  //
  ///////////////////////////////////////////////////////



  if (looprun == 1)
    {

      // Opens up ncurses mode and initializes all the important variables
      open_ncurses ();

    }

  // If looprun does not equal 1, exit the main routine NOW!
  else
    return 0;


  ///////////////////////////////////////////////////////
  //
  // This is the "main loop" of the program that gets
  // executed over and over again so long as "looprun"
  // is equal to a value of 1.  Otherwise, the program
  // cleanly exits.
  //
  ///////////////////////////////////////////////////////

  while (looprun)
    {

      // Increments the scan counter, counting the number of times this
      // loop has been executed
      ++scan_count;


      // Get the current UNIX system time
      time_current = (int) (time (NULL));


      // Read PV input from the simulated process ("plant") function
      // and read process load from the same simulated process.
      sim_process (0);
      pid[0].PV = plant[0].PV_sensed;
      pid[0].LOAD = plant[0].load;


      ///////////////////////////////////////////////////////////
      //
      // This next "if" conditional sets the effective scan rate
      // of the PID algorithm:
      //
      //   e.g. (scan_count % 1 == 0) calls the pid_position() 
      //                              function every scan
      //
      //   e.g. (scan_count % 10 == 0) calls the pid_position() 
      //                              function every 10 scans
      //
      // The purpose of this is to slow down the scan rate of the 
      // PID function without slowing down the operator interface 
      // response time or the trend graph response time.  It is 
      // a useful tool for educational purposes, to show how PID 
      // scan rate affects control quality.
      //
      ///////////////////////////////////////////////////////////

      // Calculate the output of the PID algorithm
      if (scan_count % 1 == 0)
          pid_position (0);


      // Bounding trend interval to reasonable limits
      if (trend_interval < 1)
	trend_interval = 1;

      if (trend_interval > 9999)
	trend_interval = 9999;


      // Calls the trend-shifting function for PID loop 0
      // and plots trend data, every trend_interval number
      // of scans through the main while() loop
      if (scan_count % trend_interval == 0)
	trend_shift_plot (0);


      // Update the operator interface display for PID loop 0
      display_plot (0);


      // Read keyboard key strokes from the human operator
      // for PID loop 0
      keyboard_scan (0);


      // Write output to the simulated control valve function
      sim_valve (0);


      // Update the last scan time value
      time_lastscan = time_current;


      // Delays the PID and process simulation loop a 
      // specified number of nanoseconds using the 
      // awesome nanosleep() function.
      nanosleep (&loopdelay, NULL);


      // Call for loop to exit if PV exceeds Hi-Hi or Lo-Lo alarm values
      if ((pid[0].PV > pid[0].PV_hihi) || (pid[0].PV < pid[0].PV_lolo))
	looprun = 0;
    }


  ///////////////////////////////////////////////////////////
  //
  //  These instructions are executed only when the while()
  //  loop stops, which is triggered by looprun = 0.  Their
  //  purpose is to cleanly exit the program, including all
  //  the "housekeeping" instructions necessary to get out
  //  of ncurses mode and return to normal terminal mode.
  //
  ///////////////////////////////////////////////////////////

  // Closes out ncurses mode and returns to normal terminal mode
  close_ncurses ();

  // Display error message if Hi-Hi alarm limit was tripped
  if (pid[0].PV > pid[0].PV_hihi)
    printf ("\a    GAME OVER -- High-High Alarm Limit Tripped!!    \a\n");

  // Display error message if Lo-Lo alarm limit was tripped
  if (pid[0].PV < pid[0].PV_lolo)
    printf ("\a    GAME OVER -- Low-Low Alarm Limit Tripped!!    \a\n");

  // Force of habit here -- I like all functions to return *something*
  return 1;

}









int
set_defaults (void)
{

  int n;

  // In order to avoid having the same "random" number values every time this 
  // program starts up, we must "seed" the random number generator with a unique
  // number value.  I have chosen to use the UNIX system time count (in seconds)
  // as the random number seed.
  srandom ((int) (time (NULL)));


  // PID controller 0 parameters
  pid[0].SP = 50;		// Initialize setpoint to 50%
  pid[0].BIAS = 50;		// Initialize bias to 50%
  pid[0].K_P = 0.5;		// Initialize gain at a value of 0.5
  pid[0].K_I = 0;		// Zero repeats per minute integral
  pid[0].K_D = 0;		// Zero seconds derivative
  pid[0].PV_hihi = 95;
  pid[0].PV_hi = 90;
  pid[0].PV_lo = 10;
  pid[0].PV_lolo = 5;
  pid[0].URV = 100;
  pid[0].LRV = 0;
  pid[0].UNIT = "%";
  pid[0].windup_hilimit = 99;	// Integral windup high limit
  pid[0].windup_lolimit = 1;	// Integral windup low limit
  pid[0].I_db = 0;		// Integral deadband (integration halts at error values less than this)
  pid[0].action = 0;		// 0 = Reverse      ;  1 = Direct
  pid[0].am_mode = 0;		// 0 = Manual       ;  1 = Automatic
  pid[0].equation = 0;		// 0 = Ideal        ;  1 = Parallel
  pid[0].type = 0;		// 0 = Simulation ; 1 = Single-loop ; 2 = Ratio ; 3 Cascade 

  // General parameters
  looprun = 1;			// Allows the PID loop to run
  select_mode = 0;		// User interface mode (0 = adjust OUT in manual or SP in automatic)
  scan_count = 0;		// Resets the scan counter at zero
  scans_per_second = 1.0;	// Initializes the scan/second value to 1
  trend_interval = 5;

  // Loop execution delay time
  loopdelay.tv_sec = 0;		// whole seconds
  loopdelay.tv_nsec = 200000000;	// nanoseconds (200,000,000 nanoseconds = 0.2 seconds)

  // This pre-populates the pen[] array with 50% values for PV, SP, and OUT
  // so that the trend graph does not begin with a history of everything 
  // being at 0% (which is unrealistic)
  for (n = (TRENDWIDTH - 1); n > 0; --n)
    {
      pen[n].PV = 50;
      pen[n].SP = 50;
      pen[n].OUT = 50;
    }

  // This pre-populates the deadtime[] array with 50% values for PV
  // for the same reason
  for (n = 0; n < DEADTIME_FIFO; ++n)
    {
      deadtime[0][n] = 50;
    }



  ////////////////////////////////////////////////////////////
  //
  // Here is where we set all the default parameters for the
  // simulated process and simulated control valve.  Note that
  // the user may select parameter options other than what is
  // specified here in the process_options_screen() function.
  // However, any parameters uninitialized by user choice will
  // be guaranteed to have the following values since the
  // set_defaults() function is called before the
  // process_options_screen() function.
  //
  ////////////////////////////////////////////////////////////

  // Simulated process parameters 
  plant[0].PV_sensed = 50.0;	// Initial value for the process variable as sensed by the xmtr
  plant[0].PV = 50.0;		// Initial value for the process variable
  plant[0].MV = 50.0;		// Initial value for the control valve's effect on the process
  plant[0].load = 0.0;		// Initial value for the process load
  plant[0].loadstepmax = randomvalue (0.0, 5.0);	// Load step size randomized between 0.0% and 5.0%
  plant[0].loadinterval = (int) (randomvalue (1, 5));	// Load interval randomized between 1 and 5 seconds
  plant[0].loadmax = 50.0;
  plant[0].loadmin = -50.0;
  plant[0].lag1 = randomvalue (1.0, 30.0);	// Process lag time randomized between 1.0 and 30.0 seconds
  plant[0].dead = randomvalue (0.0, 3.0);	// Process dead time randomized between 0.0 and 3.0 seconds
  plant[0].k = randomvalue (0.0, 1.5);	// Process characteristic randomized (always positive, never runaway)
  plant[0].noiselevel = randomvalue (0.0, 10.0);	// Process noise randomized between 0.0% and 10.0%

  // Simulated control valve parameters 
  valve[0].stem_pos = 50.0;
  valve[0].gain = randomvalue (0.5, 2.0);	// Valve gain randomized between 0.5 and 2.0
  valve[0].stick = randomvalue (0.0, 5.0);	// Valve "stick" parameter randomized between 0.0 and 5.0
  valve[0].slip = randomvalue (0.0, valve[0].stick);	// Valve "stick" parameter randomized between 0.0 and the previously assigned "stick" value

  // Force of habit here -- I like all functions to return *something*
  return 1;
}









int
splash_screen (void)
{
  char key;
  int m, n;

  clear_screen ();

  printf ("\n");
  printf ("Welcome to the caSCADA PID loop tuning simulator! \n");

  printf ("\n");
  printf ("By Tony R. Kuphaldt \n");
  printf ("This program comes with ABSOLUTELY NO WARRANTY! \n");
  printf ("This is free software, released into the Public \n");
  printf ("Domain under the Creative Commons CC0 Universal License. \n");

  printf ("\n");
  printf ("This is the size of the trend display (%i wide by %i high):\n",
	  TRENDWIDTH, TRENDHEIGHT);
  printf ("\n");

  printf ("+");

  for (m = 0; m < TRENDWIDTH; ++m)
    printf ("-");

  printf ("+\n");

  for (n = 0; n < TRENDHEIGHT; ++n)
    {
      printf ("|");

      for (m = 0; m < TRENDWIDTH; ++m)
	printf (" ");

      printf ("|");

      if (n == 2)
	printf ("HH");

      if (n == 4)
	printf ("H");

      if (n == TRENDHEIGHT - 5)
	printf ("L");

      if (n == TRENDHEIGHT - 3)
	printf ("LL");

      printf ("\n");

    }

  printf ("+");

  for (m = 0; m < TRENDWIDTH; ++m)
    printf ("-");

  printf ("+\n");

  printf ("\n");
  printf
    ("If the trend display size is not to your liking, please quit now and\n");
  printf ("edit the source code (file: controller.h).\n");

  printf ("\nEnter Y to continue, anything else to quit ");
  scanf ("%c", &key);

  if (key == 'Y' || key == 'y')
    {
      process_options_screen ();
      return 1;
    }

  else
    return 0;


}











int
process_options_screen (void)
{
  int n;

  clear_screen ();

  printf ("Choose which process you desire to simulate: \n\n");

  printf ("(0) Surprise me! \n");
  printf
    ("(1) Generic self-regulating process with no noise and steady load \n");
  printf ("(2) Generic integrating process with no noise and steady load \n");
  printf ("(3) Generic runaway process with no noise and steady load \n");
  printf ("(4) Generic self-regulating process with no noise and varying load \n");
  printf ("(5) Generic integrating process with no noise and varying load \n");
  printf ("(6) Typical liquid flow control process \n");
  printf ("(7) Typical liquid level control process \n");
  printf ("(8) Typical gas pressure control process \n");
  printf ("(9) Typical temperature control process \n");

  printf ("\nEnter selection ");
  scanf ("%i", &n);


  if (n == 1)			// Generic self-regulating process
    {
      plant[0].loadstepmax = 0.0;
      plant[0].loadinterval = 4;
      plant[0].loadmax = 50.0;
      plant[0].loadmin = -50.0;
      plant[0].lag1 = 10.0;
      plant[0].dead = 0.0;
      plant[0].k = 1.2;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 2)		// Generic integrating process
    {
      plant[0].loadstepmax = 0.0;
      plant[0].loadinterval = 4;
      plant[0].loadmax = 50.0;
      plant[0].loadmin = -50.0;
      plant[0].lag1 = 15.0;
      plant[0].dead = 0.0;
      plant[0].k = 0.0;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 3)		// Generic runaway process
    {
      plant[0].loadstepmax = 0.0;
      plant[0].loadinterval = 4;
      plant[0].loadmax = 50.0;
      plant[0].loadmin = -50.0;
      plant[0].lag1 = 20.0;
      plant[0].dead = 0.0;
      plant[0].k = -0.2;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 4)		// Generic self-regulating process with load but no noise
    {
      plant[0].loadstepmax = 7.0;
      plant[0].loadinterval = 4;
      plant[0].loadmax = 40.0;
      plant[0].loadmin = -40.0;
      plant[0].lag1 = 8.0;
      plant[0].dead = 0.0;
      plant[0].k = 1.2;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 5)		// Generic integrating process with load but no noise
    {
      plant[0].loadstepmax = 7.0;
      plant[0].loadinterval = 4;
      plant[0].loadmax = 40.0;
      plant[0].loadmin = -40.0;
      plant[0].lag1 = 6.0;
      plant[0].dead = 0.0;
      plant[0].k = 0.05;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 6)		// Typical liquid flow control process
    {
      plant[0].loadstepmax = 5.0;
      plant[0].loadinterval = 5;
      plant[0].loadmax = 50.0;
      plant[0].loadmin = -50.0;
      plant[0].lag1 = 0.2;
      plant[0].dead = 0.0;
      plant[0].k = 1.0;
      plant[0].noiselevel = 5.0;
    }

  else if (n == 7)		// Typical liquid level control process
    {
      plant[0].loadstepmax = 2.0;
      plant[0].loadinterval = 5;
      plant[0].loadmax = 50.0;
      plant[0].loadmin = -50.0;
      plant[0].lag1 = 30.0;
      plant[0].dead = 0.0;
      plant[0].k = 0.1;
      plant[0].noiselevel = 0.5;
    }

  else if (n == 8)		// Typical gas pressure control process
    {
      plant[0].loadstepmax = 1.0;
      plant[0].loadinterval = 3;
      plant[0].loadmax = 30.0;
      plant[0].loadmin = -30.0;
      plant[0].lag1 = 90.0;
      plant[0].dead = 0.5;
      plant[0].k = 0.5;
      plant[0].noiselevel = 0.0;
    }

  else if (n == 9)		// Typical temperature control process
    {
      plant[0].loadstepmax = 0.2;
      plant[0].loadinterval = 10;
      plant[0].loadmax = 30.0;
      plant[0].loadmin = -30.0;
      plant[0].lag1 = 180.0;
      plant[0].dead = 0.0;
      plant[0].k = 0.3;
      plant[0].noiselevel = 0.0;
    }





  else				// Accept all process parameters specified in set_default() function
    printf (" ");		// Do nothing



  clear_screen ();

  printf ("Choose which control valve you desire to simulate: \n\n");

  printf ("(0) Surprise me! \n");
  printf ("(1) Perfect valve (no stiction, gain of 1) \n");
  printf ("(2) Typical control valve \n");
  printf ("(3) Really bad control valve \n");

  printf
    ("\nPlease note that the computer will randomly choose either an air-to-open \n");
  printf
    ("or an air-to-close control valve action for you.  This means you must \n");
  printf
    ("empirically determine the process response to a manual-mode (open-loop) \n");
  printf 
    ("step-change in order to decide which controller action (e.g. direct or \n");
  printf
    ("reverse) will be necessary for stable feedback control.  The controller \n");
  printf
    ("defaults to reverse action, but may be set for direct action if needed. \n");

  printf
    ("\nFurthermore, the computer will randomly choose alarm points for the \n");
  printf
    ("process variable.  The High (H) and Low (L) alarm thresholds are merely \n");
  printf
    ("informational, but the High-High (HH) and Low-Low (LL) thresholds will \n");
  printf ("trigger the program to shut down if crossed!!! \n");

  printf ("\nEnter selection ");
  scanf ("%i", &n);

  // In order to avoid having the same "random" number values every time this 
  // program starts up, we must "seed" the random number generator with a unique
  // number value.  I have chosen to use the UNIX system time count (in seconds)
  // as the random number seed.
  srandom ((int) (time (NULL)));

  if (n == 1)			// Perfect control valve
    {
      valve[0].gain = 1.0;
      valve[0].stick = 0.0;
      valve[0].slip = 0.0;
    }

  else if (n == 2)		// Typical control valve
    {
      valve[0].gain = 1.5;
      valve[0].stick = 3.0;
      valve[0].slip = 1.0;
    }

  else if (n == 3)		// Really bad control valve
    {
      valve[0].gain = 2.0;
      valve[0].stick = 10.0;
      valve[0].slip = 4.2;
    }

  else				// Accept all valve parameters specified in set_default() function
    printf (" ");		// Do nothing

  // Here is where we randomly assign a control valve action, by inverting
  // the mathematical sign of the valve's gain value half of the time as
  // determined by the random() function:
  if (random () > (RAND_MAX / 2))
    valve[0].gain = -valve[0].gain;


  // Assigning random values to controller alarm points
  pid[0].PV_hi = randomvalue (60.0, 90.0);
  pid[0].PV_hihi = randomvalue (pid[0].PV_hi + 5, 100.0);
  pid[0].PV_lo = randomvalue (10.0, 40.0);
  pid[0].PV_lolo = randomvalue (0.0, pid[0].PV_lo - 5.0);

  return 1;

}










int
clear_screen (void)
{
  int n;


  // This simply prints 50 linefeed characters to the screen,
  // which is a simple way of clearing the console of text!
  for (n = 0; n < 80; ++n)
    {
      printf ("\n");
    }

  return 1;

}








float
randomvalue (float LRV, float URV)
{

  // This function returns a random number lying within the range
  // of LRV (Lower Range Value) to URV (Upper Range Value) sent
  // as arguments to the function.

  // This function returns a floating-point value with a peak-peak range
  // equal to "span" and biased by "offset"
  return (float) ((URV - LRV) * random () / RAND_MAX) + LRV;

}
