# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.72 92/09/14 11:44:22 rws Exp $
#
# The cpp used on this machine replaces all newlines and multiple tabs and
# spaces in a macro expansion with a single space.  Imake tries to compensate
# for this, but is not always successful.
#

IMAKEFLAGS += -DBUILDUTILITIES -DINSTALLUTILITIES -UBUILDTESTS -UINSTALLTESTS -DVerboseMake=0 -DBUILDDAEMONS -DINSTALLDAEMONS -DTopDir=/home/nmcl/JTSArjuna/JavaArjunaLite -DInstallTopDir=/usr/local/JTSArjuna

CPPFLAGS += -I$(JAVAARJUNALITETOPDIR)/config -I$(JAVAARJUNALITETOPDIR)/lib -DDEFAULT_OS_ROOT=\"$(INSTALLTOPDIR)/ObjectStore\" -DDEFAULT_LS_ROOT=\"$(INSTALLTOPDIR)/LockStore\"

JAVAARJUNALITETOPDIR=/home/nmcl/JTSArjuna/JavaArjunaLite

IMAKEFLAGS += -I$(JAVAARJUNALITETOPDIR)/config

CPPINCLUDES += -I$(JAVAARJUNALITETOPDIR)/include

CPLUSPLUSFLAGS += -ptr$(JAVAARJUNALITETOPDIR) -pti$(JAVAARJUNALITETOPDIR)/include

LDFLAGS += -ptr$(JAVAARJUNALITETOPDIR) -pti$(JAVAARJUNALITETOPDIR)/include

CPPFLAGS += -DARJUNA_VERSION=\"2.4.2\"

JAVAHARNESSTOPDIR=/home/nmcl/JTSArjuna/JavaHarness

IMAKEFLAGS += -DCPP_PROGRAM=/usr/ccs/lib/cpp -I$(JAVAHARNESSTOPDIR)/config -DJavaCProg=/usr/local/jdk1.1.6/bin/javac -DJavaDocProg=/usr/local/jdk1.1.6/bin/javadoc -DJarProg=/usr/local/jdk1.1.6/bin/jar -DRMICProg=/usr/local/jdk1.1.6/bin/rmic -DJavaProg=/usr/local/jdk1.1.6/bin/java

CPPFLAGS += -DJDK_1_1

JAVAC = /usr/local/jdk1.1.6/bin/javac
JAVADOC = /usr/local/jdk1.1.6/bin/javadoc
RMIC = /usr/local/jdk1.1.6/bin/rmic
JAVA = /usr/local/jdk1.1.6/bin/java
JAR = /usr/local/jdk1.1.6/bin/jar
CPP = /usr/ccs/lib/cpp

GANDIVATOPDIR=/home/nmcl/JTSArjuna/Gandiva

IMAKEFLAGS += -I$(GANDIVATOPDIR)/config

CPPFLAGS += -DHAS_MTSAFE_SELECT

CPPINCLUDES += -I$(GANDIVATOPDIR)/include

CPLUSPLUSFLAGS += -DHAVE_TEMPLATE_REPOSITORY -ptr$(GANDIVATOPDIR) -pti$(GANDIVATOPDIR)/include

LDFLAGS += -ptr$(GANDIVATOPDIR) -pti$(GANDIVATOPDIR)/include

LDLIBRARIES += $(GANDIVATOPDIR)/lib/libGandiva.a

LDLIBDEPENDS += $(GANDIVATOPDIR)/lib/libGandiva.a

