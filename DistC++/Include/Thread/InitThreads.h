/*
 * Copyright (C) 1995-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InitThreads.h,v 1.1 1997/09/25 15:28:11 nmcl Exp $
 */

#ifndef INITTHREADS_H_
#define INITTHREADS_H_

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
