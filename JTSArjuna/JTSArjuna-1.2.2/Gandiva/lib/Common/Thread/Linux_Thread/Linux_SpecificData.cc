/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linux_SpecificData.cc,v 1.1 1998/01/12 13:07:05 nmcl Exp $
 */

#include <os/sys/types.h>
#include <os/errno.h>
#include <os/pthread/pthread.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class Linux_SpecificDataImple : public ThreadDataImple
{
public:
    Linux_SpecificDataImple ();
    ~Linux_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
    
private:
    pthread_key_t _key;
};


Linux_SpecificDataImple::Linux_SpecificDataImple ()
						 : _key(0)
{
    if (pthread_key_create(&_key, 0) == 0)
	valid(TRUE);
}

Linux_SpecificDataImple::~Linux_SpecificDataImple ()
{
#ifdef PTHREAD_DRAFT_8    
    /*
     * It is up to the user to guarantee that all thread data
     * is NULL at this point, or the results are not specified.
     */
    
    (void) pthread_key_delete(_key);
#else
    removeSpecific();
#endif
}

Boolean Linux_SpecificDataImple::setSpecific (void* threadData)
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

Boolean Linux_SpecificDataImple::getSpecific (void*& threadData)
{
    /*
     * To check for insufficient memory.
     */
    
    if (valid())
    {
	threadData = pthread_getspecific(_key);

	if (threadData)
	    return TRUE;
	else
	    return getFromOverflow(threadData);
    }
    else
	return FALSE;
}

/*
 * Assume we cannot be on both lists at once!
 */

Boolean Linux_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	/*
	 * We have to do it this way since setspecific doesn't return
	 * an error. Could call getspecific first ...
	 */
	
	if (removeFromOverflow())
	    return TRUE;
	
	if (pthread_setspecific(_key, (any_t) 0) == 0)
	    return TRUE;
    }
    
    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    return new Linux_SpecificDataImple;
}
