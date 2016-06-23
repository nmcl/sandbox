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
# $Id: Makefile,v 1.1 1992/01/28 11:46:00 nmcl Exp $
#
# "RPC daemon" makefile
#

MAKE = make

CC = cc
C++ = g++

CPPFLAGS = -DDEBUG
CFLAGS =
C++FLAGS =
LDFLAGS =

MDEPFLAGS =

LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I../Ports/include -I../RPC/include
LOCAL_CFLAGS = 
LOCAL_C++FLAGS =
LOCAL_LDFLAGS = -L/usr/local/Arjuna/lib -L../Ports/lib -L../RPC/lib -lC++RPC -lPorts -lArjuna

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

SOURCES = Client.cc Server.cc

all: Client Server

Client: Client.o
	$(C++) -o Client Client.o $(LOCAL_LDFLAGS) $(LDFLAGS)	

Client.o : Client.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Client.cc

Server: Server.o
	$(C++) -o Server Server.o $(LOCAL_LDFLAGS) $(LDFLAGS)	

Server.o : Server.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Server.cc

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	$(STRIPDEPEND)
	rm -f $(MANAGERPROG)

install: $(LIB)

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
