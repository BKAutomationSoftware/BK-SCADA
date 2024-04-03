/*************************************************************************
test.c

This program is used to test out some of my own Modbus read/write
functions, built on LibModbus function calls.  It is an entirely
stand-alone program, using only the code in this source file.
It could be made more elegant by using some of the shared code
in other caSCADA source files, but the point here is to 
demonstrate how to everything in one place, as a tutorial of sorts
for anyone learning to write Modbus code.

My original use of this code was to poll data from an Emerson 
"Smart Wireless Gateway", which in turn read data from a pair of 
WirelessHART industrial transmitters (sensors).

Compile with this command-line instruction:

    gcc -Wall -o test test.c -lmodbus


*************************************************************************/

#include <stdio.h>
#include <unistd.h>		// Necessary for the sleep() function
#include <modbus.h>		// Necessary for all modbus connection and data read/write functions

#define IP_ADDRESS "169.254.8.1"
#define TCP_PORT 502

// Prototyping functions
int read_16int (int address);
float read_32float (int address, int byteswap);


// Declaring Modbus/TCP device pointer
modbus_t *Device;


int
main (void)
{

  // Define Modbus/TCP device and port
  Device = modbus_new_tcp (IP_ADDRESS, TCP_PORT);


  // What to do if this function returns an error
  if (Device == NULL)
    {
      fprintf (stderr, "Unable to establish TCP port \n");
      return -1;
    }


  // This very important function call instructs the libmodbus
  // library functions to attempt recovery in the event of
  // network interruptions.  Without it, the default behavior
  // of libmodbus is to do nothing at all, leaving any network
  // error recovery to the user!
  modbus_set_error_recovery (Device, MODBUS_ERROR_RECOVERY_LINK);

  // Establish a Modbus connection and what to do if this fails
  if (modbus_connect (Device) == -1)
    {
      fprintf (stderr, "Unable to connect to Modbus device \n");
      modbus_free (Device);
      return -1;
    }




  // Main loop
  while (1)
    {

//      printf("Register 30001 = %i  \n", read_16int(30001));
//      printf("Register 30002 = %i  \n", read_16int(30002));
//      printf("Register 30015 = %i  \n", read_16int(30015));
//      printf("Register 30016 = %i  \n", read_16int(30016));
      printf ("Register 40001 = %i  \n", read_16int (40001));
      printf ("Register 42001 = %i  \n", read_16int (42001));
      printf ("Register 42002 = %i  \n", read_16int (42002));
      printf ("AIN0 = %f Volts \n", read_32float (40001, 2));
      printf ("AIN1 = %f Volts \n", read_32float (40003, 2));
      printf ("\n");
//      printf("TT-41 case temperature = %f deg F \n", read_32float(30005, 0));
//      printf("TT-41 battery voltage = %f Volts \n", read_32float(30007, 0));
//      printf("PT-44 pressure = %f Inches water column \n", read_32float(30011, 0));
//      printf("PT-44 case temperature = %f deg C \n", read_32float(30015, 0));
//      printf("PT-44 battery voltage = %f Volts \n", read_32float(30017, 0));

      sleep (1);

    }



  // Close Modbus connection
  modbus_close (Device);

  // Free Modbus device and TCP port
  modbus_free (Device);

  // Force of habit here -- I like all functions to return *something*
  return 1;

}












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
