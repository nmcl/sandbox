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
 * $Id: Time.h,v 1.3 1993/03/18 15:06:53 n048z Exp $
 *
 * Class to provide an oo interface to the timeval structure
 * used by gettimeofday etc.
 *
 */

#include <System/sys/time.h>
#include <System/iostream.h>

class Time : public timeval
{
public:
    Time();
    Time(struct timeval);
    Time(unsigned long, unsigned long);
    ~Time();

    void get();

    ostream& printOn(ostream&);
    Time operator-(Time&);
};

extern ostream& operator<<(ostream& o, Time& T);

#endif
