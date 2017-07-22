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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/CC/3.0.1/Debug/include -I../ -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = 

LOCAL_MDEPFLAGS = -I/usr/local/CC/3.0.1/include

LIBRARYS = -L/usr/local/Arjuna/CC/3.0.1/Debug/lib -lArjuna -lArjServers -lStub -lRajdoot -lCommon \
	   -L/usr/lib -lrpcsvc

OBJECTS = TrackerDriver.o Dependency.o DependencyStore.o Tracker.o

SOURCES	= Dependency.cc DependencyStore.cc

HEADERS	= Dependency.h DependencyStore.h

all: TrackerDriver

TrackerDriver: $(OBJECTS)
	$(C++) -o TrackerDriver $(OBJECTS) $(LIBRARYS) $(LDFLAGS) $(LOCAL_LDFLAGS)

TrackerDriver.o: TrackerDriver.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) TrackerDriver.cc

Dependency.o: Dependency.cc Dependency.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Dependency.cc

DependencyStore.o: DependencyStore.cc DependencyStore.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) DependencyStore.cc

Tracker.o: Tracker.cc Tracker.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Tracker.cc

clean:
	rm -f *.o

vclean: clean
	rm -f TrackerDriver DependencyData *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
