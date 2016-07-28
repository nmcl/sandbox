/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepInterface.h,v 1.5 1995/01/20 10:19:09 ngdp Exp $
 */

#ifndef REPINTERFACE_H_
#define REPINTERFACE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef NAMEINFO_H_
#  include <ArjServers/NameInfo.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

/*
 * Enum: Replication Database Status (RepDatabaseStatus)
 * Options for the number of replication datbase and their location
 */

enum RepDatabaseStatus
{
    UNKNOWN_REP_STATUS, NO_NAMEINFO, SINGLE_LOCAL, ONLY_REMOTE, REPLICATED_HERE,
    TWO_COPIES_ONE_HERE, OVERRIDE
};

/*
 * Class: Replication Interface (RepInterface)
 * Generic interface for accessing the GroupView database
 */

class RepInterface
{
public:
    RepInterface(int&, ostream&);
    ~RepInterface();

    // Setup operations
    Boolean setupInterface();
    Boolean updateState();
    Boolean repRecoveryRequired();

    // Standard NSInterface/NameServer operations
    OutCome recover (const char*);
    Boolean testAndSet (ReplicaDescriptor&, const Uid&, const char*, pid_t);
    OutCome needsUpdating (const Uid&, Boolean&, ReplicaDescriptor&, 
                           const char*, pid_t);
    OutCome getNodeUpdateList (ReplicaDescriptor&, ReplicaDescriptor&, 
                               const char*);
    OutCome include (const ReplicaDescriptor&, const Uid&);
    OutCome releaseGroup (const Uid&, const char*, pid_t);

private:
    unsigned long      getHostInetAddr(char*);
    RepDatabaseStatus  getDBStatus();
    Boolean            checkOverride();
    static Boolean     rpc_handler (RPC_Status, Int32);

    // RepInterface state
    static int               refCount;
    static char              *host;
    static unsigned long     inetAddr;
    static NameInfo          *NI;
    static NSInterface       *NSI;
    static LocalNameServer   *NS;
    static Uid               *nameServerUid;
    static NameData          *replicationInfo;
    static Boolean           setup;
    static RepDatabaseStatus DBStatus;

    Boolean          (*old_handler) (RPC_Status, Int32);
    ostream&         outStrm;
};
#endif // REPINTERFACE_H_
