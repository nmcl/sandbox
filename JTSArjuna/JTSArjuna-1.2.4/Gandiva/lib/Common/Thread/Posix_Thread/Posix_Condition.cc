/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Posix_Condition.cc,v 1.2 1998/06/01 14:17:11 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_PTHREAD_PTHREAD_H_
#  include <os/pthread/pthread.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_SYS_TIME_H_
#  include <os/sys/time.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_CONDITION_H_
#  include <Common/Thread/Condition.h>
#endif

class ConditionData
{
public:
    ConditionData ();
    ~ConditionData ();

    pthread_cond_t condition;
    pthread_mutex_t mp;    
};

ConditionData::ConditionData ()
{
#if defined(PTHREAD_DRAFT_8) || defined(PTHREAD_SUN)
    pthread_cond_init(&condition, pthread_mutexcond_default);
#else
    pthread_condattr_t cattr;

    pthread_condattr_init(&cattr);
    pthread_cond_init(condition, &cattr);
#endif

    pthread_mutexattr_t mattr;
    
    pthread_mutexattr_init(&mattr);
    
#if defined(PTHREAD_DRAFT_6) || defined(PTHREAD_SUN)
    int pshared = PTHREAD_PROCESS_PRIVATE; // default behavior;

    pthread_mutexattr_setpshared(&mattr, pshared);
#else
#ifdef HAVE_RECURSIVE_MUTEX
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEXTYPE_RECURSIVE);
#endif    
#endif
    
    pthread_mutex_init(&mp, &mattr);    
}

ConditionData::~ConditionData ()
{
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mp);
}


Condition::Condition (ConditionTest* test, Boolean canDelete)
		     : _conditionData(new ConditionData()),
		       _condition(test),
		       _canDelete(canDelete)
{
}

Condition::~Condition ()
{
    if (_conditionData)
	delete _conditionData;

    if (_canDelete && _condition)
	delete _condition;
}

ConditionStatus Condition::wait ()
{
    if (!_condition)
	return CN_ERROR;

    ConditionStatus status = CN_ERROR;

    pthread_mutex_lock(&_conditionData->mp);
    
    while (!_condition->test())
    {
	if (pthread_cond_wait(&_conditionData->condition, &_conditionData->mp) != 0)
	    break;
    }

    if (_condition->test())
	status = CN_DONE;

    pthread_mutex_unlock(&_conditionData->mp);

    return status;
}
    
ConditionStatus Condition::wait (unsigned long waittime)
{
    if (!_condition)
	return CN_ERROR;

    ConditionStatus status = CN_ERROR;
    Boolean timeout = FALSE;
    int err = 0;
    timeval tv;
    timespec to;
	    
#ifdef HAS_BSD_GETTIMEOFDAY
    ::gettimeofday(&tv, NULL);
#else
    ::gettimeofday(&tv);
#endif	

    unsigned int seconds = waittime/1000000;
    unsigned int usecs = waittime%1000000;
	
    to.tv_sec = tv.tv_sec + seconds;
    to.tv_nsec = (tv.tv_usec + usecs) * 1000; /* convert to nanoseconds */

    while (to.tv_nsec >= 1000000000)
    {
	to.tv_sec++;
	to.tv_nsec -= 1000000000;
    }

    pthread_mutex_lock(&_conditionData->mp);
	    
    /*
     * Just because a condition is signalled does not mean that
     * the 'condition state' has been set correctly. We may have
     * to loop.
     */

    while (!_condition->test())
    {
	timeout = (pthread_cond_timedwait(&_conditionData->condition,
					  &_conditionData->mp, &to) == ETIMEDOUT);

	if (timeout)
	    break;
    }

    if (timeout)
	status = CN_TIMEOUT;
    else
    {
	if (condition->test())
	    status = CN_DONE;
    }

    pthread_mutex_unlock(&_conditionData->mp);

    return status;
}

ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;

    if ((pthread_mutex_lock(&_conditionData->mp) == 0) && 
	(pthread_cond_signal(&_conditionData->condition) == 0) &&
	(pthread_mutex_unlock(&_conditionData->mp) == 0))
    {
	return CN_DONE;
    }
    else
    {
	return CN_ERROR;
    }
}

ConditionStatus Condition::broadcast ()
{
    if (!_condition)
	return CN_ERROR;

    if ((pthread_mutex_lock(&_conditionData->mp) == 0) &&
	(pthread_cond_broadcast(&_conditionData->condition) == 0) &&
	(pthread_mutex_unlock(&_conditionData->mp) == 0))
    {
	return CN_DONE;
    }
    else
    {
	return CN_ERROR;
    }
}

Condition* Condition::create (ConditionTest* cond, Boolean canDelete)
{
    Condition::initialize();

    return new Condition(cond, canDelete);
}
