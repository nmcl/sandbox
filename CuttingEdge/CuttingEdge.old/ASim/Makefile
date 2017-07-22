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

OBJECTS	= main.o Selector.o SButton.o Scroll.o Message.o Menu.o Window.o

SOURCES	= main.cc Selector.cc SButton.cc Scroll.cc Message.cc Menu.cc Window.cc

HEADERS	= Selector.h SButton.h Scroll.h Menu.h Window.h common.h

all: main

main: $(OBJECTS)
	$(C++) -o main $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

main.o: main.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) main.cc

Selector.o: Selector.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Selector.cc

SButton.o: SButton.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) SButton.cc

Scroll.o: Scroll.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Scroll.cc

Window.o: Window.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Window.cc

Message.o: Message.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Message.cc

Menu.o: Menu.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Menu.cc

GrowBox.o: GrowBox.c
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) GrowBox.c

A2: GrowBox.o A2.c
	$(C++) -o A2 $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) A2.c GrowBox.o  $(LDFLAGS) $(LOCAL_LDFLAGS)

clean:
	rm -f *.o

vclean: clean
	rm -f main *~*