OBJ_INIT_QANBOBUFFERIMPLE = $(GANDIVATOPDIR)/lib/Init_QANBOBufferImple.o
OBJ_INIT_GANDIVADEDICATEDINVENTORYIMPLE = $(GANDIVATOPDIR)/lib/Init_GandivaDedicatedInventoryImple.o
OBJ_PRIM_GANDIVADEDICATEDINVENTORYIMPLE = $(GANDIVATOPDIR)/lib/Prim_GandivaDedicatedInventoryImple.o
OBJ_INIT_CREATEFUNCTIONLISTINVENTORYIMPLE = $(GANDIVATOPDIR)/lib/Init_CreateFunctionListInventoryImple.o
OBJ_PRIM_CREATEFUNCTIONLISTINVENTORYIMPLE = $(GANDIVATOPDIR)/lib/Prim_CreateFunctionListInventoryImple.o
OBJ_INIT_INSNAMESERVICEIMPLE = $(GANDIVATOPDIR)/lib/Init_INSNameServiceImple.o
OBJ_INIT_LNSNAMESERVICEIMPLE = $(GANDIVATOPDIR)/lib/Init_LNSNameServiceImple.o
OBJ_INIT_SNSNAMESERVICEIMPLE = $(GANDIVATOPDIR)/lib/Init_SNSNameServiceImple.o

IMAKEFLAGS += -TGandiva.tmpl

IMAKEFLAGS += -DCPLUSPLUSProg=CC -DComp_Name=Sun_C++_3.0.1 -DSCC3_0_1

MAKEDEPENDFLAGS +=  -I/opt/SUNWspro/SC3.0.1/include/CC

CPPFLAGS += -DSCC3_0_1

CPPINCLUDES +=  -I/opt/SUNWspro/SC3.0.1/include/CC

IMAKEFLAGS += -DSys_Name=Solaris2 -DSYSV -DSOLARIS2

CPPFLAGS += -DSYSV -DSOLARIS2 -DHAS_SYSINFO -DHAS_FDATASYNC -DHAS_BSD_GETTIMEOFDAY -DHAS_INT_USLEEP -DHAVE_SELECT

LDLIBRARIES += -lsocket -lnsl

#
# Make generated for System=Solaris2, Compiler=Sun_C++_3.0.1.
#

TOPDIR          = /home/nmcl/JTSArjuna/JavaArjunaLite

INSTALLTOPDIR   = /usr/local/JTSArjuna

CURDIR          = .

CPLUSPLUS       = CC

MAKE            = make
IMAKE           = imake

MAKEDEPEND      = makedepend

AR              = ar rv
RANLIB          = ranlib

INSTALL         = install

RM              = rm -f
RMDIR           = rm -rf
CP              = cp
MV              = mv

all : all.local

all.local ::

Makefile ::
	@echo "Building 'Makefile'"
	@$(IMAKE) $(IMAKEFLAGS)

Makefiles ::

depend : depend.local

depend.local ::

install : all.local install.local

install.local ::

tidy : tidy.local

clean : tidy.local clean.local

purge : tidy.local clean.local purge.local

$(INSTALLTOPDIR) :
	@echo Creating directory $(INSTALLTOPDIR)
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)

$(INSTALLTOPDIR)/bin : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/bin
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/bin

$(INSTALLTOPDIR)/config : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/config
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/config

$(INSTALLTOPDIR)/include : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/include
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/include

$(INSTALLTOPDIR)/lib : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/lib
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/lib

$(INSTALLTOPDIR)/man : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/man
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/man

#
# Copyright (C) 1996, 1997, 1998,
#
# Department of Computing Science,
# University of Newcastle upon Tyne,
# Newcastle upon Tyne,
# UK.
#
# $Id: Imakefile,v 1.6 1998/06/24 15:07:15 nmcl Exp $
#

SOURCES = ActionHierarchy.java \
	  ActionInfo.java \
	  ActionStatus.java \
	  CheckedAction.java \
	  BasicAction.java \
	  AtomicAction.java \
	  Lock.java \
	  LockManager.java \
	  LockMode.java \
	  LockStatus.java \
	  LockList.java \
	  LockListIterator.java \
	  ObjectStatus.java \
	  ObjectType.java \
	  PrepareOutcome.java \
	  RecordList.java \
	  RecordListIterator.java \
	  StateManager.java \
	  TopLevelAction.java \
	  AbstractRecord.java \
	  ActivationRecord.java \
	  CadaverActivationRecord.java \
	  CadaverLockManager.java \
	  CadaverLockRecord.java \
	  CadaverRecord.java \
	  LockRecord.java \
	  PersistenceRecord.java \
	  RecoveryRecord.java \
	  RecordType.java \
	  LockResult.java \
	  ConflictType.java \
	  ActionList.java \
	  ObjectModel.java \
	  StateManagerAttribute.java \
	  LockManagerAttribute.java

