/*************************************************************************
ModProbe -- a simple Modbus/TCP testing utility based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

This program reads arbitrary Modbus registers specified by the user.

*************************************************************************/

#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters


// Prototyping all functions
int convert_int_float (int);


// Declaring all global variables
uint16_t inreg_word[10000];
float value_ABCD;
float value_DCBA;
float value_CDAB;
float value_BADC;


int
main (void)
{

  int n;
  int address;
  int num;
  int read_count;

  // Opens up Modbus network connection
  open_modbus_connection ();


  printf ("Welcome to ModProbe!\n\n");

  while (1)
    {

      printf
	("\n\nEnter the absolute address of the Modbus register you wish to read, and the number of contiguous registers to be read.\n\n");

//    printf ("Addresses 00001 through 09999 = Discrete output `coil' registers \n");
//    printf ("Addresses 10001 through 19999 = Discrete input `contact' registers \n");
      printf ("Addresses 30001 through 39999 = Analog input registers \n");
      printf ("Addresses 40001 through 49999 = `Holding' registers \n\n");

      printf ("Absolute address = ");
      scanf ("%i", &address);

      printf ("Number of addresses to read = ");
      scanf ("%i", &num);

      // Prevents reading less than one register or more than 98
      if (num < 1)
	num = 1;

      if (num > 9998)
	num = 9998;

      // `coil' register address range
      if (address > 0 && address < 10000)
	{
	  printf
	    ("This function is not yet supported by modprobe -- sorry!\n\n");
	  close_modbus_connection ();
	  return -2;
	  // read_count = modbus_read_registers (Device, address, num, inreg_word);
	}

      // `contact' register address range
      else if (address > 10000 && address < 20000)
	{
	  printf
	    ("This function is not yet supported by modprobe -- sorry!\n\n");
	  close_modbus_connection ();
	  return -2;
	  // read_count = modbus_read_registers (Device, address, num, inreg_word);
	}

      // `analog input' register address range
      else if (address > 30000 && address < 40000)
	{
	  read_count =
	    modbus_read_input_registers (Device, address - 30001, num + 1,
					 inreg_word);
	}

      // `holding' register address range
      else if (address > 40000 && address < 50000)
	{
	  read_count =
	    modbus_read_registers (Device, address - 40001, num + 1,
				   inreg_word);
	}

      else
	{
	  printf ("That is an invalid address value!\n\n");
	  close_modbus_connection ();
	  return -2;
	}

      // If read command is unsuccessful, get out of ncurses display mode,
      // print an error message, and then return to ncurses display mode.
      if (read_count < 1)
	{
	  fprintf (stderr, "Unable to read data from the Modbus device! \n");
	  close_modbus_connection ();
	  return -2;
	}


      printf ("\n");


      for (n = 0; n < num; ++n)
	{
	  convert_int_float (n);
	  printf ("Register %i = %i\n", address + n, inreg_word[n]);
	  printf ("\t%i/%i FP (ABCD order) = %.2f \n", address + n,
		  address + n + 1, value_ABCD);
	  printf ("\t%i/%i FP (DCBA order) = %.2f \n", address + n,
		  address + n + 1, value_DCBA);
	  printf ("\t%i/%i FP (CDAB order) = %.2f \n", address + n,
		  address + n + 1, value_CDAB);
	  printf ("\t%i/%i FP (BADC order) = %.2f \n", address + n,
		  address + n + 1, value_BADC);
	}

    }



  // Closes out Modbus network connection
  close_modbus_connection ();


  // Force of habit here -- I like all functions to return *something*
  return 1;

}







/********************************************/





int
convert_int_float (int n)
{

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


  in.word[0] = inreg_word[n];
  in.word[1] = inreg_word[n + 1];

  //  Re-ordering the bytes according to the pattern ABCD
  out.byte[0] = in.byte[0];	// A
  out.byte[1] = in.byte[1];	// B
  out.byte[2] = in.byte[2];	// C
  out.byte[3] = in.byte[3];	// D
  value_ABCD = out.real;

  //  Re-ordering the bytes according to the pattern DCBA
  out.byte[0] = in.byte[3];	// D
  out.byte[1] = in.byte[2];	// C
  out.byte[2] = in.byte[1];	// B
  out.byte[3] = in.byte[0];	// A
  value_DCBA = out.real;

  //  Re-ordering the bytes according to the pattern CDAB
  out.byte[0] = in.byte[2];	// C
  out.byte[1] = in.byte[3];	// D
  out.byte[2] = in.byte[0];	// A
  out.byte[3] = in.byte[1];	// B
  value_CDAB = out.real;

  //  Re-ordering the bytes according to the pattern BADC
  out.byte[0] = in.byte[1];	// B
  out.byte[1] = in.byte[0];	// A
  out.byte[2] = in.byte[3];	// D
  out.byte[3] = in.byte[2];	// C
  value_BADC = out.real;

  // Force of habit here -- I like all functions to return *something*
  return 1;

}
