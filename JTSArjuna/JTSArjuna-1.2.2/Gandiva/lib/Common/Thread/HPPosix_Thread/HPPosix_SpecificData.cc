/*
 * Copyright (C) 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HPPosix_SpecificData.cc,v 1.1 1998/01/12 13:07:01 nmcl Exp $
 */

#include <os/sys/types.h>
#include <os/errno.h>
#include <os/pthread/pthread.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class HPPosix_SpecificDataImple : public ThreadDataImple
{
public:
    HPPosix_SpecificDataImple ();
    ~HPPosix_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();

private:
    pthread_key_t _key;
};


HPPosix_SpecificDataImple::HPPosix_SpecificDataImple ()
						 : _key(0)
{
    if (pthread_keycreate(&_key, 0) == 0)
	valid(TRUE);
}

HPPosix_SpecificDataImple::~HPPosix_SpecificDataImple ()
{
    removeSpecific();
}

Boolean HPPosix_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	int error = pthread_setspecific(_key, (any_t) threadData);

	if (error == ENOMEM)
	    return addToOverflow(threadData);
	else
	    return (Boolean) (error == 0);
    }
    
    return FALSE;
}

Boolean HPPosix_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	/*
	 * Need to check this - what does the error value mean?
	 * Currently assume that it means the thread was not on
	 * the list.
	 */
	
	if (pthread_getspecific(_key, &threadData) == 0)
	    return TRUE;
	else
	    return getFromOverflow(threadData);
    }

    return FALSE;
}

Boolean HPPosix_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	if (removeFromOverflow())
	    return TRUE;
	
	if (pthread_setspecific(_key, (any_t) 0) == 0)
	    return TRUE;
    }
    
    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    return new HPPosix_SpecificDataImple;
}
