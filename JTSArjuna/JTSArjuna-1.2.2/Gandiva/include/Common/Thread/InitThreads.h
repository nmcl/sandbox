/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.h,v 1.1 1998/01/12 12:56:05 nmcl Exp $
 */

#ifndef COMMON_THREAD_INITTHREADS_H_
#define COMMON_THREAD_INITTHREADS_H_

class InitThreads
{
public:
    InitThreads ();
    ~InitThreads ();
    
private:
    static int initCount;
};

static InitThreads initThreadSystem;

#endif
