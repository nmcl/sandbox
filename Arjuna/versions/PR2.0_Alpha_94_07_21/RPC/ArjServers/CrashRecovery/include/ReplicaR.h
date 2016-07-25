/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicaR.h,v 1.7 1993/09/08 14:52:32 nmcl Exp $
 */

#ifndef REPLICAR_H_
#define REPLICAR_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

/*
 *
 * Replica Record Class
 *
 */

#ifndef CRASHRECR_H_
#  include "CrashRecR.h"
#endif

#ifndef NSINTERFACE_H_
#  include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <RPC/ArjServers/GroupData.h>
#endif

enum RepStatus
{
    NEEDSUPDATE, MAYNEEDUPDATE
};

class ReplicaRecord : public CrashRecRecord
{
public:
    /* Constructor and destructor */

    ReplicaRecord(ReplicaDescriptor*, Boolean, NSInterface*);
    ~ReplicaRecord();

    ostream& print (ostream&) const;
    ostream& id (ostream&) const;
    Boolean resolve();

    Boolean updateReplica(const ReplicaDescriptor &);
    Boolean hideReplica();
    Boolean revealReplica();
    Boolean removeHiddenCopy();
    Boolean GVInclude();


private:
    GroupData repDes;
    RepStatus status;
    Boolean hidden;
    Uid hiddenUid;

    static NSInterface *GV;
    static int NSIRefCount;
};

#endif // REPLICAR_H_
