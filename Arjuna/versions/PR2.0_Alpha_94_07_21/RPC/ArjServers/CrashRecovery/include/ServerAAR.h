/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerAAR.h,v 1.4 1993/10/06 13:44:31 ndbi Exp $
 */

#ifndef SERVERAAR_H_
#define SERVERAAR_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

/*
 *
 * Server Atomic Action Record Class
 *
 */

#ifndef CRASHRECR_H_
#  include "CrashRecR.h"
#endif

enum SAAStatus
{
    UNRESOLVED_SAA, COMMIT_SAA, ABORT_SAA, COMMITTED_SAA, ABORTED_SAA
};

class ServerAARecord : public CrashRecRecord
{
public:
    /* Constructor and destructor */

    ServerAARecord(const Uid& actUid);
    ~ServerAARecord();

    ostream& print (ostream& strm) const;
    ostream& id (ostream& strm) const;
    Boolean resolve();

private:
    Uid            serverAAUid;
    char*          coordin_nodename;
    SAAStatus      status;
};

#endif // SERVERAAR_H_
