/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BASICTIME_H_

#ifndef STUB
#  include "BasicTime_stub.h"
#else

/*
 * The remainder of this file processed only by the stub generator.
 */

#define BASICTIME_H_

/*
 * $Id: BasicTime.h,v 1.1 1994/01/06 12:20:57 ngdp Exp $
 *
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

// @NoRemote, @NoMarshall
class RpcBuffer;

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class BasicTime
{
public:
    BasicTime();

#ifdef SYSV
    BasicTime(clock_t);
#else
    BasicTime(timeval);
#endif

    ~BasicTime();

    ostream& printOn(ostream&) const;

    BasicTime operator-(const BasicTime&) const;
    BasicTime operator+(const BasicTime&) const;
    BasicTime& operator=(const BasicTime&);
    
    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);
    
private:

#ifdef SYSV
    clock_t theTime;
#else
    timeval theTime;
#endif
};

inline ostream& operator<< ( ostream& s, const BasicTime& t )
{
    return t.printOn(s);
}

inline BasicTime& BasicTime::operator= ( const BasicTime& setTo )
{
    theTime = setTo.theTime;
    return *this;
}

#  endif
#endif
