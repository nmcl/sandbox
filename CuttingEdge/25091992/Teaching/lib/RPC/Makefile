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

LOCAL_CPPFLAGS = -I../../include
LOCAL_C++FLAGS =
LOCAL_LDFLAGS =

OBJECTS = Client.o \
          Server.o

SOURCES = Client.cc \
          Server.cc

LIBRARY = libRPC.a

all: $(LIBRARY)

Client.o : Client.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Client.cc

Server.o : Server.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Server.cc

$(LIBRARY): $(OBJECTS)
	ar rv $@ $?
	ranlib $@

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f $(LIBRARY)
