/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NSInterface.cc,v
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

#ifndef LSPOBJSTORE_H_
#  include <Arjuna/LSPObjStore.h>
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
#  include <Configure.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


const char* filename = "/tmp/NameServer.lock";
char* ndLocation = "/StateManager/LockManager/NameInfo";


NameServer* NSInterface::NS[rep_number] = { 0 };
ClientRpc* NSInterface::RpcClient[rep_number] = { 0 };
RpcControl* NSInterface::RpcCont[rep_number] = { 0 };
int NSInterface::use_count = 0;
int NSInterface::activated = 0;


DataStore::DataStore ()
{
    rd = rd1 = 0;
    buff = os = 0;
    groupUid = 0;
    versionNumber = UseCount = 0;
    isModified = FALSE;
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



NSInterface::NSInterface (Boolean& done)
                                       : X(0),
                                         Z(0),
                                         position(0),
					 Renamed(FALSE),
                                         responded(0),
					 NS_Uid(0),
					 old_handler(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NSInterface::NSInterface (Boolean& done)\n" << flush;
#endif

    // Create new NameServer database.

    done = lockAndCreate();
    initialise();
}

NSInterface::NSInterface (int& res)
                                  : X(0),
                                    Z(0),
				    position(0),
				    Renamed(FALSE),
                                    responded(0),
				    NS_Uid(0),
				    old_handler(0)
  
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NSInterface::NSInterface (int& res)\n" << flush;
#endif

    // Use existing NameServer database(s).

    AtomicAction A;
    unsigned short ret;
    char* path = dirPath();
    char* uidType = uidString();
    Boolean done = FALSE;
    Uid Y(uidType);
    struct stat buf;

    res = -1;
    initialise();

    A.Begin();
    
    if (stat(path, &buf) == -1)
    {
	if (lockAndCreate())
	    res = 0;
    }
    else
    {
	int pathSize = ::strlen(path) + ::strlen(uidType) +2;
	char* pathName = ::new char[pathSize];
	::memset(pathName, '\0', pathSize);
	::strcpy(pathName, path);
	::strcpy(pathName+::strlen(path), "/");
	::strcpy(pathName+::strlen(path)+1, uidType);
	
	if (stat(pathName, &buf) == 0)
	{
	    // database already exists, so no need to create new one

	    Renamed = TRUE;
	    X = new NameInfo(Y, done);

	    if (done)
	    {
		NameData *marker, *ptr = 0;
		done = FALSE;
	    
		marker = X->getReplicationInfo(done, repLevel);
		if (done)
		{
		    quorum = (repLevel/2)+1;
		    ptr = marker;
		    int i = 0;

		    if (NSInterface::use_count == 0)
		    {
			// turn off stub generated abort if call fails.
			
			old_handler = setHandler(NSInterface::rpc_handler);

			while (ptr != 0)
			{
			    ArjName[i].SetObjName("NameServer");
			    ArjName[i].SetServiceName("NameServer");
			    ArjName[i].SetHostName(ptr->hostname);
			
			    NSInterface::RpcClient[i] = new ClientRpc(&ArjName[i]);

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
			    NSInterface::RpcClient[i]->SetTimeout(8000);
			    NSInterface::RpcClient[i]->SetRetry(5);
#else
			    NSInterface::RpcClient[i]->SetTimeout(2000);
#endif
#endif
			    NSInterface::RpcCont[i] = new RpcControl(NSInterface::RpcClient[i]);
			    ret = 1;

			    NSInterface::NS[i] = new NameServer(*ptr->myUID, ret, NSInterface::RpcCont[i]);
			    
			    if (ret == 0)
				NSInterface::activated++;
			    else
			    {
				NSInterface::NS[i] = 0;
				NSInterface::RpcCont[i] = 0;
			    }

			    ptr = ptr->next;
			    i++;
			}

			// turn stub generator abort back on for other servers.
			setHandler(old_handler);
		    }

		    NSInterface::use_count++;
		    if (NSInterface::activated >= quorum)
			res = 0;
		    else
		        error_stream << WARNING << "NSInterface::Insufficient replicas started.\n" << flush;
		}
	    }
	}
	else
	{
	    if (lockAndCreate())
		res = 0;
	}

	if (pathName)
#ifndef __GNUG__
	    ::delete [] pathName;
#else
	    ::delete pathName;
#endif	
    }
    
    if (res == 0)
    {
        if (A.End() != COMMITTED)
	   res = -1;
    }
    else
        A.Abort();

    if (path)
#ifndef __GNU__
	::delete [] path;
#else
        ::delete path;
#endif
    
    if (uidType)
#ifndef __GNU__
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

    if (Z != 0)
    {
        error_stream << WARNING << "NSInterface action still active. Will abort." << endl;
	Z->Abort();
	delete Z;
    }

    if (!Renamed)
	(void) closeAndUnlock();

    NSInterface::use_count--;

    if (NSInterface::use_count == 0)
    {
	for (int i = 0; i < rep_number; i++)
	{
	    if (NSInterface::NS[i])
	    {
		delete NSInterface::NS[i];
		NSInterface::NS[i] = 0;

		if (NSInterface::RpcCont[i])
		{
		    delete NSInterface::RpcCont[i];
		    NSInterface::RpcCont[i] = 0;
		}
		if (NSInterface::RpcClient[i])
		{
		    delete NSInterface::RpcClient[i];
		    NSInterface::RpcClient[i] = 0;
		}
	    }
	}
    }

    if (X)
	delete X;

    remove();

    // make sure stub generator abort is turned back on.

    if (old_handler)
	setHandler(old_handler);
}

