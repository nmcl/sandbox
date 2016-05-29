MAKE = make

CC  = cc
C++ = CC
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   =
CPPFLAGS =
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/lib -lm

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= main.o Bus.o BusInterface.o Memory.o Ram.o Rom.o Connector.o \
	Processor.o MultiConnector.o

SOURCES	= main.cc Bus.cc BusInterface.cc Memory.cc Ram.cc Rom.cc Connector.cc \
	Processor.cc MultiConnector.cc

HEADERS	= common.h Bus.h BusInterface.h Memory.h Ram.h Rom.h Connector.h \
	Processor.h MultiConnector.h

all: main

main: $(OBJECTS) $(HEADERS)
	$(C++) -o main $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

main.o: main.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) main.cc

Bus.o: Bus.cc Bus.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Bus.cc

BusInterface.o: BusInterface.cc BusInterface.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) BusInterface.cc

Memory.o: Memory.cc Memory.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Memory.cc

Ram.o: Ram.cc Ram.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Ram.cc

Rom.o: Rom.cc Rom.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Rom.cc

Connector.o: Connector.cc Connector.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Connector.cc

Processor.o: Processor.cc Processor.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Processor.cc

MultiConnector.o: MultiConnector.cc MultiConnector.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MultiConnector.cc

clean:
	rm -f *.o

vclean: clean
	rm -f main *~*
