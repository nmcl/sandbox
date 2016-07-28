/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AAStatus.h,v 1.4 1994/08/04 08:59:08 ndbi Exp $
 */

#ifndef AASTATUS_H_
#define AASTATUS_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

/* 
 * Enum: ActionCondition
 * Records the status of an atomic action
 */

enum ActionCondition
{
    ACTION_STATUS_UNKNOWN, ACTION_COMMITTED, ACTION_ABORTED, ACTION_ATTACHED
};

ostream& operator<< (ostream&, ActionCondition);

/*
 * Class: AAStatus
 * Used to check for the existence of AtomicAction states in the object store
 */

class AAStatus
{
public:
    AAStatus (int &);
    ~AAStatus ();

    ActionCondition isCommitted(const Uid&, Boolean);

private:
    Boolean pingProcess(int);
};

#endif // AASTATUS_H_
