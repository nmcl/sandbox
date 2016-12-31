/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: Utility.h,v 1.4 1996/02/07 16:07:45 nsmw Exp $
 */

#ifndef COMMON_UTILITY_H_
#define COMMON_UTILITY_H_

#ifndef OS_UNISTD_H_
#   include <os/unistd.h>
#endif

#ifndef WIN32
#   ifndef OS_SYS_TYPES_H_
#       include <os/sys/types.h>
#   endif
#endif

/*
 * Prototypes for some generally useful routines
 */

/*
 * Append strings together and return the result.
 */

extern char* appendStrings (const char* appendTo, const char* toAppend);
extern char* appendStrings (const char* appendTo, const char* toAppend1,
			    const char* toAppend2);

#ifdef WIN32
class WinInit
{
public:
    WinInit ();
    ~WinInit ();
};

extern const WinInit _win;
#endif

#endif
