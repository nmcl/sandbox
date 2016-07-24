MAKE = make

CC  = gcc
C++ = gcc
CPP = cpp
LEX = lex

CFLAGS   = 
CPPFLAGS = -DMSDOS -I. -IC:\Desktop\Arjuna\Include -IC:\Desktop\gnu\gppincl -IC:\Desktop\gnu\gccincl -IC:\Desktop\gnu\os2incl
C++FLAGS = 
LDFLAGS  = 
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = 

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  =

LOCAL_MDEPFLAGS =

LIBRARIES	= -lArj -lObStr -lCom -lgcc -lgpp

OBJECTS	= Driver.obj AtomInt.obj

SOURCES	= Driver.cc AtomInt.cc

all: Driver $(OBJECTS)

Driver: $(OBJECTS)
	$(C++) $(LDFLAGS) -o Driver $(OBJECTS) $(LIBRARIES)
	
Driver.obj: Driver.cc AtomInt.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Driver.cc

AtomInt.obj: AtoInt.cc AtomInt.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS)  AtomInt.cc

clean:
	del *.obj

vclean: clean
	del Driver.exe