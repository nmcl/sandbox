/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupData.h,v 1.6 1995/09/18 11:32:57 nmcl Exp $
 */

#ifndef GROUPDATA_H_
#define GROUPDATA_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef REPLICADATA_STUB_H_
#  include "ReplicaData_stub.h"
#endif


class ostream;
class ReplicaDescriptor;


class GroupData : public ReplicaData
{
public:
    GroupData ();
    GroupData (const GroupData&);
    GroupData (const ReplicaDescriptor&);
    virtual ~GroupData ();

    Boolean pack   (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    GroupData& operator=  (const ReplicaDescriptor&);
    GroupData& operator=  (const GroupData&);
    Boolean    operator== (const ReplicaDescriptor&);
    Boolean    operator== (const GroupData&);
    Boolean    operator!= (const ReplicaDescriptor&);
    Boolean    operator!= (const GroupData&);
    GroupData& operator+= (const GroupData&);

    Boolean    isMemberOfList (const Uid&) const;
    long       getNumber      () const;

    static void       deleteAll    (GroupData*&);
    static GroupData* assembleView (const GroupData&, GroupData*);

    virtual ostream& print (ostream&) const;

    GroupData* next;
};

extern const GroupData NIL_GD;

extern ostream& operator<< (ostream& strm, const GroupData& gd);

#endif
