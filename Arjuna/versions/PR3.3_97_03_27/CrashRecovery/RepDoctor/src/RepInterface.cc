/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepInterface.cc,v 1.6 1995/01/20 10:19:04 ngdp Exp $
 */

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
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

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef REPINTERFACE_H_
#  include <RepInterface.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#include <ArjServers/NameServ_stub.h>

/*
 * Declare statics
 */

int RepInterface::refCount = 0;
char* RepInterface::host = 0;
unsigned long RepInterface::inetAddr = 0;
NameInfo* RepInterface::NI = 0;
NSInterface* RepInterface::NSI = 0;
LocalNameServer* RepInterface::NS = 0;
Uid* RepInterface::nameServerUid = 0;
NameData* RepInterface::replicationInfo = 0;
Boolean RepInterface::setup = FALSE;
RepDatabaseStatus RepInterface::DBStatus = UNKNOWN_REP_STATUS;

/*
 * Enum: Replication Database Status (RepDatabaseStatus)
 * Options for the number of replication datbase and their location
 */

ostream& operator<< (ostream& strm, RepDatabaseStatus s)
{
    switch (s)
    {
        case NO_NAMEINFO:
            strm << "no nameinfo database";
            break;
        case SINGLE_LOCAL:
            strm << "single local database";
            break;
        case ONLY_REMOTE:
            strm << "all database copies are remote";
            break;
        case REPLICATED_HERE:
            strm << "replicated database with copy here";
            break;
        case TWO_COPIES_ONE_HERE:
            strm << "two databases with one copy here";
            break;
        case OVERRIDE:
            strm << "OVERRIDING: using local copy";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Class: Replication Interface (RepInterface)
 * Generic interface for accessing the GroupView database
 */

//-Public-Functions------------------------------------------------------------

RepInterface::RepInterface (int& res, ostream& strm)
                           : outStrm(strm)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepInterface::RepInterface()" << endl;
#endif

    if (!host)
    {
        host = new char[MAXHOSTNAMELEN];
        memset(host, '\0', MAXHOSTNAMELEN);
        gethostname(host, MAXHOSTNAMELEN);
        inetAddr = getHostInetAddr(host);
    }

    if ( setupInterface() )
        res = 0;

    // Increment reference count
    refCount++;

}

RepInterface::~RepInterface()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepInterface::~RepInterface()" << endl;
#endif

    if (--refCount == 0)
    {
        if (NI)
            delete NI;

        if (NSI)
            delete NSI;

        if (host)
            delete host;

        if (NS)
            delete NS;

        if (replicationInfo)
            delete replicationInfo;
    }
}

Boolean RepInterface::setupInterface()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepInterface::setupInterface()" << endl;
#endif

    if (setup)
        return TRUE;

    DBStatus = getDBStatus();
    switch ( DBStatus )
    {
        case NO_NAMEINFO:
        {
            /*
             * There is no replication database registered, therefore we have
             * no work to do for replication recovery
             */

            outStrm << "RepInterface: No database info registered" << endl;
            setup = TRUE;
        }
        break;

        case SINGLE_LOCAL:
        case TWO_COPIES_ONE_HERE:
        case OVERRIDE:
        {
            /*
             * Either there is only one copy of the replication database and it
             * is located on this machine or there are 2 copies and 1 is on this
             * machine.
             * In the 2nd case, no operations could have been performed on
             * the database since NSInterface could not provide quorum. The
             * local copy is therefore consistent.
             * This is also used if the administrator has elected to override
             * the standard recovery procedure; typically used if all of the
             * hosts having copies of the database have crashed.
             * Create a local instance of NameServer to use for recovery
             */

             outStrm << "RepInterface: Creating local name server..." << endl;

             int result = -1;
             NS = new LocalNameServer(*nameServerUid, result);

             if (result != 0)
             {
                 outStrm << "RepInterface: *WARNING* "
                         << "Failed to create local name server" << endl;
                 if (NS)
                     delete NS;
                 NS = 0;
             }
             else
             {
                 outStrm << "RepInterface: Constructed name server successfully"
                         << endl;
                 setup = TRUE;
             }
        }
        break;
        
        case ONLY_REMOTE:
        case REPLICATED_HERE:
        {
            /*
             * ONLY_REMOTE     - Every copy of the replication database is 
             *                   remote from us.
             *
             * REPLICATED_HERE - The replication database is replicated and a 
             *                   copy is resident on the recovering node.
             *
             * For both instances create an instance of the NSInterface.
             */

            int result;

            outStrm << "RepInterface: Creating NSInterface..." << endl;

            NSI = new NSInterface(result);
            if (result != 0)
            {
                outStrm << "RepInterface: *WARNING* "
                        << "Failed to create NSInterface" << endl;
                if (NSI)
                    delete NSI;
                NSI = 0;
            }
            else
            {
                outStrm << "RepInterface: Constructed NSInterface successfully"
                     << endl;
                setup = TRUE;
            }
        }
        break;

        default:
            outStrm << "RepInterface: *WARNING* Failed to setup "
                    << "replication interface" << endl;
        break;
    }

    return setup;
}

