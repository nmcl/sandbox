/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Register.h,v
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#include <RPC/ArjServers/NameServ_stub.h>

#ifndef ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <RPC/ArjServers/RemoteOSTypes.h>
#endif


class RemoteStateDaemon;


class ListData
{
    friend class Register;

public:
    ListData ();
    ListData (const char*);
    ~ListData ();

    const char* name () const;
    
    ListData& operator= (const char*);
    ListData& operator= (const ListData&);
    
    ListData* next;

private:
    char* hostName;
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

    void registerGroupUid (const Uid&);
    void registerTypeName (TypeName);

    void registerObject (const Uid&, TypeName);
    Boolean replicateState (ListData*);

private:
    Boolean registerInfo ();

    Boolean readObjectState (ListData*, Buffer&);
    Boolean writeObjectState (ListData*, Buffer);

    RemoteStateDaemon* checkDaemon (char*);

    Uid *group_uid, *register_uid;
    ReplicaDescriptor* rd;
    StateData* head;
    TypeName* tn;
    NSInterface* NS;
};


#include "Register.n"

#endif
