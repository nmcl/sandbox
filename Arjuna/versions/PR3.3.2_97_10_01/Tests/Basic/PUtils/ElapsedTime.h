/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef ELAPSEDTIME_H_

#ifndef STUB
#  include "ElapsedTime_stub.h"
#else

/*
 * The remainder of this file processed only by the stub generator.
 */
#define ELAPSEDTIME_H_

/*
 * $Id: ElapsedTime.h,v 1.1 1994/01/06 12:21:08 ngdp Exp $
 *
 *
 */

#include "BasicTime.h"

//@NoRemote,@NoMarshall
class ostream;

// @NoRemote
class ElapsedTime
{
public:
    ElapsedTime();
    ElapsedTime (const BasicTime&);
    ~ElapsedTime();

    void get();
    ostream& printOn(ostream&) const;

    ElapsedTime operator- (const ElapsedTime&);
    ElapsedTime operator+ (const ElapsedTime&);
private:
    BasicTime elapsed;
};

inline ostream& operator<< ( ostream& o, const ElapsedTime& T )
{
    return T.printOn(o);
}

inline ElapsedTime ElapsedTime::operator- ( const ElapsedTime& other )
{
    return elapsed - other.elapsed;
}

inline ElapsedTime ElapsedTime::operator+ ( const ElapsedTime& other )
{
    return elapsed + other.elapsed;
}

#  endif
#endif
