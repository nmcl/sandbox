/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_SpecificData.cc,v 1.1 1998/01/12 13:07:47 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class Single_SpecificDataImple : public ThreadDataImple
{
public:
    Single_SpecificDataImple ();
    ~Single_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};


Single_SpecificDataImple::Single_SpecificDataImple ()
{
    valid(TRUE);
}

Single_SpecificDataImple::~Single_SpecificDataImple ()
{
    removeSpecific();
}

Boolean Single_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	return addToOverflow(threadData);
    }
    
    return FALSE;    
}

Boolean Single_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean Single_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	removeFromOverflow();
	return TRUE;
    }

    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    return new Single_SpecificDataImple;
}
