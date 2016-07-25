/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Migration.cc,v 
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef STATEDAEMON_H_
#  include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#ifndef MIGRATION_H_
#  include <RPC/ArjServers/Migration.h>
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#  ifndef RAJDOOT_RPCUSER_H_
#    include <RPC/Rajdoot/rpcuser.h>
#  endif
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

Migrate::Migrate (int& res)
                          : tn(0),
			    S1(0),
			    S2(0),
			    RPCC1(0),
			    RPCC2(0)
{
    int opcode = -1;
    NS = new NSInterface(opcode);

    if (opcode == 0)
	res = 0;
}

Migrate::~Migrate ()
{
    if (NS)
	delete NS;

    if (S1)
	delete S1;

    if (S2)
	delete S2;

    if (RPCC1)
	delete RPCC1;

    if (RPCC2)
	delete RPCC2;

    if (tn)
#ifndef __GNUG__
	::delete [] tn;
#else
        ::delete tn;
#endif
}

void Migrate::typeIs (TypeName t)
{
    tn = ::new char[strlen(t) +1];
    ::memset(tn, '\0', strlen(t) +1);
    ::memcpy(tn, t, strlen(t));
}

Boolean Migrate::readFrom (const char* from, const Uid& replica, Buffer& buff)
{
    ArjunaName AN1;
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;
#endif
    int res = -1;
    Boolean status = FALSE;

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
#endif
    AN1.SetObjName("StateDaemon");
    AN1.SetServiceName("StateDaemon");
    AN1.SetHostName(from);
    ClientRpc* CRPC1 = new ClientRpc(&AN1);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC1->SetTimeout(3000);
    CRPC1->SetRetry(1);
    CRPC1->SetObjectUid(rUid);
#endif
    RPCC1 = new RpcControl(CRPC1);

    S1 = new RemoteStateDaemon(res, FALSE, RPCC1);

    if (res == 0)
    {
	res = -1;
	buff = S1->readState(replica, tn, res);
	if (res == 0)
	    status = TRUE;
    }

    return status;
}

Boolean Migrate::writeTo (const char* to, const Uid& replica, Buffer buff)
{
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;
#endif
    ArjunaName AN2;
    int res = -1;
    Boolean status = FALSE, result;

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
#endif
    AN2.SetObjName("StateDaemon");
    AN2.SetServiceName("StateDaemon");
    AN2.SetHostName(to);
    ClientRpc* CRPC2 = new ClientRpc(&AN2);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC2->SetTimeout(3000);
    CRPC2->SetRetry(1);
    CRPC2->SetObjectUid(rUid);
#endif
    RPCC2 = new RpcControl(CRPC2);

    res = -1;
    S2 = new RemoteStateDaemon(res, FALSE, RPCC2);
    if (res == 0)
    {
	res = -1;
	result = S2->writeCommitted(replica, tn, buff, res);
	if (res == 0)
	{
	    if (result)
		status = TRUE;
	}
    }

    return status;
}

MigrateStatus Migrate::moveFromTo (const char* from, const char* to,
				   const Uid& group, const Uid& replica)
{
    AtomicAction A;
    Buffer buff;
    int res = -1;
    MigrateStatus status;
    char hostName[MAXHOSTNAMELEN+1];
    pid_t pid = getpid();
    ReplicaDescriptor dummy;

    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    (void) ::gethostname(hostName, MAXHOSTNAMELEN);

    if (tn == 0)
	return MoveFailed;

    A.Begin();

    if (NS->testAndSet(dummy, group, hostName, pid))
    {
	if (readFrom(from, replica, buff))
	{
	    if (writeTo(from, replica, buff))
	    {
		if ((NS->alterLocation(group, replica, to) != Done) || (NS->releaseGroup(group, hostName, pid) != Done))
		{
		    A.Abort();
		    (void) S2->removeState(replica, tn, res);
		    return RegisterFailed;
		}
		else
		{
		    if (A.End() == COMMITTED)
			status = MovedOk;
		    else
			status = MoveFailed;
		}
	    }
	    else
		status = ToNodeFailed;
	}
	else
	    status = FromNodeFailed;
    }
    else
	status = GroupLocked;

    if ((status != MovedOk) && (status != MoveFailed))
	A.Abort();

    return status;
}
