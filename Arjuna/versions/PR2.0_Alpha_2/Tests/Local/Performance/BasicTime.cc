/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BasicTime.cc,v 1.1 1993/11/03 15:01:12 nmcl Exp $
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

BasicTime::~BasicTime()
{
}

ostream& BasicTime::printOn ( ostream& strm )
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

ostream& operator<< ( ostream& o, BasicTime& T )
{
    return T.printOn(o);
}

BasicTime BasicTime::operator- ( BasicTime& greater )
{
    BasicTime newtime;
    BasicTime *first, *second;

#ifndef SYSV
    if ((greater.theTime.tv_sec > theTime.tv_sec) ||
	((greater.theTime.tv_sec == theTime.tv_sec) &&
	 (greater.theTime.tv_usec > theTime.tv_usec)))
#else
    if (greater.theTime > theTime)
#endif
    {
	first = this;
	second = &greater;
    }
    else
    {
	first = &greater;
	second = this;
    }

#ifndef SYSV
    newtime.theTime.tv_sec = second->theTime.tv_sec - first->theTime.tv_sec;
    newtime.theTime.tv_usec = second->theTime.tv_usec - first->theTime.tv_usec;

    if (second->theTime.tv_usec < first->theTime.tv_usec)
    {
        newtime.theTime.tv_sec--;
        newtime.theTime.tv_usec += 1000000;
    }
#else
    newtime.theTime = second->theTime - first->theTime;
#endif

    return newtime;
}
