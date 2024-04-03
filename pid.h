/*************************************************************************
caSCADA -- PID control system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

*************************************************************************/






/**************************************************************************** 
		Declare DAQ variable arrays
****************************************************************************/

// The following fourteen-element array stores all the "raw" floating-point
// values read from the DAQ's analog input channels (AIN0 through AIN13)
float ain[14];


// The following two-element array stores all the "raw" floating-point
// values to be written to the the DAQ's analog output channels (DAC0 and DAC1)
float aout[2];



/****************************************************************************
			Function prototypes 
****************************************************************************/

// The purpose of a function prototype in ANSI standard C programming is to
// tell the compiler what arguments will be passed to each function, and what
// sort of value will be returned by that function.  "void" means no argument
// or value is passed/returned.

// Prototype for reading a floating-point variable from any Modbus device
float read_32float (int address, int byteswap);

// Prototype for reading any generic integer variable via Modbus
int read_16int (int address);

// Prototype for writing a floating-point variable to any Modbus device
int write_32float (float real, int address, int byteswap);

// Prototype for the introductory screen function
int splash_screen (void);

// Prototype for the function initializing all PID control parameters
int set_defaults (void);

// Prototype for dual-output sequencing
int dual_output (int type);

