/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_SpecificData.cc,v 1.2 1998/01/16 14:32:10 nmcl Exp $
 */

/*
 * Implementation of thread specific state for ON threads
 */

#include <os/CORBA.ch>
#include <os/ON_init.h>

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class RCPORB_SpecificDataImple : public ThreadDataImple
{
public:
    RCPORB_SpecificDataImple ();
    ~RCPORB_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();

private:
    ON_ThreadKey key_;
};


RCPORB_SpecificDataImple::RCPORB_SpecificDataImple ()
						   : key_(0)
{
    if (ON_thread_key_create(&key_, 0) == 0)
	valid(TRUE);
}

RCPORB_SpecificDataImple::~RCPORB_SpecificDataImple ()
{
    removeSpecific();
    valid(FALSE);
}

Boolean RCPORB_SpecificDataImple::setSpecific ( void *anyData )
{
    if (valid())
    {
	int error = 0;

        if (((error = ON_thread_setspecific(key_, anyData)) != 0) &&
            ((error != ENOMEM) || !(addToOverflow(anyData))))
        {
            return FALSE;
        }

        return TRUE;
    }
    
    return FALSE;
}

Boolean RCPORB_SpecificDataImple::getSpecific ( void *& anyData )
{
    if (valid())
    {
	if (ON_thread_getspecific(key_, &anyData) == 0)
	{
	    return TRUE;
	}
        else
            return getFromOverflow(anyData);
    }
    
    return FALSE;
}


Boolean RCPORB_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	ON_thread_setspecific(key_, 0);
	removeFromOverflow();
	return TRUE;
    }

    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    RCPORB_SpecificDataImple* temp = new RCPORB_SpecificDataImple();

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


