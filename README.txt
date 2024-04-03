============================================================
caSCADA version 3.5 -- an overview

caSCADA is a collection of programs designed to poll analog data from 
a data acquisition unit such as the LabJack model T7 and serve this data 
up in a form readable by other programs.  The general term for any such 
system is "Supervisory Control and Data Acquisition", or "SCADA".  This
system is simple and inexpensive to the point of being crude, so I call 
it the "cheap-ass Supervisory Control and Data Acquisition" or "caSCADA".
The fact that I live near the Cascade mountain range of the United
States is purely coincidental.  ;-)

The "poll" application in caSCADA is designed around the concept of 
"data channels", each one of which may or may not be directly 
associated with a real-world variable.  Each data channel is 
configured and conditioned by its own unique function (a segment of 
programming code), each function defined by a unique file (e.g. 
f_channel_00.c) for easy access and editing by multiple teams of 
programmers.  In other words, caSCADA is a system designed to be 
easily modified by a class full of students simultaneously learning 
C programming and SCADA system function.  Its educational bias is
clearly evident in its design.

In a student learning environment, each student (or team of students)
is assigned one channel in the caSCADA system.  That student (or team)
then decides where their channel's data will come from, and what 
computation (if any) needs to be done on the raw signal to make it
presentable as a real-world measurement.  For example, a student team
might be assigned caSCADA channel 4 to measure ambient air temperature.
This will mean that team must connect a suitable air temperature sensor
to one of the data acquisition unit's analog inputs and then edit the
source file "f_channel_04.c" with code that will manipulate the raw 
signal from that analog input into a form readable as air temperature
complete with appropriate units of measurement (e.g. degrees F or
degrees C).

caSCADA also offers closed-loop control capability with a different
application called "pid".  Proportional-Integral-Derivative control
is the most popular closed-loop control algorithm in industry, and
its implementation within caSCADA is fairly robust.  In addition to
basic single-loop PID control, "pid" also supports multiple analog
outputs for split-range valve control, feedforward control 
capability, and a user-adjustable integral deadband.

Furthermore, a physical process and PID control simulation package
called "looptune" is also provided in the caSCADA software suite.  This
is useful as an educational tool for learning how to tune PID 
controllers.  The "looptune" application uses the exact same PID 
control code as the "pid" application, and therefore exhibits all the
same features.

Lastly, a Modbus polling utility called "modprobe" is also included
within the caSCADA software suite.  This utility is useful when
initially configuring caSCADA to read data from a new data 
acquisition unit (i.e. anything other than a LabJack model T7).


============================================================
caSCADA AND MODBUS

caSCADA acquires its data from acquisition hardware through the use of
the "Modbus" communication protocol.  A separate C library called 
"libmodbus" provides this functionality, and must be linked with the
caSCADA code during compilation.  "libmodbus" was not developed by
me, but itself is an open-source project and may be downloaded freely
from the internet.  caSCADA was developed using version 3.0.6 of 
"libmodbus".

Being based on Modbus makes caSCADA extremely versatile: able to 
gather data from virtually any industrial DAQ with an Ethernet port
(using Modbus/TCP).  Although Modbus serial support is included with
the libmodbus library, caSCADA has not been written to take advantage
of that network functionality.  It should be noted that certain 
manufacturers such as Moxa sell Ethernet-to-serial converters 
designed to translate between Modbus/TCP and Modbus serial, and that
caSCADA has been successfully used in this capacity with a serial
Modbus device.

The code instructing caSCADA's "poll" application to establish a 
Modbus/TCP connection with the data acquisition unit is found within
the "cascada_poll.c" source code file.  It is within this file that 
we specify the IP network address of the data acquisition unit.

It should also be noted that this early version of caSCADA is coded
to communicate only with a LabJack model T7 data acquisition unit,
although Modbus theoretically makes it possible for caSCADA to talk
with many other DAQ devices.  One of the LabJack-specific details is
the translation of two 16-bit Modbus registers into one 32-bit
floating-point number for each of the LabJack's analog signals: the
byte order for this is CDAB, rather than ABCD or even DCBA.  Since
the Modbus standard does not specify how 32-bit floating-point
numbers will be represented by two successive 16-bit registers, it's
anyone's guess how the next device will be designed.  Suffice to say
if you wish to re-code caSCADA to communicate with some Modbus 
device other than a LabJack model T7, you may have some problem-solving
to do!

