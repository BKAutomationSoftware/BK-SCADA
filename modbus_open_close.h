/*************************************************************************
caSCADA -- PID control system program based on libmodbus 3.0.6
By Tony R. Kuphaldt
Last update 10 May 2019

This software is released under the CC0 1.0 Universal license,
which is equivalent to Public Domain.

*************************************************************************/





#include <modbus.h>		// Necessary for all modbus connection and 
                                // data read/write functions, and is included
				// within this header file for the sake of
				// declaring the Modbus pointer ("*Device")




/**************************************************************************** 
		Define Modbus network parameters 
****************************************************************************/

#define IP_ADDRESS "169.254.8.1"	// IPv4 address of the Modbus/TCP data acquisition unit
#define TCP_PORT 502		// TCP port number used for Modbus/TCP (502 is standard)
#define MODBUS_SLAVE 1		// Slave device address used for serial Modbus






/**************************************************************************** 
		Declare Modbus network pointers 
****************************************************************************/

// Declaring Modbus/TCP device pointer
modbus_t *Device;







/****************************************************************************
			Function prototypes 
****************************************************************************/

// Prototype for initializing the Modbus network connection
int open_modbus_connection (void);

// Prototype for closing the Modbus network connection
int close_modbus_connection (void);



