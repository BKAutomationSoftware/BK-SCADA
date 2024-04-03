/*************************************************************************
caSCADA -- PID loop simulation program
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This function simulates a throttling control valve as the final control
element (FCE) of a PID-controlled loop.

*************************************************************************/

#include <stdio.h>
#include <math.h>
#include "controller.h"		// Contains all the declarations specific to caSCADA's PID controller code
#include "looptune.h"		// Contains all the declarations specific to caSCADA's PID loop simulation functionality

int
sim_valve (int c)
{
  /////////////////////////////////////////////////////////////////
  // Control valves are imperfect, and the point of this function
  // is to codify that imperfection.  The "pid_out" value is what's
  // output by the PID controller, while the "MV" (global) value is
  // the final effect of the valve on the process.  In a perfect 
  // world, MV = pid_out.
  /////////////////////////////////////////////////////////////////


  // This code mimics a control valve with "stiction": one that
  // tends to stick in place via static friction ("stick") and 
  // then breaks loose ("slip") to move a certain amount.
  if ((pid[c].OUT - valve[c].stem_pos) > valve[c].stick)
    valve[c].stem_pos = valve[c].stem_pos + valve[c].slip;

  if ((valve[c].stem_pos - pid[c].OUT) > valve[c].stick)
    valve[c].stem_pos = valve[c].stem_pos - valve[c].slip;



  // Assuming a perfect valve if either "stick" or "slip" values
  // are less than one.
  if (valve[c].stick < 1.0 || valve[c].slip < 1.0)
    valve[c].stem_pos = pid[c].OUT;



  // Linear-acting valve following y = mx + b equation
  // where "m" is the valve gain and "b" is the offset.
  // The purpose of this is to ensure no matter when the
  // valve's gain is set for, a 50% stem position should
  // yield a 50% MV.
  // If m = 1, then offset should be 0
  // If m = 2, then offset should be -50
  // If m = 3, then offset should be -100
  plant[c].MV =
    (valve[c].gain * valve[c].stem_pos) - ((valve[c].gain - 1) * 50);


  // Bounding MV between limits of 0% and 100%
  if (plant[c].MV > 100.0)
    plant[c].MV = 100.0;

  if (plant[c].MV < 0.0)
    plant[c].MV = 0.0;

  // Return an integer (unrelated to valve action)
  return 0;

}
