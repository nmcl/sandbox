/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjState.h,v 1.1 1993/11/03 14:27:55 nmcl Exp $
 */

#ifndef OBJSTATE_H_
#define OBJSTATE_H_

/*
 *
 * Object State Class
 *
 */

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
