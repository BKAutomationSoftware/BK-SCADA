/*************************************************************************
caSCADA -- PID controller program 
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This file contains all the declarations necessary for the PID algorithm
and the ncurses-based user interface.  Three independent PID algorithms
are supported, as an array of structures.

*************************************************************************/




/**************************************************************************** 
		Declare PID structure
****************************************************************************/

// "data_pid" is the type of structure.  Here we define 3 of them, each one
// named "pid" followed by a number in brackets distinguishing the 3
// structures from each other.  That number may be thought of being analogous
// to subscripts for mathematical variables (e.g. x_1, x_2).
// This data structure holds all the variables that each PID algorithm needs
// in order to properly function.

struct data_pid
{
  float PV;			// Process Variable from analog input channel
  float SP;			// Setpoint
  float LOAD;			// Load variable (may be feedforward, may be simulated!)
  float OUT;			// Output to analog output channel
  float BIAS;			// Bias (also used as the integral term)
  float K_P;			// Proportional (gain) tuning coefficient
  float K_I;			// Integral (reset) tuning coefficient
  float K_D;			// Derivative (rate) tuning coefficient
  float PV_hihi;		// Hi-Hi alarm threshold 
  float PV_hi;			// Hi alarm threshold
  float PV_lo;			// Lo alarm threshold
  float PV_lolo;		// Lo-Lo alarm threshold
  float URV;			// Upper Range Value for PV and SP (engineering units)
  float LRV;			// Lower Range Value for PV and SP (engineering units) 
  char *UNIT;			// Units of measurement for LRV and URV
  float windup_hilimit;		// Hi limit where integral action is told to halt
  float windup_lolimit;		// Lo limit where integral action is told to halt
  float I_db;			// Integral deadband (integration halts at error values less than this)
  float FF;			// Feedforward contribution to the controller's output signal
  float FF_lv;			// Feedforward load variable from analog input channel, used for preemptive control action 
  float FF_gain;		// Feedforward gain value, multiplied by FFWD analog input percentage
  float FF_bias;		// Feedforward bias value, added to product of FF_gain and FFWD analog value
  int action;			// 0 = Reverse            ;  1 = Direct
  int am_mode;			// 0 = Manual             ;  1 = Automatic
  int equation;			// 0 = Ideal              ;  1 = Parallel
  int type;			// 0 = Simulation ; 1 = Single-loop ; 2 = Ratio (FUTURE) ; 3 Cascade (FUTURE)
} pid[3];




/**************************************************************************** 
		Declare trend graph variables
****************************************************************************/

// This trend structure stores a fixed number of PV, SP, and OUT samples

// TRENDWIDTH=78 and TRENDHEIGHT=10 will work for ANY 80-column x 24-row terminal
// TRENDWIDTH=120 and TRENDHEIGHT=25 works nicely for most modern VGA terminals
// TRENDWIDTH=200 and TRENDHEIGHT=45 works for some high-resolution VGA terminals
#define TRENDWIDTH 120
#define TRENDHEIGHT 25

struct trendgraph
{
  float PV;	// Process variable trend array
  float SP;	// Setpoint trend array
  float LOAD;	// Load variable trend array
  float OUT;	// Output trend array
} pen[TRENDWIDTH];

int trend_interval;		// The number of seconds between trend updates
float timebase;			// Measured in trend ticks per second





/**************************************************************************** 
		Declare other global variables
****************************************************************************/

int looprun;			// This global variable allows the program loop to execute 
			// when equal to 1.  Setting this variable to 0 causes the
			// program to cleanly exit.


int select_mode;		// This global variable controls which parameter is being adjusted
			// 0 = operating mode -- adjusting OUT in manual or SP in automatic
			// 1 = adjust K_P
			// 2 = adjust FF_gain
			// 3 = adjust K_I
			// 4 = adjust integral deadband
			// 5 = adjust K_D
			// 6 = adjust FF_bias
			// 7 = adjust action (direct/reverse)
			// 8 = adjust algorithm (ideal/parallel)
			// 9 = adjust trend interval

int time_current;		// This is the current UNIX system time cast into integer format
int time_lastscan;		// This is the previous UNIX system time (sampled during the last scan)
int scan_count;			// This is a running tally of how many scans have been executed
float scans_per_second;		// This is an estimate of how many program scans occur each second


/****************************************************************************
			Function prototypes 
****************************************************************************/

// The purpose of a function prototype in ANSI standard C programming is to
// tell the compiler what arguments will be passed to each function, and what
// sort of value will be returned by that function.  "void" means no argument
// or value is passed/returned.

// Prototype for the analog input channel reading function    
int pid_position (int);

// Prototype for plotting static displays to the screen
int display_plot (int);

// Prototype for reading keyboard input
int keyboard_scan (int);

// Prototype for the PID tuning adjustment environment
int tuning_entry (void);

// Prototype for shifting trend data arrays and plotting trend data points
int trend_shift_plot (int);

// Prototype for capturing trend graph data
int capture_trend (void);

// Prototype for initializing the ncurses screen environment
int open_ncurses (void);

// Prototype for closing the ncurses screen environment
int close_ncurses (void);
