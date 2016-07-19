/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Register.h
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#include <RPC/ArjServers/NameServ_stub.h>

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

class RemoteStateDaemon;

class ReplicaData
{
public:
    ReplicaData ();
    ReplicaData (const char*);
    ~ReplicaData ();

    char* hostName;

    ReplicaData& operator= (const char*);
    ReplicaData& operator= (const ReplicaData&);
    
    ReplicaData* next;
};

class StateData
{
public:
    StateData ();
    ~StateData ();

    ClientRpc* RpcClient;
    RpcControl* RpcCont;
    ArjunaName ArjName;
    RemoteStateDaemon* SD;
    char *hostName;

    StateData* next;
};


class Register
{
public:
    Register ();
    ~Register ();

    void RegisterGroupUid (const Uid&);
    void RegisterTypeName (TypeName);

    void RegisterObject (const Uid&, TypeName);
    Boolean ReplicateState (ReplicaData*);

private:
    Boolean registerInfo ();

    Boolean readObjectState (ReplicaData*, Buffer&);
    Boolean writeObjectState (ReplicaData*, Buffer);

    RemoteStateDaemon* checkDaemon (char*);

    Uid *group_uid, *register_uid;
    ReplicaDescriptor* rd;
    StateData* head;
    TypeName* tn;
    NSInterface* NS;
};

#endif
