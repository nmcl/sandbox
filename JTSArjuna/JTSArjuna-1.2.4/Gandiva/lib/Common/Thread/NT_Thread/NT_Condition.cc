/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_Condition.cc,v 1.1 1998/06/01 13:57:27 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_WINDOWS_H_
#  include <os/windows.h>
#endif

#ifndef OS_PROCESS_H_
#  include <os/process.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
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

    HANDLE condition;
    HANDLE mp;
};

ConditionData::ConditionData ()
{
    condition = CreateSemaphore(NULL, 0, 0, NULL);
    WaitForSingleObject(condition, INFINITE);

    mp = CreateMutex(NULL, 0, NULL);
}

ConditionData::~ConditionData ()
{
    CloseHandle(condition);
    CloseHandle(mp);
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
    Boolean error = FALSE;

    WaitForSingleObject(_conditionData->mp);

    while (!_condition->test() && !error)
    {
	ReleaseMutex(_conditionData->mp);
	
	if (WaitForSingleObject(_conditionData->condition, INFINITE) != WAIT_OBJECT_0)
	{
	    error = TRUE;
	}
	else
	{
	    if (WaitForSingleObject(_conditionData->mp) == WAIT_OBJECT_0)
		error = TRUE;
	    
	    ReleaseSemaphore(_conditionData->condition, 1, NULL);
	}
    }

    if (!error && _condition->test())
	status = CN_DONE;

    ReleaseMutex(_conditionData->mp);

    return status;
}
    
ConditionStatus Condition::wait (unsigned long waittime)
{
    if (!_condition)
	return CN_ERROR;

    ConditionStatus status = CN_ERROR;
    Boolean error = FALSE;
    
    WaitForSingleObject(_conditionData->mp);
	    
    /*
     * Just because a condition is signalled does not mean that
     * the 'condition state' has been set correctly. We may have
     * to loop.
     */

    while (!_condition->test() && !error)
    {
	ReleaseMutex(_conditionData->mp);
	
	if (WaitForSingleObject(_conditionData->condition, waittime) == WAIT_TIMEOUT)
	{
	    timeout = TRUE;
	    break;
	}
	
	if (WaitForSingleObject(_conditionData->mp) == WAIT_OBJECT_0)
	    error = TRUE;
	    
	ReleaseSemaphore(_conditionData->condition, 1, NULL);
    }

    if (timeout)
	status = CN_TIMEOUT;
    else
    {
	if (!error && condition->test())
	    status = CN_DONE;
    }

    if (!timeout)
	ReleaseMutex(_conditionData->mp);

    return status;
}

ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;

    if ((WaitForSingleObject(_conditionData->mp) == WAIT_OBJECT_0) &&
	ReleaseSemaphore(_conditionData->condition, 1, NULL) &&
	ReleaseMutex(_conditionData->mp))
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

    if (WaitForSingleObject(_conditionData->mp) == WAIT_OBJECT_0)
    {
	for (;;)
	{
	    /*
	     * It should be ok to call release on this semaphore to wake
	     * up any waiters since we have the mutex, which will prevent
	     * any new threads from trying to call wait while we're doing
	     * this, and thus prevent a potentially infinite release.
	     */
	    
	    if (ReleaseSemaphore(_conditionData->condition, 1, NULL) < 0)
		break;
	}

	/*
	 * Now release the mutex, and wake up any of the broadcasted
	 * waiters.
	 */
	
	ReleaseMutex(_conditionData->mp);

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
