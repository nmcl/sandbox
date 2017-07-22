MAKE = make

CC  = cc
C++ = CC3.0.1
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
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/CC/3.0.1/g/include -I../ -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = 

LOCAL_MDEPFLAGS = -I/usr/local/CC/3.0.1/include

LIBRARYS = -L/usr/local/Arjuna/CC/3.0.1/g/lib -lArjuna -lArjServers -lStub -lRajdoot -lCommon \
	   -L/usr/lib

LIB	= libMessage.a

OBJECTS = GroupList.o Client.o SequenceNumber.o Server.o GroupID.o

SOURCES	= GroupList.cc Client.cc SequenceNumber.cc Server.cc GroupID.cc

HEADERS	= GroupList.h Client.h SequenceNumber.h Server.h GroupID.h

all: $(LIB) ClientTest ServerTest

ClientTest: $(OBJECTS) ClientTest.o
	$(C++) -o ClientTest ClientTest.o $(LIB) $(LIBRARYS) $(LDFLAGS) $(LOCAL_LDFLAGS)

ClientTest.o: ClientTest.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ClientTest.cc

ServerTest: $(OBJECTS) ServerTest.o
	$(C++) -o ServerTest ServerTest.o $(LIB) $(LIBRARYS) $(LDFLAGS) $(LOCAL_LDFLAGS)

ServerTest.o: ServerTest.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ServerTest.cc

GroupList.o: GroupList.cc GroupList.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) GroupList.cc

GroupID.o: GroupID.cc GroupID.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) GroupID.cc

Client.o: Client.cc Client.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Client.cc

Server.o: Server.cc Server.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Server.cc

SequenceNumber.o: SequenceNumber.cc SequenceNumber.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) SequenceNumber.cc

$(LIB) : $(OBJECTS)
	ar rv $@ $?
	ranlib $(LIB)

install:
	cp ServerTest /usr/local/Arjuna/CC/3.0.1/g/servers/ServerTest
	chmod a+rx /usr/local/Arjuna/CC/3.0.1/g/servers/ServerTest

clean:
	rm -f *.o

vclean: clean
	rm -f *~* *.bak ClientTest ServerTest $(LIB)

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.
