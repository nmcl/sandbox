/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_Condition.cc,v 1.1 1998/06/01 14:31:22 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_THREAD_H_
#  include <os/thread.h>
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

    mutex_t mp;    
    cond_t condition;
};

ConditionData::ConditionData ()
{
    mutex_init(&mp, USYNC_THREAD, NULL);    
    cond_init(&condition, USYNC_THREAD, 0);    
}

ConditionData::~ConditionData ()
{
    mutex_destroy(&mp);
    cond_destroy(&condition);
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

    mutex_lock(&_conditionData->mp);

    while (!_condition->test())
    {
	if (cond_wait(&_conditionData->condition, &_conditionData->mp) != 0)
	    break;
    }

    if (_condition->test())
	status = CN_DONE;

    mutex_unlock(&_conditionData->mp);

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

    mutex_lock(&_conditionData->mp);
	    
    /*
     * Just because a condition is signalled does not mean that
     * the 'condition state' has been set correctly. We may have
     * to loop.
     */

    while (!_condition->test())
    {
	timeout = (cond_timedwait(&_conditionData->condition,
				  &_conditionData->mp, &to) == ETIMEDOUT);

	if (timeout)
	    break;
    }

    if (timeout)
	status = CN_TIMEOUT;
    else
    {
	if (_condition->test())
	    status = CN_DONE;
    }

    mutex_unlock(&_conditionData->mp);

    return status;
}

ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;

    if ((mutex_lock(&_conditionData->mp) == 0) &&
	(cond_signal(&_conditionData->condition) == 0) &&
	(mutex_unlock(&_conditionData->mp) == 0))
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

    if ((mutex_lock(&_conditionData->mp) == 0) &&
	(cond_broadcast(&_conditionData->condition) == 0) &&
	(mutex_unlock(&_conditionData->mp) == 0))
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
