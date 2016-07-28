/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NSInterface.cc,v 1.37 1995/08/10 13:03:29 nmcl Exp $
 */

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYSENT_H_
#  include <System/unistd.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef NAMEINFO_H_
#  include <ArjServers/NameInfo.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


NameServer* NSInterface::NS[rep_number] = { 0 };
ClientRpc* NSInterface::RpcClient[rep_number] = { 0 };
ClientRpcManager* NSInterface::RpcMan[rep_number] = { 0 };
int NSInterface::use_count = 0;
int NSInterface::activated = 0;
int NSInterface::repLevel = 0;
int NSInterface::readQuorum = 0;
int NSInterface::writeQuorum = 0;
char* NSInterface::nodeName = (char*) 0;
pid_t NSInterface::processID = -1;



DataStore::DataStore ()
{
    rd = rd1 = (ReplicaDescriptor*) 0;
    buff = (Buffer*) 0;
    os = (ObjectState*) 0;
    groupUid = (Uid*) 0;
    versionNumber = UseCount = 0;
    isModified = FALSE;
    position = 0;
}

DataStore::~DataStore ()
{
    if (rd)
	delete rd;
    if (rd1)
	delete rd1;
    if (buff)
	delete buff;
    if (os)
	delete os;
    if (groupUid)
	delete groupUid;
}



NSInterface::NSInterface (int& res)
			 : old_handler(0),
			   X(0),
			   responded(0),
			   position(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NSInterface::NSInterface (int& res)\n" << flush;
#endif

    AtomicAction   A;
    int            ret;
    char*          uidType = uidString();
    Boolean        done    = FALSE;
    Uid            Y(uidType);

    initialise();
    if (NSInterface::use_count > 0)
    {
	NSInterface::use_count++;
	if (NSInterface::activated >= NSInterface::readQuorum)
	    res = 0;
	else
	    error_stream << WARNING << "NSInterface::Insufficient replicas started for read operations.\n" << flush;

	if (NSInterface::activated < NSInterface::writeQuorum)
	    error_stream << WARNING << "NSInterface::Insufficient replicas started for write operations.\n" << flush;
    }
    else
    {
	A.Begin();
	
	/* database already exists, so no need to create new one. */

	X = new NameInfo(Y, done);
	if (done)
	{
	    NameData *marker, *ptr = (NameData*) 0;

	    done = FALSE;
	    marker = X->getReplicationInfo(done, NSInterface::repLevel);
	    if ((done) && (NSInterface::repLevel > 0))
	    {
		int i = 0;

		/* turn off stub generated abort if call fails. */

		old_handler = setHandler(NSInterface::rpc_handler);

		// currently these are the same, but they need not be.
		
		NSInterface::readQuorum = NSInterface::writeQuorum = (NSInterface::repLevel/2)+1;

		ptr = marker;
		while (ptr != (NameData*) 0)
		{
		    ArjName[i].setObjectName("NameServer");
		    ArjName[i].setServiceName("NameServer");
		    ArjName[i].setHostName(ptr->hostname);
		    NSInterface::RpcClient[i] = new ClientRpc(&ArjName[i]);

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
		    NSInterface::RpcClient[i]->setTimeout(DebugTimeout);
		    NSInterface::RpcClient[i]->setRetry(DebugRetry);
#else
		    NSInterface::RpcClient[i]->setTimeout(NormalTimeout);
		    NSInterface::RpcClient[i]->setRetry(NormalRetry);
#endif
#endif
		    NSInterface::RpcMan[i] = new ClientRpcManager(NSInterface::RpcClient[i]);
		    ret = -1;
		    NSInterface::NS[i] = new NameServer(*ptr->myUID, ret, NSInterface::RpcMan[i]);

		    if (ret == 0)
			NSInterface::activated++;
		    else
		    {
#ifndef FASTTERMINATE
			delete NSInterface::NS[i];
			delete NSInterface::RpcMan[i];
			delete NSInterface::RpcClient[i];
#endif			
			NSInterface::NS[i] = (NameServer*) 0;
			NSInterface::RpcMan[i] = (ClientRpcManager*) 0;
			NSInterface::RpcClient[i] = (ClientRpc*) 0;
		    }
		    ptr = ptr->next;
		    i++;
		}

		/* turn stub generator abort back on for other servers. */
		setHandler(old_handler);
		
		NSInterface::use_count++;
		if (NSInterface::activated >= NSInterface::readQuorum)
		    res = 0;
		else
		    error_stream << WARNING << "NSInterface::Insufficient replicas started for read operations.\n" << flush;

		if (NSInterface::activated < NSInterface::writeQuorum)
	    	    error_stream << WARNING << "NSInterface::Insufficient replicas started for write operations.\n" << flush;
	    }
	    else
		error_stream << WARNING
			     << "NSInterface - NameInfo state has not been initialised. Remember to set up the name server." << endl;
	}
	else
	    error_stream << WARNING
			 << "NSInterface - could not find NameInfo state. Remember to set up the name server." << endl;
	
	if (res == 0)
	    res = ((A.End() == COMMITTED) ? 0 : -1);
	else
	    A.Abort();
    }
    
    if (uidType)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] uidType;
