/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.cc,v 1.1 1997/09/25 15:32:25 nmcl Exp $
 */

#ifndef INITTHREADS_H_
#  include <Thread/InitThreads.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef TSHASHLIST_H_
#  include <Template/TSHashList.h>
#endif

int InitThreads::initCount = 0;

ThreadSafeHashList<Thread> *allThreads = 0;

InitThreads::InitThreads ()
{
    if (initCount++ == 0)
    {
	allThreads = new ThreadSafeHashList<Thread>(37);
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

