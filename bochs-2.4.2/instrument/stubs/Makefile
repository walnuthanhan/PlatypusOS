# Copyright (C) 2001  MandrakeSoft S.A.
#
#   MandrakeSoft S.A.
#   43, rue d'Aboukir
#   75002 Paris - France
#   http://www.linux-mandrake.com/
#   http://www.mandrakesoft.com/
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA



.SUFFIXES: .cc

srcdir = .


SHELL = /bin/sh



CC = gcc
CFLAGS = -pipe -O3 -I/sw/include -fomit-frame-pointer -finline-functions -falign-loops=16 -falign-jumps=16 -falign-functions=16 -falign-labels=16 -falign-loops-max-skip=15 -falign-jumps-max-skip=15 -fprefetch-loop-arrays  -fpascal-strings -fno-common -Wno-four-char-constants -Wno-unknown-pragmas -Dmacintosh -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES
CXX = g++
CXXFLAGS = -pipe -O3 -I/sw/include -fomit-frame-pointer -finline-functions -falign-loops=16 -falign-jumps=16 -falign-functions=16 -falign-labels=16 -falign-loops-max-skip=15 -falign-jumps-max-skip=15 -fprefetch-loop-arrays  -fpascal-strings -fno-common -Wno-four-char-constants -Wno-unknown-pragmas -Dmacintosh -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES

LDFLAGS = -L/sw/lib
LIBS =  -ldl
RANLIB = ranlib


# ===========================================================
# end of configurable options
# ===========================================================


BX_OBJS = \
  instrument.o

BX_INCLUDES =

BX_INCDIRS = -I../.. -I$(srcdir)/../.. -I. -I$(srcdir)/.

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(BX_INCDIRS) $< -o $@


.c.o:
	$(CC) -c $(CFLAGS) $(BX_INCDIRS) $< -o $@



libinstrument.a: $(BX_OBJS)
	rm -f  libinstrument.a
	ar rv $@ $(BX_OBJS)
	$(RANLIB) libinstrument.a

$(BX_OBJS): $(BX_INCLUDES)


clean:
	rm -f  *.o
	rm -f  *.a

dist-clean: clean
	rm -f  Makefile