void NSInterface::initialise ()
{
    for (int i = 0; i < rep_number; i++)
	dataList[i] = 0;
}

Boolean NSInterface::rpc_handler () { return FALSE; }

void NSInterface::remove ()
{
    for (int i = 0; i < rep_number; i++)
    {
	if (dataList[i])
	{
	    delete dataList[i];
	    dataList[i] = 0;
	}
    }
}

void NSInterface::zeroList ()
{
    remove();
    initialise();
}

Boolean NSInterface::lockAndCreate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::lockAndCreate ()\n" << flush;
#endif

    Boolean status = FALSE;

    if (openAndLock())
    {
	if (createNewDatabase())
	    status = TRUE;
	else
	    error_stream << WARNING << "Could not create new name server info. database" << endl;
    }

    (void) closeAndUnlock();

    return status;
}

Boolean NSInterface::createNewDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::createNewDatabase ()\n" << flush;
#endif

    unsigned short ret = 1;
    Boolean result = FALSE;
    AtomicAction A;

    g_version_number = s_version_number = 0;
    repLevel = 1;
    quorum = 1;
    position = 0;
    X = new NameInfo(result);

    if (result)
    {
	A.Begin();

	if (NSInterface::use_count == 0)
	{
	    old_handler = setHandler(NSInterface::rpc_handler);
	    NSInterface::NS[0] = new NameServer(ret);
	    setHandler(old_handler);
	}

	if (ret == 0)
	{
	    NSInterface::use_count++;

	    NameData* Y = new NameData;
    
	    Y->hostname = ::new char[MAXHOSTNAMELEN];
	    ::memset(Y->hostname, '\0', MAXHOSTNAMELEN);
	    (void) gethostname(Y->hostname, MAXHOSTNAMELEN);
	    Y->myUID = new Uid(NSInterface::NS[0]->get_uid());

	    if (X->setReplicationInfo(Y))
	    {
		if (A.End() != COMMITTED)
		    result = FALSE;
	    }
	    else
		result = FALSE;

	    if (Y)
		delete Y;
	}
	
	if ((ret == 0) && (result))
	{
	    if (A.End() == COMMITTED)
		return TRUE;
	}
	else
	    A.Abort();
    }

    return FALSE;
}

