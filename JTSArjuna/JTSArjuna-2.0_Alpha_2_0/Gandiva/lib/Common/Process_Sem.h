/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Process_Sem.h,v 1.6 1998/02/28 13:05:34 nmcl Exp $
 */

#ifndef COMMON_PROCESS_SEM_H_
#define COMMON_PROCESS_SEM_H_

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef OS_SYS_IPC_H_
#  include <os/sys/ipc.h>
#endif

#ifndef OS_SYS_SEM_H_
#  include <os/sys/sem.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class ProcessSemaphore : public Semaphore
{
    friend Semaphore;

public:
    virtual SemaphoreStatus wait    ();
    virtual SemaphoreStatus signal  ();
    virtual SemaphoreStatus trywait ();

protected:
    ProcessSemaphore (const key_t key, unsigned int number);
    virtual ~ProcessSemaphore ();

private:
    static const int MaxRetry;

    Boolean create (unsigned int);

    key_t            semKey;
    int              semId;
    int              useCount;
    Mutex            *mutex;  // to protect from concurrent threads
    
    ProcessSemaphore *next;
    ProcessSemaphore *prev;

    static ProcessSemaphore* headOfList;
    static ClassName* _className;

public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*             castup (const ClassName&) const;
    static  ProcessSemaphore* castup (Resource*);    
};


class ProcessMutex : public Mutex, public ProcessSemaphore
{
    friend Mutex;

public:
    virtual MutexStatus wait    ();
    virtual MutexStatus signal  ();
    virtual MutexStatus trywait ();

protected:
    ProcessMutex (const key_t key);
    virtual ~ProcessMutex ();

private:
    MutexStatus convert (Semaphore::SemaphoreStatus);

    static ClassName* _className;

public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*         castup (const ClassName&) const;
    static  ProcessMutex* castup (Resource*);    
};

#endif
