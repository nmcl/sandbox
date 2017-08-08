/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_Condition.cc,v 1.2 1998/07/21 12:41:00 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_OMNITHREAD_H_
#  include <os/omnithread.h>
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

    omni_mutex* mp;
    omni_condition* condition;
};

ConditionData::ConditionData ()
			     : mp(0),
			       condition(0)
{
}

ConditionData::~ConditionData ()
{
    if (mp)
	delete mp;
    
    if (condition)
	delete condition;
}


Condition::Condition (ConditionTest* test, Boolean canDelete)
		     : _conditionData(new ConditionData()),
		       _condition(test),
		       _canDelete(canDelete)
{
    _conditionData->mp = new omni_mutex();
    _conditionData->condition = new omni_condition(_conditionData->mp);
}

Condition::~Condition ()
{
    if (_conditionData)
	delete _conditionData;

    if (_canDelete && _condition)
	delete _condition;
}

Condition::ConditionStatus Condition::wait ()
{
    if (!_condition)
	return CN_ERROR;

    Condition::ConditionStatus status = CN_ERROR;

    try
    {
	_conditionData->mp->lock();
    }
    catch (...)
    {
	return CN_ERROR;
    }

    while (!_condition->test())
    {
	try
	{
	    _conditionData->condition->wait();
	}
	catch (...)
	{
	    break;
	}
    }

    if (_condition->test())
	status = CN_DONE;

    try
    {
	_conditionData->mp->unlock();
    }
    catch (...)
    {
	status = CN_ERROR;
    }

    return status;
}
    
Condition::ConditionStatus Condition::wait (unsigned long waittime)
{
    if (!_condition)
	return CN_ERROR;
    
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

    try
    {
	_conditionData->mp->lock();
    }
    catch (...)
    {
	return CN_ERROR;
    }
	    
    /*
     * Just because a condition is signalled does not mean that
     * the 'condition state' has been set correctly. We may have
     * to loop.
     */

    while (!_condition->test())
    {
	timeout = (_conditionData->condition->timedwait(to.tv_sec, to.tv_nsec) == 0);

	if (timeout)
	    break;
    }

    Condition::ConditionStatus status = CN_ERROR;
    
    if (timeout)
	status = CN_TIMEOUT;
    else
    {
	if (_condition->test())
	    status = CN_DONE;
    }

    try
    {
	_conditionData->mp->unlock();
    }
    catch (...)
    {
	status = CN_ERROR;
    }

    return status;
}

Condition::ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;

    try
    {
	_conditionData->mp->lock();
	_conditionData->condition->signal();
	_conditionData->mp->unlock();

	return CN_DONE;
    }
    catch (...)
    {
    }

    return CN_ERROR;
}

Condition::ConditionStatus Condition::broadcast ()
{
    if (!_condition)
	return CN_ERROR;

    try
    {
	_conditionData->mp->lock();
	_conditionData->condition->broadcast();
	_conditionData->mp->unlock();
	
	return CN_DONE;
    }
    catch (...)
    {
    }

    return CN_ERROR;
}

Condition* Condition::create (ConditionTest* cond, Boolean canDelete)
{
    Condition::initialize();

    return new Condition(cond, canDelete);
}
