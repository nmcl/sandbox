/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecArray.h,v 1.4 1993/12/10 11:45:38 nmcl Exp $
 */


#ifndef RECARRAY_H_
#define RECARRAY_H_

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

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef RECINT_H_
#  include "RecInt.h"
#endif


const int SIZE = 2;


class RecArray : public LockManager
{
public:
    RecArray (int&);
    RecArray (const Uid&, int&);
    ~RecArray ();

    Boolean setElement (int, int);
    int getElement (int);

    Boolean hasRemoteState () const;

    Boolean associate ();
    Boolean disassociate ();

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    RecInt* rArray[SIZE];
};

#endif
