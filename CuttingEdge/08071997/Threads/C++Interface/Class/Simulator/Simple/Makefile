MAKE = make

CC  = cc
C++ = CC
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   = -g
CPPFLAGS =
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I../.. -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L../.. -lThreads \
	-L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= AC.o Machine.o MainBody.o Client.o Object.o \
	Placement.o Simulation.o RandomPolicy.o Protocol.o \
	SimplePolicy.o

SOURCES	= AC.cc Machine.cc MainBody.cc Client.cc Object.cc \
	Placement.cc Simulation.cc RandomPolicy.cc Protocol.cc \
	SimplePolicy.cc

HEADERS	= AC.h Machine.h MainBody.h Client.h Object.h \
	Placement.h RandomPolicy.h Protocol.h \
	SimplePolicy.h

all: Simulation

Simulation: $(OBJECTS) $(HEADERS)
	$(C++) -o Simulation $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

Simulation.o: Simulation.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Simulation.cc

AC.o: AC.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) AC.cc

Protocol.o: Protocol.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Protocol.cc

Machine.o: Machine.cc Machine.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Machine.cc

MainBody.o: MainBody.cc MainBody.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MainBody.cc

Client.o: Client.cc Client.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Client.cc

Object.o: Object.cc Object.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Object.cc

Placement.o: Placement.cc Placement.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Placement.cc

RandomPolicy.o: RandomPolicy.cc RandomPolicy.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RandomPolicy.cc

SimplePolicy.o: SimplePolicy.cc SimplePolicy.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) SimplePolicy.cc

clean:
	rm -f *.o

vclean: clean
	rm -f Simulation *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

AC.o: AC.h Protocol.h
Client.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
Client.o: ../../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Client.o: /usr/include/sys/sysmacros.h /usr/local/CC/2.1/include/iostream.h
Client.o: /usr/local/CC/2.1/include/memory.h
Client.o: /usr/local/CC/2.1/include/sys/time.h
Client.o: /usr/local/CC/2.1/include/sys/types.h
Client.o: /usr/local/CC/2.1/include/time.h Client.h Object.h Protocol.h
Machine.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
Machine.o: ../../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Machine.o: /usr/include/sys/sysmacros.h /usr/local/CC/2.1/include/iostream.h
Machine.o: /usr/local/CC/2.1/include/memory.h
Machine.o: /usr/local/CC/2.1/include/sys/time.h
Machine.o: /usr/local/CC/2.1/include/sys/types.h
Machine.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h Object.h
Machine.o: Protocol.h
MainBody.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
MainBody.o: ../../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
MainBody.o: /usr/include/sys/sysmacros.h /usr/local/CC/2.1/include/iostream.h
MainBody.o: /usr/local/CC/2.1/include/memory.h
MainBody.o: /usr/local/CC/2.1/include/sys/time.h
MainBody.o: /usr/local/CC/2.1/include/sys/types.h
MainBody.o: /usr/local/CC/2.1/include/time.h AC.h Client.h Machine.h
MainBody.o: MainBody.h Object.h Placement.h Protocol.h RandomPolicy.h
MainBody.o: SimplePolicy.h
Object.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
Object.o: ../../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Object.o: /usr/include/sys/sysmacros.h /usr/local/CC/2.1/include/iostream.h
Object.o: /usr/local/CC/2.1/include/memory.h
Object.o: /usr/local/CC/2.1/include/sys/time.h
Object.o: /usr/local/CC/2.1/include/sys/types.h
Object.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h Object.h
Object.o: Protocol.h
Placement.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
Placement.o: ../../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Placement.o: /usr/include/sys/sysmacros.h /usr/local/CC/2.1/include/iostream.h
Placement.o: /usr/local/CC/2.1/include/memory.h
Placement.o: /usr/local/CC/2.1/include/sys/time.h
Placement.o: /usr/local/CC/2.1/include/sys/types.h
Placement.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h Object.h
Placement.o: Placement.h Protocol.h RandomPolicy.h SimplePolicy.h
Protocol.o: Protocol.h
RandomPolicy.o: ../../Process.h ../../Random.h ../../common.h
RandomPolicy.o: ../../lwp_thread.h ../../thread.h /usr/include/lwp/lwp.h
RandomPolicy.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
RandomPolicy.o: /usr/local/CC/2.1/include/iostream.h
RandomPolicy.o: /usr/local/CC/2.1/include/memory.h
RandomPolicy.o: /usr/local/CC/2.1/include/sys/time.h
RandomPolicy.o: /usr/local/CC/2.1/include/sys/types.h
RandomPolicy.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h Object.h
RandomPolicy.o: Protocol.h RandomPolicy.h
SimplePolicy.o: ../../Process.h ../../Random.h ../../common.h
SimplePolicy.o: ../../lwp_thread.h ../../thread.h /usr/include/lwp/lwp.h
SimplePolicy.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
SimplePolicy.o: /usr/local/CC/2.1/include/iostream.h
SimplePolicy.o: /usr/local/CC/2.1/include/memory.h
SimplePolicy.o: /usr/local/CC/2.1/include/sys/time.h
SimplePolicy.o: /usr/local/CC/2.1/include/sys/types.h
SimplePolicy.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h Object.h
SimplePolicy.o: Protocol.h SimplePolicy.h
Simulation.o: ../../Process.h ../../Random.h ../../common.h ../../lwp_thread.h
Simulation.o: ../../thread.h /usr/include/lwp/lwp.h
Simulation.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
Simulation.o: /usr/local/CC/2.1/include/iostream.h
Simulation.o: /usr/local/CC/2.1/include/memory.h
Simulation.o: /usr/local/CC/2.1/include/sys/time.h
Simulation.o: /usr/local/CC/2.1/include/sys/types.h
Simulation.o: /usr/local/CC/2.1/include/time.h Client.h Machine.h MainBody.h
Simulation.o: Object.h Protocol.h
