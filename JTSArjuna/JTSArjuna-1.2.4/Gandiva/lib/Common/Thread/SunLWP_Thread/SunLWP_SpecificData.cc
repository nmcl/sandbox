/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SunLWP_SpecificData.cc,v 1.1 1998/01/12 13:07:57 nmcl Exp $
 */

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class SunLWP_SpecificDataImple : public ThreadDataImple
{
public:
    SunLWP_SpecificDataImple ();
    ~SunLWP_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};


SunLWP_SpecificDataImple::SunLWP_SpecificDataImple ()
{
    valid(TRUE);
}

SunLWP_SpecificDataImple::~SunLWP_SpecificDataImple ()
{
    removeSpecific();
}


Boolean SunLWP_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	return addToOverflow(threadData);
    }
    
    return FALSE;    
}

Boolean SunLWP_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean SunLWP_SpecificDataImple::removeSpecific ()
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
    return new SunLWP_SpecificDataImple;
}
