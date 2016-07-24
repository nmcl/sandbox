/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecInt.h,v 1.2 1993/11/20 11:03:58 nmcl Exp $
 */


#ifndef RECINT_H_
#define RECINT_H_

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif


class RecInt : public LockManager
{
public:
    RecInt (int&);
    RecInt (const Uid&, int&);
    ~RecInt ();

    Boolean change (int);
    int value ();

    Boolean hasRemoteState () const;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    int number;
};

#endif
