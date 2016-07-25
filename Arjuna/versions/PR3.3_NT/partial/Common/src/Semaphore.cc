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
#  include <System/errno.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef WIN32

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef SYS_SEM_H_
#  include <System/sys/sem.h>
#endif

#else

#ifndef WINDOWS_H_
#  include <System/windows.h>
#endif

#include <System/stdio.h>

#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
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
#ifndef WIN32
		       semId(-1)
#else
			   semId(0),
			   fileName(0)
#endif
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
 * Assumes that a IPC_RMID implies an implicit signal.
 *
 * We don't need to do most of this for Windows NT.
 */

Semaphore::~Semaphore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::~Semaphore()" << endl;
    debug_stream.unlock();
    
#endif

#ifdef WIN32

	if (haveSemaphore)
		signal();

	CloseHandle(semId);

	if (fileName)
		::delete [] fileName;

#else

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

#endif
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

#ifndef WIN32

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

#else

	if (haveSemaphore)
		return SEM_LOCKED;

	DWORD res;
	int retry = RETRY_MAX;

	do
	{
		res = WaitForSingleObject(semId, INFINITE);

		if (res == WAIT_OBJECT_0)
		{
			haveSemaphore = TRUE;
			return SEM_LOCKED;
		}

	} while (--retry >= 0);

	return SEM_ERROR;

#endif
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

#ifndef WIN32

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

#else

	if (haveSemaphore)
		return SEM_LOCKED;

	DWORD res;
	int retry = RETRY_MAX;

	do
	{
		res = WaitForSingleObject(semId, 0);

		if (res == WAIT_OBJECT_0)
		{
			haveSemaphore = TRUE;
			return SEM_LOCKED;
		}

	} while (--retry >= 0);

	return SEM_ERROR;

#endif
}

Semaphore::Status Semaphore::signal ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::signal" << endl;
    debug_stream.unlock();
    
#endif

#ifndef WIN32

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

#else
	if (!haveSemaphore)
		return SEM_UNLOCKED;

	if (ReleaseSemaphore(semId, 1, NULL))
	{
		haveSemaphore = FALSE;
		return SEM_UNLOCKED;
	}

	return SEM_ERROR;

#endif
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

#ifndef WIN32

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

#else

	/*
	 * Get from getenv.
	 */

	fileName = ::new char[MAX_PATH];
	::memset(fileName, '\0', MAX_PATH);

	sprintf(fileName, "c:/temp/Semaphore_%d", semKey);

	int retry = 5;

	do
	{
		semId = CreateSemaphore(NULL, initVal, initVal, fileName);

		if (semId == NULL)
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				semId = OpenSemaphore(SEMAPHORE_ALL_ACCESS | SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
									FALSE, fileName);

			if (semId != NULL)
				return TRUE;
		}
		else
			return TRUE;

	} while (--retry >= 0);

	return FALSE;

#endif
}
