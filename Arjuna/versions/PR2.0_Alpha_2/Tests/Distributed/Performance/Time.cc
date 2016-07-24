/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Time.cc,v 1.1 1993/11/03 14:58:39 nmcl Exp $
 *
 * Class to provide an oo interface to the timeval structure
 * used by gettimeofday etc.
 *
 */

#include <System/iostream.h>
#include "Time.h"

Time::Time()
{
    tv_sec = tv_usec = 0;
}

Time::Time(struct timeval time)
{
    tv_sec = time.tv_sec;
    tv_usec = time.tv_usec;
}

Time::Time(unsigned long secs, unsigned long usecs)
{
    tv_sec = secs;
    tv_usec = usecs;
}

Time::~Time()
{
}

void Time::get()
{
    gettimeofday(this, 0);
}

ostream& Time::printOn(ostream& strm)
{
    double val = (double) tv_sec;
    val += (double) tv_usec / 1e6;

    strm.setf(ios::fixed);
    strm << val;
    return strm;
}

ostream& operator<<(ostream& o, Time& T)
{
    return T.printOn(o);
}

Time Time::operator-(Time& greater)
{
    Time newtime;
    Time *first, *second;

    if ((greater.tv_sec > tv_sec) || ((greater.tv_sec == tv_sec) &&
				      (greater.tv_usec > tv_usec)))
    {
	first = this;
	second = &greater;
    }
    else
    {
	first = &greater;
	second = this;
    }

    newtime.tv_sec = second->tv_sec - first->tv_sec;
    newtime.tv_usec = second->tv_usec - first->tv_usec;

    if (second->tv_usec < first->tv_usec)
    {
	newtime.tv_sec--;
	newtime.tv_usec += 1000000;
    }

    return newtime;
}
