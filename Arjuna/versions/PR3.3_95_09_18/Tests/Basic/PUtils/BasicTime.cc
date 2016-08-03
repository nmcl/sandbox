/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BasicTime.cc,v 1.1 1994/01/06 12:20:56 ngdp Exp $
 *
 */

#ifndef SYSV
#  include <System/sys/time.h>
#else
#  include <System/unistd.h>
#  include <System/sys/times.h>
#  include <System/sys/limits.h>
#  define TICK (sysconf(_SC_CLK_TCK))
#endif

#include <System/iostream.h>
#include "BasicTime.h"

#ifndef SYSV
BasicTime::BasicTime ()
{
    theTime.tv_sec = theTime.tv_usec = 0;
}
#else
BasicTime::BasicTime () :theTime(0)
{
}
#endif

#ifndef SYSV
BasicTime::BasicTime ( timeval tv )
{
    theTime.tv_sec = tv.tv_sec; theTime.tv_usec = tv.tv_usec;
}
#else
BasicTime::BasicTime ( clock_t tv ) :theTime(tv)
{
}
#endif

BasicTime::~BasicTime()
{
}

ostream& BasicTime::printOn ( ostream& strm ) const
{
#ifndef SYSV
    double val = ((double) theTime.tv_sec * 1000000 + theTime.tv_usec) / 1000000;   
#else
    double val = (double) theTime / TICK;
#endif

    strm.setf(ios::fixed);
    strm << val;
    return strm;
}

BasicTime BasicTime::operator- ( const BasicTime& other ) const
{
    BasicTime newtime;
    const BasicTime *less, *greater;

#ifndef SYSV
    if ((other.theTime.tv_sec > theTime.tv_sec) ||
	((other.theTime.tv_sec == theTime.tv_sec) &&
	 (other.theTime.tv_usec > theTime.tv_usec)))
#else
    if (other.theTime > theTime)
#endif
    {
	less = this;
	greater = &other;
    }
    else
    {
	less = &other;
	greater = this;
    }

#ifndef SYSV
    newtime.theTime.tv_sec = greater->theTime.tv_sec - less->theTime.tv_sec;
    newtime.theTime.tv_usec = greater->theTime.tv_usec - less->theTime.tv_usec;

    while (newtime.theTime.tv_usec < 0)
    {
        newtime.theTime.tv_sec--;
        newtime.theTime.tv_usec += 1000000;
    }
#else
    newtime.theTime = greater->theTime - less->theTime;
#endif

    return newtime;
}

BasicTime BasicTime::operator+ ( const BasicTime& other ) const
{
    BasicTime newtime;

#ifndef SYSV
    newtime.theTime.tv_sec = theTime.tv_sec + other.theTime.tv_sec;
    newtime.theTime.tv_usec = theTime.tv_usec + other.theTime.tv_usec;

    while (newtime.theTime.tv_usec > 1000000)
    {
        newtime.theTime.tv_sec++;
        newtime.theTime.tv_usec -= 1000000;
    }
#else
    newtime.theTime = theTime + other.theTime;
#endif

    return newtime;
}
