MAKE = make

CC  = gcc
C++ = gcc
CPP = cpp
LEX = lex

CFLAGS   = 
CPPFLAGS = -DMSDOS -I..\include -IC:\Desktop\Arjuna\Include -IC:\Desktop\gnu\gppincl -IC:\Desktop\gnu\gccincl -IC:\Desktop\gnu\os2incl
C++FLAGS = 
LDFLAGS  = 
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = 

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  =

LOCAL_MDEPFLAGS =

LIB	= libCom.lib

OBJECTS	= Boolean.obj Buffer.obj Error.obj \
		  Uid.obj

SOURCES	= Boolean.cc Buffer.cc Error.cc \
		  Uid.cc 

all: $(LIB)

Boolean.obj: Boolean.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Boolean.cc

Buffer.obj: Buffer.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Buffer.cc

Debug.obj: Debug.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Debug.cc

Error.obj: Error.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Error.cc

Filtsbuf.obj: Filtsbuf.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Filtsbuf.cc

Uid.obj: Uid.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Uid.cc

Utility.obj: Utility.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Utility.cc

$(LIB): $(OBJECTS)
	glib $(LIB) -a Boolean.obj
	glib $(LIB) -a Buffer.obj
	glib $(LIB) -a Error.obj
	glib $(LIB) -a Uid.obj

clean:
	del *.obj

vclean: clean
	del $(LIB)
