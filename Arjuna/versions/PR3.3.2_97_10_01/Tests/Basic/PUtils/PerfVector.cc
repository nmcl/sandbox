/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: PerfVector.cc,v 1.3 1994/12/09 15:40:07 ngdp Exp $
 *
 */


#include <System/iostream.h>

#include "PerfVector.h"

PerformanceVector::PerformanceVector ()
{
}

PerformanceVector::PerformanceVector ( const PerformanceVector& other)
{
    *this = other;
}

PerformanceVector::~PerformanceVector ()
{
}
      
ostream& PerformanceVector::printOn ( ostream& strm )
{
    for (int i = LOCK; i < LAST; i++)
    {
	switch (i)
	{
	case LOCK:
	    strm << "Concurrency Control : ";
	    break;
	case ACTION:
	    strm << "Action Management : ";
	    break;
	case OBJSTORELOAD:
	    strm << "ObjectStore Load : ";
	    break;
	case OBJSTOREUNLOAD:
	    strm << "ObjectStore Unload : ";
	    break;
	}
	
	strm << systemTimes[i] << " " << elapsed[i] << endl;
    }

    return strm;
}
    
void PerformanceVector::setElement ( VectorIndex vi, const CpuTime& cpu, 
				     const ElapsedTime& el )
{
//    cout << "index :" << vi << " Times : " << cpu << el << endl;
    
    systemTimes[vi] = cpu;
    elapsed[vi] = el;
}


PerformanceVector PerformanceVector::operator+ ( const PerformanceVector& other )
{
    PerformanceVector v;

    for (int i = LOCK; i < LAST; i++)
    {
	v.systemTimes[i] = systemTimes[i] + other.systemTimes[i];
	v.elapsed[i] = elapsed[i] + other.elapsed[i];
    }
    
    return v;
}

PerformanceVector& PerformanceVector::operator+= ( const PerformanceVector& other )
{

    if (this == &other)
	return *this;
    
    for (int i = LOCK; i < LAST; i++)
    {
	systemTimes[i] = systemTimes[i] + other.systemTimes[i];
	elapsed[i] = elapsed[i] + other.elapsed[i];
    }
    
    return *this;
}

PerformanceVector& PerformanceVector::operator= ( const PerformanceVector& other )
{

    if (this == &other)
	return *this;
    
    for (int i = LOCK; i < LAST; i++)
    {
	systemTimes[i] = other.systemTimes[i];
	elapsed[i] = other.elapsed[i];
    }
    
    return *this;
}
