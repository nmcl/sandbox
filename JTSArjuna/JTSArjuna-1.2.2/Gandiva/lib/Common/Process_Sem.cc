/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Process_Sem.cc,v 1.7 1998/02/28 13:05:33 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_FCNTL_H_
#  include <os/fcntl.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_PROCESS_SEM_H_
#  include <Common/Process_Sem.h>
#endif

static Mutex* _listMutex = (Mutex*) 0;

const int         ProcessSemaphore::MaxRetry = 5;
ProcessSemaphore* ProcessSemaphore::headOfList = (ProcessSemaphore*) 0;
ClassName*        ProcessSemaphore::_className = (ClassName*) 0;
ClassName*        ProcessMutex::_className = (ClassName*) 0;


ProcessSemaphore::ProcessSemaphore (const key_t key, unsigned int number)
                                   : semKey(key),
                                     semId(-1),
				     useCount(0),
				     mutex(Mutex::create()),
				     next(NULL),
				     prev(NULL)
{
    create(number);
}

ProcessSemaphore::~ProcessSemaphore ()
{
    if (wait() == DONE)
    {
#if defined(hpux) || defined(_AIX)
	if (semId != -1)
	    ::semctl(semId, 1, IPC_RMID, 0);
#else
	union semun arg;
	arg.val = 0;

	if (semId != -1)
#ifndef __linux__
	    ::semctl(semId, 1, IPC_RMID, arg);
#else
            // why do I have to do this?!
	    ::semctl(semId, 0, IPC_RMID, arg);
#endif

#endif
	    if (prev)
		prev->next = next;
	    else
		ProcessSemaphore::headOfList = next;

	    if (next)
		next->prev = prev;
    }
    else
	cerr << "ProcessSemaphore::~ProcessSemaphore - could not get lock!" << endl;

    semId = -1;

    if (mutex)
	Resource::unref(mutex);
}

Boolean ProcessSemaphore::create (unsigned int number)
{
    int res, retry = MaxRetry;

    do
    {
	if ((semId = ::semget(semKey, 1, 0666 | IPC_CREAT | IPC_EXCL)) != -1)
	{
#if defined(hpux) || defined(_AIX)
	    if ((res = ::semctl(semId, 0, SETVAL, number)) != -1)
#else
	    union semun arg;
	    arg.val = number;

	    if ((res = ::semctl(semId, 0, SETVAL, arg)) != -1)
#endif
		return TRUE;
	}
	else
	{
	    if ((errno == EEXIST) && ((semId = ::semget(semKey, 1, 0666)) != -1))
		    return TRUE;
	}
    } while (--retry >= 0);

    return FALSE;
}
    
Semaphore::SemaphoreStatus ProcessSemaphore::wait ()
{
    if (!mutex || (mutex->lock() != Mutex::DONE))
	return Semaphore::ERROR;
    
    if (useCount > 0)
    {
	useCount++;
	return DONE;
    }
    
    struct sembuf sembuf;
    int res, retry = MaxRetry;

    do
    {
        sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) == -1)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((errno == EIDRM) || (errno == EINVAL)) && create(numberAllowed))
	        continue;
	    else
	    {
		useCount = 1;
		mutex->unlock();
		
	        return Semaphore::ERROR;
	    }
	}
	else
	{
	    mutex->unlock();
	    return Semaphore::DONE;
	}

    } while (--retry >= 0);

    mutex->unlock();
    
    return Semaphore::ERROR;
}

/*
 * Should have mutex locked by now.
 */

Semaphore::SemaphoreStatus ProcessSemaphore::signal ()
{
    if (!mutex)
	return Semaphore::ERROR;
    
    if (useCount == 0)
	return ERROR;

    if (useCount-- > 0)
    {
	mutex->unlock();
	return DONE;
    }

    struct sembuf sembuf;
    int res, retry = MaxRetry;

    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = 1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) == -1)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((errno == EIDRM) || (errno == EINVAL)) && create(numberAllowed))
		continue;
	    else
	    {
		mutex->unlock();
		return Semaphore::ERROR;
	    }
	}
	else
	{
	    mutex->unlock();
	    return Semaphore::DONE;
	}

    } while (--retry >= 0);

    mutex->unlock();
    
    return Semaphore::ERROR;
}

