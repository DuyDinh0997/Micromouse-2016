LDIR=/usr/local/gcc-arm-none-eabi-4_9-2015q3/bin/
CC=$(LDIR)arm-none-eabi-gcc
AR=$(LDIR)arm-none-eabi-ar

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

vpath %.c buzzer feedback motionprofile

CFLAGS  = -g -O0 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Ibuzzer -Ifeedback -I../boot -Imotionprofile -Imotion -I. -Icelltocell

#SRCS  = stm32f4_discovery.c
SRCS = mouse.c 
SRCS += buzzer/buzzer.c 
SRCS += feedback/pid.c 
SRCS += motionprofile/trapProfile.c
SRCS += motion/updateType.c
SRCS += motion/motion.c
SRCS += celltocell/*.c

OBJS = $(SRCS:.c=.o)

.PHONY: libmouse.a

all: libmouse.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libmouse.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libmouse.a
