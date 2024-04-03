/*************************************************************************
caSCADA -- Data acquisition system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

These functions manage the opening and closing of a Modbus/TCP 
connection, complete with error-checking and recovery.

*************************************************************************/

#include <stdio.h>
#include "modbus_open_close.h"	// Contains Modbus network parameters



int
open_modbus_connection (void)
{

  // Define Modbus/TCP device and port
  Device = modbus_new_tcp (IP_ADDRESS, TCP_PORT);


  // What to do if this function returns an error
  if (Device == NULL)
    {
      fprintf (stderr, "Unable to establish TCP port \n");
      fprintf (stderr, "Try diagnosing network problem using `ping' \n");
      return -1;
    }

  // This sets the slave ID address which is used in systems where
  // this program's Modbus/TCP packets get converted into serial
  // (e.g. RS-232, RS-422, RS-485) and received by a Modbus
  // ASCII or RTU slave device in serial form.  Serial Modbus 
  // devices absolutely require this slave address number within
  // the Modbus data frame, and so this function provides that
  // necessary slave address.  If the Device in question is a
  // native Modbus/TCP (i.e. does not use serial communication)
  // then the slave address is irrelevant.
  modbus_set_slave (Device, MODBUS_SLAVE);

  // This very important function call instructs the libmodbus
  // library functions to attempt recovery in the event of
  // network interruptions.  Without it, the default behavior
  // of libmodbus is to do nothing at all, leaving any network
  // error recovery to the developer!
  modbus_set_error_recovery (Device, MODBUS_ERROR_RECOVERY_LINK);


  while (modbus_connect (Device) == -1)
    {
      fprintf (stderr, "Trying to connect to the Modbus device\n");
    }


  // Force of habit here -- I like all functions to return *something*
  return 1;
}







int
close_modbus_connection (void)
{
  // Close Modbus connection
  modbus_close (Device);

  // Free Modbus device and TCP port
  modbus_free (Device);

  // Force of habit here -- I like all functions to return *something*
  return 1;
}