char* NSInterface::uidString ()
{
    unsigned long hostAddr = hostInetAddr();
    char* uidName = ::new char[80];

    ::memset(uidName, '\0', 80);
    sprintf(uidName, "%x:0:0:0", hostAddr);
    return uidName;
}

char* NSInterface::dirPath ()
{
    int len = ::strlen(OBJECTSTOREDIR_Q)+::strlen(ndLocation);
    char* pathName = ::new char[len+1];
    ::memset(pathName, '\0', len+1);
    ::strcpy(pathName, OBJECTSTOREDIR_Q);
    ::strcpy(pathName+::strlen(OBJECTSTOREDIR_Q), ndLocation);
    return pathName;
}

Boolean NSInterface::openAndLock ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::openAndLock ()\n" << flush;
#endif

    fp = ::open(filename, O_WRONLY | O_CREAT | O_EXCL, 0666);

    if (fp < 0)
    {
	error_stream << WARNING << "file " << filename << " exists" << endl;
	return FALSE;
    }
    else
	return TRUE;
}

Boolean NSInterface::closeAndUnlock ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::closeAndUnlock ()\n" << flush;
#endif

    Boolean result = FALSE;
    LocalSimplePersistentObjectStore LP;
    char* oldName = 0;
    char* pathName = dirPath();
    char* uidType = uidString();
    char* newName = ::new char[::strlen(pathName) + ::strlen(uidType) + 2];
    
    if (Renamed)
	result = TRUE;
    else
    {
	if ((!X) || (AtomicAction::Current() != 0))
	    result = FALSE;
	else
	{
	    if (!NS_Uid)
		NS_Uid = new Uid(X->get_uid());

	    ::memset(newName, '\0', ::strlen(pathName) + ::strlen(uidType) + 2);
	    ::strcpy(newName, pathName);
	    ::strcpy(newName+::strlen(pathName), "/");
	    ::strcpy(newName+::strlen(pathName)+1, uidType);

	    oldName = LP.genPathName(*NS_Uid, ndLocation, ORIGINAL);
	    
	    if (::strcmp(oldName, uidType) != 0)
	    {
		if (::rename(oldName, newName) == 0)
		    result = TRUE;
		else
		    error_stream << WARNING << "NSInterface - error " << errno
				 << " occurred while attempting to rename object state." << endl;

		::close(fp);
		::unlink(filename);

		if (result)
		    Renamed = TRUE;
	    }
	    else
	    {
		Renamed = TRUE;
		result = TRUE;
	    }

	    if (!Renamed)
		error_stream << WARNING << "NSInterface - Manual rename required - "
			     << oldName << " to " << newName << endl;
	}
    }
    
    if (newName)
#ifndef __GNUG__
	::delete [] newName;
#else
        ::delete newName;
#endif
    
    if (oldName)
#ifndef __GNUG__
	::delete [] oldName;
#else
        ::delete oldName;
#endif

    if (uidType)
#ifndef __GNUG__
	::delete [] uidType;
#else
        ::delete uidType;
#endif

    if (pathName)
#ifndef __GNUG__
	::delete [] pathName;
#else
        ::delete pathName;
#endif

    return result;
}

unsigned long NSInterface::hostInetAddr ()
{
    static unsigned long myAddr;
    static char hostname[256];
    struct hostent *h;
    ::gethostname(hostname, 256);
    h = ::gethostbyname(hostname);
    ::memcpy(&myAddr, h->h_addr, h->h_length);
    return myAddr;
}

