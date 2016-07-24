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

LIB	= libArj.lib

OBJECTS	= AbstR.obj ActiveR.obj AtomicA.obj \
		  CadverAR.obj CadverLM.obj CadverR.obj Lock.obj \
		  LockList.obj LockLstI.obj LockMan.obj LockR.obj \
		  ObjState.obj PersistR.obj RecList.obj RecListI.obj \
		  RecordT.obj RecovryR.obj StateMan.obj

SOURCES	= AbstR.cc ActiveR.cc AtomicA.cc \
		  CadverAR.cc CadverLM.cc CadverR.cc Lock.cc \
		  LockList.cc LockLstI.cc LockMan.cc LockR.cc \
		  ObjState.cc PersistR.cc RecList.cc RecListI.cc \
		  RecordT.cc RecovryR.cc StateMan.cc

all: $(LIB)

AbstR.obj: AbstR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) AbstR.cc

ActiveR.obj: ActiveR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ActiveR.cc

AtomicA.obj: AtomicA.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) AtomicA.cc

CadverAR.obj: CadverAR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) CadverAR.cc

CadverLM.obj: CadverLM.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) CadverLM.cc

CadverR.obj: CadverR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) CadverR.cc

Lock.obj: Lock.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Lock.cc

LockList.obj: LockList.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) LockList.cc

LockLstI.obj: LockLstI.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) LockLstI.cc

LockMan.obj: LockMan.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) LockMan.cc

LockR.obj: LockR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) LockR.cc

ObjState.obj: ObjState.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ObjState.cc

PersistR.obj: PersistR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) PersistR.cc

RecList.obj: RecList.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RecList.cc

RecListI.obj: RecListI.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RecListI.cc

RecordT.obj: RecordT.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RecordT.cc

RecovryR.obj: RecovryR.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) RecovryR.cc

StateMan.obj: StateMan.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) StateMan.cc


$(LIB): $(OBJECTS)
	glib $(LIB) -a AbstR.obj
	glib $(LIB) -a ActiveR.obj
	glib $(LIB) -a AtomicA.obj
	glib $(LIB) -a CadverAR.obj
	glib $(LIB) -a CadverLM.obj
	glib $(LIB) -a CadverR.obj
	glib $(LIB) -a Lock.obj
	glib $(LIB) -a LockList.obj
	glib $(LIB) -a LockLstI.obj
	glib $(LIB) -a LockMan.obj
	glib $(LIB) -a LockR.obj
	glib $(LIB) -a ObjState.obj
	glib $(LIB) -a PersistR.obj
	glib $(LIB) -a RecList.obj
	glib $(LIB) -a RecListI.obj
	glib $(LIB) -a RecordT.obj
	glib $(LIB) -a RecovryR.obj
	glib $(LIB) -a StateMan.obj

clean:
	del *.obj

vclean: clean
	del $(LIB)