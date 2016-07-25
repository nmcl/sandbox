/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef CPUTIME_H_
#define CPUTIME_H_

/*
 * $Id: CpuTime.h,v 1.1 1993/09/06 10:41:40 ngdp Exp $
 *
 *
 */

#include "BasicTime.h"
class ostream;

class CpuTime
{
public:
    CpuTime();
    CpuTime (const BasicTime&, const BasicTime&);
    ~CpuTime();

    void get();
    ostream& printOn(ostream&);

    CpuTime operator- (CpuTime&);

private:
    BasicTime systemTime;
    BasicTime userTime;
};

inline ostream& operator<<(ostream& o, CpuTime& T)
{
    return T.printOn(o);
}

inline CpuTime CpuTime::operator- ( CpuTime& other )
{
    return CpuTime(userTime - other.userTime, systemTime - other.systemTime);
}

#endif