Boolean NSInterface::isQuorum (Boolean groupview, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::isQuorum (Boolean groupview, unsigned long& version)" << endl;
#endif

    unsigned short is_same;

    if (quorum == 1)
	return TRUE;

    if (groupview)
    {
	for (int i = 0; i < repLevel; i++)
	{
	    if (g_version_number < g_versions[i])
	    {
		is_same = 0;
		g_version_number = g_versions[i];
		version = g_version_number;
	    }

	    if (g_versions[i] == g_version_number)
		is_same++;
	}
    }
    else
    {
	for (int j = 0; j < repLevel; j++)
	{
	    if (s_version_number < s_versions[j])
	    {
		is_same = 0;
		s_version_number = s_versions[j];
		version = s_version_number;
	    }

	    if (s_versions[j] == s_version_number)
		is_same++;
	}
    }

    if (is_same >= quorum)
	return TRUE;
    else
	return FALSE;
}

void NSInterface::copyUid (Uid& group, unsigned long version) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		group = *dataList[i]->groupUid;
		return;
	    }
	}
    }
}

void NSInterface::copyUseCount (long& UseCount, unsigned long version) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		UseCount = dataList[i]->UseCount;
		return;
	    }
	}
    }
}

void NSInterface::copyBoolean (Boolean& isModified,
			       unsigned long version) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		isModified = dataList[i]->isModified;
		return;
	    }
	}
    }
}
    
void NSInterface::copyReplicaDescriptor (ReplicaDescriptor& rd, unsigned long version, Boolean unknown) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		if (!unknown)
		    rd = *dataList[i]->rd;
		else
		    rd = *dataList[i]->rd1;
		return;
	    }
	}
    }
}

void NSInterface::copyBuffer (Buffer& buff, unsigned long version) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		buff = *dataList[i]->buff;
		return;
	    }
	}
    }
}

void NSInterface::copyObjectState (ObjectState& os, unsigned long version) const
{
    for (int i = 0; i < repLevel; i++)
    {
	if (dataList[i])
	{
	    if ((dataList[i]->versionNumber == version) || (quorum == 1))
	    {
		os = *dataList[i]->os;
		return;
	    }
	}
    }
}

void NSInterface::beginAction ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void NSInterface::beginAction ()" << endl;
#endif

    if (Z)
    {
        error_stream << WARNING << "NSInterface action still active. Will abort." << endl;
	Z->Abort();
    }
    zeroList();
    old_handler = setHandler(NSInterface::rpc_handler);
    Z = new AtomicAction;
    Z->Begin();
    responded = 0;
}

