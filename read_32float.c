/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This function reads a single 32-bit floating-point value from a 
pair of contiguous 16-bit Modbus registers.  The first (starting)
address is passed to the function as an argument.  Another 
integer argument specifies the desired byte-ordering necessary
to piece together a 32-bit floating-point variable from two
16-bit integer values.

*************************************************************************/

#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters


float
read_32float (int address, int byteswap)
{

  int read_count;

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

  // Reading two contiguous "Analog Input" registers
  if (address > 30000 && address < 40000)
    read_count =
      modbus_read_input_registers (Device, address - 30001, 2, in.word);

  // Reading two contiguous "Holding" registers
  else if (address > 40000 && address < 50000)
    read_count = modbus_read_registers (Device, address - 40001, 2, in.word);

  // If address is out of range, print an error message
  else
    {
      fprintf (stderr,
	       "Invalid Modbus address for reading a floating-point variable! \n");
      return 0;
    }

  // If read command is unsuccessful, print an error message
  if (read_count < 1)
    {
      fprintf (stderr, "Unable to read data from the Modbus device! \n");
      return 0;
    }

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

  return out.real;

}
