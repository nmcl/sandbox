/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PERFVECTOR_H_

#ifndef STUB
#  include "PerfVector_stub.h"
#else

#define PERFVECTOR_H_

/*
 * $Id: PerfVector.h,v 1.4 1994/12/09 15:40:09 ngdp Exp $
 *
 *
 */

#include "CpuTime.h"
#include "ElapsedTime.h"

enum VectorIndex { LOCK = 0, ACTION, OBJSTORELOAD, OBJSTOREUNLOAD, LAST };

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote
class PerformanceVector
{
public:
    PerformanceVector();
    PerformanceVector(const PerformanceVector&);
    ~PerformanceVector();

    void get();

    // @NoRemote
    ostream& printOn(ostream&);

    void setElement (VectorIndex, const CpuTime&, const ElapsedTime& );
    
    PerformanceVector operator+ (const PerformanceVector&);
    PerformanceVector& operator+= (const PerformanceVector&);
    PerformanceVector& operator= (const PerformanceVector&);

private:
    CpuTime systemTimes[LAST];
    ElapsedTime elapsed[LAST];
    
};

inline ostream& operator<<(ostream& o, PerformanceVector& T)
{
    return T.printOn(o);
}

#endif
#endif