The "modprobe" utility which is part of caSCADA is useful in 
determining the byte order used to represent floating-point data in 
any Modbus device.  This utility will read successive 16-bit Modbus
registers and then show the equivalent 32-bit floating-point
representations of that data for several byte-swap orderings.  You
may then peruse those multiple representations to see which of them
makes the most sense.


============================================================
COMPILING THE CODE

When you write code in the C language, you are writing ASCII
characters to a text file, ending with the ".c" extension to
identify it as C-language source code.  This source code follows
conventions that make it readable to human beings familiar with
the C programming language, but this code is not yet in a form that
may be executed by the computer's microprocessor.  To translate the
C-language source code into binary code that is executable by the
microprocessor, the programmer must "compile" that code using a 
piece of software called a "compiler".

The most popular C-language compiler made for Linux operating systems
(by far) is called "gcc".  Like most Linux utilities it is invoked
at the command line by typing "gcc" followed by source filenames and
options instructing gcc how to do its job.  While the caSCADA source
code is simple enough to use gcc directly for compilation, an easier
method has been provided to invoke gcc which does not require full
knowledge of all the source code files and compilation options.

A special kind of file called a "Makefile" is used to guide the
compilation process.  If you make some changes to one or more of the
source code files and wish to re-compile, all you need to do is 
type the word "make" followed by the name of the executable file
you wish to compile.  For example, if you edit files that are part of
the "simulate" executable which populates the analog variables with
random numbers to simulate connection to a real LabJack T7, you 
would issue this command:

    make simulate

The Makefile then determine what needs to be compiled, and does all
the detailed work for you.  When you're done you will have an updated
file called "simulate" in that directory ready to run!

To run this executable as a background process (i.e. running silently
with no user interaction), you need to tell the Linux operating system
to run from the directory you're currently in, with an ampersand 
symbol to tell it to run in the background like this:

    ./simulate &

If you wish to halt this running process, you may use the "kill" 
command.  Note that every time you make a change to the caSCADA source
code and re-compile the executable, you will need to halt the old
version that's running before you start the new version!  I strongly
recommend using the "ps" command to check which processes are running
before killing anything.


============================================================
PROCESSES

First, let's clarify some terms.  In the world of computing, a "process" 
is a piece of software executed and managed by the operating system of a 
computer.  In the world of industrial instrumentation, a "process" is a 
physical system being monitored and/or controlled.  In this README 
document, I will use the term "process" in the computing sense of the 
word, not the instrumentation sense.

Multiple "processes" need to run on the computer in order for caSCADA 
system to function.  This section of the document lists the different 
processes and briefly describes each one's function:


-----------------
poll:

This process uses the industry-standard Modbus/TCP protocol to read 
analog data from the data acquisition hardware and condition the 
channel data.  In the version of caSCADA presented here, that hardware 
is the LabJack model T7 data acquisition unit (DAQ).  The "poll" 
process periodically reads all 14 analog input voltages from the 
LabJack DAQ and stores this data in an array of floating-point 
variables (ain[0] through ain[13]) residing in the computer's random-
access memory (RAM).

The "poll" process also writes to the LabJack's two analog outputs
from another floating-point array, this one consisting of aout[0]
and aout[1].  The LabJack model T7's analog outputs (labeled DAC0
and DAC1) are restricted to a 0-5 volt DC signal range, and so this
process also clamps the aout[n] array values to those limits.
 
The "poll" process then calls a series of functions to condition
floating-point data channels, which may or may not sample from the
analog input array or write to the analog output array.  This
process opens and writes to three data files located within the
same directory as the "poll" executable file:

  data.dat

  data.txt

  data.html

