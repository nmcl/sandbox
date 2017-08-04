/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.cc,v 1.2 1995/07/26 15:22:53 ngdp Exp $
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

