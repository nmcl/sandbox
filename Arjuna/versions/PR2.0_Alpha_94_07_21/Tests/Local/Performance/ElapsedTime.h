/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef ELAPSEDTIME_H_
#define ELAPSEDTIME_H_

/*
 * $Id: ElapsedTime.h,v 1.1 1993/09/06 10:41:46 ngdp Exp $
 *
 *
 */

#include "BasicTime.h"
class ostream;

class ElapsedTime
{
public:
    ElapsedTime();
    ElapsedTime (const BasicTime&);
    ~ElapsedTime();

    void get();
    ostream& printOn(ostream&);

    ElapsedTime operator- (ElapsedTime&);

private:
    BasicTime elapsed;
};

inline ostream& operator<<(ostream& o, ElapsedTime& T)
{
    return T.printOn(o);
}

inline ElapsedTime ElapsedTime::operator- ( ElapsedTime& other )
{
    return elapsed - other.elapsed;
}
#endif
