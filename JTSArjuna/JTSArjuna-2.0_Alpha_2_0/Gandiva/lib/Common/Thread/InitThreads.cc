/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.cc,v 1.1 1998/01/12 13:06:44 nmcl Exp $
 */

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

int InitThreads::initCount = 0;

InitThreads::InitThreads ()
{
    if (initCount++ == 0)
    {
	Thread::initialize();
    }
}

InitThreads::~InitThreads ()
{
    if (--initCount == 0)
    {
	Thread::shutdown();
    }
}

