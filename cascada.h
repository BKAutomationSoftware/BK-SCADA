/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

*************************************************************************/




/**************************************************************************** 
		Declare floating-point channel structures 
****************************************************************************/

// "data_fp" is the type of structure.  Here we define 100 of them, each one
// named "f_channel" followed by a number in brackets distinguishing the 100
// structures from each other.  That number may be thought of being analogous
// to subscripts for mathematical variables (e.g. x_1, x_2).

struct data_fp
{
  float value;
  char *tag;
  char *unit;
  int status;
  char *comment;
} f_channel[100];


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

// Prototype for the analog input channel reading function    
int read_LJT7_analogin (void);

// Prototype for reading a floating-point variable from any Modbus device
float read_32float (int address, int byteswap);

// Prototype for reading any generic integer variable via Modbus
int read_16int (int address);

// Prototype for generating simulated analog input values in lieu of Modbus polls
int read_sim_analogin (int mode);

// Prototype for the analog output channel writing function    
int write_LJT7_analogout (void);

// Prototype for writing a floating-point variable to any Modbus device
int write_32float (float real, int address, int byteswap);

// Prototype for the text file writing function 
int write_datatext (void);

// Prototype for the HTML file writing function 
int write_datahtml (void);

int f_channel_00(void); // These functions condition data for each channel in the f_channel structure
int f_channel_01(void); 
int f_channel_02(void); 
int f_channel_03(void); 
int f_channel_04(void); 
int f_channel_05(void); 
int f_channel_06(void); 
int f_channel_07(void); 
int f_channel_08(void); 
int f_channel_09(void); 
int f_channel_10(void); 
int f_channel_11(void); 
int f_channel_12(void); 
int f_channel_13(void); 
int f_channel_14(void); 
int f_channel_15(void); 
int f_channel_16(void); 
int f_channel_17(void); 
int f_channel_18(void); 
int f_channel_19(void); 
int f_channel_20(void); 
int f_channel_21(void); 
int f_channel_22(void); 
int f_channel_23(void); 
int f_channel_24(void); 
int f_channel_25(void); 
int f_channel_26(void); 
int f_channel_27(void); 
int f_channel_28(void); 
int f_channel_29(void); 
int f_channel_30(void); 
int f_channel_31(void); 
int f_channel_32(void); 
int f_channel_33(void); 
int f_channel_34(void); 
int f_channel_35(void); 
int f_channel_36(void); 
int f_channel_37(void); 
int f_channel_38(void); 
int f_channel_39(void); 
int f_channel_40(void); 
int f_channel_41(void); 
int f_channel_42(void); 
int f_channel_43(void); 
int f_channel_44(void); 
int f_channel_45(void); 
int f_channel_46(void); 
int f_channel_47(void); 
int f_channel_48(void); 
int f_channel_49(void); 
int f_channel_50(void); 
int f_channel_51(void); 
int f_channel_52(void); 
int f_channel_53(void); 
int f_channel_54(void); 
int f_channel_55(void); 
int f_channel_56(void); 
int f_channel_57(void); 
int f_channel_58(void); 
int f_channel_59(void); 
int f_channel_60(void); 
int f_channel_61(void); 
int f_channel_62(void); 
int f_channel_63(void); 
int f_channel_64(void); 
int f_channel_65(void); 
int f_channel_66(void); 
int f_channel_67(void); 
int f_channel_68(void); 
int f_channel_69(void); 

int i_channel_00(void); // These functions condition data for each channel in the i_channel structure

