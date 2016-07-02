MAKE = make

CC  = gcc
C++ = g++
CPP = gcc
LEX = lex

MDEP = 
STRIPDEPEND = 

CFLAGS   = -g
CPPFLAGS = -DDEBUG
C++FLAGS = -g
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = 

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = 

LOCAL_MDEPFLAGS = 

INSTALLDIR = /usr/local/bin

OBJECTS = MonitorDriver.o Monitor.o common.o

SOURCES	= MonitorDriver.cc Monitor.cc pingdaemon.cc common.cc Download.cc

HEADERS	= Monitor.h Common.h

all: MonitorDriver pingdaemon Download

MonitorDriver: $(OBJECTS)
	$(C++) -o MonitorDriver $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

MonitorDriver.o: MonitorDriver.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MonitorDriver.cc

Monitor.o: Monitor.cc Monitor.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Monitor.cc

common.o: common.cc Common.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) common.cc

pingdaemon: pingdaemon.o
	$(C++) -o pingdaemon pingdaemon.o common.o $(LDFLAGS) $(LOCAL_LDFLAGS)

pingdaemon.o: pingdaemon.cc Common.h common.o
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) pingdaemon.cc

Download: Download.o
	$(C++) -o Download Download.o common.o $(LDFLAGS) $(LOCAL_LDFLAGS)

Download.o: Download.cc Common.h common.o
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Download.cc

install: all
	cp MonitorDriver $(INSTALLDIR)/.
	cp pingdaemon $(INSTALLDIR)/.
	cp Download $(INSTALLDIR)/.
	chmod a+x $(INSTALLDIR)/MonitorDriver $(INSTALLDIR)/pingdaemon $(INSTALLDIR)/Download

clean:
	rm -f *.o

purge: clean
	rm -f MonitorDriver pingdaemon Download *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