The "data.dat" data file is extremely small, containing a single 
memory pointer to tell other processes where the data structures 
begin in the computer's RAM memory.  Any other process able to read 
this pointer from the data file and having access to the data 
structure as defined in the "cascada.h" header file will then be 
able to seamlessly access all that data.  The "data.dat" pointer 
file is opened, written, and closed at the start-up of the poll 
process.  Once the poll process is running, the starting memory 
location for all the data structures does not change because they 
are all statically allocated (in other words, caSCADA does *not* 
use dynamic memory allocation).

The "data.txt" text file is a tab-delimited report showing the 
"tag", "value", "units", "status", and "comments" contents of the data
structure for each data channel.  This file is opened by poll upon 
start-up, kept open until that process terminates, and re-written with
every loop cycle.

The "data.html" file contains the exact same information as the 
"data.txt" file, but coded in HTML 4.0 format in the form of a table.
This may be easier for some user applications to read than the 
plain-text file.


-----------------
simulate:

This process uses a random number generator function to simulate the
data acquistion unit reading real voltage signals.  Thus, the analog
input variables (ain[0] through ain[13]) are continually updated with
new values so make it seem like the DAQ is sensing real signals.  
Like poll, this "simulate" process calls the same set of channel 
conditioning functions, and also writes data to the same three data 
files (a data file containing a pointer to the data structure, a text
file displaying current channel data, and an HTML file displaying 
current channel data in table format).

This "simulate" process is useful for testing and diagnostic purposes,
when there is no DAQ hardware for the real poll process to talk to.  
All processes and other applications designed to read the data or text 
files generated by the real poll process will be able to read the 
files generated by the simulate process.


-----------------
pid: 

This process reads analog data from AIN0 of the DAQ as the 
"process variable" and writes analog data to DAC0 of the
DAQ as the "manipulated variable" to form a single-loop control
system.  Standard PID control is the method of closed-loop
regulation.  An ncurses-bases display provides a crude ASCII-art
trend graph, with all functions accessible via function keys or
letter keys.

Feedforward control capability occurs using AIN1 of the DAQ
as the load ("wild") variable.  This is scaled into a percentage
and then multiplied by the feedforward gain (FFG) and added to 
the feedforward bias (FFB), and finally added to the PID output
percentage to drive the final control element.  The concept of
feedforward is to preemtptively move the final control element
in response to a measured load change in order to ensure the 
process variable remains stable without ever deviating from 
setpoint.

The ncurses-based trend graph uses lower-case letter characters
to display PV (p), SP (s), OUT (o), and LOAD (l).  The load pen
is optional, and turns off whenever the feedforward gain is 
set to less than 0.05 or greater than -0.05 (essentially 
whenever the feedforward gain is too low to be useful).  The
same is true of the load percentage display, underneath the 
OUT display percentage: it is visible only when FFG is set
to some usable value.

Tuning parameters include the following:

* K_P = Proportional gain (unitless)
* K_I = Integral gain (repeats per minute)
* IDB = Integral deadband (percent)
* K_D = Derivative gain (seconds)
* FFG = Feedforward gain (unitless)
* FFB = Feedforward bias (percent)

Integral deadband (IDB) is a very useful feature which turns
off integral action whenever the process variable gets close
enough to setpoint.  The IDB value in percentage is the 
threshold for "close enough".  This is very useful when the
final control element exhibits hysteresis, and integral 
control action tends to make it cycle back and forth as it
fruitlessly "hunts" for the perfect value.  Integral deadband
sacrifices perfect fidelity to setpoint in exchange for 
cessation of cycling.

When feedforward gain (FFG) is set to 0.0, all feedforward 
control action is turned off.  In order to "tune" feedforward
action, one must introduce load changes to the process while
disabling PID control, adjusting FFG and FFB such that the
process variable remains as undisturbed as possible by those
load changes.  Disabling PID control action is easily done 
by setting the algorithm to "Ideal" and setting K_P to zero.
Keeping the controller in automatic mode allows feedforward 
control action to remain working.

Manual mode forces total manual control of the final control
element.  In manual mode, neither the process variable input
(AIN0) nor the feedforward input (AIN1) nor the setpoint have
any effect at all on the controller's output (DAC0).


