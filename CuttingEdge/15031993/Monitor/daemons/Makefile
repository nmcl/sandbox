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

OBJECTS = MonitorDriver.o Monitor.o

SOURCES	= Monitor.cc

HEADERS	= Monitor.h

all: MonitorDriver

MonitorDriver: $(OBJECTS)
	$(C++) -o MonitorDriver $(OBJECTS) $(LIBRARYS) $(LDFLAGS) $(LOCAL_LDFLAGS)

MonitorDriver.o: MonitorDriver.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MonitorDriver.cc

Monitor.o: Monitor.cc Monitor.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Monitor.cc

clean:
	rm -f *.o

vclean: clean
	rm -f MonitorDriver *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
