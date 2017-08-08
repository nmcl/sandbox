/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_Condition.cc,v 1.1 1998/06/01 13:54:44 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_CTHREAD_H_
#  include <os/cthread.h>
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

    condition_t condition;
    mutex_t mp;    
};

ConditionData::ConditionData ()
{
    mutex_alloc(&mp, N_CURRENT);
    condition_alloc(&condition, N_CURRENT);
    
    mutex_init(mp);
    condition_init(condition);
}

ConditionData::~ConditionData ()
{
    mutex_free(mp);
    condition_free(condition);
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

    mutex_lock(_conditionData->mp);
	    
    while (!_condition->test())
    {
	condition_wait(_conditionData->condition, _conditionData->mp);
    }

    if (_condition->test())
	status = CN_DONE;

    mutex_unlock(_conditionData->mp);

    return status;
}
    
ConditionStatus Condition::wait (unsigned long waittime)
{
    if (!_condition)
	return CN_ERROR;

    ConditionStatus status = CN_ERROR;
    timeval stime;
	    
#ifdef HAS_BSD_GETTIMEOFDAY
    ::gettimeofday(&stime, NULL);
#else
    ::gettimeofday(&stime);
#endif	

    mutex_lock(_conditionData->mp);    
	    
    /*
     * Just because a condition is signalled does not mean that
     * the 'condition state' has been set correctly. We may have
     * to loop.
     */

    while (!_condition->test())
    {
	condition_wait(_conditionData->condition, _conditionData->mp);	
    }

    timeval ftime;
    
#ifdef HAS_BSD_GETTIMEOFDAY
    ::gettimeofday(&ftime, NULL);
#else
    ::gettimeofday(&ftime);
#endif	

    if ((ftime.tv_sec*1000000+ftime.tv_usec) -
	(stime.tv_sec*1000000+stime.tv_usec) > waittime*1000)
    {
	status = CN_TIMEOUT;
    }
    else
    {
	if (condition->test())
	    status = CN_DONE;
    }

    mutex_unlock(_conditionData->mp);

    return status;
}

ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;

    if ((mutex_lock(_conditionData->mp) == 0) && 
	(condition_signal(_conditionData->condition) == 0) &&
	(mutex_unlock(_conditionData->mp) == 0))
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

    if ((mutex_lock(_conditionData->mp) == 0) &&
	(condition_broadcast(_conditionData->condition) == 0) &&
	(mutex_unlock(_conditionData->mp) == 0))
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
