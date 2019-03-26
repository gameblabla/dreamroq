#
# Makefile for Linux
# It's mostly for conveniance and also so i don't have to recompile SDL2 each time.
#

CC = nspire-gcc
CXX = nspire-g++

OUTPUTNAME = roq.elf

DEFINES = -DALWAYS_16BIT
INCLUDES = -I.
OPT_FLAGS  = -Ofast -marm -fno-PIC -fno-common -std=gnu11 -fdata-sections -ffunction-sections -marm -march=armv5te+nofp -mtune=arm926ej-s+nofp -flto

CFLAGS = $(DEFINES) $(INCLUDES) $(OPT_FLAGS)
LDFLAGS = -Wl,--as-needed -Wl,--gc-sections -flto

# Redream (main engine)
OBJS =  \
  nsp-dreamroq.o \
  dreamroqlib.o \
  n2DLib.o \
  ticks.o \
  main.o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $< 
	
	
all: executable

executable : $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUTNAME) $(OBJS)  $(LDFLAGS)

clean:
	rm $(OBJS) $(OUTPUTNAME)
