# Makefile generated by imake - do not edit!
# $TOG: imake.c /main/97 1997/06/20 20:23:51 kaleb $

#
# Make generated for Linux 2.0.29 - "i586"
#

SHELL = /bin/sh
TOPDIR = /home/nmcl/Arjuna

CURDIR = .

MAKE = make
IMAKE = imake

MAKEDEPEND = $(TOPDIR)/Utilities/Tools/bin/makedepend

CPLUSPLUS = "g++"
CPLUSPLUS_CROSS = "g++"

AR = ar
SED = sed
RANLIB = ranlib
PURIFY = "exec"
YACC = "bison"
LEX = "flex"

MV = mv
CP = cp
RM = rm -f
CAT = cat
MKDIR = mkdir
CHMOD = chmod

ARFLAGS = rv
xMAKEFLAGS = -j 1 -l 1 --no-print-directory
IMAKEFLAGS = -TConfig/Arjuna.tmpl -I""$(TOPDIR)/Include $(CPPFLAGS)
STUBFLAGS = +w +V
MAKEDEPENDFLAGS = -I""$(CPLUSPLUSINCLUDE) -I""$(TOPDIR)/Include

YACCFLAGS = -y
LEXFLAGS =
IV_CPPFLAGS = IV_CPPFLAGS_DEFAULT
IV_LDFLAGS = IV_LDFLAGS_DEFAULT
X11_LDFLAGS = X11_LDFLAGS_DEFAULT

CPLUSPLUSINCLUDE = /usr/include/g++

INSTALL_TOPDIR = /home/nmcl/Install/ArjunaInstall

STUBGEN = $(TOPDIR)/StubGen/src/tatsu

LIBCOMMON = $(TOPDIR)/lib/libCommon.a

LIBSTUB = $(TOPDIR)/lib/libArjuna.a

LIBRAJDOOT = $(TOPDIR)/lib/libArjuna.a

LIBTHREAD = $(TOPDIR)/lib/libNoneThread.a

LIBNONETHREAD = $(TOPDIR)/lib/libNoneThread.a

LIBRPC = $(LIBRAJDOOT)

LIBARJUNA = $(TOPDIR)/lib/libArjuna.a

LIBOBJCONFIG = $(TOPDIR)/lib/libObjConfig.a

LIBOBJECTSTORE = $(TOPDIR)/lib/libArjuna.a

LIBARJSERVERS = $(TOPDIR)/lib/libArjServers.a

LIBREPLICATION = $(TOPDIR)/lib/libReplication.a

LIBCRASH = $(TOPDIR)/lib/libCrashRec.a

ARJUNA_INCLUDES = $(TOPDIR)/Include

SMSTUB_H = $(ARJUNA_INCLUDES)/Arjuna/StateMan_stub.h

SMSTUBCLASS_H = $(ARJUNA_INCLUDES)/Arjuna/StateManager_stubclass.h

LMSTUB_H = $(ARJUNA_INCLUDES)/Arjuna/LockMan_stub.h

LMSTUBCLASS_H = $(ARJUNA_INCLUDES)/Arjuna/LockManager_stubclass.h

OBSTATESTUB_H = $(ARJUNA_INCLUDES)/Arjuna/ObjState_stub.h

UIDSTUB_H = $(ARJUNA_INCLUDES)/Common/Uid_stub.h

BUFFSTUB_H = $(ARJUNA_INCLUDES)/Common/Buffer_stub.h

ANSTUB_H = $(ARJUNA_INCLUDES)/ArjServers/ArjName_stub.h

NSSTUB_H = $(ARJUNA_INCLUDES)/ArjServers/NameServ_stub.h

NSSTUBCLASS_H = $(ARJUNA_INCLUDES)/ArjServers/NameServer_stubclass.h

RDSTUB_H = $(ARJUNA_INCLUDES)/ArjServers/RepDesc_stub.h

RDSTUB_H = $(ARJUNA_INCLUDES)/ArjServers/ReplicaData_stub.h

RPOSSTUB_H = $(ARJUNA_INCLUDES)/ArjServers/RPObjStore_stub.h

RPOSSTUBCLASS_H = $(ARJUNA_INCLUDES)/ArjServers/RPObjStore_stubclass.h

all :: Makefile

#
# Copyright (C) 1993
#
# Department of Computing Science,
# The University,
# Newcastle upon Tyne,
# UK.
#
# $Id: Imakefile,v 1.3 1994/06/01 14:39:50 ngdp Exp $
#

SOURCES   = NameServEdit.cc

OBJECTS   = NameServEdit.o

