/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisSpecific.cc,v 1.2 1998/02/06 12:12:35 nmcl Exp $
 */

/*
 * Implementation of thread specific state for Solaris threads
 */

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef SOLARISSPECIFIC_H_
#  include "SolarisSpecific.h"
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

ThreadSpecific *ThreadSpecific::create ( )
{
    SolarisSpecific *temp = new SolarisSpecific();

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

SolarisSpecific::SolarisSpecific ()
				 : key_(0)
{
    if (thr_keycreate(&key_, 0) == 0)
	valid(TRUE);
}

SolarisSpecific::~SolarisSpecific ()
{
    destroySpecific();
}

Boolean SolarisSpecific::getSpecific ( void *& anyData )
{
    if (valid())
    {
	if (thr_getspecific(key_, &anyData) == 0)
	    return TRUE;
	else
	    return getFromOverflow(anyData);
    }
    
    return FALSE;
}

Boolean SolarisSpecific::setSpecific ( void *anyData )
{
    if (valid())
    {
	int error = 0;
	
	if (((error = thr_setspecific(key_,anyData)) != 0) &&
	    ((error != ENOMEM) || !(addToOverflow(anyData))))
	{
	    return FALSE;
	}

	return TRUE;
    }
    
    return FALSE;
}

Boolean SolarisSpecific::destroySpecific ()
{
    if (valid())
    {
	thr_setspecific(key_, 0);
	removeFromOverflow();
	
	return TRUE;
    }

    return FALSE;
}

