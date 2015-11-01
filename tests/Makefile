# Sources
SRCS = ../maze/*/*.c 

# Project name

PROJ_NAME=micromouse
OUTPATH=.

###################################################

CC = g++
CFLAGS = -g -O0 -Wall

###################################################

ROOT=$(shell pwd)

CFLAGS += -I. -I../boot -I../mouse  
CFLAGS += -I../mouse/feedback -I../mouse/motionprofile
CFLAGS += -I../maze/algorithm -I../maze/globals
CFLAGS += -D FAKE_MICROMOUSE

OBJS = $(SRCS:.c=.o)

###################################################

all: 
	#gcc -o stacktest $(CFLAGS) stacktest.c $(SRCS) -lm
	gcc -o mazetest $(CFLAGS) mazetest.c $(SRCS) -lm
	#gcc -o motion $(CFLAGS) motionprofile.c $(SRCS) -lm

clean:
	rm -f *.o