LIBRARIES = $(LIBARJSERVERS) $(LIBSTUB) $(LIBRPC) $(LIBOBJECTSTORE) $(LIBARJUNA) $(LIBCOMMON)

LOCAL_CPPFLAGS = -I$(ARJUNA_INCLUDES)

all :: all.local
all.local : stubs NameServEdit
	@echo Made $? [in $(CURDIR) ] as $@

$(INSTALL_TOPDIR)/bin :: $(INSTALL_TOPDIR)
	@if test ! -d $(INSTALL_TOPDIR)/bin; \
	then \
	echo Creating Directory $(INSTALL_TOPDIR)/bin; \
	$(MKDIR) $(INSTALL_TOPDIR)/bin; \
	$(CHMOD) u=rwx,g=rx,o=rx $(INSTALL_TOPDIR)/bin; \
	fi

install.local :: $(INSTALL_TOPDIR)/bin $(INSTALL_TOPDIR)/bin/NameServEdit

$(INSTALL_TOPDIR)/bin :: $(INSTALL_TOPDIR)
	@if test ! -d $(INSTALL_TOPDIR)/bin; \
	then \
	echo Creating Directory $(INSTALL_TOPDIR)/bin; \
	$(MKDIR) $(INSTALL_TOPDIR)/bin; \
	$(CHMOD) u=rwx,g=rx,o=rx $(INSTALL_TOPDIR)/bin; \
	fi

$(INSTALL_TOPDIR)/bin/NameServEdit : NameServEdit
	@echo Copying NameServEdit to $(INSTALL_TOPDIR)/bin/NameServEdit
	@$(CP) NameServEdit $(INSTALL_TOPDIR)/bin/NameServEdit
	@$(CHMOD) u=rwx,g=rx,o=rx $(INSTALL_TOPDIR)/bin/NameServEdit

NameServEdit :  $(OBJECTS) $(LIBRARIES)
	@echo Linking $@
	@eval $(PURIFY) $(CPLUSPLUS) $(LOCAL_LDFLAGS) $(LDFLAGS) -o NameServEdit  $(OBJECTS) $(LIBRARIES)  $(LOCAL_LIBRARIES) $(xLIBRARIES)

NameServEdit.o : NameServEdit.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) NameServEdit.cc

tidy :: tidy.local

tidy.local ::
	@$(RM) core *~ \#*\# *.bak *.BAK *\%

clean :: clean.local

clean.local :: tidy.local
	@$(RM) -f .dummy $(OBJECTS)

purge :: purge.local

purge.local :: clean.local
	@$(RM) -f Makefile NameServEdit *.stamp

CPPFLAGS =   -DHAVE_BOOLEAN_TYPE -DHAVE_WCHAR_TYPE -DHAVE_LONG_LONG -DGCC_STATIC_INIT_BUG -DMEMFNS_IN_STRING_H -DNEED_MALLOC_T -DHAVE_MSGCONTROL -D__LINUX__ -DSYSV -D__CONSTVALUE="" -D__CONSTVALUE2="" -D__NORETURN="" -D__NORETURN2=""
CPLUSPLUSFLAGS =
LDFLAGS =

xLIBRARIES = $(LIBTHREAD) $(LIBCOMMON) $(LIBARJUNA) $(LIBCOMMON)

CPPFLAGS_CROSS =  -DHAVE_BOOLEAN_TYPE -DHAVE_WCHAR_TYPE -DHAVE_LONG_LONG -DGCC_STATIC_INIT_BUG -DMEMFNS_IN_STRING_H -DNEED_MALLOC_T -DHAVE_MSGCONTROL -D__LINUX__ -DSYSV -D__CONSTVALUE="" -D__CONSTVALUE2="" -D__NORETURN="" -D__NORETURN2=""
CPLUSPLUSFLAGS_CROSS =
LDFLAGS_CROSS =
xLIBRARIES_CROSS =

Makefile :: Imakefile
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(CURDIR)"/Imakefile"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/m_unknown.cf
	@echo
	@echo ***
	@echo *** WARNING : $(CURDIR)/Makefile may be out of date,
	@echo ***         : older than $(TOPDIR)/Include/Config/m_unknown.cf
	@echo ***

Makefile :: $(TOPDIR)/Include/Config/s_linux_2_0.cf
	@echo
	@echo ***
	@echo *** WARNING : $(CURDIR)/Makefile may be out of date,
	@echo ***         : older than $(TOPDIR)/Include/Config/s_linux_2_0.cf
	@echo ***

