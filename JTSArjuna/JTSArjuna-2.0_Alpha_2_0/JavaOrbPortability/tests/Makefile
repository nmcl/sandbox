# Makefile generated by imake - do not edit!
# $XConsortium: imake.c,v 1.72 92/09/14 11:44:22 rws Exp $
#
# The cpp used on this machine replaces all newlines and multiple tabs and
# spaces in a macro expansion with a single space.  Imake tries to compensate
# for this, but is not always successful.
#

IMAKEFLAGS += -DBUILDUTILITIES -DINSTALLUTILITIES -UBUILDTESTS -UINSTALLTESTS -DVerboseMake=0 -DTopDir=/home/nmcl/JTSArjuna/JavaOrbPortability -DInstallTopDir=/usr/local/JTSArjuna

CPPINCLUDES += -I$(JAVAORBPORTABILITYTOPDIR)/config -I$(JAVAORBPORTABILITYTOPDIR)/lib

JAVAORBPORTABILITYTOPDIR=/home/nmcl/JTSArjuna/JavaOrbPortability

CPPFLAGS += -DCORBA_2_2 -DSUPPORT_IDL_PREFIX -DORB_OTS_SUPPORT -DBROKEN_WRONG_TRANSACTION

JAVAIDL = jidl
IDLIR = irfeed
IDLFLAGS =
IRFLAGS =
IDL_CFLAGS =
IDL_SFLAGS =
IDL_HFLAGS =
ORB_ROOT = /usr/local/ORBacus
ORB_INCLUDES = -I$(ORB_ROOT)/include
ORB_LDFLAGS =
ORB_LIBS =

CPPFLAGS += $(ORB_INCLUDES)

LDFLAGS += $(ORB_LDFLAGS)

LDLIBRARIES += $(ORB_LIBS)

IMAKEFLAGS += -DORB_ROOT_DEFAULT=/usr/local/ORBacus -DSUPPORT_IDL_PREFIX -DIDL2JAVA=jidl -DIDL2IR=irfeed -DORBACUS -I$(JAVAORBPORTABILITYTOPDIR)/config

CPPFLAGS += -DUSE_CONFIG_FILE -DCOSSERVICES_ROOT=\"$(INSTALLTOPDIR)/etc/\" -DCONFIG_FILE=\"CosServices.cfg\" -DORBACUS -DHAVE_BOA -DJAVAIDL

CPPINCLUDES += -I$(JAVAORBPORTABILITYTOPDIR)/include

IDLFLAGS += --tie

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

TOPDIR          = /home/nmcl/JTSArjuna/JavaOrbPortability

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
# Copyright (C) 1996, 1997, 1998, 1999,
#
# Department of Computing Science,
# University of Newcastle upon Tyne,
# Newcastle upon Tyne,
# UK.
#
# $Id: Imakefile,v 1.2 1999/03/08 15:13:19 nmcl Exp $
#

SOURCES = DemoPostInit.java \
	  DemoPreInit.java \
	  InitTest.java

OBJECTS = DemoPostInit.class \
	  DemoPreInit.class \
	  InitTest.class

DemoPostInit.class: DemoPostInit.java
	@echo Generating Java bytecode DemoPostInit.class
	@$(JAVAC)  DemoPostInit.java

DemoPreInit.class: DemoPreInit.java
	@echo Generating Java bytecode DemoPreInit.class
	@$(JAVAC)  DemoPreInit.java

InitTest.class: InitTest.java
	@echo Generating Java bytecode InitTest.class
	@$(JAVAC)  InitTest.java

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
	@$(RM) core *~ \#*\# *.bak *.BAK *\%

clean.local ::
	@$(RM) *.o gmon.out *.d *.tcov

purge.local ::
	@$(RM) Makefile

