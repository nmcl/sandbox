/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerAAR.h,v 1.5 1994/09/02 14:22:15 ndbi Exp $
 */

#ifndef SERVERAAR_H_
#define SERVERAAR_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef CRASHRECR_H_
#  include <CrashRecR.h>
#endif

/*
 * Enum: Server Atomic Action Status (SAAStatus)
 * Status of a server stomic action record
 */

enum SAAStatus
{
    UNRESOLVED_SAA,
    COMMIT_SAA, ABORT_SAA, IGNORE_SAA,
    COMMITTED_SAA, ABORTED_SAA
};

ostream& operator<< (ostream&, SAAStatus);

/*
 * Class: ServerAARecord
 * Responsible for the recovery of server atomic actions
 */

class ServerAARecord : public CrashRecRecord
{
public:
    ServerAARecord(ostream&, Boolean, const Uid&);
    ServerAARecord(ostream&, Boolean, const Uid&, char*);
    ~ServerAARecord();

    virtual Boolean resolve();
    virtual void print (ostream&);
    virtual void id (ostream&);
    virtual const CRRecordType type() const;
    virtual const Uid& value() const;

private:
    void getRemoteAAStatus(const Uid&);
    void getLocalAAStatus(const Uid&);

    virtual void setThreshold(int);
    virtual int getThreshold();
    virtual void alert();
    virtual void calm();

    // ServerAARecord state
    Uid            serverAAUid;
    char*          coordin_nodename;
    char*          recoveringHost;
    SAAStatus      status;

    static int SAAThreshold;
};

#endif // SERVERAAR_H_
