# Sources
SRCS = ../maze/*/*.c ../mouse/motionprofile/*.c 

# Project name

PROJ_NAME=micromouse
OUTPATH=.

###################################################

CC = g++
CFLAGS = -g -O0 -Wall

###################################################

ROOT=$(shell pwd)

CFLAGS += -I. -I../boot -I../mouse  
CFLAGS += -I../mouse/feedback -I../mouse/motionprofile -I../mouse/menu -I../mouse/motion
CFLAGS += -I../maze/algorithm -I../maze/globals -I../maze/generators
CFLAGS += -D FAKE_MICROMOUSE

OBJS = $(SRCS:.c=.o)

###################################################

all: 
	#gcc -o stacktest $(CFLAGS) stacktest.c $(SRCS) -lm
	#gcc -o menutest $(CFLAGS) menutest.c $(SRCS) -lm
	#gcc -o dfttest $(CFLAGS) dfttest.c $(SRCS) -lm
	#gcc -o mazetest $(CFLAGS) mazetest.c $(SRCS) -lm
	gcc -o motion $(CFLAGS) motionprofile.c $(SRCS) -lm

clean:
	rm -f *.o
	find -type f -executable -delete
