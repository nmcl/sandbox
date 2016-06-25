MAKE = make

CC  = cc
C++ = g++
CPP = gcc
LEX = lex

CFLAGS   = -g
CPPFLAGS =
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I../ -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L.. -lThreads \
	-L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= AC.o Machine.o MainBody.o Client.o Object.o \
	Placement.o Simulation.o RandomPolicy.o Protocol.o \
	SimplePolicy.o Voting.o

SOURCES	= AC.cc Machine.cc MainBody.cc Client.cc Object.cc \
	Placement.cc Simulation.cc RandomPolicy.cc Protocol.cc \
	SimplePolicy.cc Voting.o

HEADERS	= AC.h Machine.h MainBody.h Client.h Object.h \
	Placement.h RandomPolicy.h Protocol.h \
	SimplePolicy.h Voting.o

all: Simulation

Simulation: $(OBJECTS) $(HEADERS)
	$(C++) -o Simulation $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

Simulation.o: Simulation.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Simulation.cc

AC.o: AC.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) AC.cc

Voting.o: Voting.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Voting.cc

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