#else
        ::delete uidType;
#endif
}

NSInterface::~NSInterface ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NSInterface::~NSInterface ()\n" << flush;
#endif

    NSInterface::use_count--;
    if (NSInterface::use_count == 0)
    {
	for (int i = 0; i < rep_number; i++)
	{
	    if (NSInterface::NS[i])
	    {
		delete NSInterface::NS[i];
		NSInterface::NS[i] = (NameServer*) 0;
		if (NSInterface::RpcMan[i])
		{
		    delete NSInterface::RpcMan[i];
		    NSInterface::RpcMan[i] = (ClientRpcManager*) 0;
		}
		if (NSInterface::RpcClient[i])
		{
		    delete NSInterface::RpcClient[i];
		    NSInterface::RpcClient[i] = (ClientRpc*) 0;
		}
	    }
	}
	if (NSInterface::nodeName)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] NSInterface::nodeName;
#else
	    ::delete NSInterface::nodeName;
#endif
	    NSInterface::nodeName = (char*) 0;
	    NSInterface::processID = -1;
	}
    }

    if (X)
	delete X;

    remove();

    /* make sure stub generator abort is turned back on. */

    if (old_handler)
	setHandler(old_handler);
}

void NSInterface::initialise ()
{
    if (NSInterface::processID == -1)
    {
	NSInterface::processID = ::getpid();
	NSInterface::nodeName = ::new char[MAXHOSTNAMELEN+1];
	::memset(NSInterface::nodeName, '\0', MAXHOSTNAMELEN+1);
	::gethostname(NSInterface::nodeName, MAXHOSTNAMELEN);
    }
    for (int i = 0; i < rep_number; i++)
	dataList[i] = (DataStore*) 0;
}

Boolean NSInterface::rpc_handler ( RPC_Status, Int32 ) { return FALSE; }

void NSInterface::remove ()
{
    for (int i = 0; i < rep_number; i++)
    {
	if (dataList[i])
	{
	    delete dataList[i];
	    dataList[i] = (DataStore*) 0;
	}
    }
}

void NSInterface::zeroList ()
{
    remove();
    initialise();
}

char* NSInterface::uidString () const
{
    unsigned long hostAddr = hostInetAddr();
    char* uidName = ::new char[80];
    ::memset(uidName, '\0', 80);
    sprintf(uidName, "%lx:0:0:0", hostAddr);
    return uidName;
}

unsigned long NSInterface::hostInetAddr () const
{
    static unsigned long myAddr;
    static char hostname[256];
    struct hostent *h;
    ::gethostname(hostname, 256);
    h = ::gethostbyname(hostname);
    ::memcpy(&myAddr, h->h_addr, h->h_length);
    return myAddr;
}

Boolean NSInterface::isQuorum (Boolean groupview, unsigned long& version, LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::isQuorum (Boolean groupview, unsigned long& version)" << endl;
#endif

    int is_same = 0;
    int quorum = ((mode == READ) ? NSInterface::readQuorum : NSInterface::writeQuorum);

    g_version_number = s_version_number = 0;

    if (groupview)
    {
	for (int i = 0; i < NSInterface::repLevel; i++)
	{
	    if (g_version_number < g_versions[i])
	    {
		is_same = 0;
		version = g_version_number = g_versions[i];
	    }
	    if (g_versions[i] == g_version_number)
		is_same++;
	}
    }
    else
    {
	for (int j = 0; j < NSInterface::repLevel; j++)
	{
	    if (s_version_number < s_versions[j])
	    {
		is_same = 0;
		version = s_version_number = s_versions[j];
	    }
	    if (s_versions[j] == s_version_number)
		is_same++;
	}
    }

    return ((is_same >= quorum) ? TRUE : FALSE);
}

