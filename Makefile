#
# makefile for chiptest and scope programs
# CS241 mp4
#
# make all1  for chiptest, SAPC and UNIX versions
#
# make all2  for scope, SAPC and UNIX versions
#
# make all3  for all2 plus fscope.lnx
#
# We require warning free ansi compatible C:
#
WFLAGS = -Wall -Wstrict-prototypes -Wmissing-prototypes \
		-Wno-uninitialized -Wshadow -ansi \
		-D__USE_FIXED_PROTOTYPES__

# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module
PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include
PC_CC  = $(SAPC_GNUBIN)/i386-gcc

# tell gcc to use $(PC_INC), for stdio, hardware headers--
# Here we just do debug version, it's plenty fast for this
PC_CFLAGS = $(WFLAGS) -I$(PC_INC) -g

PC_AS   = $(SAPC_GNUBIN)/i386-as
PC_LD   = $(SAPC_GNUBIN)/i386-ld
PC_NM   = $(SAPC_GNUBIN)/i386-nm

# SAPC File suffixes:
# .c	C source (often useful both for UNIX host and SAPC)
# .s 	assembly language source
# .opc  relocatable machine code, initialized data, etc., for SA PC
# .lnx  executable image (bits as in memory), for SA PC (Linux a.out format)
# .syms text file of .lnx's symbols and their values (the "symbol table")
# Symbol file "syms"--for most recently built executable in directory

# UNIX setup
CC = gcc
# Basic flags: warnings, debug flag
CFLAGS = $(WFLAGS) -g

all1: chiptest.lnx chiptest

all2: scope.lnx scope

all3: scope.lnx scope fscope.lnx

CHIP_OFILES = chiptest.o ls00_chip.o ls138_chip.o
CHIP_OPCFILES = chiptest.opc ls00_chip.opc ls138_chip.opc

# PC executable--tell ld to start code at 0x1000e0, load special startup
# module, special PC C libraries--
# Code has 0x20 byte header, so use "go 100100" (hopefully easier to
# remember than 100020)
# loads CHIP_OPCFILES

chiptest.lnx: $(CHIP_OPCFILES)
	$(PC_LD) -N -Ttext 1000e0 -o chiptest.lnx \
	$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
	$(CHIP_OPCFILES) $(PC_LIB)/libc.a

ls00_chip.opc: ls00_chip.c chip.h
	$(PC_CC) $(PC_CFLAGS) -c -o ls00_chip.opc ls00_chip.c
ls138_chip.opc: ls138_chip.c chip.h
	$(PC_CC) $(PC_CFLAGS) -c -o ls138_chip.opc ls138_chip.c
chiptest.opc: chiptest.c chip.h
	$(PC_CC) $(PC_CFLAGS) -c -o chiptest.opc chiptest.c

# ************** UNIX build of chiptest ***********

chiptest: $(CHIP_OFILES)
	$(CC) $(CFLAGS) -o chiptest $(CHIP_OFILES)

#make knows how to compile .c to .o, just need "dependencies"--
ls00_chip.o: ls00_chip.c chip.h

########################################################################
#
#   scope for UNIX, scope and fscope for SAPC
#
SCOPE_OFILES = scope.o ccollect.o
SCOPE_OPCFILES = scope.opc ccollect.opc
FSCOPE_OPCFILES = scope.opc ascollect.opc


scope.lnx: $(SCOPE_OPCFILES)
	$(PC_LD) -N -Ttext 1000e0 -o scope.lnx \
		$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
		$(SCOPE_OPCFILES) $(PC_LIB)/libc.a
	$(PC_NM) -n scope.lnx> syms

#scope with assembler collect--"fast" scope--only for SAPC
fscope.lnx: $(FSCOPE_OPCFILES)
	$(PC_LD) -N -Ttext 1000e0 -o fscope.lnx \
		$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
		$(FSCOPE_OPCFILES) $(PC_LIB)/libc.a
	$(PC_NM) -n fscope.lnx> syms

scope.opc: scope.c
	$(PC_CC) $(PC_CFLAGS) -c -o scope.opc scope.c

ccollect.opc: ccollect.c
	$(PC_CC) $(PC_CFLAGS) -c -o ccollect.opc ccollect.c

ascollect.opc: ascollect.s
	$(PC_AS) -o ascollect.opc  ascollect.s

# ************* UNIX build of scope--

scope:	$(SCOPE_OFILES)
	$(CC) -g -o scope $(SCOPE_OFILES)

ccollect.o: ccollect.c
scope.o: scope.c
# **************end of UNIX part for scope********

clean:
	rm -f *.opc *.lnx *.o chiptest scope core

