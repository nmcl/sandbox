MAKE = make

CC  = cc
C++ = CC3.0.1
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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I../ -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L../ -lThreads \
	-L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= AC.o Machine.o MainBody.o Client.o Object.o \
	Simulation.o RandomPolicy.o Protocol.o \
	Voting.o Passive.o SingleCopy.o

SOURCES	= AC.cc Machine.cc MainBody.cc Client.cc Object.cc \
	Simulation.cc RandomPolicy.cc Protocol.cc \
	Voting.cc Passive.cc SingleCopy.cc

HEADERS	= AC.h Machine.h MainBody.h Client.h Object.h \
	RandomPolicy.h Protocol.h \
	Voting.h Passive.h SingleCopy.h

all: Simulation

Simulation: $(OBJECTS) $(HEADERS)
	$(C++) -o Simulation $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

Simulation.o: Simulation.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Simulation.cc

AC.o: AC.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) AC.cc

Voting.o: Voting.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Voting.cc

Passive.o: Passive.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Passive.cc

SingleCopy.o: SingleCopy.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) SingleCopy.cc

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

RandomPolicy.o: RandomPolicy.cc RandomPolicy.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RandomPolicy.cc

clean:
	rm -f *.o Results* results.* Client_* Log nodeFile

vclean: clean
	rm -f Simulation *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