void NSInterface::copyUid (Uid& group, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
		group = *(dataList[i]->groupUid);
		return;
	    }
	}
    }
}

void NSInterface::copyUseCount (long& UseCount, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
		UseCount = dataList[i]->UseCount;
		return;
	    }
	}
    }
}

void NSInterface::copyBoolean (Boolean& isModified, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
		isModified = dataList[i]->isModified;
		return;
	    }
	}
    }
}

void NSInterface::copyPosition (int& pos, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i]->versionNumber == version)
	{
	    pos = dataList[i]->position;
	    return;
	}
    }
}

void NSInterface::copyReplicaDescriptor (ReplicaDescriptor& rd, unsigned long version, Boolean unknown) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
	        rd = ((unknown == FALSE) ? *(dataList[i]->rd) : *(dataList[i]->rd1));
		return;
	    }
	}
    }
}

void NSInterface::copyBuffer (Buffer& buff, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
		buff = *(dataList[i]->buff);
		return;
	    }
	}
    }
}

void NSInterface::copyObjectState (ObjectState& state, unsigned long version) const
{
    for (int i = 0; i < NSInterface::repLevel; i++)
    {
	if (dataList[i])
	{
	    if (dataList[i]->versionNumber == version)
	    {
		state = *(dataList[i]->os);
		return;
	    }
	}
    }
}

void NSInterface::beginAction ( AtomicAction &Z )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void NSInterface::beginAction ()" << endl;
#endif

    zeroList();
    old_handler = setHandler(NSInterface::rpc_handler);
    Z.Begin();
    responded = 0;
}

Boolean NSInterface::endAction ( AtomicAction &Z, const Boolean dbType,
				 unsigned long& version, LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::endAction ( " << ((dbType == TRUE) ? " GroupViewDatabase " : " NameServer ") << " )" << endl;
#endif

    Boolean result = FALSE;
    int quorum = ((mode == READ) ? NSInterface::readQuorum : NSInterface::writeQuorum);

    setHandler(old_handler);
    version = 0;

    if (responded >= quorum)
    {
	if (isQuorum(dbType, version, mode))
	    result = ((Z.End() == COMMITTED) ? TRUE : FALSE);
	else
	    Z.Abort();
    }
    else
	Z.Abort();

    return result;
}

Boolean NSInterface::testAndSet (ReplicaDescriptor& objects, const Uid& group, const char* nodename, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::testAndSet ( ReplicaDescriptor& objects, " << group
		 << ", " << nodename << ", " << userPid << " )" << endl;
#endif
    
    int res;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = objects;
	    result = NSInterface::NS[i]->testAndSet(*dataList[i]->rd, group, nodename, userPid, res, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (res == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;    // force the action to abort to guarantee consistency
		    responded = 0;
		}
	    }
	}
    }

    result = endAction(A, TRUE, version, WRITE);
    if (result)
        copyReplicaDescriptor(objects, version);

    return result;
}

