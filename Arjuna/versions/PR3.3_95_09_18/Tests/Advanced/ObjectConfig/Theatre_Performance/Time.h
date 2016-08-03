/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef TIME_H_
#define TIME_H_

/*
 * $Id: Time.h,v 1.2 1994/01/10 09:50:37 ngdp Exp $
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

class ostream;

class Time
{
public:
    Time();

#ifdef SYSV
    Time(clock_t);
#else
    Time(timeval);
#endif

    ~Time();

    void get ();
    
    ostream& printOn(ostream&) const;

    Time operator-(const Time&) const;
    Time operator+(const Time&) const;
    Time& operator=(const Time&);
    
private:

#ifdef SYSV
    clock_t theTime;
#else
    timeval theTime;
#endif
};

inline ostream& operator<< ( ostream& s, const Time& t )
{
    return t.printOn(s);
}

inline Time& Time::operator= ( const Time& setTo )
{
    theTime = setTo.theTime;
    return *this;
}

#endif
