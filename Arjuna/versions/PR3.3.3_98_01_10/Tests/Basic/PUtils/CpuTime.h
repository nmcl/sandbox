/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef CPUTIME_H_

#ifndef STUB
#  include "CpuTime_stub.h"
#else

/*
 * The remainder of this file processed only by the stub generator.
 */
#define CPUTIME_H_

/*
 * $Id: CpuTime.h,v 1.1 1994/01/06 12:21:04 ngdp Exp $
 *
 *
 */

#include "BasicTime.h"

//@NoRemote,@NoMarshall
class ostream;

// @NoRemote
class CpuTime
{
public:
    CpuTime();
    CpuTime (const BasicTime& sysT, const BasicTime& usrT);
    ~CpuTime();

    void get();
    ostream& printOn(ostream&) const;

    CpuTime operator- (const CpuTime&);
    CpuTime operator+ (const CpuTime&);

private:
    BasicTime systemTime;
    BasicTime userTime;
};

inline ostream& operator<< ( ostream& o, const CpuTime& T )
{
    return T.printOn(o);
}

inline CpuTime CpuTime::operator- ( const CpuTime& other )
{
    return CpuTime(systemTime - other.systemTime, userTime - other.userTime);
}

inline CpuTime CpuTime::operator+ ( const CpuTime& other )
{
    return CpuTime(systemTime + other.systemTime, userTime + other.userTime);
}

#  endif
#endif
