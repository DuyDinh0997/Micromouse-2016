# Sources
SRCS = ../maze/*/*.c ../mouse/motionprofile/*.c ../mouse/*.c ../boot/processorMock.c
SRCS += ../mouse/feedback/*.c ../boot/setupCircularBuffer.c ../mouse/buzzer/*.c
# Project name

PROJ_NAME=micromouse
OUTPATH=.

###################################################

CC = g++
CFLAGS = -g -O0 -Wall -pthread 

###################################################

ROOT=$(shell pwd)

CFLAGS += -I. -I../boot -I../mouse  
CFLAGS += -I../mouse/feedback -I../mouse/motionprofile -I../mouse/menu -I../mouse/motion
CFLAGS += -I../maze/algorithm -I../maze/globals -I../maze/generators -I../boot
CFLAGS += -D FAKE_MICROMOUSE

OBJS = $(SRCS:.c=.o)

###################################################

all: 
	#gcc -o stacktest $(CFLAGS) stacktest.c $(SRCS) -lm
	#gcc -o menutest $(CFLAGS) menutest.c $(SRCS) -lm
	#gcc -o dfttest $(CFLAGS) dfttest.c $(SRCS) -lm
	#gcc -o mazetest $(CFLAGS) mazetest.c $(SRCS) -lm
	gcc -o motion $(CFLAGS) motionprofile.c $(SRCS) -lm
	gcc -o sim $(CFLAGS) simulator.c $(SRCS) -lm

clean:
	rm -f *.o
	find -type f -executable -delete