Boolean NSInterface::endAction (const Boolean dbType, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean NSInterface::endAction ( " << ((dbType == TRUE) ? " GroupViewDatabase " : " NameServer ") << " )" << endl;
#endif

    Boolean result = FALSE;

    setHandler(old_handler);
    version = 0;

    if (Z == 0)
        error_stream << WARNING << "NSInterface - attempt to end non-running action." << endl;
    else
    {
        if (responded >= quorum)
	{
	    if (isQuorum(dbType, version))
	        result = ((Z->End() == COMMITTED) ? TRUE : FALSE);
	    else
		Z->Abort();
	}
	else
	    Z->Abort();

	delete Z;
	Z = 0;
    }

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
    Boolean result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = objects;

	    result = NSInterface::NS[i]->testAndSet(*dataList[i]->rd, group, nodename, userPid, res, g_versions[i]);
	    if (res == 0)
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
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
    int res;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->addTemplate(group, toUse, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::deleteTemplate (const Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::deleteTemplate ( " << group << " )" << endl;
#endif

    OutCome result;
    int res;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->deleteTemplate(group, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::replaceTemplate (const Uid& group, Buffer& toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::replaceTemplate ( " << group << ", Buffer& toUse )" << endl;
#endif

    OutCome result;
    int res;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->replaceTemplate(group, toUse, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

Boolean NSInterface::getTemplate (const Uid& group, Buffer& toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getTemplate (const Uid& group, Buffer& toUse)" << endl;
#endif

    int res;
    Boolean result;
    unsigned long version;
    
    beginAction();
    
    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;
	    
	    result = NSInterface::NS[i]->getTemplate(group, *dataList[i]->buff, res, g_versions[i]);
	    if (result)
		if (res == 0) responded++;
	}
    }
    
    result = endAction(TRUE, version);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->assignCompositeObject(groupAssignTo, objects, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::assignCompositeObject (const Uid& groupAssignTo, const Uid& groupToAssign)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::assignCompositeObject ( " << groupAssignTo << ", " << groupToAssign << " )" << endl;
#endif

    int res;
    OutCome result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;

	    result = NSInterface::NS[i]->assignCompositeObject(groupAssignTo, groupToAssign, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::removeCompositeObject (const Uid& groupAssignedTo, const ReplicaDescriptor& objects)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;   
    debug_stream << "OutCome NSInterface::removeCompositeObject ( " << groupAssignedTo << ", const ReplicaDescriptor& objects)" << endl;
#endif

    int res;
    OutCome result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->removeCompositeObject(groupAssignedTo, objects, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::removeCompositeObject (const Uid& groupAssignedTo, const Uid& groupAssigned)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;   
    debug_stream << "OutCome NSInterface::removeCompositeObject ( " << groupAssignedTo << ", " << groupAssigned << " )" << endl;
#endif

    int res;
    OutCome result;
    unsigned long version;

    beginAction();
    
    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;

	    result = NSInterface::NS[i]->removeCompositeObject(groupAssignedTo, groupAssigned, res, g_versions[i]);
	    
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

Boolean NSInterface::getExclusiveAccess ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getExclusiveAccess ()" << endl;
#endif

    int res;
    Boolean result;
    unsigned long version;
    
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;

	    result = NSInterface::NS[i]->getExclusiveAccess(res);
	    if (res == 0)
		if (result) responded++;
	}
    }

    return endAction(TRUE, version);
}

Boolean NSInterface::groupAssociation (const Uid& replicaUid, Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::groupAssociation ( " << replicaUid << ", Uid& group )" << endl;
#endif    

    int res;
    Boolean result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
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
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;

	    result = NSInterface::NS[i]->alterLocation(group, replica, location, res, g_versions[i]);
	    if (res == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

long NSInterface::getUseCount (const Uid& group)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "long NSInterface::getUseCount ( " << group << " )" << endl;
#endif
    
    int res;
    Boolean result;
    unsigned long version;
    long UseCount = -1;

    beginAction();
    
    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    res = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    
	    result = NSInterface::NS[i]->getUseCount(group, dataList[i]->UseCount, res, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    if (res == 0)
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
    if (result)
        copyUseCount(UseCount, version);
    return UseCount;
}

Boolean NSInterface::getView (ReplicaDescriptor& rd, const Uid& u, const char* hostName,
			      pid_t userPid, const LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getView (ReplicaDescriptor& rd, " << u << ", "
		 << hostName << ", " << userPid << ", "
		 << ((mode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif
    
    int ret;
    Boolean result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    
	    result = NSInterface::NS[i]->getView(*dataList[i]->rd, u, hostName, userPid, ret, g_versions[i], mode);
	    dataList[i]->versionNumber = g_versions[i];
	    if (ret == 0)
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->addReplicas(rd, u, ret, g_versions[i]);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->deleteReplicas(u, rd, ret, g_versions[i], deleteAll);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->releaseGroup(u, hostName, userPid, ret, g_versions[i]);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

Boolean NSInterface::traverseDatabase (ReplicaDescriptor& rd, Uid& u, const char* nodeName, int& pos, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::traverseDatabase (ReplicaDescriptor& rd, " << u
		 << ", " << nodeName << ", " << pos << ", " << userPid << " )" << endl;
#endif
    
    int ret;
    Boolean result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->rd = new ReplicaDescriptor;
	    *dataList[i]->rd = rd;
	    
	    result = NSInterface::NS[i]->traverseDatabase(*dataList[i]->rd, u, pos, nodeName, userPid, ret, g_versions[i]);
	    dataList[i]->versionNumber = g_versions[i];
	    if (ret == 0)
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
    if (result)
        copyReplicaDescriptor(rd, version);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->include(rd, u, ret, g_versions[i]);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::exclude (const ReplicaDescriptor& rd, const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::exclude (const ReplicaDescriptor& rd, " << u << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;
	    result = NSInterface::NS[i]->exclude(rd, u, ret, g_versions[i]);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
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
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
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
		if (result == Done) responded++;
	}
    }

    result = ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
    if (result == Done)
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
    int ret;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    g_versions[i] = 0;

	    result = NSInterface::NS[i]->recover(name, ret, g_versions[i]);
	    if (ret == 0)
		if (result == Done) responded++;
	}
    }

    return ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
}

OutCome NSInterface::getNodeUpdateList (ReplicaDescriptor& rd, ReplicaDescriptor& unknownList, const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NSInterface::getNodeUpdateList ( ReplicaDescriptor& rd, ReplicaDescriptor& unknownList, "
		 << name << " )" << endl;
#endif
    
    int ret;
    OutCome result;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
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
		if (result == Done) responded++;
	}
    }

    result = ((endAction(TRUE, version) == TRUE) ? Done : NotDone);
    if (result == Done)
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
    Boolean result;
    unsigned long version;

    done = FALSE;
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->storeName(result, key_buff, data_buff, s_versions[i]);
	    if (result) responded++;
	}
    }

    done = endAction(FALSE, version);
}

void NSInterface::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff)" << endl;
#endif    

    int ret;
    Boolean result;
    unsigned long version;

    done = FALSE;
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;

	    NSInterface::NS[i]->fetchName(result, key_buff, *dataList[i]->buff, s_versions[i]);
	    dataList[i]->versionNumber = s_versions[i];
	    if (result) responded++;
	}
    }

    done = endAction(FALSE, version);
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
    Boolean result;
    unsigned long version;

    done = FALSE;
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->removeName(result, key_buff, s_versions[i]);
	    if (result) responded++;
	}
    }

    done = endAction(FALSE, version);
}

void NSInterface::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff)" << endl;
#endif
    
    int ret;
    Boolean result;
    unsigned long version;

    done = FALSE;
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    NSInterface::NS[i]->replaceName(result, key_buff, data_buff, s_versions[i]);
	    if (result) responded++;
	}
    }

    done = endAction(FALSE, version);
}