Boolean RepInterface::updateState()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepInterface::updateState()" << endl;
#endif

    Boolean success = FALSE;

    if (setupInterface())
    {
        /*
         * If necessary update the state of a GroupView database
         */

        switch(DBStatus)
        {
            case REPLICATED_HERE:
            {
                // Update local NameServer from NSInterface

                outStrm << "RepInterface: About to update the local "
                        << "replication database..." << endl;

                ObjectState newState;

                if ( NSI && (NSI->getGroupViewState(newState)) )
                {
                    int result = -1;
                    LocalNameServer* lNS = new LocalNameServer(*nameServerUid,
                                                               result);

                    if (result != 0)
                    {
                        outStrm << "RepInterface: *WARNING* Failed to create "
                                << "local name server" << endl;

                        if (lNS)
                            delete lNS;
                        lNS = 0;
                    }
                    else
                    {
                        int res = -1;
                        lNS->imposeNewGroupViewState(newState, res);
                        if (res != 0)
                        {
                            outStrm << "RepInterface: *WARNING* Failed to "
                                    << "impose state on local name server"
                                    << endl;
                        }
                        else
                        {
                            outStrm << "RepInterface: Updated local database "
                                    << "successfully" <<  endl;
                            success = TRUE;
                        }
                    }
                }
                else
                {
                    outStrm << "RepInterface: *WARNING* Could not get state "
                            << "of replication database" << endl;
                }
            }
            break;

            case TWO_COPIES_ONE_HERE:
            {
                // Update remote NameServer from local NameServer

                NameData* ptr = replicationInfo;
                int repDBCount = 0;
                Uid* rNSUid;
                char* rNSHost;

                while (ptr != 0)
                {
                    repDBCount++;

                    if ( getHostInetAddr(ptr->hostname) != inetAddr )
                    {
                        rNSUid = new Uid(*ptr->myUID);
                        rNSHost = ::new char[::strlen( ptr->hostname) +1];
                        ::memset(rNSHost, '\0', ::strlen( ptr->hostname)+1);
                        ::memcpy(rNSHost, ptr->hostname,
                                 ::strlen(ptr->hostname));
                    }
		    ptr = ptr->next;
	        }

                if (repDBCount == 2)
                {
                    ClientRpc* RpcClientNS = new ClientRpc("NameServer");
                    RpcClientNS->setHost(rNSHost);
                    old_handler = setHandler(rpc_handler);
                    ClientRpcManager* RpcConNS 
                                            = new ClientRpcManager(RpcClientNS);
                    int unsRes = -1;
                    NameServer* remNS = new NameServer(*rNSUid, unsRes,
                                                       RpcConNS);
                    if (unsRes == 0)
                    {
                        ObjectState newState;
                        unsigned long dummyVersion = 0;
                        int intRes = 0;
                        if ( NS && (NS->getGroupViewState(newState, intRes,
                                                          dummyVersion)) )
                        {
                            remNS->imposeNewGroupViewState(newState, intRes);

                            if (intRes != 0)
                            {
                                outStrm << "RepInterface: *WARNING* "
                                        << "Failed to impose state"
                                        << "on remote name server" << endl;
                            }
                            else
                            {
                                outStrm << "RepInterface: Updated remote "
                                        << "database successfully" <<  endl;
                                success = TRUE;
                            }
                        }
                        else
                        {

                            outStrm << "RepInterface: *WARNING* Could not get "
                                    << "state of replication database" << endl;
                        }
                    }
                    else
                    {
                        outStrm << "RepInterface: *WARNING* Failed to start "
                                << "NameServer on " << "host " << rNSHost  
                                << endl;
                    }
                    if (remNS)
                        delete remNS;
                    if (RpcConNS)
                        delete RpcConNS;
                    if (RpcClientNS)
                        delete RpcClientNS;
                    setHandler(old_handler);

                }
                else
                {
                    // NameInfo database has changed
                    outStrm << "RepInterface: *WARNING* NameInfo DB has changed"
                            << endl;
                }

                if (!success)
                {
                    outStrm << "RepInterface: Could not update remote "
                            << "NameServer.\nThis will be made consistent when "
                            << rNSHost << " recovers." << endl;

                    success = TRUE;
                }
            }
            break;

            default:
            {
                success = TRUE;
            }
            break;
        }
    }
    return success;
}

