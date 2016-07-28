/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.cc,v 1.2 1995/10/02 15:14:58 ngdp Exp $
 */

/*
 *
 * Basic Semaphore Class Implementation.
 * Provides interface to System V semaphore operations with the more
 * usual wait() and signal() notation. Currently only implements mutual
 * exclusion semaphores.
 *
 * Using System V IPC semaphores can be destroyed even if other processes
 * are sharing them! Thus wait and signal check to see if the semaphore 
 * exists and if not attempt to recreate it (they assume it NEEDS to exist).
 * The number of retries is determined by the value of RETRY_MAX.
 * Note that since IPC semaphores are scarce resources we cannot just fail
 * to destroy them.
 */

#ifndef ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef SYS_IPC_H_
#  include <os/sys/ipc.h>
#endif

#ifndef SYS_SEM_H_
#  include <os/sys/sem.h>
#endif

#ifndef FCNTL_H_
#  include <os/fcntl.h>
#endif

#ifndef STRSTREAM_H_
#  include <os/strstream.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
#endif

static const char RCSid[] = "$Id: Semaphore.cc,v 1.2 1995/10/02 15:14:58 ngdp Exp $";

static const int RETRY_MAX = 100;	/* Number of times to retry create */

/*
 * Public constructors and destructor. Create mutex semaphore with
 * given key.
 */

Semaphore::Semaphore ( const key_t semK, unsigned int init )
                     : initVal(init),
		       semKey(semK),
		       semId(-1)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::Semaphore(" << semK << ")" << endl;
    debug_stream.unlock();
    
#endif

    semCreate();
}

/*
 * Destroy semaphore. This uses IPC_RMID to actually remove the
 * semaphore - they do not have usage counts that can be checked
 * unlike shared memory segments. This means that if multiple processes
 * share a semaphore (as they do) it may get removed. If needed the 
 * semaphore will be recreated by the wait and signal operations.
 *
 * To ensure exclusivity and avoid races in the recreation the destruction
 * is controlled by acquiring the same semaphore! This ensures that the only
 * process in the critical section is the one doing the destruction - all
 * others will be held on a wait and will then race to recreate it.
 * Assumes that a IPC_RMID implies an implicit signal
 */

Semaphore::~Semaphore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::~Semaphore()" << endl;
    debug_stream.unlock();
    
#endif

    if (wait() == SEM_LOCKED)
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
    }
    
    semId = -1;
}

/*
 * Public non-virtual functions
 */

/* 
 * Note we do not use SEM_UNDO. Since we assume several processes are
 * using mutex semaphores for a reason we leave the semaphore locked
 * on failure to allow some form of external cleanup.
 */

Semaphore::Status Semaphore::wait ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::wait" << endl;
    debug_stream.unlock();
    
#endif

    struct sembuf sembuf;
    int res, retry;

    retry = RETRY_MAX;

    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) != 0)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((errno == EIDRM) || (errno == EINVAL)) &&
		semCreate())
		continue;
	    else
	    {
		return SEM_ERROR;
	    }
	} else
	    return SEM_LOCKED;
    } while (--retry >= 0);

    return SEM_ERROR;
}

/* 
 * Note we do not use SEM_UNDO. Since we assume several processes are
 * using mutex semaphores for a reason we leave the semaphore locked
 * on failure to allow some form of external cleanup.
 * Returns TRUE if wait ok, FALSE otherwise
 */

Semaphore::Status Semaphore::trywait ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::trywait" << endl;
    debug_stream.unlock();
    
#endif

    struct sembuf sembuf;
    int res, retry;

    retry = RETRY_MAX;

    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = IPC_NOWAIT;

	if ((res = ::semop(semId, &sembuf, 1)) != 0)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (errno == EAGAIN)
		return SEM_BLOCKED;
	    
	    if (((errno == EIDRM) || (errno == EINVAL)) &&
		semCreate())
		continue;
	    else
	    {
		return SEM_ERROR;
	    }
	} else
	    return SEM_LOCKED;
    } while (--retry >= 0);

    return SEM_ERROR;
}

Semaphore::Status Semaphore::signal ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::signal" << endl;
    debug_stream.unlock();
    
#endif

    struct sembuf sembuf;
    int res, retry;

    retry = RETRY_MAX;
    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = 1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) != 0)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((errno == EIDRM) || (errno == EINVAL)) &&
		semCreate())
		continue;
	    else
	    {
		return SEM_ERROR;
	    }
	} else
	    return SEM_UNLOCKED;
    } while (--retry >= 0);

    return SEM_ERROR;
}

/*
 * Private non-virtual functions
 */

Boolean Semaphore::semCreate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PRIVATE;
    debug_stream << "Semaphore::semCreate()" << endl;
    debug_stream.unlock();
    
#endif

    int res, retry;

    retry = 5;
    do
    {
	if ((semId = ::semget(semKey, 1, 0666 | IPC_CREAT | IPC_EXCL)) >= 0)
	{
	    /* Created by me - set initial value */
#if defined(hpux) || defined(_AIX)
	    if ((res = ::semctl(semId, 0, SETVAL, initVal)) == 0)
#else
	    union semun arg;
	    arg.val = initVal;

	    if ((res = ::semctl(semId, 0, SETVAL, arg)) == 0)
#endif
	    {
		return TRUE;
	    }
	}
	else
	{
	    if ((errno == EEXIST) &&
		((semId = ::semget(semKey, 1, 0666)) >= 0))
		    return TRUE;
	}
    } while (--retry >= 0);

    return FALSE;
	
}
