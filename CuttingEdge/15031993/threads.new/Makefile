#
# Copyright 1990, 1991,
# Computing Laboratory, University of Newcastle upon Tyne, UK.
#
# Permission to use, copy, modify and distribute this program for
# evaluation, teaching and/or research purposes only and without fee is
# hereby granted, providing that this copyright and permission notice
# appear on all copies and supporting documentation, and that similar
# conditions are imposed on any individual or organization to whom the
# program is distributed. The University of Newcastle upon Tyne makes no
# representation about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

#
# $Id: Makefile,v 1.2 1992/07/21 14:12:17 nmcl Exp $
#

TOP  = ../..
MAKE = make

CC = cc
C++ = CC3.0.1

MDEP = $(TOP)/Util/bin/makedepend
STRIPDEPEND = $(TOP)/Util/bin/stripdepend

CPPFLAGS =
CFLAGS =
C++FLAGS =
LDFLAGS =

MDEPFLAGS =

LOCAL_CPPFLAGS = -I. -I/usr/local/CC/3.0.1/include
LOCAL_CFLAGS = 
LOCAL_C++FLAGS =
LOCAL_LDFLAGS =  -L. -lGNUlwp
		
LOCAL_MDEPFLAGS = -I/usr/local/include/CC

LIB = libGNUlwp.a

HEADERS = gnulwp.h

SOURCES = clk.c gnulwp.c queue.c sem.c sig.c sun4.c

OBJ	= clk.o gnulwp.o queue.o sem.o sig.o sun4.o

all: $(LIB) timer producer bm

$(LIB):	$(OBJ)
	ar rv $(LIB) $(OBJ)
	ranlib $(LIB)

bm: bm.o $(LIB) $(HEADERS)
	$(C++) -o bm bm.o $(LDFLAGS) $(LOCAL_LDFLAGS)

bm.o: bm.c
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) bm.c

producer: producer.o $(LIB) $(HEADERS)
	$(C++) -o producer producer.o $(LDFLAGS) $(LOCAL_LDFLAGS)

producer.o: producer.c
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) producer.c

timer: timer.o $(LIB) $(HEADERS)
	$(C++) -o timer timer.o $(LDFLAGS) $(LOCAL_LDFLAGS)

timer.o: timer.c
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) timer.c

clk.o: clk.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) clk.c

gnulwp.o: gnulwp.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) gnulwp.c

sun4.o: sun4.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) sun4.c

queue.o: queue.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) queue.c

sem.o: sem.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) sem.c

sig.o: sig.c $(HEADERS)
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) sig.c

tidy:
	rm -f core *~ \#*\# *.bak *.BAK libGNUlwp.a producer timer bm

clean: tidy
	rm -f *.o 

purge: clean
	$(STRIPDEPEND)

install:

depend: 
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE
