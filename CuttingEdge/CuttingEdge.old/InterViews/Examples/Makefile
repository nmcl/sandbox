MAKE = make

CC  = cc
C++ = CC
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   =
CPPFLAGS =
C++FLAGS = -g
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

OBJECTS	= scroller.o

SOURCE	= scroller.cc

HEADERS	=

all: scroller

scroller: $(OBJECTS)
	$(C++) -o scroller scroller.o $(LDFLAGS) $(LOCAL_LDFLAGS)

scroller.o: scroller.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) scroller.cc

clean:
	rm *.o

vclean: clean
	rm scroller



