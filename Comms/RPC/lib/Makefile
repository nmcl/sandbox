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
# $Id: Makefile,v 1.2 1992/01/28 11:48:22 nmcl Exp $
#
# "RPC" classes library makefile
#

MAKE = make

CC = cc
C++ = g++

CPPFLAGS =
CFLAGS =
C++FLAGS =
LDFLAGS =

MDEPFLAGS =

LOCAL_CPPFLAGS =  -I/usr/local/Arjuna/include -I../../Ports/include -I../include
LOCAL_CFLAGS = 
LOCAL_C++FLAGS =
LOCAL_LDFLAGS =

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

RPCLIB    =  libC++RPC.a

RPCOBJECTS = ClientManager.o \
             ServerManager.o \
             ClientRPC.o \
             ServerRPC.o

SOURCES =    ClientManager.cc \
             ServerManager.cc \
             ClientRPC.cc \
             ServerRPC.cc

all: $(RPCLIB)

ClientManager.o : ClientManager.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ClientManager.cc

ServerManager.o : ServerManager.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ServerManager.cc

ClientRPC.o : ClientRPC.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ClientRPC.cc

ServerRPC.o : ServerRPC.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ServerRPC.cc

$(RPCLIB): $(RPCOBJECTS)
	ar rv $@ $?
	ranlib $@

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f $(RPCLIB)
	$(STRIPDEPEND)

install: $(RPCLIB)

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

ClientManager.o: ../../Ports/include/Port.h ../../Ports/include/PortAddress.h
ClientManager.o: ../../Ports/include/Scatter.h ../include/ClientManager.h
ClientManager.o: ../include/Manager.h /usr/include/netinet/in.h
ClientManager.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
ClientManager.o: /usr/local/Arjuna/include/Arjuna/Buffer.h
ClientManager.o: /usr/local/Arjuna/include/Arjuna/commondefs.h
ClientManager.o: /usr/local/Arjuna/include/Std/boolean.h
ClientManager.o: /usr/local/Arjuna/include/Std/standard.h
ClientManager.o: /usr/local/include/CC/errno.h /usr/local/include/CC/malloc.h
ClientManager.o: /usr/local/include/CC/netdb.h /usr/local/include/CC/pwd.h
ClientManager.o: /usr/local/include/CC/rand48.h /usr/local/include/CC/regcmp.h
ClientManager.o: /usr/local/include/CC/stddef.h /usr/local/include/CC/stdio.h
ClientManager.o: /usr/local/include/CC/stdlib.h
ClientManager.o: /usr/local/include/CC/sys/socket.h
ClientManager.o: /usr/local/include/CC/sys/types.h
ClientManager.o: /usr/local/include/CC/sys/uio.h
ClientRPC.o: ../../Ports/include/Port.h ../../Ports/include/PortAddress.h
ClientRPC.o: ../../Ports/include/Scatter.h ../include/ClientManager.h
ClientRPC.o: ../include/ClientRPC.h ../include/RPC.h /usr/include/netinet/in.h
ClientRPC.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
ClientRPC.o: /usr/local/Arjuna/include/Arjuna/Buffer.h
ClientRPC.o: /usr/local/Arjuna/include/Arjuna/commondefs.h
ClientRPC.o: /usr/local/Arjuna/include/Std/boolean.h
ClientRPC.o: /usr/local/Arjuna/include/Std/standard.h
ClientRPC.o: /usr/local/include/CC/errno.h /usr/local/include/CC/malloc.h
ClientRPC.o: /usr/local/include/CC/netdb.h /usr/local/include/CC/pwd.h
ClientRPC.o: /usr/local/include/CC/rand48.h /usr/local/include/CC/regcmp.h
ClientRPC.o: /usr/local/include/CC/stddef.h /usr/local/include/CC/stdio.h
ClientRPC.o: /usr/local/include/CC/stdlib.h /usr/local/include/CC/sys/socket.h
ClientRPC.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/sys/uio.h
ServerManager.o: ../../Ports/include/Port.h ../../Ports/include/PortAddress.h
ServerManager.o: ../../Ports/include/Scatter.h ../include/Manager.h
ServerManager.o: ../include/ServerManager.h /usr/include/netinet/in.h
ServerManager.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
ServerManager.o: /usr/local/Arjuna/include/Arjuna/Buffer.h
ServerManager.o: /usr/local/Arjuna/include/Arjuna/commondefs.h
ServerManager.o: /usr/local/Arjuna/include/Std/boolean.h
ServerManager.o: /usr/local/Arjuna/include/Std/standard.h
ServerManager.o: /usr/local/include/CC/errno.h /usr/local/include/CC/malloc.h
ServerManager.o: /usr/local/include/CC/netdb.h /usr/local/include/CC/pwd.h
ServerManager.o: /usr/local/include/CC/rand48.h /usr/local/include/CC/regcmp.h
ServerManager.o: /usr/local/include/CC/stddef.h /usr/local/include/CC/stdio.h
ServerManager.o: /usr/local/include/CC/stdlib.h
ServerManager.o: /usr/local/include/CC/sys/socket.h
ServerManager.o: /usr/local/include/CC/sys/types.h
ServerManager.o: /usr/local/include/CC/sys/uio.h
ServerRPC.o: ../../Ports/include/Port.h ../../Ports/include/PortAddress.h
ServerRPC.o: ../../Ports/include/Scatter.h ../include/RPC.h
ServerRPC.o: ../include/ServerManager.h ../include/ServerRPC.h
ServerRPC.o: /usr/include/netinet/in.h /usr/include/sys/stdtypes.h
ServerRPC.o: /usr/include/sys/sysmacros.h
ServerRPC.o: /usr/local/Arjuna/include/Arjuna/Buffer.h
ServerRPC.o: /usr/local/Arjuna/include/Arjuna/commondefs.h
ServerRPC.o: /usr/local/Arjuna/include/Std/boolean.h
ServerRPC.o: /usr/local/Arjuna/include/Std/standard.h
ServerRPC.o: /usr/local/include/CC/errno.h /usr/local/include/CC/malloc.h
ServerRPC.o: /usr/local/include/CC/netdb.h /usr/local/include/CC/pwd.h
ServerRPC.o: /usr/local/include/CC/rand48.h /usr/local/include/CC/regcmp.h
ServerRPC.o: /usr/local/include/CC/stddef.h /usr/local/include/CC/stdio.h
ServerRPC.o: /usr/local/include/CC/stdlib.h /usr/local/include/CC/sys/socket.h
ServerRPC.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/sys/uio.h
