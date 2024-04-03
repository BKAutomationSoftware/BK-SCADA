/*************************************************************************
caSCADA -- PID loop simulation program
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This function simulates a physical process that may be controlled by a
PID controller for the purposes of loop tuning simulation.

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>		// Necessary for the random() function
#include <math.h>
#include "controller.h"		// Contains all the declarations specific to caSCADA's PID controller code
#include "looptune.h"		// Contains all the declarations specific to caSCADA's PID loop simulation functionality

int
sim_process (int c)
{

  int n;

  // Create a randomly-walking load for the process.  The "loadstepmax" parameter establishes
  // how far the load value may change during each update.  The "loadinterval" establishes
  // how often (in whole seconds) the load gets updated.  The check for inequality between the
  // current and last-scan system times ensures the load gets updated only once per
  // interval (and not multiple times during the one second of time when the modulo value
  // equals 0).  The noise() function is used to create the random load-step value.
  if (time_current % plant[c].loadinterval == 0
      && time_current != time_lastscan)
    plant[c].load =
      plant[c].load + noise (plant[c].loadstepmax * 2, -plant[c].loadstepmax);


  // Bounding load between limits of "loadmax" and "loadmin"
  if (plant[c].load > plant[c].loadmax)
    plant[c].load = plant[c].loadmax;

  if (plant[c].load < plant[c].loadmin)
    plant[c].load = plant[c].loadmin;




  //////////////////////////////////////////////////////////////////////////
  // The following mathematical model for a process is based on the
  // concept of mass/energy balance.  The PV will increment or decrement
  // based on the imbalance between Inflow and Outflow of either mass
  // or energy.  This may be written as a simple integral function:  
  //
  //                  /
  //                 /
  //              1  |
  // (Delta)PV = --- | (Inflow - Outflow) dt
  //             tau |
  //                 / 
  //                /
  //
  // In this simulation we will assume that Inflow consists of the 
  // combined effect of MV and load, while Outflow depends on the PV:
  //
  // (1) For a purely integrating process, Outflow is fixed while 
  // Inflow is a function of MV and load.
  //
  // (2) For a self-regulating process, Outflow is directly proportional
  // to PV, so that as PV increases in response to greater MV and/or load,
  // the Outflow will eventually rise to meet the Inflow to make PV
  // self-stabilize.
  //
  // (3) For a runway process, Outflow is negatively proportional to PV,
  // so that as PV increases the Outflow decreases which accelerates the
  // PV's increase.
  //
  // Thus, the self-reg/integ/runaway characteristic of a linear process
  // is set by a simple proportionality coefficient ("k" factor):
  //     Outflow = k * PV
  //
  //    When k is positive, its value is the reciprocal of process gain:
  //      e.g. when k=2, the process gain is 0.5 (self-regulating)
  //
  //    When k is zero, the process is purely integrating
  //
  //    When k is negative, the process becomes runaway
  //
  //////////////////////////////////////////////////////////////////////////

  // Calculate the mass/energy Outflow rate based on PV value and "k" factor
  plant[c].outflow = plant[c].k * (plant[c].PV - 50.0);

  // Perform numerical integration, incrementing or decrementing the PV 
  // according to any imbalance between Inflow and Outflow:
  plant[c].PV =
    plant[c].PV +
    ((1 / plant[c].lag1) *
     ((plant[c].MV - 50.0) + plant[c].load -
      plant[c].outflow) * (1 / scans_per_second));

  // Shifting the FIFO shift register for dead time
  for (n = (DEADTIME_FIFO - 1); n > 0; --n)
    {
      deadtime[c][n] = deadtime[c][n - 1];
    }

  // Updating the most recent element of the FIFO register
  // with the current process variable value  
  deadtime[c][0] = plant[c].PV;

  // Here we calculate which element of the FIFO shift
  // register to read for the time-delayed process
  // variable value.  Both deadtime and scans_per_second
  // are floating-point values, but we need an integer
  // value to specify the shift register array element
  // which is why we use the (int) cast.
  n = (int) (plant[c].dead * scans_per_second);

  // Bounding dead time delay between limits of 0 and FIFO size
  if (n < 0)
    n = 0;

  if (n > (DEADTIME_FIFO - 1))
    n = DEADTIME_FIFO - 1;

  // Adding dead time and noise to the PV 
  plant[c].PV_sensed =
    deadtime[c][n] + noise (plant[c].noiselevel, -plant[c].noiselevel / 2);

  // Return an integer (unrelated to process function)
  return 0;

}






float
noise (float span, float offset)
{

  // "span" is the peak-to-peak range of the returned value
  // "offset" is the bias value added to the raw random value
  // If offset = 0, then all returned values will be positive
  // If offset = -span, then all returned values will be negative
  // If offset = -span/2, then all returned values will distributed around zero

  // This function returns a floating-point value with a peak-peak range
  // equal to "span" and biased by "offset"
  return (float) (span * random () / RAND_MAX) + offset;

}
