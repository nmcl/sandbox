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
#include <Config/s_darwin.cf>
#include <Config/j_11_0.cf>

#define OSNAME Darwin
#define OSRELEASE 17.7.0
#define OSMAJOR 17
#define OSMINOR 7
#define MACHINE_HW "x86_64"

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO  0
#endif

/*
 *  Make and Source configuration
 */

#ifndef TOPDIR_DEFAULT
#define TOPDIR_DEFAULT /Users/marklittle/github/sandbox/JavaArjuna/v0.1.0
#endif

#ifndef PURIFY_PROGRAM
#define PURIFY_PROGRAM  "purify"
#endif

#ifndef JAVA_PROGRAM
#define JAVA_PROGRAM  "javac"
#endif

#ifndef JAVAFLAGS_DEFAULT
#define JAVAFLAGS_DEFAULT 
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
#define INSTALL_TOPDIR_DEFAULT /usr/local/JavaArjuna
#endif

/*
 * Location of servers to be initiated
 */

#ifndef SERVER_LOCATION
#define SERVER_LOCATION /usr/local/JavaArjuna/servers
#endif

/*
 *  Quoted versions.
 */

#ifndef INSTALL_TOPDIR_DEFAULT_Q
#define INSTALL_TOPDIR_DEFAULT_Q "/usr/local/JavaArjuna"
#endif

#ifndef SERVER_LOCATION_Q
#define SERVER_LOCATION_Q "/usr/local/JavaArjuna/servers"
#endif


#endif
