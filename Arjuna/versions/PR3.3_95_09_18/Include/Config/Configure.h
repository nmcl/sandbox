/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

/*
 *  Configure.h
 */

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef CONFIGURE_H_
#  define CONFIGURE_H_
#include <Config/m_unknown.cf>
#include <Config/v_unknown.cf>
#include <Config/s_linux_2_0.cf>
#include <Config/c_gcc_2_7.cf>

#define OSNAME Linux
#define OSRELEASE 2.0.29
#define OSMAJOR 2
#define OSMINOR 0
#define MACHINE_HW "i586"


#define CPLUSPLUS_CROSS_PROGRAM "g++"

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

/*
 *  Make and Source configuration
 */

#ifndef VERBOSE_MAKE
#define VERBOSE_MAKE NO
#endif

#ifndef TOPDIR_DEFAULT
#define TOPDIR_DEFAULT /home/nmcl/Arjuna
#endif

#define RPC_RajdootAction_ 1
#define RPC_Rajdoot_       2

#ifndef RPC_SYSTEM_TYPE
#define RPC_SYSTEM_TYPE RPC_RajdootAction_
#endif

#ifndef PURIFY_PROGRAM
#define PURIFY_PROGRAM  "exec"
#endif

#ifndef LEX_PROGRAM
#define LEX_PROGRAM  "flex"
#endif

#ifndef LEXFLAGS_DEFAULT
#define LEXFLAGS_DEFAULT 
#endif

#ifndef LEX_LIBRARY
#define LEX_LIBRARY 
#endif

#ifndef YACC_PROGRAM
#define YACC_PROGRAM  "bison"
#endif

#ifndef YACCFLAGS_DEFAULT
#define YACCFLAGS_DEFAULT -y
#endif

#ifndef YACC_LIBRARY
#define YACC_LIBRARY 
#endif

#ifndef CPLUSPLUS_PROGRAM
#define CPLUSPLUS_PROGRAM  "g++"
#endif

#ifndef CPLUSPLUS_INCLUDE_DEFAULT
#define CPLUSPLUS_INCLUDE_DEFAULT /usr/include/g++
#endif

#ifndef CPPFLAGS_DEFAULT
#define CPPFLAGS_DEFAULT 
#endif

#ifndef CPLUSPLUSFLAGS_DEFAULT
#define CPLUSPLUSFLAGS_DEFAULT 
#endif

#ifndef LDFLAGS_DEFAULT
#define LDFLAGS_DEFAULT 
#endif

#ifndef xLIBRARIES_DEFAULT
#define xLIBRARIES_DEFAULT 
#endif

#ifndef THREAD_LIB_DEFAULT
#define THREAD_LIB_DEFAULT  libNoneThread.a
#endif

/*
 *  Installation details
 */

#ifndef INSTALL_TOPDIR_DEFAULT
#define INSTALL_TOPDIR_DEFAULT /home/nmcl/Install/ArjunaInstall
#endif

#ifndef HAVE_INTERVIEWS
#define HAVE_INTERVIEWS NO
#endif

/*
 *  Crash Recovery Information
 */

#ifndef ADMIN_EMAIL
#define ADMIN_EMAIL "nmcl@tarry-f.ncl.ac.uk"
#endif

#ifndef MAILER
#define MAILER "/bin/mail"
#endif

/*
 *   ClassLib Information
 */

#ifndef OBJECTSTOREDIR
#define OBJECTSTOREDIR /home/nmcl/Install/ArjunaInstall/ObjectStore
#endif

#ifndef ATOMICACTIONDIR
#define ATOMICACTIONDIR /home/nmcl/Install/ArjunaInstall/ObjectStore/StateManager/AtomicAction
#endif

#ifndef SERVERATOMICACTIONDIR
#define SERVERATOMICACTIONDIR /home/nmcl/Install/ArjunaInstall/ObjectStore/StateManager/AtomicAction/ServerAtomicAction
#endif

/*
 *   Rajdoot Information
 */

#ifndef C_LIST  
#define C_LIST /home/nmcl/Install/ArjunaInstall/adm/c_list
#endif

#ifndef C_COUNT
#define C_COUNT /home/nmcl/Install/ArjunaInstall/adm/c_count
#endif

#ifndef MANPID_FILE
#define MANPID_FILE /home/nmcl/Install/ArjunaInstall/adm/manager_pid
#endif

#ifndef NSLOCK_FILE
#define NSLOCK_FILE 
#endif

#ifndef DUPSERVER
#define DUPSERVER /home/nmcl/Install/ArjunaInstall/etc/dupserver
#endif

#ifndef GMSERVER
#define GMSERVER /home/nmcl/Install/ArjunaInstall/etc/groupsrvr
#endif

#ifndef PATH_SIZE
#define PATH_SIZE 10
#endif

#ifndef BROADCAST_NET
#define BROADCAST_NET "localnet"
#endif

#ifndef RAJDOOT_USE_BROADCAST
#define RAJDOOT_USE_BROADCAST NO
#endif

/*
 * Location of servers to be initiated
 */

#ifndef SERVER_LOCATION
#define SERVER_LOCATION /home/nmcl/Install/ArjunaInstall/servers
#endif

#ifndef NAME_SERVER_SERVICE
#define NAME_SERVER_SERVICE NameServ
#endif

/*
 *  Quoted versions.
 */

#ifndef INSTALL_TOPDIR_DEFAULT_Q
#define INSTALL_TOPDIR_DEFAULT_Q "/home/nmcl/Install/ArjunaInstall"
#endif

#ifndef OBJECTSTOREDIR_Q
#define OBJECTSTOREDIR_Q "/home/nmcl/Install/ArjunaInstall/ObjectStore"
#endif

#ifndef ATOMICACTIONDIR_Q
#define ATOMICACTIONDIR_Q "/home/nmcl/Install/ArjunaInstall/ObjectStore/StateManager/AtomicAction"
#endif

#ifndef SERVERATOMICACTIONDIR_Q
#define SERVERATOMICACTIONDIR_Q "/home/nmcl/Install/ArjunaInstall/ObjectStore/StateManager/AtomicAction/ServerAtomicAction"
#endif

#ifndef C_LIST_Q  
#define C_LIST_Q "/home/nmcl/Install/ArjunaInstall/adm/c_list"
#endif

#ifndef C_COUNT_Q
#define C_COUNT_Q "/home/nmcl/Install/ArjunaInstall/adm/c_count"
#endif

#ifndef MANPID_FILE_Q
#define MANPID_FILE_Q "/home/nmcl/Install/ArjunaInstall/adm/manager_pid"
#endif

#ifndef NSLOCK_FILE_Q
#define NSLOCK_FILE_Q ""
#endif

#ifndef DUPSERVER_Q
#define DUPSERVER_Q "/home/nmcl/Install/ArjunaInstall/etc/dupserver"
#endif

#ifndef GMSERVER_Q
#define GMSERVER_Q "/home/nmcl/Install/ArjunaInstall/etc/groupsrvr"
#endif

#ifndef SERVER_LOCATION_Q
#define SERVER_LOCATION_Q "/home/nmcl/Install/ArjunaInstall/servers"
#endif

#ifndef NAME_SERVER_SERVICE_Q
#define NAME_SERVER_SERVICE_Q "NameServ"
#endif

#ifndef DEFAULT_OBJECTSTORE_Q
#define DEFAULT_OBJECTSTORE_Q "HashedStore"
#endif

#endif
