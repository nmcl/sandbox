/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * Stack interface definition. (AtomicAction)
 */

enum stackOutcome { unknown, stack_full, stack_empty, not_done, done };

const int stackSize = 32;

#include <Common/Boolean.h>
#include <Common/Uid.h>
#include <Arjuna/LockMan.h>
#include <Arjuna/ObjState.h>

class Stack : public LockManager
{
public:
    Stack(enum stackOutcome& outcome);
    Stack(Uid uid, enum stackOutcome& outcome);
    ~Stack();

    void Push(enum stackOutcome& outcome, int  value);
    void Pop (enum stackOutcome& outcome, int& value);

    virtual Boolean save_state(ObjectState& os, ObjectType ot);
    virtual Boolean restore_state(ObjectState& os, ObjectType ot);
    virtual const TypeName type() const;
private:
    int values[stackSize];
    int top;
};
