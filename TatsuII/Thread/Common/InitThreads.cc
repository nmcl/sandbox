/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.cc,v 1.1 1997/06/09 19:53:05 nmcl Exp $
 */

#ifndef INITTHREADS_H_
#  include <Thread/InitThreads.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADLIST_H_
#  include <Thread/ThreadList.h>
#endif

#ifndef THREADID_H_
#  include <Thread/ThreadID.h>
#endif

int InitThreads::initCount = 0;

ThreadList *allThreads = 0;

InitThreads::InitThreads ()
{
    if (initCount++ == 0)
    {
	allThreads = new ThreadList();
	Thread::init();
    }
    
}

InitThreads::~InitThreads ()
{
    if (--initCount == 0)
    {
	Thread::shutdown();

	if (allThreads)
	    delete allThreads;
    }
}

