/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.h,v
 */

#ifndef REPLICALIST_H_
#define REPLICALIST_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


#ifndef GROUPDATA_H_
#  include <RPC/ArjServers/GroupData.h>
#endif


class TemplateObject;
class CompositeObject;

class UserData
{
    friend class ReplicaList;

public:
    UserData ();
    ~UserData ();

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

private:
    char* hosts;
    pid_t pids;

    UserData* next;
};


class ReplicaList : public LockManager
{
    friend class ToPurge;
    friend class GroupView;
    friend class CompositeObject;

public:
    static ReplicaList* create (int&);
    static ReplicaList* create (const Uid&, int&);
    static void remove (ReplicaList*&);

    /*
     * Standard replica group operations
     */

    ReplicaDescriptor* getList (const char*, pid_t, unsigned long&);
    OutCome addToList (const ReplicaDescriptor&, unsigned long&, Boolean = FALSE);
    OutCome deleteFromList (const ReplicaDescriptor&, long&, unsigned long&, Boolean = FALSE);
    Boolean getUseCount (long&, unsigned long&);

    OutCome alterLocation (const Uid&, const char*, unsigned long&);
    OutCome releaseGroup (const char*, pid_t, unsigned long&);

    /*
     * operations concerned with crash recovery
     */

    OutCome recover (const char*, unsigned long&);
    Boolean getExclusiveAccess ();
    OutCome hasChanged (const Uid&, Boolean&, Boolean&);
    OutCome changeStatus (const ReplicaDescriptor&, const Boolean&, unsigned long&);
    OutCome getHostInfo (ReplicaDescriptor&, ReplicaDescriptor&, const char*, unsigned long&);
    Boolean isMember (const Uid&, unsigned long&);
    Boolean getState (ObjectState&, unsigned long&);
    Boolean imposeState (ObjectState&);
    
    /*
     * operations concerned with complex objects
     */

    OutCome addTemplate (Buffer&, unsigned long&);
    Boolean getTemplate (Buffer&, unsigned long&);
    OutCome deleteTemplate (unsigned long&);
    OutCome replaceTemplate (Buffer&, unsigned long&);

    OutCome assignCompositeObject (const Uid&, unsigned long&);
    OutCome assignCompositeObject (const ReplicaDescriptor&, unsigned long&);
    OutCome removeCompositeObject (const Uid&, unsigned long&);
    OutCome removeCompositeObject (const ReplicaDescriptor&, unsigned long&);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    virtual ostream& printContents (ostream&) const;

private:
    ReplicaList (int&);
    ReplicaList (const Uid&, int&);
    ~ReplicaList ();

    Boolean isMember (const GroupData&);
    Boolean isNew (const char*, pid_t) const;
    void addElement (const ReplicaDescriptor&);

    Boolean searchComposite (const Uid&);
    Boolean searchWildCard ();
    int numberOfTemplateReplicas () const;
    OutCome changeTemplateStatus (const ReplicaDescriptor&, const Boolean&);

    Uid* group;
    unsigned long use_count, v_number;

    GroupData* self;
    TemplateObject* mask;
    CompositeObject* envelope;
    UserData* clients;

    unsigned long linkCount;
    ReplicaList* next;

    static ReplicaList* headOfList;
};


extern ostream& operator<< (ostream& strm, const ReplicaList& rl);

#endif