Semaphore::SemaphoreStatus ProcessSemaphore::trywait ()
{
    if (!mutex || (mutex->lock() != Mutex::DONE))
	return Semaphore::ERROR;
    
    if (useCount > 0)
    {
	useCount++;
	return DONE;
    }
    
    struct sembuf sembuf;
    int res, retry = MaxRetry;

    do
    {
        sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = IPC_NOWAIT;

	if ((res = ::semop(semId, &sembuf, 1)) == -1)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((errno == EIDRM) || (errno == EINVAL)) && create(numberAllowed))
	        continue;
	    else
	    {
		mutex->unlock();
		
		if (errno == EAGAIN)
		    return WOULD_BLOCK;
		else
		    return Semaphore::ERROR;
	    }
	}
	else
	    return Semaphore::DONE;

    } while (--retry >= 0);

    mutex->unlock();
    
    return Semaphore::ERROR;
}

const ClassName& ProcessSemaphore::name ()
{
    if (_className == (ClassName*) 0)
	_className = new ClassName("ProcessSemaphore");

    return *_className;
}

const ClassName& ProcessSemaphore::className () const
{
    return ProcessSemaphore::name();
}

void* ProcessSemaphore::castup (const ClassName& _type) const
{
    if (_type == name())
	return (void*) this;
    else
	return Semaphore::castup(_type);
}

ProcessSemaphore* ProcessSemaphore::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (ProcessSemaphore*) 0;
    else
	return (ProcessSemaphore*) toCast->castup(name());
}


ProcessMutex::ProcessMutex (const key_t key)
			   : ProcessSemaphore(key, 1)
{
}

ProcessMutex::~ProcessMutex ()
{
}

Mutex::MutexStatus ProcessMutex::convert (Semaphore::SemaphoreStatus res)
{
    switch(res)
    {
    case Semaphore::DONE:
	return Mutex::DONE;
	break;
    case Semaphore::ERROR:
	return Mutex::ERROR;
	break;
    }

    return Mutex::WOULD_BLOCK;
}

Mutex::MutexStatus ProcessMutex::wait ()
{
    return convert(ProcessSemaphore::wait());
}

Mutex::MutexStatus ProcessMutex::signal ()
{
    return convert(ProcessSemaphore::signal());
}

Mutex::MutexStatus ProcessMutex::trywait ()
{
    return convert(ProcessSemaphore::trywait());
}

const ClassName& ProcessMutex::name ()
{
    if (_className == (ClassName*) 0)
	_className = new ClassName("ProcessMutex");

    return *_className;
}

const ClassName& ProcessMutex::className () const
{
    return ProcessMutex::name();
}

void* ProcessMutex::castup (const ClassName& _type) const
{
    if (_type == name())
	return (void*) this;
    else
	return Mutex::castup(_type);
}

ProcessMutex* ProcessMutex::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (ProcessMutex*) 0;
    else
	return (ProcessMutex*) toCast->castup(name());
}


void Semaphore::initialize ()
{
}

Semaphore* Semaphore::create (unsigned int number, SemaphoreSetup* setup)
{
    if (!setup)
	return (Semaphore*) 0;

    if (!listMutex)
	listMutex = Mutex::create();

    if (listMutex->lock() != Mutex::DONE)
	return (Semaphore*) 0;
    
    unsigned int semKey = setup->key;
    ProcessSemaphore* ptr = ProcessSemaphore::headOfList;

    while ((ptr) && (ptr->semKey != semKey))
	ptr = ptr->next;

    /*
     * The caller will reference count this and it will only be destroyed when
     * the count reaches zero.
     */
    
    if (!ptr)
    {
	ptr = new ProcessSemaphore(semKey, number);
	ptr->next = ProcessSemaphore::headOfList;
	ProcessSemaphore::headOfList->prev = ptr;
	ProcessSemaphore::headOfList = ptr;
    }

    listMutex->unlock();
    
    return ptr;
}


void Mutex::initialize ()
{
}

Mutex* Mutex::create (MutexSetup* setup)
{
    if (!setup)
	return (Mutex*) 0;

    if (!listMutex)
	listMutex = Mutex::create();

    if (listMutex->lock() != Mutex::DONE)
	return (Mutex*) 0;
    
    unsigned int semKey = setup->key;
    ProcessMutex* ptr = ProcessSemaphore::headOfList;

    while ((ptr) && (ptr->semKey != semKey))
	ptr = ptr->next;

    if (!ptr)
    {
	ptr = new ProcessMutex(semKey);
	ptr->next = ProcessSemaphore::headOfList;
	ProcessSemaphore::headOfList->prev = ptr;
	ProcessSemaphore::headOfList = ptr;
    }

    listMutex->unlock();
    
    return ptr;
}
