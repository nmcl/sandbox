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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

LIB	= libThreads.a

OBJECTS	= Process.o ProcessIterator.o ProcessList.o ProcessCons.o \
	thread.o lwp_thread.o Random.o

SOURCES	= Process.cc ProcessIterator.cc ProcessList.cc ProcessCons.cc \
	thread.cc lwp_thread.cc Random.cc

HEADERS	= ProcessCons.h ProcessIterator.h ProcessList.h common.h \
	thread.h lwp_thread.h Random.h

all: $(LIB)

Process.o: Process.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Process.cc

ProcessCons.o: ProcessCons.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessCons.cc

ProcessIterator.o: ProcessIterator.cc ProcessIterator.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessIterator.cc

ProcessList.o: ProcessList.cc ProcessList.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessList.cc

Random.o: Random.cc Random.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Random.cc

thread.o: thread.cc thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) thread.cc

lwp_thread.o: lwp_thread.cc lwp_thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) lwp_thread.cc

$(LIB) : $(OBJECTS)
	ar rv $@ $?
	ranlib $(LIB)

clean:
	rm -f *.o

vclean: clean
	rm -f *~* libThreads.a
