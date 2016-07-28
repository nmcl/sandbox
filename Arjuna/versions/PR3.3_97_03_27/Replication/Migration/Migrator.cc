/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Migrator.cc,v 1.18 1995/09/30 10:22:21 nmcl Exp $
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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef MIGRATOR_H_
#  include <Replication/Migrator.h>
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#  ifndef RAJDOOT_RPCUSER_H_
#    include <RPC/Rajdoot/rpcuser.h>
#  endif
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


Migrator::Migrator (const char* root1, const char* root2)
		   : tn(0),
		     NS(0),
		     S1(0),
		     S2(0),
		     RPCC1(0),
		     RPCC2(0),
		     CRPC1(0),
		     CRPC2(0),
		     objectStoreRoot1(root1),
		     objectStoreRoot2(root2)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Migrator::Migrator ()" << endl;
#endif
}

Migrator::Migrator (int& res, const char* root1, const char* root2)
		   : tn(0),
		     NS(0),
		     S1(0),
		     S2(0),
		     RPCC1(0),
		     RPCC2(0),
		     CRPC1(0),
		     CRPC2(0),		     
		     objectStoreRoot1(root1),
		     objectStoreRoot2(root2)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Migrator::Migrator (int& res)" << endl;
#endif

    res = ((setupDatabase() == TRUE) ? 0 : -1);
}

Migrator::~Migrator ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;    
    debug_stream << "Migrator::~Migrator ()" << endl;
#endif

    tidyup();
    
    if (NS)
	delete NS;
    if (tn)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] tn;
#else
        ::delete tn;
#endif
}

void Migrator::tidyup()
{
    if (S1)
	delete S1;
    if (S2)
	delete S2;
    if (RPCC1)
	delete RPCC1;
    if (RPCC2)
	delete RPCC2;
    if (CRPC1)
	delete CRPC1;
    if (CRPC2)
	delete CRPC2;
}

Boolean Migrator::setupDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean Migrator::setupDatabase ()" << endl;
#endif
    int opcode = -1;

    if (NS)
        return TRUE;

    NS = new NSInterface(opcode);
    return ((opcode == 0) ? TRUE : FALSE);
}

void Migrator::typeIs (TypeName t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void Migrator::typeIs ( " << t << " )" << endl;
#endif    

    tn = ::new char[strlen(t) +1];
    ::memset(tn, '\0', strlen(t) +1);
    ::memcpy(tn, t, strlen(t));
}

Boolean Migrator::readFrom (const char* from, const Uid& replica, ObjectState*& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean Migrator::readFrom ( " << from << ", " << replica << " )" << endl;
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID   rUid;
#endif
    ArjunaName AN1;
    int        res    = -1;
    Boolean    status = FALSE;
    
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = RPOBJSTOREUID;
#endif
    AN1.setObjectName("RPObjStore");
    AN1.setServiceName("RPObjStore");
    AN1.setHostName(from);
    CRPC1 = new ClientRpc(&AN1);
    CRPC1->disableAtomic();
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC1->setTimeout(NormalTimeout);
    CRPC1->setRetry(NormalRetry);
    CRPC1->setObjectUid(rUid);
#endif
    RPCC1 = new ClientRpcManager(CRPC1);

    S1 = new RemoteRPObjStore(res, RPCC1);

    if (res == 0)
    {
	res = -1;
	buff = S1->read_committed(replica, tn, objectStoreRoot1, res);
	if (res == 0)
	    status = TRUE;
    }

    return status;
}

