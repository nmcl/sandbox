/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadControl.cc,v 1.3 1998/01/12 13:08:21 nmcl Exp $
 */

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif

ThreadControl::ThreadControl (Thread* t)
			     : _toControl(t)
{
    Resource::ref(_toControl);
}

ThreadControl::~ThreadControl ()
{
    Resource::unref(_toControl);
}

const ClassName& ThreadControl::name ()
{
    return Gandiva::Control::ThreadControl::name();
}

const ClassName& ThreadControl::className () const
{
    return Gandiva::Control::ThreadControl::name();    
}

void* ThreadControl::castup (const ClassName& _type) const
{
    if (_type == ThreadControl::name())
	return (void*) this;
    else
	return (void*) 0;
}

ThreadControl* ThreadControl::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (ThreadControl*) 0;
    else
	return (ThreadControl*) toCast->castup(ThreadControl::name());
}
