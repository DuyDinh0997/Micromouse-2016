# Sources

SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c utils.c
SRCS += boot/*.c mouse/*.c mouse/buzzer/*.c mouse/feedback/*.c mouse/motionprofile/*.c

# Project name

PROJ_NAME=micromouse
OUTPATH=build

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O0 -Wall -Tstm32f4/stm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
CFLAGS += -msoft-float
endif


###################################################

vpath %.c stm32f4
vpath %.a stm32f4/lib

ROOT=$(shell pwd)

CFLAGS += -I. -Istm32f4 -Istm32f4/lib -Istm32f4/lib/inc -Iboot -Imouse 
CFLAGS += -Istm32f4/lib/inc/core -Istm32f4/lib/inc/peripherals  
CFLAGS += -Imouse/feedback -Imouse/motionprofile

SRCS += stm32f4/lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C stm32f4/lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Lstm32f4/lib -lstm32f4 -lm
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C stm32f4/lib # Remove this line if you don't want to clean the libs as well
	