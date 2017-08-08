/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_SpecificData.cc,v 1.1 1998/01/12 13:07:52 nmcl Exp $
 */

#include <os/thread.h>
#include <os/errno.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class Solaris_SpecificDataImple : public ThreadDataImple
{
public:
    Solaris_SpecificDataImple ();
    ~Solaris_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
    
private:
    thread_key_t  _key;
};


Solaris_SpecificDataImple::Solaris_SpecificDataImple ()
						     : _key(0)
{
    if (thr_keycreate(&_key, 0) == 0)
	valid(TRUE);
}

Solaris_SpecificDataImple::~Solaris_SpecificDataImple ()
{
    /*
     * It is up to the user to guarantee that all thread data
     * is NULL at this point, or the results are not specified.
     */
    
    //    (void) thr_keydelete(_key);

    removeSpecific();
}

Boolean Solaris_SpecificDataImple::setSpecific (void* threadData)
{
    if (valid())
    {
	int error = thr_setspecific(_key, (void*) threadData);

	if (error == ENOMEM)
	    return addToOverflow(threadData);
	else
	    return (Boolean) (error == 0);
    }
    
    return FALSE;
}

Boolean Solaris_SpecificDataImple::getSpecific (void*& threadData)
{
    if (valid())
    {
	int error = thr_getspecific(_key, &threadData);

	if ((!threadData) || (error != 0))
	    return getFromOverflow(threadData);
	else
	    return TRUE;
    }

    return FALSE;
}

Boolean Solaris_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	if (removeFromOverflow())
	    return TRUE;
	
	if (thr_setspecific(_key, (void*) 0) == 0)
	    return TRUE;
    }
    
    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    return new Solaris_SpecificDataImple;
}
