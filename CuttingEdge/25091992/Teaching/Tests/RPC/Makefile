#
# Copyright 1990, 1991, 1992,
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
# $Id$
#


MAKE = make

C++ = CC

CPPFLAGS =
C++FLAGS =
LDFLAGS =

LOCAL_CPPFLAGS = -I../../include -I../include
LOCAL_C++FLAGS =
LOCAL_LDFLAGS = -L../../lib -L../lib -lRPC -lPorts -lCommon

PROGRAMS = Producer Consumer Manager 

all: $(PROGRAMS)

Producer: Producer.o
	$(C++) -o Producer Producer.o $(LOCAL_LDFLAGS) $(LDFLAGS)

Producer.o : Producer.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Producer.cc

Consumer: Consumer.o
	$(C++) -o Consumer Consumer.o $(LOCAL_LDFLAGS) $(LDFLAGS)

Consumer.o : Consumer.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Consumer.cc

Manager: Manager.o
	$(C++) -o Manager Manager.o $(LOCAL_LDFLAGS) $(LDFLAGS)

Manager.o : Manager.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Manager.cc

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f $(PROGRAMS)
