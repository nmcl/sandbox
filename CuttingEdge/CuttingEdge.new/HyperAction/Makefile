#
# Copyright 1990, 1991,
# Computing Laboratory, University of Newcastle upon Tyne, UK.
#
# Permission to use, copy, modify and distribute this program for
# evaluation, teaching and/or research purposes only and without fee is
# hereby granted, providing that this copyright and permission notice
# appear on all copies and supporting documentation, and that similar
# conditions are imposed on any individual or organization to whom the
# program is distributed. The University of Newcastle upon Tyne makes no
# representation about the suitability of this software for any purpose.
# It is provided "as is" without express or implied warranty.
#

#
# $Id: Makefile,v 1.3 1991/09/26 13:27:14 nsmw Exp $
#

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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I/usr/local/InterViews3.0/include

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = /usr/lib/debug/malloc.o -L/usr/local/InterViews3.0/lib/SUN4 \
                 -lIV -lX11

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS  = HyperAction.o \
           HyperMainWindow.o \
           HyperWindow.o \
           HyperDeck.o \
           HyperCard.o \
           HyperText.o \
           HyperWord.o \
           HyperImage.o \
           Selector.o \
           read.o

SOURCES  = HyperAction.cc \
           HyperMainWindow.cc \
           HyperWindow.cc \
           HyperDeck.cc \
           HyperCard.cc \
           HyperText.cc \
           HyperWord.cc \
           HyperImage.cc \
           Selector.cc \
           read.cc

HEADERS  = HyperAction.h \
           HyperMainWindow.h \
           HyperWindow.h \
           HyperDeck.h \
           HyperCard.h \
           HyperText.h \
           HyperWord.h \
           HyperImage.h \
           Selector.h \
           read.h

all: input text_to_hypertext HyperAction

HyperAction: $(OBJECTS)
	$(C++) -o HyperAction $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

HyperAction.o: HyperAction.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperAction.cc

HyperMainWindow.o: HyperMainWindow.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperMainWindow.cc

HyperWindow.o: HyperWindow.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperWindow.cc

HyperDeck.o: HyperDeck.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperDeck.cc

HyperCard.o: HyperCard.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperCard.cc

HyperText.o: HyperText.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperText.cc

HyperWord.o: HyperWord.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperWord.cc

HyperImage.o: HyperImage.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) HyperImage.cc

Selector.o: Selector.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Selector.cc

read.o: read.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) read.cc

text_to_hypertext: text_to_hypertext.o
	$(CC) -o text_to_hypertext text_to_hypertext.o $(LDFLAGS) $(LOCAL_LDFLAGS)

text_to_hypertext.o: text_to_hypertext.c
	$(CC) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(CFLAGS) $(LOCAL_CFLAGS) text_to_hypertext.c

text_to_hypertext.c: text_to_hypertext.l
	$(LEX) $(LEXFLAGS) text_to_hypertext.l
	mv lex.yy.c text_to_hypertext.c

input: input.cpp
	$(CPP) -P -I_data input.cpp input

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f HyperAction input
	$(STRIPDEPEND)

install: 

depend:
	$(MDEP) $(CPPFLAGS) -I_data $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
