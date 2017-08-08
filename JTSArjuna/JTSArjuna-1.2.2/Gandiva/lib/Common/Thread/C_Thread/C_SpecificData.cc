/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_SpecificData.cc,v 1.1 1998/01/12 13:06:56 nmcl Exp $
 */

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class C_SpecificDataImple : public ThreadDataImple
{
public:
    C_SpecificDataImple ();
    ~C_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};


C_SpecificDataImple::C_SpecificDataImple ()
{
    valid(TRUE);
}

C_SpecificDataImple::~C_SpecificDataImple ()
{
    removeSpecific();
}

Boolean C_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	return addToOverflow(threadData);
    }
    
    return FALSE;    
}

Boolean C_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean C_SpecificDataImple::removeSpecific ()
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
    return new C_SpecificDataImple;
}
