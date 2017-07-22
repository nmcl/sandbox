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
# $Id: Makefile,v 1.4 1991/06/10 18:42:18 nsmw Exp $
#

MAKE = make

CC  = cc
C++ = CC
CPP = /lib/cpp

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   = 
CPPFLAGS =
C++FLAGS = -g
LDFLAGS  = -g

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include \
                 -I/usr/local/InterViews3.0/include

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/local/Arjuna/lib \
                 -L/usr/local/InterViews3.0/lib/SUN4 \
                 -lArjuna -lIV -lX -lm


LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS  = ActiveColour.o \
           ActiveColourEdit.o \
           Area.o \
           Driver.o \
           ColourAction.o \
           ColourValue.o

SOURCES  = ActiveColour.cc \
           ActiveColourEdit.cc \
           Area.cc \
           Driver.cc \
           ColourAction.cc \
           ColourValue.cc

all: Driver

Driver: $(OBJECTS)
	$(C++) -o Driver $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

ActiveColour.o: ActiveColour.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ActiveColour.cc

ActiveColourEdit.o: ActiveColourEdit.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ActiveColourEdit.cc

Area.o: Area.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Area.cc

Driver.o: Driver.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Driver.cc

ColourAction.o: ColourAction.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ColourAction.cc

ColourValue.o: ColourValue.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ColourValue.cc

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f Driver
	$(STRIPDEPEND)

install: 

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
