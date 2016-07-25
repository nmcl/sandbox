/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjState.h,v 1.7 1993/03/22 09:36:59 ngdp Exp $
 */

#ifndef OBJSTATE_H_
#define OBJSTATE_H_

/*
 *
 * Object State Class
 *
 */

#ifdef STUB
#  pragma @NoRename
#endif 

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

// @NoRemote, @UserMarshall
class ObjectState : public Buffer
{
public:
    /* Constructors & destructors */

    ObjectState ();
    ObjectState (const ObjectState& copyFrom);
    ObjectState (const Uid& newUid, TypeName tName);
    ObjectState (const Uid& newUid, TypeName tName, size_t size, char *buffer);
    ~ObjectState ();

    /* non-virtual member functions and operators */

    char *buffer () const;
    Boolean notempty () const;
    size_t size () const;
    const Uid& stateUid () const;
    const TypeName type () const;

    ObjectState& operator= (const ObjectState& objstate);

    /* virtual member functions and operators */

    virtual Boolean packInto (Buffer& buff) const;
    virtual Boolean unpackFrom (Buffer& buff);
    
    virtual ostream& print (ostream& strm) const;

private:
    /* non-virtual member functions */

    void copytype (const TypeName tName);

    /* private state variables */

    Uid bufferUid;    
    TypeName imageType;

};

#include <Arjuna/ObjState.n>

#endif
