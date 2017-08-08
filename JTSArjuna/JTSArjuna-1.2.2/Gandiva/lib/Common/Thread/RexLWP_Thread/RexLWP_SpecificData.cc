/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RexLWP_SpecificData.cc,v 1.1 1998/01/12 13:07:43 nmcl Exp $
 */

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class RexLWP_SpecificDataImple : public ThreadDataImple
{
public:
    RexLWP_SpecificDataImple ();
    ~RexLWP_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};


RexLWP_SpecificDataImple::RexLWP_SpecificDataImple ()
{
    valid(TRUE);
}

RexLWP_SpecificDataImple::~RexLWP_SpecificDataImple ()
{
    removeSpecific();
}

Boolean RexLWP_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	return addToOverflow(threadData);
    }
    
    return FALSE;    
}

Boolean RexLWP_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean RexLWP_SpecificDataImple::removeSpecific ()
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
    return new RexLWP_SpecificDataImple;
}
