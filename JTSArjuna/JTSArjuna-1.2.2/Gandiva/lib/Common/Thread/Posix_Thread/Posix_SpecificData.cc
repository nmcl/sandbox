/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Posix_SpecificData.cc,v 1.1 1998/01/12 13:07:31 nmcl Exp $
 */

#include <os/sys/types.h>
#include <os/errno.h>
#include <os/pthread/pthread.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class Posix_SpecificDataImple : public ThreadDataImple
{
public:
    Posix_SpecificDataImple ();
    ~Posix_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();

private:
    pthread_key_t _key;
};


Posix_SpecificDataImple::Posix_SpecificDataImple ()
						 : _key(0)
{
    if (pthread_key_create(&_key, 0) == 0)
	valid(TRUE);
}

Posix_SpecificDataImple::~Posix_SpecificDataImple ()
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

Boolean Posix_SpecificDataImple::setSpecific (void* threadData)
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

Boolean Posix_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_SUN)
	threadData = pthread_getspecific(_key);

	if (threadData)
	    return TRUE;
	else
	    return getFromOverflow(threadData);
#else
	/*
	 * Need to check this - what does the error value mean?
	 * Currently assume that it means the thread was not on
	 * the list.
	 */
	
	if (pthread_getspecific(_key, &threadData) == 0)
	    return TRUE;
	else
	    return getFromOverflow(threadData);
#endif	
    }

    return FALSE;
}

Boolean Posix_SpecificDataImple::removeSpecific ()
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
    return new Posix_SpecificDataImple;
}
