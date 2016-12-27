/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_Condition.cc,v 1.2 1999/05/11 11:02:26 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_UNISTD_H_
#  include <os/unistd.h>
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

Condition::Condition (ConditionTest* cond, Boolean canDelete)
		     : _conditionData(0),
		       _condition(cond),
		       _canDelete(canDelete)
{
}

Condition::~Condition ()
{
    if (_canDelete && _condition)
	delete _condition;
}

ConditionStatus Condition::wait ()
{
    if (!_condition)
	return CN_ERROR;

    return ((_condition->test()) ? CN_DONE : CN_ERROR);
}
    
ConditionStatus Condition::wait (unsigned long waitTime)
{
    if (!_condition)
	return CN_ERROR;

    usleep((unsigned int) waitTime);
    
    return ((_condition->test()) ? CN_DONE : CN_TIMEOUT);
}

ConditionStatus Condition::signal ()
{
    if (!_condition)
	return CN_ERROR;
    else
	return CN_DONE;
}

ConditionStatus Condition::broadcast ()
{
    if (!_condition)
	return CN_ERROR;
    else
	return CN_DONE;
}

Condition* Condition::create (ConditionTest* cond, Boolean canDelete)
{
    Condition::initialize();

    return new Condition(cond, canDelete);
}