Boolean RepInterface::repRecoveryRequired()
{
  return ((DBStatus == NO_NAMEINFO) ? FALSE : TRUE);
}

//-Public-GroupView-Functions--------------------------------------------------

OutCome RepInterface::recover (const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "OutCome RepInterface::recover ( " << name << " )" << endl;
#endif

    int res = -1;
    OutCome result = NOTDONE;
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->recover(name, res, dummyVersion);
        else if (NSI)
            result = NSI->recover(name);
    }
    
    return result;
}

Boolean RepInterface::testAndSet(ReplicaDescriptor& objects, const Uid& group,
                                 const char* nodename, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "Boolean RepInterface::testAndSet"
                 << "(ReplicaDescriptor& objects, " << group
		 << ", " << nodename << ", " << userPid << " )" << endl;
#endif

    int res = -1;
    Boolean result = FALSE;
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->testAndSet(objects, group, nodename, userPid, res,
                                    dummyVersion);
        else if (NSI)
            result = NSI->testAndSet(objects, group, nodename, userPid);
    }
    
    return result;
}


OutCome RepInterface::needsUpdating(const Uid& u, Boolean& isModified,
                                    ReplicaDescriptor& rd,
				    const char* nodename, pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "OutCome RepInterface::needsUpdating ( " << u 
                 << ", ReplicaDescriptor& rd, "
		 << nodename << ", " << userPid << " )" << endl;
#endif

    int res = -1;
    OutCome result = NOTDONE;
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->needsUpdating(u, isModified, rd, nodename, userPid,
                                       res, dummyVersion);
        else if (NSI)
            result = NSI->needsUpdating(u, isModified, rd, nodename, userPid);
    }
    
    return result;
}

OutCome RepInterface::getNodeUpdateList (ReplicaDescriptor& rd,
                                         ReplicaDescriptor& unknownList,
                                         const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "OutCome RepInterface::getNodeUpdateList("
                 << "ReplicaDescriptor& rd, ReplicaDescriptor& unknownList, "
		 << name << " )" << endl;
#endif

    int res = -1;
    OutCome result = NOTDONE;
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->getNodeUpdateList(rd, unknownList, name, res,
                                           dummyVersion);
        else if (NSI)
            result = NSI->getNodeUpdateList(rd, unknownList, name);
    }
    
    return result;
}


OutCome RepInterface::include (const ReplicaDescriptor& rd, const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "OutCome RepInterface::include(const ReplicaDescriptor& rd,"
                 << u << " )" << endl;
#endif

    int res = -1;
    OutCome result = NOTDONE;
    pid_t myPid = ::getpid();
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->include(rd, u, host, myPid, res, dummyVersion);
        else if (NSI)
            result = NSI->include(rd, u);
    }
    
    return result;
}


