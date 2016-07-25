/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CompositeObject.h,v
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

#ifndef NAMESERV_H_
#  include "NameServ.h"
#endif

class ReplicaList;
class GroupData;


class CompositeObject
{
    friend ReplicaList;

public:
    CompositeObject ();
    CompositeObject (const Uid&);
    virtual ~CompositeObject ();

    Boolean addObject (const Uid&, const Uid&);
    void removeObject (const Uid&);

    const Uid& getSubObject () const;
    ReplicaDescriptor* getList (const char*, pid_t) const;
    OutCome releaseGroup (const char*, pid_t) const;

    Boolean pack (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    virtual ostream& print (ostream&) const;

    static void deleteAll (CompositeObject*&);

private:
    Boolean isSubObject (const Uid&) const;
    Boolean isAcyclic (const Uid&, const Uid&) const;
    Boolean scanCompositeObject (CompositeObject*, const Uid&) const;

    Uid* subObject;
    CompositeObject* next;
};


extern ostream& operator<< (ostream& strm, const CompositeObject& co);

#endif
