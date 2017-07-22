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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I.. -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L../ -lThreads \
	-L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= Arrivals.o Breaks.o Job.o Queue.o Machine.o Main.o MachineShop.o

SOURCES	= Arrivals.cc Breaks.cc Job.cc Queue.cc Machine.cc Main.cc MachineShop.cc

HEADERS	= Arrivals.h Breaks.h Job.h Queue.h Machine.h MachineShop.h

all: Main

Main: $(OBJECTS) $(HEADERS)
	$(C++) -o Main $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

Main.o: Main.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Main.cc

MachineShop.o: MachineShop.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MachineShop.cc

Arrivals.o: Arrivals.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Arrivals.cc

Breaks.o: Breaks.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Breaks.cc

Job.o: Job.cc Job.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Job.cc

Queue.o: Queue.cc Queue.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Queue.cc

Machine.o: Machine.cc Machine.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Machine.cc

clean:
	rm -f *.o

vclean: clean
	rm -f Main *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