OutCome RepInterface::releaseGroup (const Uid& u, const char* hostName,
                                    pid_t userPid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "OutCome RepInterface::releaseGroup ( " << u 
                 << ", " << hostName << ", " << userPid << " )" << endl;
#endif

    int res = -1;
    OutCome result = NOTDONE;
    unsigned long dummyVersion = 0;

    if (setupInterface())
    {
        if (NS)
            result = NS->releaseGroup(u, hostName, userPid, res, dummyVersion);
        else if (NSI)
            result = NSI->releaseGroup(u, hostName, userPid);
    }
    
    return result;
}



//-Private-Functions-----------------------------------------------------------

unsigned long RepInterface::getHostInetAddr(char* checkHost)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepInterface::getHostInetAddr(" << checkHost << ")" << endl;
#endif

    static unsigned long theAddr;
    struct hostent* h;

    if (checkHost != 0)
    {
        h = gethostbyname(checkHost);
        memcpy(&theAddr, h->h_addr, h->h_length);
    }
    else
        theAddr = 0;

    return theAddr;
}

RepDatabaseStatus RepInterface::getDBStatus()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepInterface::getDBStatus()" << endl;
#endif

    RepDatabaseStatus DBStat = UNKNOWN_REP_STATUS;
    Boolean res = FALSE;
    Boolean localCopy = FALSE;


    char* s_uid = ::new char[80];
    memset(s_uid, '\0', 80);
    sprintf(s_uid, "%lx:0:0:0", inetAddr);
    Uid Y(s_uid);

    NameInfo* X = new NameInfo(Y, res);

    if (!res)
    {
	outStrm << "RepInterface: Could not activate NameInfo object, "
                << "assuming no replication" << endl;
	if (X)
	    delete X;
        DBStat = NO_NAMEINFO;
    }
    else
    {
        int noOfDBs = 0;
        replicationInfo = X->getReplicationInfo (res, noOfDBs);
        if (!res)
        {
            outStrm << "RepInterface: *WARNING* Error getting replication info"
                    << endl;
            DBStat = UNKNOWN_REP_STATUS;
            if (replicationInfo)
                delete replicationInfo;
            replicationInfo = 0;
        }
        else
        {
            if (noOfDBs == 0)
            {
                DBStat = NO_NAMEINFO;
            }
            else
            {
                NameData* ptr = replicationInfo;
                while (ptr != 0)
                {
                    if ( getHostInetAddr(ptr->hostname) == inetAddr )
                    {
                        localCopy = TRUE;
                        nameServerUid = ptr->myUID;
                    }
		    ptr = ptr->next;
	        }

                if (!localCopy)
                {
                    DBStat = ONLY_REMOTE;
                }
                else
                {
                    switch(noOfDBs)
                    {
                        case 1:
                            DBStat = SINGLE_LOCAL;
                            break;
                        case 2:
                            DBStat = TWO_COPIES_ONE_HERE;
                            break;
                        default:
                            DBStat = REPLICATED_HERE;
                            break;
                    }
               }
            }
        }
    }        

    if ( localCopy && (checkOverride() == TRUE) )
    {
        outStrm << "RepInterface: Overriding replicated interface to "
                << "replication database, using local copy" << endl;
        DBStat = OVERRIDE;
    }

    return DBStat;
}

Boolean RepInterface::checkOverride()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepInterface::checkOverride()" << endl;
#endif

    char *env_value;
    env_value = ::getenv("GROUPVIEW_RECOVERY_OVERRIDE");
    return ( (env_value == NULL) ? FALSE : TRUE );
}

Boolean RepInterface::rpc_handler ( RPC_Status, Int32 )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepInterface::rpc_handler()\n" << flush;
#endif

    /*
     * Prevent core dumps on RPC failures
     */

    return FALSE;
}
