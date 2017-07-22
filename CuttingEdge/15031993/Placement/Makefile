MAKE = make

CC  = cc
C++ = CC3.0.1
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   = -g
CPPFLAGS = -I../threads.new
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = 

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

LIB	= libThreads.a

OBJECTS	= Process.o ProcessIterator.o ProcessList.o ProcessCons.o \
	thread.o lwp_thread.o Random.o gnu_thread.o

SOURCES	= Process.cc ProcessIterator.cc ProcessList.cc ProcessCons.cc \
	thread.cc lwp_thread.cc Random.cc gnu_thread.cc

HEADERS	= ProcessCons.h ProcessIterator.h ProcessList.h common.h \
	thread.h lwp_thread.h Random.h gnu_thread.h

all: $(LIB) TestRandom

TestRandom: TestRandom.o $(OBJECTS) $(HEADERS)
	$(C++) -o TestRandom TestRandom.o $(LIB) $(LDFLAGS) $(LOCAL_LDFLAGS)

TestRandom.o: TestRandom.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) TestRandom.cc

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

gnu_thread.o: gnu_thread.cc gnu_thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) gnu_thread.cc

$(LIB) : $(OBJECTS)
	ar rv $@ $?
	ranlib $(LIB)

clean:
	rm -f *.o

vclean: clean
	rm -f *~* libThreads.a
