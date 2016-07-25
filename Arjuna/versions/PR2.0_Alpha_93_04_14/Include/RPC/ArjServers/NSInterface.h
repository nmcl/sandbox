/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NSInterface.h
 */

#ifndef NSINTERFACE_H_
#define NSINTERFACE_H_

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#ifndef NAMEINFO_H_
#include <RPC/ArjServers/NameInfo.h>
#endif

#include <RPC/ArjServers/NameServ_stub.h>

const int rep_number = 10; // maximum number of NameServer databases.

class dataStore
{
public:
    dataStore ();
    ~dataStore ();
    
    ReplicaDescriptor *rd, *rd1;
    Buffer* buff;
    ObjectState* os;
    unsigned long versionNumber;
};


class NSInterface
{
public:
    NSInterface (int&);
    NSInterface (Boolean&);
    ~NSInterface ();

// GroupView database operations

    Boolean Get_View (ReplicaDescriptor&, const Uid&, const LockMode = READ);
    Boolean Add_Replicas (const ReplicaDescriptor&, const Uid&);
    Boolean Delete_Replicas (const ReplicaDescriptor&, const Uid&, const Boolean = FALSE);
    Boolean Release_Group (const Uid&);
    Boolean Traverse_Database (ReplicaDescriptor&, Uid&, int&);
    Boolean ChangeReplicaStatus (const ReplicaDescriptor&, const Uid&, const Boolean);
    Boolean NeedsUpdating (const Uid&, ReplicaDescriptor&);
    Boolean GetNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, string, Boolean&);
    Boolean GetExclusiveGroupView (ReplicaDescriptor&, const Uid&);

    Boolean CheckAndSet (const Uid&);
    Boolean AlterLocation (const Uid&, const Uid&, string);

    Boolean GetGroupViewState (ObjectState&);
    Boolean ImposeGroupViewState (ObjectState&);
    
// Conventional Arjuna naming service operations

    void StoreName (Boolean&, Buffer, Buffer);
    void FetchName (Boolean&, Buffer, Buffer&);
    void RemoveName (Boolean&, Buffer);
    void ReplaceName (Boolean&, Buffer, Buffer);
    void GetNextName (Boolean&, Buffer&, Boolean&);
    void Restart (Boolean&);

private:
    Boolean IsQuorum (Boolean, unsigned long&);
    unsigned long hostInetAddr ();
    char* UidString ();
    char* dirPath ();
    Boolean lockAndCreate ();
    Boolean createNewDatabase ();

    Boolean openAndLock ();
    Boolean closeAndUnlock ();

    void Initialise ();
    void Remove ();
    void zeroList ();

    void CopyReplicaDescriptor (ReplicaDescriptor&, unsigned long, Boolean = FALSE);
    void CopyBuffer (Buffer&, unsigned long);
    void CopyObjectState (ObjectState&, unsigned long);

    Boolean IsGroup, Renamed;
    int position, fp;
    Uid* NS_Uid;
    NameInfo* X;

    unsigned long g_versions[rep_number];
    unsigned long s_versions[rep_number];
    unsigned long g_version_number, s_version_number;

    dataStore* dataList[rep_number];

    ArjunaName ArjName[rep_number];
    
    static ClientRpc* RpcClient[rep_number];
    static RpcControl* RpcCont[rep_number];
    static NameServer* NS[rep_number];
    static int use_count, activated, repLevel, quorum;
};

#endif
