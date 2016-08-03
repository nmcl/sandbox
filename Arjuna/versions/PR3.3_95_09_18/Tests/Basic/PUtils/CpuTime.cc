/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CpuTime.cc,v 1.1 1994/01/06 12:21:03 ngdp Exp $
 *
 */


#ifndef SYSV
#  include <System/sys/time.h>
#  include <System/sys/resource.h>
#else
#  include <System/unistd.h>
#  include <System/sys/times.h>
#  include <System/sys/limits.h>
#endif
#include <System/iostream.h>

#include "CpuTime.h"

CpuTime::CpuTime(const BasicTime& sysT, const BasicTime& usrT ) 
                : systemTime(sysT),
		  userTime(usrT)
{
}

CpuTime::CpuTime() : systemTime(), userTime()
{
}

CpuTime::~CpuTime()
{
}

#ifndef SYSV
void CpuTime::get()
{
    struct rusage myUsage;

    getrusage(RUSAGE_SELF, &myUsage);

    userTime = myUsage.ru_utime;
    systemTime = myUsage.ru_stime;
    
}
#else
void CpuTime::get()
{
    clock_t ticks;
    struct tms timebuffer;

    ticks = times(&timebuffer);
    userTime = timebuffer.tms_utime;
    systemTime = timebuffer.tms_stime;
}
#endif

ostream& CpuTime::printOn ( ostream& strm ) const
{
    strm << userTime << " (user secs) " << systemTime << " (system secs)";
    return strm;
}

