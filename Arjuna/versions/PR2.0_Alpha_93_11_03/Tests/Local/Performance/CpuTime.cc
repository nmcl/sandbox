/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CpuTime.cc,v 1.1 1993/11/03 15:01:15 nmcl Exp $
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

CpuTime::CpuTime(const BasicTime& osys, const BasicTime& ouser ) : systemTime(osys), userTime(ouser)
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

    userTime.setTime(myUsage.ru_stime);
    systemTime.setTime(myUsage.ru_utime);
    
}
#else
void CpuTime::get()
{
    clock_t ticks;
    struct tms timebuffer;

    ticks = times(&timebuffer);
    userTime.setTime(timebuffer.tms_stime);
    systemTime.setTime(timebuffer.tms_utime);
}
#endif

ostream& CpuTime::printOn ( ostream& strm )
{
    strm << userTime << " (user) " << systemTime << " (system)";
    return strm;
}

