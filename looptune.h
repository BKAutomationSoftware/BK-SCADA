/*************************************************************************
caSCADA -- PID loop simulation program 
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

*************************************************************************/





/**************************************************************************** 
		Declare simulated process structure and arrays
****************************************************************************/

// In process control engineering, the transfer function for a process is 
// often referred to as the "plant" which is why this data structure is so
// named.

struct data_process
{
  float PV_sensed;		// Process Variable as sensed by transmitter
  float PV;			// Process Variable as computed for the actual process
  float MV;			// Manipulated Variable (i.e. effect of valve)
  float load;			// Independent load on the process
  float loadstepmax;		// The max. load drift per update
  int loadinterval;		// The load update interval (in whole seconds)
  float loadmax;		// The maximum load value allowed
  float loadmin;		// The minimum load valve allowed
  float outflow;		// Dependent load (i.e. varies with PV)
  float k;			// outflow proportionality constant (+ self-reg, - runaway)
  float lag1;			// first-order process lag-time (in seconds)
  float dead;			// dead time (in seconds)
  float noiselevel;		// The P-P amount of noise placed on the PV
} plant[3];

#define DEADTIME_FIFO 150	// Defines the number of FIFO shift register elements

float deadtime[3][DEADTIME_FIFO];	// An array of floating-point variables for making 
				   // a dead-time shift register (FIFO), one for each
				   // process.




/**************************************************************************** 
		Declare simulated control valve structure
****************************************************************************/

// In process control engineering, the transfer function for a process is 
// often referred to as the "plant" which is why this data structure is so
// named.

struct data_controlvalve
{
  float stem_pos;		// Valve stem position (0 to 100)
  float gain;			// Valve gain (+ ATO, - ATC)
  float stick;			// Static friction in the control valve
  float slip;			// Valve stem movement once freed
} valve[3];







/****************************************************************************
			Function prototypes 
****************************************************************************/

// The purpose of a function prototype in ANSI standard C programming is to
// tell the compiler what arguments will be passed to each function, and what
// sort of value will be returned by that function.  "void" means no argument
// or value is passed/returned.

// Prototype for the introductory screen function
int splash_screen (void);

// Prototype for the process options screen function
int process_options_screen (void);

// Prototype for the console screen-clearing function
int clear_screen (void);

// Prototype for the function initializing all PID control parameters
int set_defaults (void);

// Prototype for a simulated control valve function
int sim_valve (int);

// Prototype for a simulated process function
int sim_process (int);

// Prototype for a random noise function
float noise (float, float);

// Prototype for a general-purpose random number generator
float randomvalue (float, float);