Boolean Migrator::writeTo (const char* to, const Uid& replica, ObjectState buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean Migrator::writeTo ( " << to << ", " << replica << " )" << endl;
#endif
    
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID   rUid;
#endif
    ArjunaName AN2;
    int        res = -1;
    Boolean    status = FALSE;

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    rUid.host = rUid.sec = rUid.msec = RPOBJSTOREUID;
#endif
    AN2.setObjectName("RPObjStore");
    AN2.setServiceName("RPObjStore");
    AN2.setHostName(to);
    CRPC2 = new ClientRpc(&AN2);
    CRPC2->disableAtomic();
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    CRPC2->setTimeout(NormalTimeout);
    CRPC2->setRetry(NormalRetry);
    CRPC2->setObjectUid(rUid);
#endif
    RPCC2 = new ClientRpcManager(CRPC2);

    res = -1;
    S2 = new RemoteRPObjStore(res, RPCC2);
    if (res == 0)
    {
	res = -1;
	status = S2->write_committed(replica, tn, buff, objectStoreRoot2, res);
	if (res != 0)
	    status = FALSE;
    }

    return status;
}

/*
 * Neither of the migration routines attempts to remove state if a failure occurs
 * which would prevent this, i.e., this is not failure atomic. If the state is
 * moved, and it cannot be removed from the original location then a warning is
 * printed, but the operation returns successfully.
 * [If crash recovery could deal with it, we could insert another abstract
 * record at this point to guarantee failure atomicity].
 */

/*
 * This routine simply migrates a replica and does not affect the database.
 */

MigratorStatus Migrator::moveFromTo (const char* from, const char* to,
				     const Uid& replica)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "MigratorStatus Migrator::moveFromTo ( " << from << ", " << to << ", " << replica << " )" << endl;
#endif

    tidyup();
    
    ObjectState* buff = (ObjectState*) 0;
    
    if (tn == (char*) 0)
	return MoveFailed;
    
    if (readFrom(from, replica, buff))
    {
	if (writeTo(to, replica, *buff))
	{
	    int     res    = -1;
	    Boolean status = S1->remove_committed(replica, tn, objectStoreRoot1, res);
	    
	    if ((res != 0) || ((res == 0) && (status == FALSE)))
		error_stream << WARNING << "Could not remove state " << replica << " from machine " << from << endl;
	    
	    return MovedOk;
	}
	else
	    return ToNodeFailed;
    }

    return FromNodeFailed;
}

/*
 * This routine migrates a replica and updates the database entry.
 */
    
MigratorStatus Migrator::moveFromTo (const char* from, const char* to,
				     const Uid& group, const Uid& replica)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "MigratorStatus Migrator::moveFromTo ( " << from << ", " << to << ", "
		 << group << ", " << replica << " )" << endl;
#endif

    tidyup();
    
    AtomicAction      A;
    MigratorStatus    status;
    char              hostName[MAXHOSTNAMELEN+1];
    ReplicaDescriptor dummy;
    ObjectState*      buff = (ObjectState*) 0;
    pid_t             pid  = getpid();

    if ((tn == (char*) 0) || (NS == (NSInterface*) 0))
	return MoveFailed;

    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    (void) ::gethostname(hostName, MAXHOSTNAMELEN);

    A.Begin();

    if (NS->testAndSet(dummy, group, hostName, pid))
    {
	if (readFrom(from, replica, buff))
	{
	    if (writeTo(to, replica, *buff))
	    {
		delete buff;
		
		if ((NS->alterLocation(group, replica, to) != DONE) || (NS->releaseGroup(group, hostName, pid) != DONE))
		{
		    int     res  = -1;
		    Boolean result;

		    A.Abort();
		    result = S2->remove_committed(replica, tn, objectStoreRoot2, res);
		    
		    if ((res != 0) || ((res == 0) && (result == FALSE)))
			error_stream << WARNING << "Could not remove state "
				     << replica << " from machine "
				     << to << endl;

		    return RegisterFailed;
		}
		else
		{
		    int     res = -1;
		    Boolean result = S1->remove_committed(replica, tn, objectStoreRoot1, res);
	    
		    if ((res != 0) || ((res == 0) && (result == FALSE)))
			error_stream << WARNING << "Could not remove state "
				     << replica << " from machine " << from << endl;

		    if (A.End() == COMMITTED)
			return MovedOk;
		    else
			return MoveFailed;
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

    A.Abort();

    if (buff)
	delete buff;

    return status;
}