-----------------
looptune: 

This process simulates a physical system controlled by a PID
controller, using the same code and user interface of "pid"
but requiring no connection to real-world I/O.  The purpose
of this process is to practice tuning PID-controlled loops.

When looptune is initiated, it offers a selection of process
types to simulate, and also randomizes certain parameters so 
that every instance is unique.  READ THE INSTRUCTIONS 
carefully upon start-up, to ensure you don't miss any 
important details!

The process simulation function calculates a randomly-
walking "load" variable which may be plotted on the trend
graph at the user's discretion by setting the feedforward 
gain (FFG) to any value greater than 0.05 or less than 
-0.05.  Note that feedforward action isn't actually being
applied here, but we are merely using the FFG setting as
a convenient way to turn the load pen on and off.



-----------------
modprobe: 

This is a utility designed to read Modbus registers 
specified by the user, displaying them as integer values
as well as floating-point values (as pairs of 16-bit
registers) in multiple byte-swapped orders.  Modprobe is
useful when testing a new Modbus/TCP device, to learn 
where data is stored and in what format that data is 
stored.


============================================================
DATA FORMAT AND DATA CONDITIONING FUNCTIONS

All instrument data in the caSCADA system is organized around the 
principle of "channels".  A data channel may represent data acquired
directly from a data acquisition (DAQ) unit, or a value calculated 
by some portion of the caSCADA program.  Channels are not associated
with physical inputs or outputs unless you assign them that way 
within a function.  The act of assigning a channel to some particular 
source of data and manipulating that data for a particular purpose 
is called "conditioning" the data.

A fixed number of data channels are reserved in memory as an array 
of structures defined in the "cascada.h" header file.  You may view
the contents of this plain-text file using a text editor program
or dumping the file's contents to the terminal display using the
"cat" command.  

Here is what the structure array looks like in C code, as it appears
in the "cascada.h" header file:

   CODE LISTING:
   -------------------------
   |                       |
   |  struct data_fp       |
   |  {                    |
   |    float value;       |
   |    char *tag;         |
   |    char *unit;        |
   |    int status;        |
   |    char *comment;     |
   |  } f_channel[100];    |
   |                       |
   -------------------------

For those unfamiliar with the C programming language, this data 
structure reserves addresses in the computer's RAM memory for 100 data
channels, each channel addressable by a number (0 through 99).  
Within each of these structures of data type "data_fp" is a 
floating-point variable named "value", a string of ASCII characters
named "tag", a string of ASCII characters named "unit", an integer 
number named "status", and finally a string of ASCII characters named
"comment".  In order to address any of these variables, you must begin
with the particular structure in the array you're interested in 
(e.g. channel five would be addressed as "f_channel[5]") followed by
a period symbol (".") and then the name of the variable within the 
array.

For example, the elements within floating-point data channel 4 could
be assigned fixed values by the following instructions:

	f_channel[4].value = 78.342;
	f_channel[4].tag = "TT-137";
	f_channel[4].unit = "Deg F";
        f_channel[4].status = 1;
        f_channel[4].comment = "Reactor 3-B temperature";

Channel values are assigned within a function dedicated to each 
channel, the function periodically called by a repeating loop of 
execution in the "main" program function.  In this case, the function
for floating-point data channel 4 is named "f_channel_04" and is 
found in its own .c source file named "f_channel_04.c".  Within this
function we set the text string variables as we wish, calculating the
value and status variables by any algorithm we can think of.  

Normally, this is *not* how professional SCADA software would be 
written.  Normally, one would write a single function capable of 
conditioning any data channel, and pass variables to that function 
to tell it which channel to act upon and what it should do to the data
in that channel.  Writing a single data-conditioning function makes
the code smaller and easier to manage.  The reason I've opted for 
the one-function-per-channel philosophy (and writing each channel 
function to its own source file) is to enable multiple people to edit
their own channel functions independently from each other, without 
interfering with or needing to coordinate with anyone else.  In this 
way caSCADA is most definitely an academic system, designed to be 
simultaneously edited by a large number of student programmers.

