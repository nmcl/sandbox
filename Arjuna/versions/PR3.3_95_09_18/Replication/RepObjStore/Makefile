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
# $Id: Imakefile,v 1.2 1994/11/29 14:01:25 ngdp Exp $
#

LIB = $(LIBREPLICATION)

SOURCES = RepObjStore.cc DaemonList.cc DaemonData.cc Members.cc RepObjStoreIF.cc

OBJECTS = RepObjStore.o DaemonList.o DaemonData.o Members.o RepObjStoreIF.o

LOCAL_CPPFLAGS = -I$(ARJUNA_INCLUDES) -I../include

all :: all.local
all.local : stubs $(LIB)
	@echo Made $? [in $(CURDIR) ] as $@

RepObjStore.o : RepObjStore.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) RepObjStore.cc

DaemonList.o : DaemonList.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) DaemonList.cc

DaemonData.o : DaemonData.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) DaemonData.cc

Members.o : Members.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) Members.cc

RepObjStoreIF.o : RepObjStoreIF.cc
	@echo Compiling $@
	@exec $(CPLUSPLUS) -c  $(LOCAL_CPPFLAGS) $(CPPFLAGS) $(LOCAL_CPLUSPLUSFLAGS) $(CPLUSPLUSFLAGS) RepObjStoreIF.cc

$(LIB) : $(OBJECTS)
	@echo Updating library $@
	@$(AR) $(ARFLAGS) $@ $?
	@echo Ranlib-ing library $@
	@$(RANLIB) $@

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

