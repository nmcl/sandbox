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
# $Id: Makefile,v 1.2 1991/08/19 18:44:04 nsmw Exp $
#
# "Ports" classes library makefile
#

MAKE = make

CC = cc
C++ = CC

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND= /usr/local/Arjuna/bin/stripdepend

CPPFLAGS =
CFLAGS =
C++FLAGS =
LDFLAGS =

MDEPFLAGS =

LOCAL_CPPFLAGS =  -I/usr/local/Arjuna/include -I../include
LOCAL_CFLAGS = 
LOCAL_C++FLAGS =
LOCAL_LDFLAGS =

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

PORTLIB    =     libPorts.a

PORTOBJECTS =    Port.o \
                 PortAddress.o

SOURCES =        Port.cc \
                 PortAddress.cc

all: $(PORTLIB)

Port.o : Port.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Port.cc

PortAddress.o : PortAddress.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) PortAddress.cc

$(PORTLIB): $(PORTOBJECTS)
	ar rv $@ $?
	ranlib $@

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f $(PORTLIB) $(MANAGERLIB)
	$(STRIPDEPEND)

install: $(PORTLIB) $(MANAGERLIB)

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
