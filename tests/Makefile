# Sources

LIBSRCS = ../mouse/*/*.c
SRCS = test1.c $(LIBSRCS)
SRCS2 = test2.c $(LIBSRCS)

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

OBJS = $(SRCS:.c=.o)

###################################################

all: 
	gcc -o test1 $(CFLAGS) $(SRCS) -lm
	gcc -o test2 $(CFLAGS) $(SRCS2) -lm
clean:
	rm -f *.o
	rm -f test1
	rm -f test2
