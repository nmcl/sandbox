/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PC_SpecificData.cc,v 1.1 1998/01/12 13:07:27 nmcl Exp $
 */

#include <os/iostream.h>
#include <os/sys/types.h>
#include <os/errno.h>
#include <os/pthread/pthread.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

class PC_SpecificDataImple : public ThreadDataImple
{
public:
    PC_SpecificDataImple ();
    ~PC_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();

private:
    pthread_key_t _key;
};


PC_SpecificDataImple::PC_SpecificDataImple ()
					   : _key(0)
{
    if (pthread_key_create(&_key, 0) == 0)
	valid(TRUE);
}

PC_SpecificDataImple::~PC_SpecificDataImple ()
{
    /*
     * It is up to the user to guarantee that all thread data
     * is NULL at this point, or the results are not specified.
     */

#ifdef HAVE_KEYDELETE    
    (void) pthread_key_delete(_key);
#else
    removeSpecific();
#endif    
}

Boolean PC_SpecificDataImple::setSpecific (void* threadData)
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

Boolean PC_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	int error = pthread_getspecific(_key, &threadData);

	if ((!threadData) || (error != 0))
	    return getFromOverflow(threadData);
	else
	    return TRUE;
    }

    return FALSE;
}

Boolean PC_SpecificDataImple::removeSpecific ()
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
    return new PC_SpecificDataImple;
}