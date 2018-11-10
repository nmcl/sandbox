/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.h,v 1.1 1995/07/26 14:50:52 ngdp Exp $
 */

#ifndef INITTHREADS_H_
#define INITTHREADS_H_

#ifdef STUB
#  pragma @NoRename
#endif

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
