/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NSInterface.h,v 1.25 1995/06/26 13:07:09 ngdp Exp $
 */

#ifndef NSINTERFACE_H_
#define NSINTERFACE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef NAMEINFO_H_
#  include <ArjServers/NameInfo.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState_stub.h>
#endif

#include <ArjServers/NameServ_stub.h>


/*
 * NSInterface hides the active replication of the name server. The
 * system cannot tolerate network partitions, but the version numbers
 * used by the name server attempt to solve this, and also provide
 * group locking.
 */


/* maximum number of NameServer databases. */

const int rep_number = 10;


#ifndef __linux__
const int DebugTimeout = 5000;
const int DebugRetry = 5;
#else
const int DebugTimeout = 10000;
const int DebugRetry = 10;
#endif
const int NormalTimeout = 4000;
const int NormalRetry = 4;


//@NoRemote, @NoMarshall
class AtomicAction;


//@NoRemote, @NoMarshall
class DataStore
{
public:
    DataStore  ();
    ~DataStore ();
    
    ReplicaDescriptor *rd, *rd1;
    Buffer            *buff;
    ObjectState       *os;
    Uid               *groupUid;
    int               position;
    unsigned long     versionNumber;
    long              UseCount;
    Boolean           isModified;
};


//@Remote, @NoMarshall
class NSInterface
{
public:
    NSInterface (int&);         /* use old database. */
    virtual ~NSInterface ();

    /* general GVD operations. */

    Boolean getView          (ReplicaDescriptor&, const Uid&, const char*, pid_t);
    Boolean lockGroup        (const Uid&, const char*, pid_t, LockMode);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, const char*, int&, pid_t);
    OutCome addReplicas      (const ReplicaDescriptor&, const Uid&);
    OutCome deleteReplicas   (const ReplicaDescriptor&, const Uid&, const Boolean& = FALSE);
    OutCome alterLocation    (const Uid&, const Uid&, const char*);
    OutCome exclude          (const ReplicaDescriptor&, const Uid&);
    OutCome include          (const ReplicaDescriptor&, const Uid&);
    long getUseCount         (const Uid&);

    OutCome releaseGroup     (const Uid&, const char*, pid_t);
    Boolean groupAssociation (const Uid&, Uid&);

    /* operations concerned with node recovery. */

    OutCome recover             (const char*);
    Boolean testAndSet          (ReplicaDescriptor&, const Uid&, const char*, pid_t);
    OutCome needsUpdating       (const Uid&, Boolean&, ReplicaDescriptor&, const char*, pid_t);
    OutCome getNodeUpdateList   (ReplicaDescriptor&, ReplicaDescriptor&, const char*);

    /* operations concerned with GVD recovery. */

    Boolean getGroupViewState    (ObjectState&);
    Boolean imposeGroupViewState (ObjectState&);

    Boolean getReplicaState      (const Uid&, ObjectState&);
    Boolean imposeReplicaState   (const Uid&, ObjectState&);

    /* operations concerned with composite objects. */

    OutCome addTemplate           (const Uid&, Buffer&);
    OutCome deleteTemplate        (const Uid&);
    OutCome replaceTemplate       (const Uid&, Buffer&);
    Boolean getTemplate           (const Uid&, Buffer&, LockMode = READ);
    OutCome assignCompositeObject (const Uid&, const Uid&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&);
    OutCome removeCompositeObject (const Uid&, const Uid&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&);

    OutCome splitTemplateGroup (const Uid&, const Uid&);
    OutCome mergeTemplateGroup (const Uid&, const Uid&);

    /* Conventional Arjuna naming service operations. */

    void storeName   (Boolean&, Buffer, Buffer);
    void fetchName   (Boolean&, Buffer, Buffer&);
    void removeName  (Boolean&, Buffer);
    void replaceName (Boolean&, Buffer, Buffer);
    void getNextName (Boolean&, Buffer&, Boolean&);
    void restart     (Boolean&);

private:
    void    beginAction (AtomicAction&);
    Boolean endAction   (AtomicAction&, const Boolean, unsigned long&, LockMode);

    Boolean       isQuorum          (Boolean, unsigned long&, LockMode);
    unsigned long hostInetAddr      () const;
    char*         uidString         () const;

    void initialise ();
    void remove     ();
    void zeroList   ();

    void copyReplicaDescriptor (ReplicaDescriptor&, unsigned long, Boolean = FALSE) const;
    void copyBuffer            (Buffer&, unsigned long) const;
    void copyObjectState       (ObjectState&, unsigned long) const;
    void copyUseCount          (long&, unsigned long) const;
    void copyBoolean           (Boolean&, unsigned long) const;
    void copyUid               (Uid&, unsigned long) const;
    void copyPosition          (int&, unsigned long) const;

    static Boolean rpc_handler (RPC_Status, Int32);
    Boolean (*old_handler) (RPC_Status, Int32);

    NameInfo*                X;    
    int                      responded, position, fp;
    unsigned long            g_versions[rep_number], s_versions[rep_number];
    unsigned long            g_version_number, s_version_number;
    DataStore*               dataList[rep_number];

    ArjunaName               ArjName[rep_number];

    static ClientRpc*        RpcClient[rep_number];
    static ClientRpcManager* RpcMan[rep_number];
    static NameServer*       NS[rep_number];
    static int               use_count, activated;
    static int               repLevel;
    static int               readQuorum, writeQuorum;

public:
    static char*             nodeName;
    static pid_t             processID;
};

#endif
