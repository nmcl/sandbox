/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ProcessMutex.h,v 1.1 1997/09/25 15:25:50 nmcl Exp $
 */

#ifndef PROCESSMUTEX_H_
#define PROCESSMUTEX_H_

/*
 * Basic Process level Mutex implementation.
 * Implements recursive process mutexes using SystemV semphores.
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
#endif

class ProcessMutex
{
    friend class MutexList;
    
public:
    static ProcessMutex *create (key_t);
    static void destroy (ProcessMutex*&);
    
    /* classic semaphore operations */

    Semaphore::Status signal ();
    Semaphore::Status trywait ();
    Semaphore::Status wait ();
private:
    ProcessMutex (key_t);
    ~ProcessMutex ();

    key_t mutexKey;
    Semaphore *theSem;
    int waitCount;
};

#endif
