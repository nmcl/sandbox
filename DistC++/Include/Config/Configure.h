/*
 * Copyright (C) 1993-1998,
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
#include <Config/m_sparc.cf>
#include <Config/v_sun.cf>
#include <Config/s_sunos_5_5.cf>
#include <Config/c_gcc_2_7.cf>
#include <Config/Override.h>

#define OSNAME SunOS
#define OSRELEASE 5.5.1
#define OSMAJOR 5
#define OSMINOR 5
#define MACHINE_HW "sun4m"


#define CPLUSPLUS_CROSS_PROGRAM ""

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

/*
 *  Make and Source configuration
 */

#ifndef DISTCPP_TOPDIR_DEFAULT
#define DISTCPP_TOPDIR_DEFAULT /home/nmcl/DistC++
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
#define CPLUSPLUS_INCLUDE_DEFAULT /usr/local/GNU/gcc/2.8.1/include/g++
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

/*
 *  Thread system details
 */

#ifndef THREAD_CPPFLAGS_DEFAULT
#define THREAD_CPPFLAGS_DEFAULT  -DHAVE_SOLARIS_THREADS -DHAVE_POSIX_THREADS -D_REENTRANT
#endif

#ifndef COMPILER_THREAD_FLAGS_DEFAULT
#define COMPILER_THREAD_FLAGS_DEFAULT 
#endif

#ifndef THREAD_LDFLAGS_DEFAULT
#define THREAD_LDFLAGS_DEFAULT 
#endif

#ifndef THREAD_LDLIBS_DEFAULT
#define THREAD_LDLIBS_DEFAULT  -lposix4
#endif

/*
 *  Installation details
 */

#ifndef INSTALL_DISTCPP_TOPDIR_DEFAULT
#define INSTALL_DISTCPP_TOPDIR_DEFAULT /usr/local/DistC++
#endif

/*
 * Location of servers to be initiated
 */

#ifndef SERVER_LOCATION
#define SERVER_LOCATION /usr/local/DistC++/servers
#endif

/*
 *  Quoted versions.
 */

#ifndef INSTALL_DISTCPP_TOPDIR_DEFAULT_Q
#define INSTALL_DISTCPP_TOPDIR_DEFAULT_Q "/usr/local/DistC++"
#endif

#ifndef SERVER_LOCATION_Q
#define SERVER_LOCATION_Q "/usr/local/DistC++/servers"
#endif

#ifndef MANPID_FILE_Q
#define MANPID_FILE_Q "/usr/local/DistC++/etc/manager.pid"
#endif

#endif