Here are some examples of C code for assigning values to data channel
variables.  Feel free to sample any of this code when developing your
own function for the channel you've been assigned:




Example: making channel 7 represent analog input AIN3 on the LabJack 
device, sensing voltage and displaying that voltage measurement in
units of "volts".  Here we will also assign the ISA-standard tagname
of "EI-101" (voltage indicator number 101) to the measured voltage
value, and set the status variable equal to 1 which represents a good
status.  Note how the first variable assignment sets f_channel[7].value
equal to ain[3].  A different function periodically called in the 
caSCADA system loop reads all fourteen of the LabJack T7's analog 
inputs and puts those voltage values into the global ain[] variable 
array so *any* of the analog input voltage values may be accessed 
within *any* channel conditioning function:

SOURCE CODE LISTING FOR FILE "f_channel_07.c"
--------------------------------------------------------------------------------
|                                                                              |
| #include <stdio.h>                                                           |
| #include <math.h>      // Necessary for any advanced math functions          |
| #include "cascada.h"   // Contains all the declarations specific to caSCADA  |
|                                                                              |
| int f_channel_07(void)                                                       |
| {                                                                            |
|                                                                              |
|   f_channel[7].value = ain[3];                                               |
|   f_channel[7].tag = "EI-101";                                               |
|   f_channel[7].unit = "Volts";                                               |
|   f_channel[7].status = 1;                                                   |
|   f_channel[7].comment = "Analog input AIN3";                                |
|                                                                              |
|   return 1;                                                                  |
| }                                                                            |
|                                                                              |
--------------------------------------------------------------------------------





Example: making channel 11 the mathematical average of three voltages
from inputs AIN2, AIN3, and AIN4 on the LabJack device.  Since the 
analog input variables ("ain[]") are all global, we can access any and
all of them from this function:

SOURCE CODE LISTING FOR FILE "f_channel_11.c"
--------------------------------------------------------------------------------
|                                                                              |
| #include <stdio.h>                                                           |
| #include <math.h>      // Necessary for any advanced math functions          |
| #include "cascada.h"   // Contains all the declarations specific to caSCADA  |
|                                                                              |
| int f_channel_11(void)                                                       |
| {                                                                            |
|                                                                              |
|   f_channel[11].value = (ain[2] + ain[3] + ain[4]) / 3;                      |
|   f_channel[11].tag = "EI-208";                                              |
|   f_channel[11].unit = "Volts";                                              |
|   f_channel[11].status = 1;                                                  |
|   f_channel[11].comment = "Average of analog inputs AIN2, AIN3, and AIN4";   |
|                                                                              |
|   return 1;                                                                  |
| }                                                                            |
|                                                                              |
--------------------------------------------------------------------------------





Example: assign channel 6 to display cooling water pressure measured
by a 4-20 mA transmitter with a calibrated range of 0 to 35 PSI.  
This is done by taking the voltage sensed at analog input AIN9 of the 
LabJack (measured across a 250 ohm resistor connected to the 
transmitter) and scaling this into units of PSI using a linear 
equation of the form "y = mx + b".  Since 0 PSI will be represented 
by 0 volts and 35 PSI will be represented by 5 volts, the slope of 
this equation ("m") must be equal to 8.75 and the y-intercept of this
equation ("b") must be equal to -8.75.  

SOURCE CODE LISTING FOR FILE "f_channel_06.c"
--------------------------------------------------------------------------------
|                                                                              |
| #include <stdio.h>                                                           |
| #include <math.h>      // Necessary for any advanced math functions          |
| #include "cascada.h"   // Contains all the declarations specific to caSCADA  |
|                                                                              |
| int f_channel_06(void)                                                       |
| {                                                                            |
|                                                                              |
|   f_channel[6].value = (ain[9] * 8.75) - 8.75;                               |
|   f_channel[6].tag = "PT-103";                                               |
|   f_channel[6].unit = "PSI";                                                 |
|   f_channel[6].status = 1;                                                   |
|   f_channel[6].comment = "Cooling water pressure";                           |
|                                                                              |
|   return 1;                                                                  |
| }                                                                            |
|                                                                              |
--------------------------------------------------------------------------------






