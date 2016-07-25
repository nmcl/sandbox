/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupData.h
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

#ifndef REPLICADATA_H_
#  include "ReplicaData.h"
#endif


class ostream;
class ReplicaDescriptor;
class ReplicaList;


class GroupData : public ReplicaData
{
    friend ReplicaList;
public:
    GroupData ();
    virtual ~GroupData ();

    Boolean groupIsMember (const Uid&) const;

    Boolean pack (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    GroupData& operator= (const ReplicaDescriptor&);
    GroupData& operator= (const GroupData&);
    Boolean operator== (const ReplicaDescriptor&);
    Boolean operator!= (const ReplicaDescriptor&);
    GroupData& operator+= (const GroupData&);

    virtual ostream& print (ostream&) const;

    GroupData* next;

private:
    long getNumber () const;
};


extern ostream& operator<< (ostream& strm, const GroupData& gd);

#endif
