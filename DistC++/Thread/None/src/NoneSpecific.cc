/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneSpecific.cc,v 1.1 1997/09/25 15:32:43 nmcl Exp $
 */

/*
 * Implementation of thread specific state for None threads
 */

#ifndef NONESPECIFIC_H_
#  include "NoneSpecific.h"
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

ThreadSpecific *ThreadSpecific::create ( )
{
    NoneSpecific *temp = new NoneSpecific();

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

NoneSpecific::NoneSpecific ()
{
}

NoneSpecific::~NoneSpecific ()
{
    destroySpecific();
}

Boolean NoneSpecific::getSpecific ( void *& anyData )
{
    if (valid())
    {
	return getFromOverflow(anyData);
    }
    
    return FALSE;
}

Boolean NoneSpecific::setSpecific ( void *anyData )
{
    if (valid())
    {
	return addToOverflow(anyData);
    }
    
    return FALSE;
}

Boolean NoneSpecific::destroySpecific ()
{
    if (valid())
    {
	removeFromOverflow();
	return TRUE;
    }

    return FALSE;
}

