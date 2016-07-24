/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicaR.h,v 1.5 1994/09/02 14:23:31 ndbi Exp $
 */

#ifndef REPLICAR_H_
#define REPLICAR_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef CRASHRECR_H_
#  include <CrashRecR.h>
#endif

#ifndef REPINTERFACE_H_
#  include <RepInterface.h>
#endif

/*
 * Enum: Replica Status (RepStatus)
 * Status of a particular replica
 */

enum RepStatus
{
    NEEDSUPDATE, MAYNEEDUPDATE
};

ostream& operator<< (ostream&, RepStatus);

/*
 * Class: ReplicaRecord
 * Responsible for the recovery of object replicas
 */

class ReplicaRecord : public CrashRecRecord
{
public:
    ReplicaRecord(ostream&,Boolean, ReplicaDescriptor*, Boolean,RepInterface*);
    ~ReplicaRecord();

    virtual void print (ostream&);
    virtual void id (ostream&);
    virtual Boolean resolve();
    virtual const CRRecordType type() const;
    virtual const Uid& value() const;

private:
    Boolean hideReplica();
    Boolean revealReplica();
    Boolean GVInclude();
    Boolean updateReplica(const ReplicaDescriptor&);
    
    virtual void setThreshold(int);
    virtual int getThreshold();
    virtual void alert();
    virtual void calm();
    
    // ReplicaRecord state
    GroupData repDes;
    RepStatus status;
    Boolean hidden;

    static RepInterface *GV;
    static int NSIRefCount;
    static int repThreshold;
};

#endif // REPLICAR_H_