Makefile :: $(TOPDIR)/Include/Config/c_gcc_2_7.cf
	@echo
	@echo ***
	@echo *** WARNING : $(CURDIR)/Makefile may be out of date,
	@echo ***         : older than $(TOPDIR)/Include/Config/c_gcc_2_7.cf
	@echo ***

Makefile :: $(TOPDIR)/Include/Config/Arjuna.tmpl
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Arjuna.tmpl"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Flag.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Flag.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Configure.h
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Configure.h"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Macro.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Macro.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Make.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Make.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Lib.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Lib.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Prog.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Prog.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/Rule.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/Rule.def"
	@echo "***"

Makefile :: $(TOPDIR)/Include/Config/StdParam.def
	@echo
	@echo "***"
	@echo "*** WARNING : "$(CURDIR)"/Makefile may be out of date,"
	@echo "***         : older than "$(TOPDIR)"/Include/Config/StdParam.def"
	@echo "***"

Makefiles ::

all :: all.local

all.local :

depend :: depend.local

depend.local :: $(SOURCES)
	@CC=$(CPLUSPLUS) $(MAKEDEPEND) $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_MAKEDEPENDFLAGS) $(MAKEDEPENDFLAGS) $(SOURCES)

tidy :: tidy.local

tidy.local ::
	@$(RM) core *~ \#*\# *.bak *.BAK *\%

clean :: clean.local

clean.local :: tidy.local
	@$(RM) -f .dummy $(OBJECTS)

purge :: purge.local

purge.local :: clean.local
	@$(RM) -f Makefile $(OBJECTS) *.stamp

Makefile_ ::
	@echo Making $@ [in $(CURDIR) ]
	@$(IMAKE) $(IMAKEFLAGS);$(MV) Makefile Makefile.sed;$(SED) -e '/^COMMENT$$/s//#/' -e '/^COMMENT /s//# /' -e 's/\%\%/\\"/g' < Makefile.sed > Makefile;$(RM) Makefile.sed

install :: install.local

install :: all.local

install.local ::

stubs :: $(STUBGEN) stubs.local

stubs.local ::

# DO NOT DELETE

NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/ArjName.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/ArjName_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/NSInterface.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/NameInfo.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/NameServ_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/NameServer_stubclass.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/RepDesc_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData.n
NameServEdit.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/Lock.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/LockMan.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/LockMan_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/LockManager_stubclass.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/Lock_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Arjuna/StateManager_stubclass.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Boolean.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Buffer.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Buffer.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Buffer_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/ErrCodes.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Error.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Error.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/ErrorList.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/ErrorList_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Error_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/PointerMap.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Uid.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Uid.n
NameServEdit.o: /home/nmcl/Arjuna/Include/Common/Uid_stub.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Config/Configure.h
NameServEdit.o: /home/nmcl/Arjuna/Include/Config/c_gcc_2_7.cf
NameServEdit.o: /home/nmcl/Arjuna/Include/Config/m_unknown.cf
NameServEdit.o: /home/nmcl/Arjuna/Include/Config/s_linux_2_0.cf
NameServEdit.o: /home/nmcl/Arjuna/Include/Config/v_unknown.cf
NameServEdit.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.h
NameServEdit.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.n
NameServEdit.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjectStoreNames.h
NameServEdit.o: /home/nmcl/Arjuna/Include/RPC/ClStatus.h
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/Abort.h
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.h
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.n
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.h
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.n
NameServEdit.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/iostream.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/netdb.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/stddef.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/string.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/sys/ipc.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/sys/param.h
NameServEdit.o: /home/nmcl/Arjuna/Include/System/sys/types.h
NameServEdit.o: /usr/include/_G_config.h /usr/include/asm/param.h
NameServEdit.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
NameServEdit.o: /usr/include/features.h /usr/include/g++/iostream.h
NameServEdit.o: /usr/include/g++/streambuf.h /usr/include/libio.h
NameServEdit.o: /usr/include/limits.h /usr/include/linux/ipc.h
NameServEdit.o: /usr/include/linux/limits.h /usr/include/linux/param.h
NameServEdit.o: /usr/include/linux/posix_types.h /usr/include/linux/types.h
NameServEdit.o: /usr/include/netdb.h /usr/include/paths.h
NameServEdit.o: /usr/include/posix1_lim.h /usr/include/posix2_lim.h
NameServEdit.o: /usr/include/string.h /usr/include/sys/bitypes.h
NameServEdit.o: /usr/include/sys/cdefs.h /usr/include/sys/ipc.h
NameServEdit.o: /usr/include/sys/param.h /usr/include/sys/types.h
NameServEdit.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/limits.h
NameServEdit.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/stddef.h
NameServEdit.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/syslimits.h
