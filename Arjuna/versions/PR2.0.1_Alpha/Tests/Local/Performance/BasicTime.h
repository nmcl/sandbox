/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BASICTIME_H_
#define BASICTIME_H_

/*
 * $Id: BasicTime.h,v 1.1 1993/11/03 15:01:13 nmcl Exp $
 *
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif


#ifdef SYSV
typedef clock_t clockType;
#else
typedef struct timeval clockType;
#endif

class ostream;

class BasicTime
{
public:
    BasicTime();
    ~BasicTime();

    ostream& printOn(ostream&);
    void setTime(const clockType&);

    BasicTime operator-(BasicTime&);

private:
    clockType theTime;
};

extern ostream& operator<<(ostream& o, BasicTime& T);

inline void BasicTime::setTime ( const clockType& setTo )
{
    theTime = setTo;
}
#endif
