/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaList.h,v 1.10 1994/08/05 11:35:50 nmcl Exp $
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
#  include <ArjServers/GroupData.h>
#endif


class TemplateObject;
class CompositeObject;


class UserData
{
    friend class ReplicaList;

public:
    UserData  ();
    UserData  (const char*, pid_t, unsigned long);
    ~UserData ();

    Boolean pack   (Buffer&) const;
    Boolean unpack (Buffer&);

    static void deleteAll (UserData*&);

private:
    char*         hosts;
    pid_t         pids;

   /*
    * It is necessary to have a separate useCount for each process.
    * This is so modifications of view within a process occur
    * consistently.
    */

    unsigned long useCount;

    UserData*     next;
};


class ReplicaList : public LockManager
{
    friend class ToPurge;
    friend class GroupView;
    friend CompositeObject;

public:
    static ReplicaList* create (int&);
    static ReplicaList* create (const Uid&, int&);
    static void         remove (ReplicaList*&, Boolean = TRUE);

    /*
     * Standard replica group operations
     */

    ReplicaDescriptor* getList        (const char*, pid_t, unsigned long&);
    OutCome            addToList      (const ReplicaDescriptor&, const char*, pid_t, unsigned long&, Boolean = FALSE);
    OutCome            deleteFromList (const ReplicaDescriptor&, long&, const char*, pid_t, unsigned long&, Boolean = FALSE);
    Boolean            getUseCount    (long&, unsigned long&);

    OutCome            alterLocation  (const Uid&, const char*, const char*, pid_t, unsigned long&);
    OutCome            releaseGroup   (const char*, pid_t, unsigned long&);

    /*
     * operations concerned with crash recovery
     */

    OutCome recover             (const char*, unsigned long&);
    Boolean lockGroup           (const char*, pid_t, unsigned long&, LockMode = WRITE);
    OutCome hasChanged          (const Uid&, Boolean&, Boolean&);
    OutCome changeStatus        (const ReplicaDescriptor&, const Boolean&, const char*, pid_t, unsigned long&);
    OutCome getHostInfo         (ReplicaDescriptor&, ReplicaDescriptor&, const char*, unsigned long&);
    Boolean isMember            (const Uid&, unsigned long&);
    Boolean getState            (ObjectState&, unsigned long&);
    Boolean imposeState         (ObjectState&, unsigned long&);

    /*
     * operations concerned with complex objects
     */

    OutCome addTemplate     (Buffer&, const char*, pid_t, unsigned long&);
    Boolean getTemplate     (Buffer&, LockMode, unsigned long&);
    OutCome deleteTemplate  (const char*, pid_t, unsigned long&);
    OutCome replaceTemplate (Buffer&, const char*, pid_t, unsigned long&);

    OutCome assignCompositeObject (const Uid&, const char*, pid_t, unsigned long&);
    OutCome assignCompositeObject (const ReplicaDescriptor&, const char*, pid_t, unsigned long&);
    OutCome removeCompositeObject (const Uid&, const char*, pid_t, unsigned long&);
    OutCome removeCompositeObject (const ReplicaDescriptor&, const char*, pid_t, unsigned long&);

    virtual Boolean        save_state    (ObjectState&, ObjectType);
    virtual Boolean        restore_state (ObjectState&, ObjectType);
    virtual const TypeName type          () const;

    virtual ostream& printContents (ostream&) const;

protected:
    ReplicaList (int&);
    ReplicaList (const Uid&, int&);
    virtual ~ReplicaList ();
    
private:
    Boolean isMember   (const GroupData&);
    Boolean isMember   (const ReplicaDescriptor&);
    Boolean isNew      (const char*, pid_t) const;
    void    addElement (const ReplicaDescriptor&);

    Boolean canLock    (const char*, pid_t);

    Boolean searchComposite (const Uid&);
    Boolean searchWildCard  ();

    int     numberOfTemplateReplicas () const;
    OutCome changeTemplateStatus     (const ReplicaDescriptor&, const Boolean&, const char*, pid_t);

    OutCome assembleViewFromTemplate (ReplicaDescriptor&, ReplicaDescriptor&, const char*);
    OutCome assembleView             (ReplicaDescriptor&, ReplicaDescriptor&, const char*);

    Boolean checkGroupSize           ();

    unsigned long       use_count, v_number;
    Uid*                group;
    GroupData*          self;
    TemplateObject*     mask;
    CompositeObject*    envelope;
    UserData*           clients;

    unsigned long       linkCount;
    ReplicaList*        next;

    static ReplicaList* headOfList;
};


extern ostream& operator<< (ostream& strm, const ReplicaList& rl);

#endif
