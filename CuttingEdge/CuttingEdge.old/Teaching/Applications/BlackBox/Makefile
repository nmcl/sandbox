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

MDEPFLAGS =

LOCAL_CPPFLAGS = -I../Ports/include
LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L../Ports/lib -L../lib -lPorts -lCommon

all: Client BlackBox

BlackBox : BlackBox.o
	$(C++) -o BlackBox BlackBox.o $(LDFLAGS) $(LOCAL_LDFLAGS)

BlackBox.o : BlackBox.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) BlackBox.cc

Client : Client.o
	$(C++) -o Client Client.o $(LDFLAGS) $(LOCAL_LDFLAGS)

Client.o : Client.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Client.cc

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f BlackBox Client
