/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This function writes a single 32-bit floating-point value to a 
pair of contiguous 16-bit Modbus registers.  The first (starting)
address is passed to the function as an argument.  Another 
integer argument specifies the desired byte-ordering necessary
to piece together a 32-bit floating-point variable from two
16-bit integer values.

*************************************************************************/

#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters

// Declaring Modbus/TCP device pointer
modbus_t *Device;

int
write_32float (float real, int address, int byteswap)
{

  int write_count;

  union
  {
    uint16_t word[2];
    uint8_t byte[4];
  } out;

  union
  {
    uint8_t byte[4];
    float real;
  } in;

  in.real = real;

  /* Here is where we do the byte-swapping to format a 32-bit floating-point number:
     byteswap == 1 (DCBA byte order)
     byteswap == 2 (CDAB byte order)
     byteswap == 3 (BADC byte order)
     byteswap == [else] (ABCD byte order)
   */

  //  DCBA byte order
  if (byteswap == 1)
    {
      out.byte[0] = in.byte[3];
      out.byte[1] = in.byte[2];
      out.byte[2] = in.byte[1];
      out.byte[3] = in.byte[0];
    }

  //  CDAB byte order
  else if (byteswap == 2)
    {
      out.byte[0] = in.byte[2];
      out.byte[1] = in.byte[3];
      out.byte[2] = in.byte[0];
      out.byte[3] = in.byte[1];
    }

  //  BADC byte order
  else if (byteswap == 3)
    {
      out.byte[0] = in.byte[1];
      out.byte[1] = in.byte[0];
      out.byte[2] = in.byte[3];
      out.byte[3] = in.byte[2];
    }

  //  ABCD byte order (the default byte ordering)
  else
    {
      out.byte[0] = in.byte[0];
      out.byte[1] = in.byte[1];
      out.byte[2] = in.byte[2];
      out.byte[3] = in.byte[3];
    }

  // Writing two contiguous "Holding" registers
  if (address > 40000 && address < 50000)
    write_count =
      modbus_write_registers (Device, address - 40001, 2, out.word);

  // If address is out of range, print an error message
  else
    {
      fprintf (stderr,
	       "Invalid Modbus address for writing a floating-point variable! \n");
    }

  // If write command is unsuccessful, print an error message
  if (write_count < 1)
    {
      fprintf (stderr, "Unable to write data to the Modbus device! \n");
    }


  return write_count;

}
