/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 */

/*
 * $Id: OperActions.h,v 1.1 1993/11/03 14:58:29 nmcl Exp $
 */

#ifndef OPERACTIONS_H_
#define OPERACTIONS_H_

#include <System/sys/types.h>

#include <Common/Boolean.h>

#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

// @Remote, @NoMarshall
class OperActions : public LockManager
{
    size_t size;
    char   *value;
public:
    OperActions(size_t, ObjectType);
    ~OperActions();

    void NOP();

    void BRE();
    void BWE();
    void BRA();
    void BWA();

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