void NSInterface::getNextName (Boolean& done, Buffer& data_buff, Boolean& endofdata)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NSInterface::getNextName (Boolean& done, Buffer& data_buff, Boolean& endofdata)" << endl;
#endif
    
    int ret, marker;
    Boolean result;
    unsigned long version;

    done = FALSE;
    endofdata = FALSE;
    marker = position;
    beginAction();

    for (int i = 0; i < repLevel; i++)
    {
	if (NSInterface::NS[i])
	{
	    ret = -1;
	    result = FALSE;
	    s_versions[i] = 0;
	    dataList[i] = new DataStore;
	    dataList[i]->buff = new Buffer;

	    NSInterface::NS[i]->getNextName(result, *dataList[i]->buff, marker, s_versions[i]);
	    dataList[i]->versionNumber = s_versions[i];
	    if (result)
	    {
		responded++;
		position = marker;
	    }
	}
    }

    if (position == -1)
	endofdata = TRUE;

    done = endAction(FALSE, version);
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

Boolean NSInterface::getGroupViewState (ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::getGroupViewState (ObjectState& os)" << endl;
#endif
    
    Boolean result;
    int ret;
    unsigned long version;

    beginAction();

    for (int i = 0; i < repLevel; i++)
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
		if (result) responded++;
	}
    }

    result = endAction(TRUE, version);
    if (result)
        copyObjectState(os, version);
    return result;
}

Boolean NSInterface::imposeGroupViewState (ObjectState&)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NSInterface::imposeGroupViewState (ObjectState&)" << endl;
#endif
    
    return FALSE;
}
