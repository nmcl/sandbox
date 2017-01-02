/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_SpecificData.cc,v 1.3 1998/06/01 13:38:23 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

/*
 *
 * OmniOrb thread system simple thread specfic data
 *
 * omni_threads actually supports thread specific data, but its
 * model is broken.
 */

class OmniORB_SpecificDataImple : public ThreadDataImple
{
public:
    OmniORB_SpecificDataImple ();
    ~OmniORB_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
};

OmniORB_SpecificDataImple::OmniORB_SpecificDataImple ()
{
    valid(TRUE);
}

OmniORB_SpecificDataImple::~OmniORB_SpecificDataImple ()
{
    removeSpecific();
}

Boolean OmniORB_SpecificDataImple::getSpecific ( void *& anyData )
{
    if (valid())
    {
	return getFromOverflow(anyData);
    }
    
    return FALSE;
}

Boolean OmniORB_SpecificDataImple::setSpecific ( void *anyData )
{
    if (valid())
    {
	return addToOverflow(anyData);
    }
    
    return FALSE;
}

Boolean OmniORB_SpecificDataImple::removeSpecific ()
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
    OmniORB_SpecificDataImple *temp = new OmniORB_SpecificDataImple();

    if (temp)
    {
	if (!temp->valid())
	{
	    delete temp;
	    temp = 0;
	}
    }
	
    return temp;
}
