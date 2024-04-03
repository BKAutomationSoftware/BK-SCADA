/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This function reads a single 16-bit integer value from a specified 
Modbus register.

RIGHT NOW IT ISN'T WORKING LIKE I WOULD WISH!
  --> It fails to read any *single* Modbus register with an even absolute address

*************************************************************************/

#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters
#include "cascada.h"

int
read_16int (int address)
{

  int read_count;
  uint16_t out;

  // Reading an "Analog Input" register
  if (address > 30000 && address < 40000)
    read_count =
      modbus_read_input_registers (Device, address - 30001, 1, &out);

  // Reading a "Holding" register
  else if (address > 40000 && address < 50000)
    read_count = modbus_read_registers (Device, address - 40001, 1, &out);

  // If address is out of range, print an error message
  else
    {
      fprintf (stderr,
	       "Invalid Modbus address for reading a 16-bit integer! \n");
      return 0;
    }

  // If read command is unsuccessful, print an error message
  if (read_count < 1)
    {
      fprintf (stderr, "Unable to read data from the Modbus device! \n");
      return 0;
    }

//   printf("Number of register read = %i  ", read_count);

  return (int) (out);

}
