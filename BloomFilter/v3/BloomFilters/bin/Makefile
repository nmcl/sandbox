#
#  Makefile : Copyright (C) 1997, Mark C. Little.
#

C++          = CC
CPPFLAGS     = -I/home/nmcl/C++SIM/Include -I/home/nmcl/CosServices/ArjunaLite/include
C++FLAGS     = -g
LDFLAGS      = -g -L/home/nmcl/C++SIM/lib -L/home/nmcl/CosServices/ArjunaLite/lib
LDLIBRARIES  = -lC++SIM -lStat -lCommon -lArjuna -lm -lsocket -lnsl

MAKEDEPEND   = makedepend
MDFLAGS      =

HEADERS      =

SOURCES      = BloomFilter.cc HashFunctions.cc IORHash.cc UidHash.cc BitVector.cc

OBJECTS      = BloomFilter.o HashFunctions.o IORHash.o UidHash.o BitVector.o

PROGRAM      = BloomFilter HashFunctions IORHash UidHash BitVector

all: $(PROGRAM)

install:

BloomFilter: BloomFilter.o
	$(C++) $(LDFLAGS) -o BloomFilter BloomFilter.o $(LDLIBRARIES)

BloomFilter.o: BloomFilter.cc
	$(C++) $(CPPFLAGS) $(C++FLAGS) -c BloomFilter.cc

HashFunctions: HashFunctions.o
	$(C++) $(LDFLAGS) -o HashFunctions HashFunctions.o $(LDLIBRARIES)

HashFunctions.o: HashFunctions.cc
	$(C++) $(CPPFLAGS) $(C++FLAGS) -c HashFunctions.cc

BitVector: BitVector.o
	$(C++) $(LDFLAGS) -o BitVector BitVector.o $(LDLIBRARIES)

BitVector.o: BitVector.cc
	$(C++) $(CPPFLAGS) $(C++FLAGS) -c BitVector.cc

IORHash: IORHash.o
	$(C++) $(LDFLAGS) -o IORHash IORHash.o $(LDLIBRARIES)

IORHash.o: IORHash.cc
	$(C++) $(CPPFLAGS) $(C++FLAGS) -c IORHash.cc

UidHash: UidHash.o
	$(C++) $(LDFLAGS) -o UidHash UidHash.o $(LDLIBRARIES)

UidHash.o: UidHash.cc
	$(C++) $(CPPFLAGS) $(C++FLAGS) -c UidHash.cc

tidy:
	$(RM) *~ \#*\# *.BAK *.bak core a.out

clean: tidy
	$(RM) *.o 

purge: clean
	$(RM) $(PROGRAM) out

depend: $(SOURCES)
	$(MAKEDEPEND) $(MDFLAGS) $(CPPFLAGS) $(SOURCES)
