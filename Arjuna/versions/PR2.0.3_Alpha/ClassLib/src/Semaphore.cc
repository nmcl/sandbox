/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.cc,v 1.15 1993/07/06 13:20:02 nmcl Exp $
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

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef SYS_SEM_H_
#  include <System/sys/sem.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef SEMAPHORE_H_
#  include "Semaphore.h"
#endif

static const char RCSid[] = "$Id: Semaphore.cc,v 1.15 1993/07/06 13:20:02 nmcl Exp $";

static const int RETRY_MAX = 100;	/* Number of times to retry create */

/*
 * Public constructors and destructor. Create mutex semaphore with
 * given key.
 */

Semaphore::Semaphore ( const key_t semK )
                     : semKey(semK),
		       semId(-1)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::Semaphore(" << semK << ")\n" << flush;
#endif

    semCreate();
}

/*
 * Create mutex semaphore with private key
 */

Semaphore::Semaphore ()
                     : semKey(IPC_PRIVATE),
		       semId(-1)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::Semaphore()\n" << flush;
#endif

    semCreate();
}

/*
 * Destroy semaphore. This uses IPC_RMID to actually remove the
 * semaphore - they do not have usage counts that can be checked
 * unlike shared memory segments. This means that if multiple processes
 * share a semaphore (as they do) it may get removed. If needed the 
 * semaphore will be recreated by the wait and signal operations.
 */

Semaphore::~Semaphore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::~Semaphore()\n" << flush;
#endif

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

void Semaphore::wait ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::wait\n" << flush;
#endif

    struct sembuf sembuf;
    int res, retry;

    retry = RETRY_MAX;

    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) == -1)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((::errno == EIDRM) || (::errno == EINVAL)) &&
		semCreate())
		continue;
	    else
	    {
		error_stream << FATAL << "wait: cannot recreate semaphore with key "
			     << semKey << endl << flush;
		ArjunaFatal();
	    }
	} else
	    return;
    } while (--retry >= 0);

    error_stream << FATAL << "wait: cannot manipulate semaphore with key "
		 << semKey << endl << flush;
    ArjunaFatal(); 
}

void Semaphore::signal ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "Semaphore::signal\n" << flush;
#endif

    struct sembuf sembuf;
    int res, retry;

    retry = RETRY_MAX;
    do
    {
	sembuf.sem_num = 0;
	sembuf.sem_op = 1;
	sembuf.sem_flg = 0;

	if ((res = ::semop(semId, &sembuf, 1)) == -1)
	{
	    /* Failed. See if semaphore was destroyed */
	    if (((::errno == EIDRM) || (::errno == EINVAL)) &&
		semCreate())
		continue;
	    else
	    {
		error_stream << FATAL << "signal: cannot recreate semaphore with key "
			     << semKey << endl << flush;
		ArjunaFatal();
	    }
	} else
	    return;
    } while (--retry >= 0);

    error_stream << FATAL << "signal: cannot manipulate semaphore with key "
		 << semKey << endl << flush;
    
    ArjunaFatal(); 
}

/*
 * Private non-virtual functions
 */

Boolean Semaphore::semCreate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PRIVATE;
    debug_stream << "Semaphore::semCreate()\n" << flush;
#endif

    int res, retry;

    retry = 5;
    do
    {
	if ((semId = ::semget(semKey, 1, 0666 | IPC_CREAT | IPC_EXCL)) != -1)
	{
	    /* Created by me - set for mutual exclusion */
#if defined(hpux) || defined(_AIX)
	    if ((res = ::semctl(semId, 0, SETVAL, 1)) != -1)
#else
	    union semun arg;
	    arg.val = 1;

	    if ((res = ::semctl(semId, 0, SETVAL, arg)) != -1)
#endif
	    {
		return TRUE;
	    }
	    else
		error_stream << WARNING
			     << "Initialise of semaphore failed\n" << flush;
	}
	else
	{
	    if ((::errno == EEXIST) &&
		((semId = ::semget(semKey, 1, 0666)) != -1))
		    return TRUE;
	}
    } while (--retry >= 0);

    error_stream << FATAL << "Cannot recreate semaphore system - terminating!\n";
    ArjunaFatal();

    /* not really reached */

    return FALSE;
	
}
