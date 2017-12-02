/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CompositeObject.h,v 1.4 1995/02/22 11:26:46 nmcl Exp $
 */

#ifndef COMPOBJECT_H_
#define COMPOBJECT_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif


class ReplicaDescriptor;


class CompositeObject
{
    friend class ReplicaList;

public:
    enum OutCome { NOTDONE, DONE };

    CompositeObject ();
    CompositeObject (const Uid&);
    virtual ~CompositeObject ();

    Boolean            addObject    (const Uid&, const Uid&);
    void               removeObject (const Uid&);
    const Uid&         getSubObject () const;
    ReplicaDescriptor* getList      (const char*, pid_t) const;

    CompositeObject::OutCome releaseGroup (const char*, pid_t) const;

    Boolean pack   (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    virtual ostream& print (ostream&) const;

    static void deleteAll (CompositeObject*&);

private:
    Boolean isSubObject         (const Uid&) const;
    Boolean isAcyclic           (const Uid&, const Uid&) const;
    Boolean scanCompositeObject (CompositeObject*, const Uid&) const;

    Uid*             subObject;  // pointer to replica group
    CompositeObject* next;
};


extern ostream& operator<< (ostream& strm, const CompositeObject& co);

#endif
