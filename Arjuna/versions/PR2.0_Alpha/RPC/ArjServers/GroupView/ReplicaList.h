/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.h
 */

#ifndef REPLICALIST_H_
#define REPLICALIST_H_

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef GROUPDATA_H_
#include "GroupData.h"
#endif

const int groupmembers = 10;

typedef char* string;

// @NoRemote, @NoMarshall
class ReplicaList : public LockManager
{
public:
    ReplicaList (int&);
    ReplicaList (const Uid&, int&);
    ~ReplicaList ();

    GroupData* GetList (unsigned long&, const char* = 0);
    Boolean AddToList (const ReplicaDescriptor&, unsigned long&);
    Boolean DeleteFromList (const ReplicaDescriptor&, long&, unsigned long&);

    Boolean Recover (const char* = 0);
    Boolean HasChanged (const Uid);
    Boolean ChangeStatus (const ReplicaDescriptor&, const Boolean, unsigned long&);
    Boolean ReleaseGroup (unsigned long&, char* = 0);
    Boolean GetHostInfo (ReplicaDescriptor&, ReplicaDescriptor&, const string, Boolean&, unsigned long&);

    Boolean GetState (ObjectState&, unsigned long&);
    Boolean ImposeState (ObjectState&);
    
    long GetUseCount ();
    Boolean GetExclusiveAccess ();
    unsigned long GetVersionNumber ();
    Boolean AlterLocation (const Uid&, string);

    Uid* group;

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    GroupData* self;
    long number; // number of replicas in group
    char* hosts[groupnumbers]; // maximum number of users for a group
    long use_count; // number of users of group
    unsigned long v_number; // version number of this element
};

extern ostream& operator<< (ostream& strm, const GroupData& gd);

#endif