OutCome NSInterface::addTemplate (const Uid& group, Buffer& toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::addTemplate ( " << group << ", Buffer& toUse )" << endl;
#endif

    OutCome result;
    Boolean lockConflict = FALSE;
    int res;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->addTemplate(group, toUse, NSInterface::nodeName, NSInterface::processID, res, g_versions[i]);
	    
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::deleteTemplate (const Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::deleteTemplate ( " << group << " )" << endl;
#endif

    OutCome result;
    Boolean lockConflict = FALSE;
    int res;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->deleteTemplate(group, NSInterface::nodeName, NSInterface::processID, res, g_versions[i]);
	    
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::replaceTemplate (const Uid& group, Buffer& toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::replaceTemplate ( " << group << ", Buffer& toUse )" << endl;
#endif

    OutCome result;
    Boolean lockConflict = FALSE;
    int res;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->replaceTemplate(group, toUse, NSInterface::nodeName, NSInterface::processID, res, g_versions[i]);
	    
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded =0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

Boolean NSInterface::getTemplate (const Uid& group, Buffer& toUse, LockMode lMode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getTemplate ( " << group << " )" << endl;
#endif

    int res;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;
	    result = NSInterface::NS[i]->getTemplate(group, *dataList[i]->buff, lMode, res, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (res == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }
    
    result = endAction(A,TRUE, version, READ);
    if (result)
	copyBuffer(toUse, version);
    
    return result;
}

OutCome NSInterface::assignCompositeObject (const Uid& groupAssignTo, const ReplicaDescriptor& objects)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::assignCompositeObject ( " << groupAssignTo << ", const ReplicaDescriptor& objects)" << endl;
#endif

    int res;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->assignCompositeObject(groupAssignTo, objects, NSInterface::nodeName,
							       NSInterface::processID, res, g_versions[i]);
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::assignCompositeObject (const Uid& groupAssignTo, const Uid& groupToAssign)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::assignCompositeObject ( " << groupAssignTo << ", " << groupToAssign << " )" << endl;
#endif

    int res;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->assignCompositeObject(groupAssignTo, groupToAssign, NSInterface::nodeName,
							       NSInterface::processID, res, g_versions[i]);
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::removeCompositeObject (const Uid& groupAssignedTo, const ReplicaDescriptor& objects)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;   
    debug_stream << "OutCome NSInterface::removeCompositeObject ( " << groupAssignedTo << ", const ReplicaDescriptor& objects)" << endl;
#endif

    int res;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->removeCompositeObject(groupAssignedTo, objects, NSInterface::nodeName,
							       NSInterface::processID, res, g_versions[i]);
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::removeCompositeObject (const Uid& groupAssignedTo, const Uid& groupAssigned)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;   
    debug_stream << "OutCome NSInterface::removeCompositeObject ( " << groupAssignedTo << ", " << groupAssigned << " )" << endl;
#endif

    int res;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->removeCompositeObject(groupAssignedTo, groupAssigned, NSInterface::nodeName,
							       NSInterface::processID, res, g_versions[i]);
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}	    
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

/*
 * Split a group from the template which is governing it. We only consider
 * available replicas.
 */

OutCome NSInterface::splitTemplateGroup (const Uid& templateUid, const Uid& toSplit)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::splitTemplateGroup ( " << templateUid << ", "
                 << toSplit << " )" << endl;
#endif

    ReplicaDescriptor  rd1;
    ReplicaDescriptor* rd2 = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* rd3 = (ReplicaDescriptor*) 0;
    AtomicAction A;
    OutCome result;

    A.Begin();

    result = ((getView(rd1, templateUid, NSInterface::nodeName, NSInterface::processID) == TRUE) ? DONE : NOTDONE);

    if (result == DONE)
    {
	if (releaseGroup(templateUid, NSInterface::nodeName, NSInterface::processID) != DONE)
	{
	    result = NOTDONE;
	    error_stream << WARNING << "NSInterface::splitTemplateGroup - could not release group "
			 << templateUid << endl;
	}
	else
	{
	    /*
	     * Do not do work if group is not a member of a template group
	     * or is not a member of this template group.
	     */

	    if (rd1.isTemplate() && rd1.templateMember(templateUid))
	    {
		rd1.getGroupList(toSplit, rd3);

		/*
		 * We do nothing if there is not a group to split from the template,
		 * i.e., no replicas have been assigned to it, and it is not
		 * a wild card template.
		 */

		if (rd3)
		{
		    rd1.getAllMembers(toSplit, rd2);
		    if (rd2)
			result = deleteReplicas(*rd2, templateUid);
		
		    if (result == DONE)
			result = addReplicas(*rd3, rd3->getGroupUid());
		}
	    }
	}
    }
		    
    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    if (rd2)
        delete rd2;

    if (rd3)
        delete rd3;

    return result;
}

/*
 * Merge a non-template group with a template group. Only consider
 * available replicas. [Change later to allow template groups to be
 * merged as well.]
 */

OutCome NSInterface::mergeTemplateGroup (const Uid& templateUid, const Uid& toMerge)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::mergeTemplateGroup ( " << templateUid << ", "
                 << toMerge << " )" << endl;
#endif

    ReplicaDescriptor  rd1, rd3;
    ReplicaDescriptor* rd2 = (ReplicaDescriptor*) 0;
    AtomicAction A;
    OutCome result;

    A.Begin();
    result = ((getView(rd1, toMerge, NSInterface::nodeName, NSInterface::processID) == TRUE) ? DONE : NOTDONE);

    if (result == DONE)
    {
	result = releaseGroup(toMerge, NSInterface::nodeName, NSInterface::processID);
	
	if (result == DONE)
	{
	    if (getView(rd3, templateUid, NSInterface::nodeName, NSInterface::processID))
	    {
		if (releaseGroup(templateUid, NSInterface::nodeName, NSInterface::processID) == DONE)
		{
		    /*
		     * Do not do work if group is already member of
		     * template group!
		     */

		    if (rd1 != rd3)
		    {
		        // check that group is a template!

			if (rd3.isTemplate())
			{
			    /* Do not add if template is wild card template. */
			    
			    if (!rd3.isWildCard() && !rd3.isGlobalWildCard())
			    {
				/*
				 * Turn group back in to template. If there are any
				 * duplicate elements, the db will remove these
				 * automatically, so we do not have to. However, we
				 * need to remove any "pure" template replicas, i.e.,
				 * those whose Uid is the same as the group Uid, to
				 * keep the list at the db as small as possible -
				 * these will be recreated when needed.
				 */

			        rd1.getAllMembers(toMerge, rd2, TRUE);

				if (rd2)
				{
				    rd2->setAllHostNames(nullRDName);
				    result = addReplicas(*rd2, templateUid);
				}
				else
				    result = NOTDONE;   // should not happen!
			    }
			    if (result == DONE)
				result = deleteReplicas(*rd2, toMerge, TRUE);
			}
			else
			    result = NOTDONE;
		    }
		    else
			result = DONE;
		}
		else
		    result = NOTDONE;
	    }
	    else
		result = NOTDONE;
	}
    }

    if (result == DONE)
        result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
        A.Abort();

    if (rd2)
        delete rd2;

    return result;
}

Boolean NSInterface::groupAssociation (const Uid& replicaUid, Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::groupAssociation ( " << replicaUid << ", Uid& group )" << endl;
#endif    

    int res;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->groupUid = new Uid(NIL_UID);
	    result = NSInterface::NS[i]->groupAssociation(replicaUid, *dataList[i]->groupUid, res, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (res == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    result = endAction(A,TRUE, version, WRITE);
    if (result)
        copyUid(group, version);
    return result;
}

OutCome NSInterface::alterLocation (const Uid& group, const Uid& replica, const char* location)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::alterLocation ( " << group << ", "
		 << replica << ", " << location << " )" << endl;
#endif
    
    int res;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->alterLocation(group, replica, location, NSInterface::nodeName,
						       NSInterface::processID, res, g_versions[i]);
	    if (res == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}	    
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

long NSInterface::getUseCount (const Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "long NSInterface::getUseCount ( " << group << " )" << endl;
#endif
    
    int res;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    long UseCount = -1;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    result = NSInterface::NS[i]->getUseCount(group, dataList[i]->UseCount, res, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (res == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}		    
	    }
	}
    }

    result = endAction(A,TRUE, version, READ);
    if (result)
        copyUseCount(UseCount, version);
    return UseCount;
}

Boolean NSInterface::lockGroup (const Uid& group, const char* hostName,
				pid_t userPid, LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::lockGroup ( " << group << ", " << hostName
                 << ", " << userPid << ", " << ((mode == WRITE) ? "WRITE": "READ") << " )" << endl;
#endif

    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);

    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->lockGroup(group, hostName, userPid, mode, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}		    
	    }
	}
    }

    return endAction(A,TRUE, version, WRITE);
}


Boolean NSInterface::getView (ReplicaDescriptor& rd, const Uid& u,
			      const char* hostName, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getView (ReplicaDescriptor& rd, " << u << ", "
		 << hostName << ", " << userPid << " )" << endl;
#endif
    
    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);

    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    result = NSInterface::NS[i]->getView(*dataList[i]->rd, u, hostName, userPid, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    result = endAction(A,TRUE, version, WRITE);
    
    if (result)
        copyReplicaDescriptor(rd, version);

    return result;
}

OutCome NSInterface::addReplicas (const ReplicaDescriptor& rd, const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::addReplicas (const ReplicaDescriptor& rd, " << u << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->addReplicas(rd, u, NSInterface::nodeName, NSInterface::processID, ret, g_versions[i]);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::deleteReplicas (const ReplicaDescriptor& rd, const Uid& u, const Boolean& deleteAll)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::deleteReplicas (const ReplicaDescriptor& rd, " << u << ", "
		 << ((deleteAll == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->deleteReplicas(u, rd, NSInterface::nodeName, NSInterface::processID, ret, g_versions[i], deleteAll);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::releaseGroup (const Uid& u, const char* hostName, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::releaseGroup ( " << u << ", " << hostName
		 << ", " << userPid << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);

    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->releaseGroup(u, hostName, userPid, ret, g_versions[i]);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}	    
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

Boolean NSInterface::traverseDatabase (ReplicaDescriptor& rd, Uid& u, const char* node, int& pos, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::traverseDatabase (ReplicaDescriptor& rd, " << u
		 << ", " << node << ", " << pos << ", " << userPid << " )" << endl;
#endif
    
    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    dataList[i]->position = pos;
	    result = NSInterface::NS[i]->traverseDatabase(*dataList[i]->rd, u, dataList[i]->position, node, userPid, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];

	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    result = endAction(A,TRUE, version, WRITE);

    if (result)
    {
	copyReplicaDescriptor(rd, version);
	copyPosition(pos, version);
    }
    
    return result;
}

OutCome NSInterface::include (const ReplicaDescriptor& rd, const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::include (const ReplicaDescriptor& rd, " << u << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->include(rd, u, NSInterface::nodeName, NSInterface::processID, ret, g_versions[i]);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}	    
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::exclude (const ReplicaDescriptor& rd, const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::exclude (const ReplicaDescriptor& rd, " << u << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->exclude(rd, u, NSInterface::nodeName, NSInterface::processID, ret, g_versions[i]);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}


OutCome NSInterface::needsUpdating (const Uid& u, Boolean& isModified, ReplicaDescriptor& rd,
				    const char* nodename, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::needsUpdating ( " << u << ", ReplicaDescriptor& rd, "
		 << nodename << ", " << userPid << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    result = NSInterface::NS[i]->needsUpdating(u, dataList[i]->isModified, *dataList[i]->rd, nodename,
						       userPid, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}	    
	    }
	}
    }

    result = ((endAction(A,TRUE, version, READ) == TRUE) ? DONE : NOTDONE);
    if (result == DONE)
    {
        copyBoolean(isModified, version);
	copyReplicaDescriptor(rd, version);
    }
    return result;
}

OutCome NSInterface::recover (const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::recover ( " << name << " )" << endl;
#endif

    OutCome result;
    Boolean lockConflict = FALSE;
    int ret;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->recover(name, ret, g_versions[i]);
	    if (ret == 0)
	    {
		if (result == DONE)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    return ((endAction(A,TRUE, version, WRITE) == TRUE) ? DONE : NOTDONE);
}

OutCome NSInterface::getNodeUpdateList (ReplicaDescriptor& rd, ReplicaDescriptor& unknownList, const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::getNodeUpdateList ( " << name << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    Boolean lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    dataList[i]->rd1 = new ReplicaDescriptor;
	    *dataList[i]->rd1 = unknownList;
	    result = NSInterface::NS[i]->getNodeUpdateList(*dataList[i]->rd, *dataList[i]->rd1, name, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if ((result == DONE) || (result == INUSE))
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    result = ((endAction(A,TRUE, version, READ) == TRUE) ? DONE : NOTDONE);
    if (result == DONE)
    {	
        copyReplicaDescriptor(rd, version);
	copyReplicaDescriptor(unknownList, version, TRUE);
    }
    return result;
}

void NSInterface::storeName (Boolean& done, Buffer key_buff, Buffer data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::storeName (Boolean& done, Buffer key_buff, Buffer data_buff)" << endl;
#endif
    
    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    done = FALSE;
    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->storeName(result, key_buff, data_buff, ret, s_versions[i]);
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    done = endAction(A, FALSE, version, WRITE);
}

void NSInterface::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)" << endl;
#endif    

    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    done = FALSE;
    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;
	    NSInterface::NS[i]->fetchName(result, key_buff, *dataList[i]->buff, ret, s_versions[i]);
	    dataList[i]->versionNumber = s_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    done = endAction(A, FALSE, version, READ);
    if (done)
        copyBuffer(data_buff, version);
}

void NSInterface::removeName (Boolean& done, Buffer key_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::removeName (Boolean& done, Buffer key_buff)" << endl;
#endif
    
    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    done = FALSE;
    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->removeName(result, key_buff, ret, s_versions[i]);
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    done = endAction(A,FALSE, version, WRITE);
}

void NSInterface::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff)" << endl;
#endif
    
    int ret;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    done = FALSE;
    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->replaceName(result, key_buff, data_buff, ret, s_versions[i]);
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    done = endAction(A,FALSE, version, WRITE);
}

void NSInterface::getNextName (Boolean& done, Buffer& data_buff, Boolean& endofdata)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::getNextName (Boolean& done, Buffer& data_buff, Boolean& endofdata)" << endl;
#endif
    
    int ret, marker;
    Boolean result, lockConflict = FALSE;
    unsigned long version;
    AtomicAction A;

    done = FALSE;
    endofdata = FALSE;
    marker = position;
    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;
	    NSInterface::NS[i]->getNextName(result, *dataList[i]->buff, marker, ret, s_versions[i]);
	    dataList[i]->versionNumber = s_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		{
		    responded++;
		    position = marker;
		}
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    if (position == -1)
	endofdata = TRUE;

    done = endAction(A,FALSE, version, READ);
    if (done)
        copyBuffer(data_buff, version);
}

void NSInterface::restart (Boolean& done)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::restart ( " << ((done == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
#endif
    
    position = 0;
    done = TRUE;
}

Boolean NSInterface::getGroupViewState (ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getGroupViewState ()" << endl;
#endif
    
    Boolean result, lockConflict = FALSE;
    int ret;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->os = new ObjectState;
	    result = NSInterface::NS[i]->getGroupViewState(*dataList[i]->os, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }
	}
    }

    result = endAction(A,TRUE, version, READ);
    if (result)
        copyObjectState(state, version);
    return result;
}

#ifdef DEBUG
Boolean NSInterface::imposeGroupViewState (ObjectState& state)
#else
Boolean NSInterface::imposeGroupViewState (ObjectState&)
#endif
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::imposeGroupViewState (ObjectState&)" << endl;
#endif

    // only allow this for debugging since we should know what we are doing!
    
#ifdef DEBUG
    Boolean result, lockConflict = FALSE;
    int ret;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;	    
	    dataList[i]->os = new ObjectState(state);
	    result = NSInterface::NS[i]->imposeNewGroupViewState(*dataList[i]->os, ret);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	    
	}
    }

    return endAction(A,TRUE, version, WRITE);
#else    
    cerr << "Error. imposeGroupViewState cannot be executed on NSInterface." << endl;
    
    return FALSE;
#endif
}

// get the state of a given replica - to be used to update another

Boolean NSInterface::getReplicaState (const Uid& u, ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getReplicaState ( " << u << " )" << endl;
#endif
    
    Boolean result, lockConflict = FALSE;
    int ret;
    unsigned long version;
    AtomicAction A;

    beginAction(A);
    for (int i = 0; (i < NSInterface::repLevel) && (!lockConflict); i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->os = new ObjectState;
	    result = NSInterface::NS[i]->getReplicaState(u, *dataList[i]->os, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    
	    if (ret == 0)
	    {
		if (result)
		    responded++;
		else
		{
		    lockConflict = TRUE;
		    responded = 0;
		}
	    }	   
	}
    }

    result = endAction(A,TRUE, version, READ);
    if (result)
        copyObjectState(os, version);
    return result;
}

// impose the given state on to an (out-of-date) replica - not implemented yet

Boolean NSInterface::imposeReplicaState (const Uid&, ObjectState&)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::imposeReplicaState (const Uid&, ObjectState&)" << endl;
#endif
    
    return FALSE;
}
