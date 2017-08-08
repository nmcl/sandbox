/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.cc,v 1.1 1998/01/12 13:06:46 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

ClassName* Semaphore::_className = (ClassName*) 0;

ostream& Semaphore::print (ostream& strm) const
{
    strm << "[ " << uid() << " " << className() << " ]";
    return strm;
}

const ClassName& Semaphore::name ()
{
    if (_className == (ClassName*) 0)
	_className = new ClassName("Semaphore");

    return *_className;
}

const ClassName& Semaphore::className () const
{
    return Semaphore::name();
}

void* Semaphore::castup (const ClassName& _type) const
{
    if (_type == Semaphore::name())
	return (void*) this;
    else
	return (void*) 0;
}

Semaphore* Semaphore::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (Semaphore*) 0;
    else
	return (Semaphore*) toCast->castup(Semaphore::name());
}

#ifdef NO_INLINES
#  define SEMAPHORE_CC_
#  include <Common/Thread/Semaphore.n>
#  undef SEMAPHORE_CC_
#endif
