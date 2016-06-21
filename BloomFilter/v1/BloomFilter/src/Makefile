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
# $Id: Makefile,v 1.1 1991/08/15 19:55:50 nsmw Exp $
#
# Bloom Filter classes library makefile
#

MAKE = make

CC = cc
C++ = g++

CPPFLAGS =
CFLAGS =
C++FLAGS = -g
LDFLAGS = -g

MDEPFLAGS =

LOCAL_CPPFLAGS = -I../include
LOCAL_CFLAGS = 
LOCAL_C++FLAGS =
LOCAL_LDFLAGS =

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

LIB = libBloomFilter.a

OBJECTS = BloomFilter.o \
          StringBloomFilter.o \
	  StringHash.o

SOURCES = BloomFilter.cc \
          StringBloomFilter.cc \
	  StringHash.cc

all: $(LIB)

BloomFilter.o : BloomFilter.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) BloomFilter.cc

StringBloomFilter.o : StringBloomFilter.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) StringBloomFilter.cc

UidBloomFilter.o : UidBloomFilter.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) UidBloomFilter.cc

StringHash.o : StringHash.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) StringHash.cc

UidHash.o : UidHash.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) UidHash.cc

$(LIB): $(OBJECTS)
	ar rv $@ $?
	ranlib $@

tidy:
	rm -f core *~ \#*\# *.bak *.BAK

clean: tidy
	rm -f *.o

purge: clean
	rm -f $(LIB)
	$(STRIPDEPEND)

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
