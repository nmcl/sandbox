# Makefile generated by imake - do not edit!
# $TOG: imake.c /main/102 1998/02/06 11:02:33 kaleb $

IMAKEFLAGS += -DINSTALLJAR -DBUILDUTILITIES -DINSTALLUTILITIES -UBUILDTESTS -UINSTALLTESTS -DVerboseMake=0 -DBUILDDAEMONS -DINSTALLDAEMONS -DTopDir=$(JAVAGANDIVATOPDIR) -DInstallTopDir=C:/PROGRA~1/Arjuna/TransactionService

CPPFLAGS += -I$(JAVAGANDIVATOPDIR)/config -I$(JAVAGANDIVATOPDIR)/lib -DPROPERTIES_DIR=\"C:/PROGRA~1/Arjuna/TransactionService/etc\" -DPROPERTIES_FILE=\"TransactionService_2_2.properties\"

JAVAGANDIVATOPDIR=C:/temp/src/JTSArjuna/2.2/JTSArjuna/ArjunaCommon

PROPERTIES_FILE=TransactionService_2_2.properties

IMAKEFLAGS += -I$(JAVAGANDIVATOPDIR)/config

CPPINCLUDES += -I$(JAVAGANDIVATOPDIR)/include

CPPFLAGS += -DGANDIVA_VERSION=\"4.3.0\"

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

TOPDIR          = C:/temp/src/JTSArjuna/2.2/JTSArjuna/ArjunaCommon

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
# Copyright (C) 1995, 1996, 1997, 1998, 1999,
#
# Arjuna Solutions Limited,
# Newcastle upon Tyne,
# Tyne and Wear,
# UK.
#
# $Id: Imakefile,v 1.2 2000/03/16 16:17:08 nmcl Exp $
#

SOURCES = BasicList.java 	  BasicListIterator.java 	  KeyedList.java 	  ListElement.java 	  ListElementWrapper.java 	  SimpleList.java 	  SimpleListIterator.java 	  HashList.java 	  HashListIterator.java 	  OrderedList.java 	  OrderedListIterator.java 	  OrderedListElement.java 	  OrderedListEntry.java 	  SimpleListEntry.java

OBJECTS = BasicList.class 	  BasicListIterator.class 	  KeyedList.class 	  ListElement.class 	  ListElementWrapper.class 	  SimpleList.class 	  SimpleListIterator.class 	  HashList.class 	  HashListIterator.class 	  OrderedList.class 	  OrderedListIterator.class 	  OrderedListElement.class 	  OrderedListEntry.class 	  SimpleListEntry.class

all.local :: $(OBJECTS)

BasicList.class: BasicList.java
	@echo Generating Java bytecode BasicList.class
	@$(JAVAC) $(JAVAC_FLAGS)  BasicList.java

BasicListIterator.class: BasicListIterator.java
	@echo Generating Java bytecode BasicListIterator.class
	@$(JAVAC) $(JAVAC_FLAGS)  BasicListIterator.java

KeyedList.class: KeyedList.java
	@echo Generating Java bytecode KeyedList.class
	@$(JAVAC) $(JAVAC_FLAGS)  KeyedList.java

SimpleList.class: SimpleList.java
	@echo Generating Java bytecode SimpleList.class
	@$(JAVAC) $(JAVAC_FLAGS)  SimpleList.java

SimpleListIterator.class: SimpleListIterator.java
	@echo Generating Java bytecode SimpleListIterator.class
	@$(JAVAC) $(JAVAC_FLAGS)  SimpleListIterator.java

HashList.class: HashList.java
	@echo Generating Java bytecode HashList.class
	@$(JAVAC) $(JAVAC_FLAGS)  HashList.java

HashListIterator.class: HashListIterator.java
	@echo Generating Java bytecode HashListIterator.class
	@$(JAVAC) $(JAVAC_FLAGS)  HashListIterator.java

ListElement.class: ListElement.java
	@echo Generating Java bytecode ListElement.class
	@$(JAVAC) $(JAVAC_FLAGS)  ListElement.java

ListElementWrapper.class: ListElementWrapper.java
	@echo Generating Java bytecode ListElementWrapper.class
	@$(JAVAC) $(JAVAC_FLAGS)  ListElementWrapper.java

OrderedList.class: OrderedList.java
	@echo Generating Java bytecode OrderedList.class
	@$(JAVAC) $(JAVAC_FLAGS)  OrderedList.java

OrderedListIterator.class: OrderedListIterator.java
	@echo Generating Java bytecode OrderedListIterator.class
	@$(JAVAC) $(JAVAC_FLAGS)  OrderedListIterator.java

OrderedListElement.class: OrderedListElement.java
	@echo Generating Java bytecode OrderedListElement.class
	@$(JAVAC) $(JAVAC_FLAGS)  OrderedListElement.java

OrderedListEntry.class: OrderedListEntry.java
	@echo Generating Java bytecode OrderedListEntry.class
	@$(JAVAC) $(JAVAC_FLAGS)  OrderedListEntry.java

SimpleListEntry.class: SimpleListEntry.java
	@echo Generating Java bytecode SimpleListEntry.class
	@$(JAVAC) $(JAVAC_FLAGS)  SimpleListEntry.java

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
	@$(RM) *.core *~ \#*\# *.bak *.BAK *\%

clean.local ::
	@$(RM) *.o gmon.out *.d *.tcov

purge.local ::
	@$(RM) Makefile