DaemonData.o: ../include/DaemonData.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/Boolean.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/Debug.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/Debug.n
DaemonData.o: /home/nmcl/Arjuna/Include/Common/FacCode.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
DaemonData.o: /home/nmcl/Arjuna/Include/Common/Filtsbuf.h
DaemonData.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
DaemonData.o: /home/nmcl/Arjuna/Include/System/iostream.h
DaemonData.o: /usr/include/_G_config.h /usr/include/g++/iostream.h
DaemonData.o: /usr/include/g++/streambuf.h /usr/include/libio.h
DaemonData.o: /usr/include/sys/cdefs.h
DaemonList.o: ../include/DaemonData.h ../include/DaemonList.h
DaemonList.o: ../include/Members.h
DaemonList.o: /home/nmcl/Arjuna/Include/ArjServers/RPObjStore_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/ArjServers/RPObjStore_stubclass.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/Lock.n
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/LockMan_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/LockManager_stubclass.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/Lock_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.n
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/PrepOut.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Arjuna/StateManager_stubclass.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Boolean.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Buffer.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Buffer.n
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Buffer_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Debug.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Debug.n
DaemonList.o: /home/nmcl/Arjuna/Include/Common/ErrCodes.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Error.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Error.n
DaemonList.o: /home/nmcl/Arjuna/Include/Common/ErrorList.n
DaemonList.o: /home/nmcl/Arjuna/Include/Common/ErrorList_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Error_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/FacCode.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Filtsbuf.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/PointerMap.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Uid.h
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Uid.n
DaemonList.o: /home/nmcl/Arjuna/Include/Common/Uid_stub.h
DaemonList.o: /home/nmcl/Arjuna/Include/Config/Configure.h
DaemonList.o: /home/nmcl/Arjuna/Include/Config/c_gcc_2_7.cf
DaemonList.o: /home/nmcl/Arjuna/Include/Config/m_unknown.cf
DaemonList.o: /home/nmcl/Arjuna/Include/Config/s_linux_2_0.cf
DaemonList.o: /home/nmcl/Arjuna/Include/Config/v_unknown.cf
DaemonList.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.h
DaemonList.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.n
DaemonList.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjectStoreNames.h
DaemonList.o: /home/nmcl/Arjuna/Include/RPC/ClStatus.h
DaemonList.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/ClRaj.h
DaemonList.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/ClRajAct.h
DaemonList.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/multicast.h
DaemonList.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/rpcuser.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/Abort.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpc.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.n
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.n
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/SvrErrCode.h
DaemonList.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/iostream.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/memory.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/protos.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/stddef.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/stdlib.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/string.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/sys/ipc.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/sys/param.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/sys/socket.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/sys/types.h
DaemonList.o: /home/nmcl/Arjuna/Include/System/sys/uio.h
DaemonList.o: /usr/include/_G_config.h /usr/include/alloca.h
DaemonList.o: /usr/include/asm/errno.h /usr/include/asm/param.h
DaemonList.o: /usr/include/asm/posix_types.h /usr/include/asm/socket.h
DaemonList.o: /usr/include/asm/sockios.h /usr/include/asm/types.h
DaemonList.o: /usr/include/errno.h /usr/include/features.h
DaemonList.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
DaemonList.o: /usr/include/libio.h /usr/include/limits.h
DaemonList.o: /usr/include/linux/errno.h /usr/include/linux/ipc.h
DaemonList.o: /usr/include/linux/limits.h /usr/include/linux/param.h
DaemonList.o: /usr/include/linux/posix_types.h /usr/include/linux/socket.h
DaemonList.o: /usr/include/linux/sockios.h /usr/include/linux/types.h
DaemonList.o: /usr/include/linux/uio.h /usr/include/posix1_lim.h
DaemonList.o: /usr/include/posix2_lim.h /usr/include/stdlib.h
DaemonList.o: /usr/include/string.h /usr/include/sys/bitypes.h
DaemonList.o: /usr/include/sys/cdefs.h /usr/include/sys/ipc.h
DaemonList.o: /usr/include/sys/param.h /usr/include/sys/socket.h
DaemonList.o: /usr/include/sys/types.h /usr/include/sys/uio.h
DaemonList.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/limits.h
DaemonList.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/stddef.h
DaemonList.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/syslimits.h
Members.o: ../include/Members.h /home/nmcl/Arjuna/Include/Common/Boolean.h
Members.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
Members.o: /home/nmcl/Arjuna/Include/Common/Debug.h
Members.o: /home/nmcl/Arjuna/Include/Common/Debug.n
Members.o: /home/nmcl/Arjuna/Include/Common/FacCode.h
Members.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
Members.o: /home/nmcl/Arjuna/Include/Common/Filtsbuf.h
Members.o: /home/nmcl/Arjuna/Include/Common/Uid.h
Members.o: /home/nmcl/Arjuna/Include/Common/Uid.n
Members.o: /home/nmcl/Arjuna/Include/Common/Uid_stub.h
Members.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
Members.o: /home/nmcl/Arjuna/Include/System/iostream.h
Members.o: /usr/include/_G_config.h /usr/include/g++/iostream.h
Members.o: /usr/include/g++/streambuf.h /usr/include/libio.h
Members.o: /usr/include/sys/cdefs.h
RepObjStore.o: ../include/DaemonData.h ../include/DaemonList.h
RepObjStore.o: ../include/Members.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/ArjName.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/ArjName_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/GroupData.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/NSInterface.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/NameInfo.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/NameServ_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/NameServer_stubclass.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/RPObjStore_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/RPObjStore_stubclass.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/RepDesc_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData.n
RepObjStore.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/AtomicA.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/AtomicA.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/Lock.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/LockMan.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/LockMan_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/LockManager_stubclass.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/Lock_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/PrepOut.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Arjuna/StateManager_stubclass.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Boolean.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Buffer.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Buffer.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Buffer_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Debug.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Debug.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/ErrCodes.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Error.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Error.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/ErrorList.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/ErrorList_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Error_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/FacCode.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Filtsbuf.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/PointerMap.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Uid.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Uid.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Common/Uid_stub.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Config/Configure.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Config/c_gcc_2_7.cf
RepObjStore.o: /home/nmcl/Arjuna/Include/Config/m_unknown.cf
RepObjStore.o: /home/nmcl/Arjuna/Include/Config/s_linux_2_0.cf
RepObjStore.o: /home/nmcl/Arjuna/Include/Config/v_unknown.cf
RepObjStore.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.h
RepObjStore.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.n
RepObjStore.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjectStoreNames.h
RepObjStore.o: /home/nmcl/Arjuna/Include/RPC/ClStatus.h
RepObjStore.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/ClRaj.h
RepObjStore.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/ClRajAct.h
RepObjStore.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/multicast.h
RepObjStore.o: /home/nmcl/Arjuna/Include/RPC/Rajdoot/rpcuser.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Replication/RepObjStore.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Replication/RepObjStore.n
RepObjStore.o: /home/nmcl/Arjuna/Include/Replication/RepObjStoreIF.h
RepObjStore.o: /home/nmcl/Arjuna/Include/Replication/RepObjStoreIF.n
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/Abort.h
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpc.h
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.h
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.n
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.h
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.n
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/SvrErrCode.h
RepObjStore.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/errno.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/iostream.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/memory.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/netdb.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/protos.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/signal.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/stddef.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/stdlib.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/string.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/strstream.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/sys/ipc.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/sys/param.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/sys/socket.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/sys/types.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/sys/uio.h
RepObjStore.o: /home/nmcl/Arjuna/Include/System/unistd.h
RepObjStore.o: /usr/include/_G_config.h /usr/include/alloca.h
RepObjStore.o: /usr/include/asm/errno.h /usr/include/asm/param.h
RepObjStore.o: /usr/include/asm/posix_types.h /usr/include/asm/signal.h
RepObjStore.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
RepObjStore.o: /usr/include/asm/types.h /usr/include/confname.h
RepObjStore.o: /usr/include/errno.h /usr/include/features.h
RepObjStore.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
RepObjStore.o: /usr/include/g++/strfile.h /usr/include/g++/strstream.h
RepObjStore.o: /usr/include/gnu/types.h /usr/include/libio.h
RepObjStore.o: /usr/include/limits.h /usr/include/linux/errno.h
RepObjStore.o: /usr/include/linux/ipc.h /usr/include/linux/limits.h
RepObjStore.o: /usr/include/linux/param.h /usr/include/linux/posix_types.h
RepObjStore.o: /usr/include/linux/signal.h /usr/include/linux/socket.h
RepObjStore.o: /usr/include/linux/sockios.h /usr/include/linux/types.h
RepObjStore.o: /usr/include/linux/uio.h /usr/include/netdb.h
RepObjStore.o: /usr/include/paths.h /usr/include/posix1_lim.h
RepObjStore.o: /usr/include/posix2_lim.h /usr/include/posix_opt.h
RepObjStore.o: /usr/include/signal.h /usr/include/stdlib.h
RepObjStore.o: /usr/include/string.h /usr/include/sys/bitypes.h
RepObjStore.o: /usr/include/sys/cdefs.h /usr/include/sys/ipc.h
RepObjStore.o: /usr/include/sys/param.h /usr/include/sys/socket.h
RepObjStore.o: /usr/include/sys/types.h /usr/include/sys/uio.h
RepObjStore.o: /usr/include/unistd.h
RepObjStore.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/limits.h
RepObjStore.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/stddef.h
RepObjStore.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/syslimits.h
RepObjStoreIF.o: ../include/ReplicationR.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ArjServers/RepDesc_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ArjServers/ReplicaData_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/AbstractR.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/AtomicA.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/AtomicA.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/ObjState_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/PrepOut.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/RecordT.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/StateMan_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Arjuna/StateManager_stubclass.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Boolean.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Buffer.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Buffer.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Buffer_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/CommonT.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Debug.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Debug.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/ErrCodes.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Error.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Error.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/ErrorList.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/ErrorList_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Error_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/FacCode.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/FacCode_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Filtsbuf.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/PointerMap.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Uid.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Uid.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Common/Uid_stub.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjStore.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/ObjectStore/ObjectStoreNames.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/RPC/ClStatus.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Replication/RepObjStore.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Replication/RepObjStore.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Replication/RepObjStoreIF.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/Replication/RepObjStoreIF.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/Abort.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/ClientRpcMan.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/RpcBuffer.n
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/StubGen/Types.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/System/iostream.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/System/signal.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/System/stddef.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/System/string.h
RepObjStoreIF.o: /home/nmcl/Arjuna/Include/System/sys/types.h
RepObjStoreIF.o: /usr/include/_G_config.h /usr/include/asm/posix_types.h
RepObjStoreIF.o: /usr/include/asm/signal.h /usr/include/asm/types.h
RepObjStoreIF.o: /usr/include/features.h /usr/include/g++/iostream.h
RepObjStoreIF.o: /usr/include/g++/streambuf.h /usr/include/libio.h
RepObjStoreIF.o: /usr/include/linux/posix_types.h /usr/include/linux/signal.h
RepObjStoreIF.o: /usr/include/linux/types.h /usr/include/signal.h
RepObjStoreIF.o: /usr/include/string.h /usr/include/sys/bitypes.h
RepObjStoreIF.o: /usr/include/sys/cdefs.h /usr/include/sys/types.h
RepObjStoreIF.o: /usr/lib/gcc-lib/i386-linux/2.7.2.1/include/stddef.h
