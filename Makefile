# This is the Makefile for the caSCADA suite of programs
# By Tony R. Kuphaldt (2015-2018)
# Last update 10 May 2019

CC = gcc
CFLAGS = -Wall
SCADA_CHANNELS = f_channel_00.c f_channel_01.c f_channel_02.c f_channel_03.c f_channel_04.c f_channel_05.c f_channel_06.c f_channel_07.c f_channel_08.c f_channel_09.c f_channel_10.c f_channel_11.c f_channel_12.c f_channel_13.c f_channel_14.c f_channel_15.c f_channel_16.c f_channel_17.c f_channel_18.c f_channel_19.c f_channel_20.c f_channel_21.c f_channel_22.c f_channel_23.c f_channel_24.c f_channel_25.c f_channel_26.c f_channel_27.c f_channel_28.c f_channel_29.c f_channel_30.c f_channel_31.c 
SCADA_FILES = write_datatext.c write_datahtml.c
MODBUS_READWRITE = modbus_open_close.c read_32float.c write_32float.c
PID_CONTROL = pid.c controller.c
LOOP_SIM = looptune.c controller.c 
PROCESS_SIM = sim_process.c sim_valve.c 

all : Makefile
	make poll
	make simulate
	make pid
	make looptune
	make modprobe

poll : cascada_poll.c cascada.h modbus_open_close.h $(SCADA_CHANNELS) $(SCADA_FILES) $(MODBUS_READWRITE)
	$(CC) $(CFLAGS) -o poll cascada_poll.c $(SCADA_CHANNELS) $(SCADA_FILES) $(MODBUS_READWRITE) -lmodbus -lm

simulate : cascada_poll_simulate.c cascada.h $(SCADA_CHANNELS) $(SCADA_FILES) 
	$(CC) $(CFLAGS) -o simulate cascada_poll_simulate.c $(SCADA_CHANNELS) $(SCADA_FILES) -lm 

pid : pid.h controller.h modbus_open_close.h $(PID_CONTROL) $(MODBUS_READWRITE)
	$(CC) $(CFLAGS) -o pid $(PID_CONTROL) $(MODBUS_READWRITE) -lm -lmodbus -lncurses 

looptune : looptune.h controller.h $(LOOP_SIM) $(PROCESS_SIM)
	$(CC) $(CFLAGS) -o looptune $(LOOP_SIM) $(PROCESS_SIM) -lm -lncurses 

modprobe : modprobe.c modbus_open_close.h $(MODBUS_READWRITE)
	$(CC) $(CFLAGS) -o modprobe modprobe.c modbus_open_close.c -lmodbus 

restore : 
	tar xvf cascada.tar

clean : 
	rm -v *.o

VERSION = 3v7

archive :
	tar cvf cascada_$(VERSION).tar *.c *.h *.txt Makefile

