/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NSInterface.h,v
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

#include <ArjServers/NameServ_stub.h>


class AtomicAction;

const int rep_number = 10; // maximum number of NameServer databases.

class DataStore
{
public:
    DataStore ();
    ~DataStore ();
    
    ReplicaDescriptor *rd, *rd1;
    Buffer* buff;
    ObjectState* os;
    Uid* groupUid;
    unsigned long versionNumber;
    long UseCount;
    Boolean isModified;
};


class NSInterface
{
public:
    NSInterface (int&);    // create new database
    NSInterface (Boolean&);// use old database OR create new one if nonexistant
    ~NSInterface ();

    // general GVD operations

    Boolean getView (ReplicaDescriptor&, const Uid&, const char*, pid_t, const LockMode = READ);
    Boolean traverseDatabase (ReplicaDescriptor&, Uid&, const char*, int&, pid_t);
    OutCome addReplicas (const ReplicaDescriptor&, const Uid&);
    OutCome deleteReplicas (const ReplicaDescriptor&, const Uid&, const Boolean& = FALSE);
    OutCome alterLocation (const Uid&, const Uid&, const char*);
    OutCome exclude (const ReplicaDescriptor&, const Uid&);
    OutCome include (const ReplicaDescriptor&, const Uid&);
    long getUseCount (const Uid&);

    OutCome releaseGroup (const Uid&, const char*, pid_t);
    Boolean groupAssociation (const Uid&, Uid&);

    // operations concerned with node recovery

    OutCome recover (const char*);
    Boolean getExclusiveAccess ();
    Boolean testAndSet (ReplicaDescriptor&, const Uid&, const char*, pid_t);
    OutCome needsUpdating (const Uid&, Boolean&, ReplicaDescriptor&, const char*, pid_t);
    OutCome getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, const char*);

    // operations concerned with GVD recovery

    Boolean getGroupViewState (ObjectState&);
    Boolean imposeGroupViewState (ObjectState&);

    // operations concerned with composite objects

    OutCome addTemplate (const Uid&, Buffer&);
    OutCome deleteTemplate (const Uid&);
    OutCome replaceTemplate (const Uid&, Buffer&);
    Boolean getTemplate (const Uid&, Buffer&);
    OutCome assignCompositeObject (const Uid&, const Uid&);
    OutCome assignCompositeObject (const Uid&, const ReplicaDescriptor&);
    OutCome removeCompositeObject (const Uid&, const Uid&);
    OutCome removeCompositeObject (const Uid&, const ReplicaDescriptor&);
    
    // Conventional Arjuna naming service operations

    void storeName (Boolean&, Buffer, Buffer);
    void fetchName (Boolean&, Buffer, Buffer&);
    void removeName (Boolean&, Buffer);
    void replaceName (Boolean&, Buffer, Buffer);
    void getNextName (Boolean&, Buffer&, Boolean&);
    void restart (Boolean&);

private:
    void beginAction ();
    Boolean endAction (const Boolean, unsigned long&);

    Boolean isQuorum (Boolean, unsigned long&);
    unsigned long hostInetAddr ();
    char* uidString ();
    char* dirPath ();
    Boolean lockAndCreate ();
    Boolean createNewDatabase ();
    Boolean openAndLock ();
    Boolean closeAndUnlock ();

    void initialise ();
    void remove ();
    void zeroList ();

    void copyReplicaDescriptor (ReplicaDescriptor&, unsigned long,
				Boolean = FALSE) const;
    void copyBuffer (Buffer&, unsigned long) const;
    void copyObjectState (ObjectState&, unsigned long) const;
    void copyUseCount (long&, unsigned long) const;
    void copyBoolean (Boolean&, unsigned long) const;
    void copyUid (Uid&, unsigned long) const;

    AtomicAction* Z;
    Boolean Renamed;
    unsigned short quorum, responded;
    int position, repLevel, fp;
    Uid* NS_Uid;
    NameInfo* X;
    unsigned long g_versions[rep_number];
    unsigned long s_versions[rep_number];
    unsigned long g_version_number, s_version_number;
    DataStore* dataList[rep_number];

    static Boolean rpc_handler ();
    Boolean (*old_handler) ();
    
    ArjunaName ArjName[rep_number];
    
    static ClientRpc* RpcClient[rep_number];
    static RpcControl* RpcCont[rep_number];
    static NameServer* NS[rep_number];
    static int use_count, activated;
};

#endif
