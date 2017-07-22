MAKE = make

CC  = cc
C++ = /usr/Local/OldLocal/CC/2.1/bin/CC
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   = -g
CPPFLAGS = -I../Threads/GnuThreads
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = 

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/lib -lm

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

LIB	= libThreads.a

OBJECTS	= Process.o ProcessIterator.o ProcessList.o ProcessCons.o \
	   Link.o Head.o Set.o Element.o \
	   thread.o lwp_thread.o Random.o gnu_thread.o

SOURCES	= Process.cc ProcessIterator.cc ProcessList.cc ProcessCons.cc \
	  Link.cc Head.cc Set.cc Element.cc \
	  thread.cc lwp_thread.cc Random.cc gnu_thread.cc

HEADERS	= ProcessCons.h ProcessIterator.h ProcessList.h common.h \
	  Link.h Head.h Set.h Element.h \
	  thread.h lwp_thread.h Random.h gnu_thread.h

INLINES = Element.n Head.n Link.n Process.n ProcessCons.n

all: $(LIB)

Process.o: Process.cc Process.h Process.n
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Process.cc

ProcessCons.o: ProcessCons.cc ProcessCons.h ProcessCons.n
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessCons.cc

ProcessIterator.o: ProcessIterator.cc ProcessIterator.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessIterator.cc

ProcessList.o: ProcessList.cc ProcessList.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ProcessList.cc

Link.o: Link.cc Link.h Link.n
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Link.cc

Head.o: Head.cc Head.h Head.n
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Head.cc

Set.o: Set.cc Set.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Set.cc

Element.o: Element.cc Element.h Element.n
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Element.cc

Random.o: Random.cc Random.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Random.cc

thread.o: thread.cc thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) thread.cc

lwp_thread.o: lwp_thread.cc lwp_thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) lwp_thread.cc

gnu_thread.o: gnu_thread.cc gnu_thread.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) gnu_thread.cc

TestRandom: TestRandom.o $(OBJECTS) $(HEADERS) $(INLINES)
	$(C++) -o TestRandom TestRandom.o $(LIB) $(LDFLAGS) $(LOCAL_LDFLAGS)

TestRandom.o: TestRandom.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) TestRandom.cc

TestSet: TestSet.o $(OBJECTS) $(HEADERS) $(INLINES)
	$(C++) -o TestSet TestSet.o $(LIB) $(LDFLAGS) $(LOCAL_LDFLAGS)

TestSet.o: TestSet.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) TestSet.cc

$(LIB) : $(OBJECTS)
	ar rv $@ $?
	ranlib $(LIB)

clean:
	rm -f *.o

vclean: clean
	rm -f *~* libThreads.a TestRandom TestSet
