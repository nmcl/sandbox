/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CPPTask_SpecificData.cc,v 1.1 1998/01/12 13:06:52 nmcl Exp $
 */

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class CPP_SpecificDataImple : public ThreadDataImple
{
public:
    CPP_SpecificDataImple ();
    ~CPP_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};


CPP_SpecificDataImple::CPP_SpecificDataImple ()
{
    valid(TRUE);
}

CPP_SpecificDataImple::~CPP_SpecificDataImple ()
{
    removeSpecific();
}


Boolean CPP_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	return addToOverflow(threadData);
    }
    
    return FALSE;    
}

Boolean CPP_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean CPP_SpecificDataImple::removeSpecific ()
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
    return new CPP_SpecificDataImple;
}
