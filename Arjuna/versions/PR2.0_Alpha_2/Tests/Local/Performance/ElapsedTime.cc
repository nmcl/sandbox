/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ElapsedTime.cc,v 1.1 1993/11/03 15:01:18 nmcl Exp $
 *
 */


#ifndef SYSV
#  include <System/sys/time.h>
#else
#  include <System/unistd.h>
#  include <System/sys/times.h>
#  include <System/sys/limits.h>
#endif
#include <System/iostream.h>

#include "ElapsedTime.h"

ElapsedTime::ElapsedTime() : elapsed()
{
}

ElapsedTime::ElapsedTime(const BasicTime& other) : elapsed(other)
{
}

ElapsedTime::~ElapsedTime()
{
}

#ifndef SYSV
void ElapsedTime::get()
{
    timeval timeofday;
    gettimeofday(&timeofday, 0);
    elapsed.setTime(timeofday);
}
#else
void ElapsedTime::get()
{
    clock_t ticks;
    tms timebuffer;

    ticks = times(&timebuffer);
    elapsed.setTime(ticks);
}
#endif


ostream& ElapsedTime::printOn ( ostream& strm )
{
    strm << elapsed;
    return strm;
}
