# Makefile generated by imake - do not edit!
# $TOG: imake.c /main/102 1998/02/06 11:02:33 kaleb $

IMAKEFLAGS += -DINSTALLJAR -DHAVE_SWING -DBUILDUTILITIES -DINSTALLUTILITIES -UBUILDTESTS -UINSTALLTESTS -DVerboseMake=0 -DBUILDDAEMONS -DINSTALLDAEMONS -DTopDir=$(JAVAARJUNACORETOPDIR) -DInstallTopDir=C:/PROGRA~1/Arjuna/TransactionService

CPPFLAGS += -I$(JAVAARJUNACORETOPDIR)/config -I$(JAVAARJUNACORETOPDIR)/lib

JAVAARJUNACORETOPDIR=C:/temp/src/JTSArjuna/2.2/JTSArjuna/ArjunaCore

IMAKEFLAGS += -I$(JAVAARJUNACORETOPDIR)/config

CPPINCLUDES += -I$(JAVAARJUNACORETOPDIR)/include

CPPFLAGS += -DARJUNA_VERSION=\"4.7.0\"

JAVAHARNESSTOPDIR=C:/temp/src/JTSArjuna/2.2/JTSArjuna/JavaHarness

IMAKEFLAGS += -DCPP_PROGRAM=gcc -DGNU_CPP -I$(JAVAHARNESSTOPDIR)/config -DJavaCProg=javac -DJavaDocProg=javadoc -DJarProg=jar -DRMICProg=rmic -DJavaProg=java

CPPFLAGS += -P -E -C -x c -D__STDC__ -I$(JAVAHARNESSTOPDIR)/config -DJDK_1_3

JAVAC = javac
JAVADOC = javadoc
RMIC = rmic
JAVA = java
JAR = jar
CPP = gcc

BUILDSYSTEMTOPDIR=/temp/src/JTSArjuna/2.2/JTSArjuna/ModuleBuildSystem

IMAKEFLAGS += -I$(BUILDSYSTEMTOPDIR)/config

CPPINCLUDES += -I$(BUILDSYSTEMTOPDIR)/include

IMAKEFLAGS += -TModuleBuildSystem.tmpl

IMAKEFLAGS += -DSys_Name=MSWindows -DWIN32 -DNEED_VOID_USLEEP -DHAS_VOID_USLEEP

CPPFLAGS += -DWIN32

#
# Make generated for System=MSWindows
#

TOPDIR          = C:/temp/src/JTSArjuna/2.2/JTSArjuna/ArjunaCore

INSTALLTOPDIR   = C:/PROGRA~1/Arjuna/TransactionService

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
	@:

Makefile ::
	@echo "Building 'Makefile'"
	@$(IMAKE) $(IMAKEFLAGS)

Makefiles ::
	@:

depend : depend.local

depend.local ::
	@:

install : all.local install.local

install.local ::
	@:

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

$(INSTALLTOPDIR)/var : $(INSTALLTOPDIR)
	@echo Creating directory $(INSTALLTOPDIR)/var
	@$(INSTALL) -d -m 0755 $(INSTALLTOPDIR)/var

#
# Copyright (C) 1998, 1999, 2000, 2001,
#
# Arjuna Solutions Limited,
# Newcastle upon Tyne,
# Tyne and Wear,
# UK.
#
# $Id: Imakefile,v 1.1.4.1.16.1.2.1 2001/04/12 13:29:50 nmcl Exp $
#

SOURCES = ActionMonitor.java 	  ObjectStoreMonitor.java 	  OTM.java 	  report.java

SWING_OBJECTS = OTM.class

UTIL_OBJECTS = ActionMonitor.class 	       ObjectStoreMonitor.class 	       report.class

all.local :: $(UTIL_OBJECTS)

all.local :: $(SWING_OBJECTS)

ActionMonitor.class: ActionMonitor.java
	@echo Generating Java bytecode ActionMonitor.class
	@$(JAVAC) $(JAVAC_FLAGS)  ActionMonitor.java

ObjectStoreMonitor.class: ObjectStoreMonitor.java
	@echo Generating Java bytecode ObjectStoreMonitor.class
	@$(JAVAC) $(JAVAC_FLAGS)  ObjectStoreMonitor.java

OTM.class: OTM.java
	@echo Generating Java bytecode OTM.class
	@$(JAVAC) $(JAVAC_FLAGS)  OTM.java

report.class: report.java
	@echo Generating Java bytecode report.class
	@$(JAVAC) $(JAVAC_FLAGS)  report.java

install.local :: $(INSTALLTOPDIR)/bin

install.local :: $(INSTALLTOPDIR)/bin/ActionMonitor.class

$(INSTALLTOPDIR)/bin/ActionMonitor.class : ActionMonitor.class
	@echo Installing ActionMonitor.class as $(INSTALLTOPDIR)/bin/ActionMonitor.class
	@$(INSTALL) -m 0755 ActionMonitor.class $(INSTALLTOPDIR)/bin/ActionMonitor.class

install.local :: $(INSTALLTOPDIR)/bin

install.local :: $(INSTALLTOPDIR)/bin/ObjectStoreMonitor.class

$(INSTALLTOPDIR)/bin/ObjectStoreMonitor.class : ObjectStoreMonitor.class
	@echo Installing ObjectStoreMonitor.class as $(INSTALLTOPDIR)/bin/ObjectStoreMonitor.class
	@$(INSTALL) -m 0755 ObjectStoreMonitor.class $(INSTALLTOPDIR)/bin/ObjectStoreMonitor.class

install.local :: $(INSTALLTOPDIR)/bin

install.local :: $(INSTALLTOPDIR)/bin/OTM.class

$(INSTALLTOPDIR)/bin/OTM.class : OTM.class
	@echo Installing OTM.class as $(INSTALLTOPDIR)/bin/OTM.class
	@$(INSTALL) -m 0755 OTM.class $(INSTALLTOPDIR)/bin/OTM.class

install.local :: $(INSTALLTOPDIR)/bin

install.local :: $(INSTALLTOPDIR)/bin/MonitorThread.class

$(INSTALLTOPDIR)/bin/MonitorThread.class : MonitorThread.class
	@echo Installing MonitorThread.class as $(INSTALLTOPDIR)/bin/MonitorThread.class
	@$(INSTALL) -m 0755 MonitorThread.class $(INSTALLTOPDIR)/bin/MonitorThread.class

install.local :: $(INSTALLTOPDIR)/bin

install.local :: $(INSTALLTOPDIR)/bin/DirEntry.class

$(INSTALLTOPDIR)/bin/DirEntry.class : DirEntry.class
	@echo Installing DirEntry.class as $(INSTALLTOPDIR)/bin/DirEntry.class
	@$(INSTALL) -m 0755 DirEntry.class $(INSTALLTOPDIR)/bin/DirEntry.class

clean.local ::
	@$(RM) *.class *.prof out

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
	@$(RM) *.core *~ \#*\# *.bak *.BAK *\%

clean.local ::
	@$(RM) *.o gmon.out *.d *.tcov

purge.local ::
	@$(RM) Makefile

