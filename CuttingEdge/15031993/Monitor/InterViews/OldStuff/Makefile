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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I/usr/local/InterViews3.0/include \
		-I/usr/local/InterViews3.0/IV-look

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/local/InterViews3.0/lib/SUN4 -L/usr/lib \
                 -lIV -lX11 -lm

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= main.o Message.o Selector.o Monitor.o

SOURCES	= main.cc Message.cc Selector.cc Monitor.cc

HEADERS	= Message.h Selector.h Monitor.h common.h

all: main

main: $(OBJECTS) $(HEADERS)
	$(C++) -o main $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

main.o: main.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) main.cc

Message.o: Message.cc Message.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Message.cc

Selector.o: Selector.cc Selector.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Selector.cc

Monitor.o: Monitor.cc Monitor.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Monitor.cc

clean:
	rm -f *.o

vclean: clean
	rm -f main *~*
