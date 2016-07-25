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
#include <System/iostream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef STATEDAEMON_H_
#include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#include <RPC/ArjServers/Migration.h>

class ClientRpc;
class RpcControl;

Migrate::Migrate (int& res)
                          : tn(0),
			    S1(0),
			    S2(0)
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

    if (tn)
	delete tn;
}

void Migrate::TypeIs (TypeName t)
{
    tn = new char[strlen(t) +1];
    memset(tn, '\0', strlen(t) +1);
    memcpy(tn, t, strlen(t));
}

Boolean Migrate::readFrom (const char* from, const Uid& replica, Buffer& buff)
{
    ArjunaName AN1;
    UNIQUEID rUid;
    int res = -1;
    Boolean status = FALSE;

    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
    AN1.SetObjName("StateDaemon");
    AN1.SetServiceName("StateDaemon");
    AN1.SetHostName(from);
    ClientRpc* CRPC1 = new ClientRpc(&AN1);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC1->SetTimeout(3000);
    CRPC1->SetRetry(1);
    CRPC1->SetObjectUid(rUid);
#endif
    RpcControl* RPCC1 = new RpcControl(CRPC1);

    S1 = new RemoteStateDaemon(res, FALSE, RPCC1);

    if (res == 0)
    {
	res = -1;
	buff = S1->ReadState(replica, tn, res);
	if (res == 0)
	    status = TRUE;
    }

    return status;
}

Boolean Migrate::writeTo (const char* to, const Uid& replica, Buffer buff)
{
    UNIQUEID rUid;
    ArjunaName AN2;
    int res = -1;
    Boolean status = FALSE, result;

    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
    AN2.SetObjName("StateDaemon");
    AN2.SetServiceName("StateDaemon");
    AN2.SetHostName(to);
    ClientRpc* CRPC2 = new ClientRpc(&AN2);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC2->SetTimeout(3000);
    CRPC2->SetRetry(1);
    CRPC2->SetObjectUid(rUid);
#endif
    RpcControl* RPCC2 = new RpcControl(CRPC2);

    res = -1;
    S2 = new RemoteStateDaemon(res, FALSE, RPCC2);
    if (res == 0)
    {
	res = -1;
	result = S2->WriteCommitted(replica, tn, buff, res);
	if (res == 0)
	{
	    if (result)
		status = TRUE;
	}
    }

    return status;
}

MigrateStatus Migrate::MoveFromTo (const char* from, const char* to,
				   const Uid& group, const Uid& replica)
{
    AtomicAction A;
    Buffer buff;
    int res = -1;
    MigrateStatus status;

    if (tn == 0)
	return MoveFailed;

    A.Begin();

    if (NS->CheckAndSet(group))
    {
	if (readFrom(from, replica, buff))
	{
	    if (writeTo(from, replica, buff))
	    {
		if ((!NS->AlterLocation(group, replica, (string) to)) || (!NS->Release_Group(group)))
		{
		    A.Abort();
		    (void) S2->RemoveState(replica, tn, res);
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