Example: making channel 15 select the median value of inputs AIN0, 
AIN1, and AIN2 on the LabJack device.  The algorithm selects the 
greater of every pair of inputs (0 versus 1, 1 versus 2, and 0 versus
2) then selects the least of those three values.  The result will be 
the middle (median) of the three inputs.  The median-selected value 
is frequently used in triple-redundant sensor systems to choose the 
best measurement of the three.  Note the use of "local" variables a, 
b, c, and result which are declared at the beginning of the function.
All the data structure variables and analog input variables are 
declared in the header file "cascada.h" and therefore are global in 
scope (i.e. accessible by any function which includes this header
file).  We need some other variables to implement this selector 
function, and so we made them up just for this function's use:

SOURCE CODE LISTING FOR FILE "f_channel_15.c"
--------------------------------------------------------------------------------
|                                                                              |
| #include <stdio.h>                                                           |
| #include <math.h>      // Necessary for any advanced math functions          |
| #include "cascada.h"   // Contains all the declarations specific to caSCADA  |
|                                                                              |
| int                                                                          |
| f_channel_15 (void)                                                          |
| {                                                                            |
|                                                                              |
|   float a, b, c, result;                                                     |
|                                                                              |
|   // a = greater of ain[0], ain[1]                                           |
|   if (ain[0] > ain[1])                                                       |
|     a = ain[0];                                                              |
|   else                                                                       |
|     a = ain[1];                                                              |
|                                                                              |
|   // b = greater of ain[1], ain[2]                                           |
|   if (ain[1] > ain[2])                                                       |
|     b = ain[1];                                                              |
|   else                                                                       |
|     b = ain[2];                                                              |
|                                                                              |
|   // c = greater of ain[0], ain[2]                                           |
|   if (ain[0] > ain[2])                                                       |
|     c = ain[0];                                                              |
|   else                                                                       |
|     c = ain[2];                                                              |
|                                                                              |
|   // result = least of a, b, c                                               |
|   if (a < b && a < c)                                                        |
|     result = a;                                                              |
|                                                                              |
|   if (b < a && b < c)                                                        |
|     result = b;                                                              |
|                                                                              |
|   if (c < a && c < b)                                                        |
|     result = c;                                                              |
|                                                                              |
|   f_channel[15].value = result;                                              |
|   f_channel[15].tag = "EI-322";                                              |
|   f_channel[15].unit = "Volts";                                              |
|   f_channel[15].status = 1;                                                  |
|   f_channel[15].comment = "Median of AIN0, 1, and 2";                        |
|                                                                              |
|   return 1;                                                                  |
| }                                                                            |
|                                                                              |
--------------------------------------------------------------------------------





Example: programming a channel to record the highest value seen
by an analog input.  This example uses a ``static'' variable 
inside the function which (unlike normal ``automatic'' variables
in C) doesn't forget its previous value upon each call of the 
function.

SOURCE CODE LISTING FOR FILE "f_channel_02.c"
--------------------------------------------------------------------------------
|                                                                              |
| #include <stdio.h>                                                           |
| #include <math.h>      // Necessary for any advanced math functions          |
| #include "cascada.h"   // Contains all the declarations specific to caSCADA  |
|                                                                              |
| int                                                                          |
| f_channel_02 (void)                                                          |
| {                                                                            |
|                                                                              |
|   static float highval;                                                      |
|                                                                              |
|   if (ain[2] > highval)                                                      |
|     highval = ain[2];                                                        |
|                                                                              |
|   f_channel[2].value = highval;                                              |
|   f_channel[2].tag = "Maximum voltage";                                      |
|   f_channel[2].unit = "Volts";                                               |
|   f_channel[2].status = 1;                                                   |
|   f_channel[2].comment = "Analog input AIN2";                                |
|                                                                              |
|   return 1;                                                                  |
| }                                                                            |
|                                                                              |
--------------------------------------------------------------------------------


