/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

*************************************************************************/

#include <stdio.h>
#include <time.h>		// Necessary for the "time" library functions to work (nanosleep)
#include "cascada.h"		// Contains all the declarations specific to caSCADA
#include "modbus_open_close.h"	// Contains Modbus network parameters


// Declaring a time value structure for the nanosleep() function.
// The "timespec" structure is already defined in time.h, and 
// we're invoking an instance of it called "loopdelay" for our 
// purposes:
struct timespec loopdelay;



int
main (void)
{

  // Set loop execution delay time for the nanosleep() delay function
  loopdelay.tv_sec = 0;		// whole seconds
  loopdelay.tv_nsec = 1000000000;	// nanoseconds (1,000,000,000 nanoseconds = 1 second)
  					// nanoseconds (500,000,000 nanoseconds = 0.5 second)

  // Opens up Modbus network connection
  open_modbus_connection ();

// TO-DO!!!
// open cascada_poll_pointer.dat file ();



// Main loop
  while (1)
    {

      // Read all 14 analog input channels on the LabJack T7 DAQ.  This function happens to
      // store the 14 floating-point values in the global array "ain[]".
      read_LJT7_analogin ();


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

      // Write 2 analog output channels on the LabJack T7 DAQ.  This function happens to
      // read from the 2 floating-point values in the global array "aout[]".
      write_LJT7_analogout ();

      // Sleep for a specified amount of time so we don't waste network bandwidth
      // and processing power.  This is done using the awesome nanosleep() function.
      nanosleep (&loopdelay, NULL);
    }


  // Closes out Modbus network connection
  close_modbus_connection ();


  // TO-DO!!!
  // close cascada_poll_pointer.dat file ();


  // Force of habit here -- I like all functions to return *something*
  return 1;

}









int
read_LJT7_analogin (void)
{

  uint16_t inreg_word[28];
  int read_count, n;

  union
  {
    uint16_t word[2];
    uint8_t byte[4];
  } in;

  union
  {
    uint8_t byte[4];
    float real;
  } out;


  // Read twenty-eight sequential 16-bit Modbus registers, starting with 0
  read_count = modbus_read_registers (Device, 0, 28, inreg_word);


  // If read command is unsuccessful . . .
  if (read_count < 1)
    {
      fprintf (stderr,
	       "No data read from DAQ -- network possibly severed! \n");
    }


  // Populate the ain[] 32-bit floating-point array from inreg_word[]
  // 16-bit integer data just read from the LabJack T7, swapping the
  // byte order as well
  for (n = 0; n < 14; ++n)
    {

      in.word[0] = inreg_word[2 * n];
      in.word[1] = inreg_word[(2 * n) + 1];
      /*
         The LabJack T7 stores its floating-point variables in a
         weird byte-order format that follows the pattern CDAB.
         This algorithm swaps the bytes as needed!
       */
      out.byte[0] = in.byte[2];
      out.byte[1] = in.byte[3];
      out.byte[2] = in.byte[0];
      out.byte[3] = in.byte[1];

      // Copy the union's data to the respective analog input array element
      ain[n] = out.real;
    }

  // Force of habit here -- I like all functions to return *something*
  return 1;
}





int
write_LJT7_analogout (void)
{

  uint16_t outreg_word[4];
  int write_count, n;

  union
  {
    uint8_t byte[4];
    float real;
  } in;

  union
  {
    uint16_t word[2];
    uint8_t byte[4];
  } out;


  // Populate the outreg_word[] 16-bit integer array with the 32-bit
  // floating-point values residing in the aout[] array, in preparation
  // for writing to the LabJack T7
  for (n = 0; n < 2; ++n)
    {

      // Safeguards against an unreasonably low value being written to
      // the analog output DAC, with 0.00 volts being the low limit
      if (aout[n] < 0)
	aout[n] = 0.00;

      // Safeguards against an unreasonably high value being written to
      // the analog output DAC, with 5.00 volts being the low limit
      if (aout[n] > 5.00)
	aout[n] = 5.00;

      // Copy the analog output array element to the union data
      in.real = aout[n];
      /*
         The LabJack T7 stores its floating-point variables in a
         weird byte-order format that follows the pattern CDAB.
         This algorithm swaps the bytes as needed!
       */
      out.byte[2] = in.byte[0];
      out.byte[3] = in.byte[1];
      out.byte[0] = in.byte[2];
      out.byte[1] = in.byte[3];

      outreg_word[2 * n] = out.word[0];
      outreg_word[(2 * n) + 1] = out.word[1];
    }


  // Write four sequential 16-bit Modbus registers, starting with 1000
  write_count = modbus_write_registers (Device, 1000, 4, outreg_word);


  // If write command is unsuccessful . . .
  if (write_count < 1)
    {
      fprintf (stderr,
	       "No data written to DAQ -- network possibly severed! \n");
    }


  // Force of habit here -- I like all functions to return *something*
  return 1;
}
