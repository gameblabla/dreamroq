#
# Makefile for Linux
# It's mostly for conveniance and also so i don't have to recompile SDL2 each time.
#

CC = nspire-gcc
CXX = nspire-g++

OUTPUTNAME = roq.elf

DEFINES = -DALWAYS_16BIT -DAUDIO
INCLUDES = -I.
OPT_FLAGS  = -Os -std=gnu11 -fdata-sections -ffunction-sections -marm -march=armv5te -mtune=arm926ej-s

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
	$(CC) -o $(OUTPUTNAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(OBJS) $(OUTPUTNAME)
