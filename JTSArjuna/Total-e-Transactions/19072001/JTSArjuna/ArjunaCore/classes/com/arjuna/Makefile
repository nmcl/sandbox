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
# Copyright (C) 1998, 1999, 2000,
#
# Arjuna Solutions Limited,
# Newcastle upon Tyne,
# Tyne and Wear,
# UK.
#
# $Id: Imakefile,v 1.1.4.1 2000/10/16 16:00:08 nmcl Exp $
#

all.local ::
	@echo "Building 'all' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) all.local)

Makefiles ::
	@echo "Building 'Makefile' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(IMAKE) $(IMAKEFLAGS))
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) Makefiles)

depend.local ::
	@echo "Constructing 'depend' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) depend.local)

install.local ::
	@echo "Performing 'install' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) install.local)

tidy.local ::
	@echo "Performing 'tidy'  in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) tidy.local)

clean.local ::
	@echo "Performing 'clean' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) clean.local)

purge.local ::
	@echo "Performing 'purge' in "$(CURDIR)/ArjunaCore
	@(cd ArjunaCore ; $(MAKE) CURDIR=$(CURDIR)/ArjunaCore $(xMAKEFLAGS) purge.local)

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

