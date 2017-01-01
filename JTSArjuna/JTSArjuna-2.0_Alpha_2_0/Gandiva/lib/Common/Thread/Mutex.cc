/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Mutex.cc,v 1.1 1998/01/12 13:06:45 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

ClassName* Mutex::_className = (ClassName*) 0;

ostream& Mutex::print (ostream& strm) const
{
    strm << "[ " << uid() << " " << className() << " ]";
    return strm;
}

const ClassName& Mutex::name ()
{
    if (_className == (ClassName*) 0)
	_className = new ClassName("Mutex");

    return *_className;
}

const ClassName& Mutex::className () const
{
    return Mutex::name();
}

void* Mutex::castup (const ClassName& _type) const
{
    if (_type == Mutex::name())
	return (void*) this;
    else
	return (void*) 0;
}

Mutex* Mutex::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (Mutex*) 0;
    else
	return (Mutex*) toCast->castup(Mutex::name());
}

#ifdef NO_INLINES
#  define MUTEX_CC_
#  include <Common/Thread/Mutex.n>
#  undef MUTEX_CC_
#endif
