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

LIB	= libObStr.lib

OBJECTS	= PObStore.obj ObjStore.obj ObStoreI.obj

SOURCES	= PObStore.cc ObjStore.cc ObStoreI.cc

all: $(LIB)

PObStore.obj: PObStore.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) PObStore.cc

ObjStore.obj: ObjStore.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ObjStore.cc

ObStoreI.obj: ObStoreI.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ObStoreI.cc

$(LIB): $(OBJECTS)
	glib $(LIB) -a PObStore.obj
	glib $(LIB) -a ObjStore.obj
	glib $(LIB) -a ObStoreI.obj

clean:
	del *.obj

vclean: clean
	del $(LIB)