OBJECTS = ActionStatus.class \
	  PrepareOutcome.class \
	  RecordType.class \
	  ConflictType.class \
	  CheckedAction.class \
	  BasicAction.class \
	  AtomicAction.class \
	  TopLevelAction.class \
	  ActionList.class \
	  ActionInfo.class \
	  ActionHierarchy.class \
	  StateManager.class \
	  LockResult.class \
	  Lock.class \
	  LockMode.class \
	  LockStatus.class \
	  LockManager.class \
	  LockList.class \
	  LockListIterator.class \
	  ObjectStatus.class \
	  ObjectType.class \
	  RecordList.class \
	  RecordListIterator.class \
	  AbstractRecord.class \
	  ActivationRecord.class \
	  CadaverActivationRecord.class \
	  CadaverLockManager.class \
	  CadaverLockRecord.class \
	  CadaverRecord.class \
	  LockRecord.class \
	  PersistenceRecord.class \
	  RecoveryRecord.class \
	  ObjectModel.class \
	  StateManagerAttribute.class \
	  LockManagerAttribute.class

all.local :: $(SOURCE)

all.local :: $(OBJECTS)

all.local :: AbstractRecord.javatmpl
AbstractRecord.java: AbstractRecord.javatmpl
	@echo Generating $@ from AbstractRecord.javatmpl
	@$(RM) AbstractRecord.cc
	@ln -s AbstractRecord.javatmpl AbstractRecord.cc
	@exec $(CPP) -C -P $(CPPINCLUDES) $(CPPFLAGS) AbstractRecord.cc > AbstractRecord.java
	@$(RM) AbstractRecord.cc

clean.local ::
	@$(RM) AbstractRecord.java

clean.local ::
	@$(RM) AbstractRecord.cc

BasicAction.class: BasicAction.java
	@echo Generating Java bytecode BasicAction.class
	@$(JAVAC)  BasicAction.java

AtomicAction.class: AtomicAction.java
	@echo Generating Java bytecode AtomicAction.class
	@$(JAVAC)  AtomicAction.java

ActionHierarchy.class: ActionHierarchy.java
	@echo Generating Java bytecode ActionHierarchy.class
	@$(JAVAC)  ActionHierarchy.java

ActionInfo.class: ActionInfo.java
	@echo Generating Java bytecode ActionInfo.class
	@$(JAVAC)  ActionInfo.java

CheckedAction.class: CheckedAction.java
	@echo Generating Java bytecode CheckedAction.class
	@$(JAVAC)  CheckedAction.java

ActionStatus.class: ActionStatus.java
	@echo Generating Java bytecode ActionStatus.class
	@$(JAVAC)  ActionStatus.java

Lock.class: Lock.java
	@echo Generating Java bytecode Lock.class
	@$(JAVAC)  Lock.java

LockMode.class: LockMode.java
	@echo Generating Java bytecode LockMode.class
	@$(JAVAC)  LockMode.java

LockStatus.class: LockStatus.java
	@echo Generating Java bytecode LockStatus.class
	@$(JAVAC)  LockStatus.java

LockManager.class: LockManager.java
	@echo Generating Java bytecode LockManager.class
	@$(JAVAC)  LockManager.java

LockList.class: LockList.java
	@echo Generating Java bytecode LockList.class
	@$(JAVAC)  LockList.java

LockListIterator.class: LockListIterator.java
	@echo Generating Java bytecode LockListIterator.class
	@$(JAVAC)  LockListIterator.java

ObjectStatus.class: ObjectStatus.java
	@echo Generating Java bytecode ObjectStatus.class
	@$(JAVAC)  ObjectStatus.java

ObjectType.class: ObjectType.java
	@echo Generating Java bytecode ObjectType.class
	@$(JAVAC)  ObjectType.java

PrepareOutcome.class: PrepareOutcome.java
	@echo Generating Java bytecode PrepareOutcome.class
	@$(JAVAC)  PrepareOutcome.java

