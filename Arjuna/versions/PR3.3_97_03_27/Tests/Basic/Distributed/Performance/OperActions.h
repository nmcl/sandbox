/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActions.h,v 1.3 1994/10/28 12:39:01 ngdp Exp $
 */

#ifndef OPERACTIONS_H_
#define OPERACTIONS_H_

#include <System/sys/types.h>

#include <Common/Boolean.h>

#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>
#include "PerfVector.h"

// @Remote, @NoMarshall
class OperActions : public LockManager
{
    size_t size;
    void   *value;
public:
    OperActions(size_t, ObjectType);
    OperActions(const Uid&);
    ~OperActions();

    PerformanceVector core ();
    PerformanceVector nop();
    PerformanceVector be();
    PerformanceVector ba();
    PerformanceVector bre();
    PerformanceVector bwe();
    PerformanceVector bra();
    PerformanceVector bwa();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