RecordList.class: RecordList.java
	@echo Generating Java bytecode RecordList.class
	@$(JAVAC)  RecordList.java

RecordListIterator.class: RecordListIterator.java
	@echo Generating Java bytecode RecordListIterator.class
	@$(JAVAC)  RecordListIterator.java

StateManager.class: StateManager.java
	@echo Generating Java bytecode StateManager.class
	@$(JAVAC)  StateManager.java

TopLevelAction.class: TopLevelAction.java
	@echo Generating Java bytecode TopLevelAction.class
	@$(JAVAC)  TopLevelAction.java

AbstractRecord.class: AbstractRecord.java
	@echo Generating Java bytecode AbstractRecord.class
	@$(JAVAC)  AbstractRecord.java

ActivationRecord.class: ActivationRecord.java
	@echo Generating Java bytecode ActivationRecord.class
	@$(JAVAC)  ActivationRecord.java

CadaverActivationRecord.class: CadaverActivationRecord.java
	@echo Generating Java bytecode CadaverActivationRecord.class
	@$(JAVAC)  CadaverActivationRecord.java

CadaverLockManager.class: CadaverLockManager.java
	@echo Generating Java bytecode CadaverLockManager.class
	@$(JAVAC)  CadaverLockManager.java

CadaverLockRecord.class: CadaverLockRecord.java
	@echo Generating Java bytecode CadaverLockRecord.class
	@$(JAVAC)  CadaverLockRecord.java

CadaverRecord.class: CadaverRecord.java
	@echo Generating Java bytecode CadaverRecord.class
	@$(JAVAC)  CadaverRecord.java

LockRecord.class: LockRecord.java
	@echo Generating Java bytecode LockRecord.class
	@$(JAVAC)  LockRecord.java

PersistenceRecord.class: PersistenceRecord.java
	@echo Generating Java bytecode PersistenceRecord.class
	@$(JAVAC)  PersistenceRecord.java

RecoveryRecord.class: RecoveryRecord.java
	@echo Generating Java bytecode RecoveryRecord.class
	@$(JAVAC)  RecoveryRecord.java

RecordType.class: RecordType.java
	@echo Generating Java bytecode RecordType.class
	@$(JAVAC)  RecordType.java

LockResult.class: LockResult.java
	@echo Generating Java bytecode LockResult.class
	@$(JAVAC)  LockResult.java

ConflictType.class: ConflictType.java
	@echo Generating Java bytecode ConflictType.class
	@$(JAVAC)  ConflictType.java

ActionList.class: ActionList.java
	@echo Generating Java bytecode ActionList.class
	@$(JAVAC)  ActionList.java

ObjectModel.class: ObjectModel.java
	@echo Generating Java bytecode ObjectModel.class
	@$(JAVAC)  ObjectModel.java

StateManagerAttribute.class: StateManagerAttribute.java
	@echo Generating Java bytecode StateManagerAttribute.class
	@$(JAVAC)  StateManagerAttribute.java

LockManagerAttribute.class: LockManagerAttribute.java
	@echo Generating Java bytecode LockManagerAttribute.class
	@$(JAVAC)  LockManagerAttribute.java

clean.local ::
	@$(RM) *.class

print:
	@echo "xMAKEFLAGS      = $(xMAKEFLAGS)"
	@echo "IMAKEFLAGS      = $(IMAKEFLAGS)"
	@echo "MAKEDEPENDFLAGS = $(MAKEDEPENDFLAGS)"
	@echo "CPPFLAGS        = $(CPPFLAGS)"
	@echo "CPPINCLUDES     = $(CPPINCLUDES)"
	@echo "CPLUSPLUSFLAGS  = $(CPLUSPLUSFLAGS)"
	@echo "LDFLAGS         = $(LDFLAGS)"
	@echo "LDLIBRARIES     = $(LDLIBRARIES)"
	@echo "LDLIBDEPENDS    = $(LDLIBDEPENDS)"

tidy.local ::
	@$(RM) core *~ \#*\# *.bak *.BAK *\%

clean.local ::
	@$(RM) *.o gmon.out *.d *.tcov

purge.local ::
	@$(RM) Makefile

