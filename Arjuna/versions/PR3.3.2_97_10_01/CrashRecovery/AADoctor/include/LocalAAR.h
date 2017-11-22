/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalAAR.h,v 1.5 1994/09/02 14:22:12 ndbi Exp $
 */

#ifndef LOCALAAR_H_
#define LOCALAAR_H_

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
 * Enum: Atomic Action Status (AAStatus)
 * Records status of AA managed by the associated AARecord
 */

enum AAStatus
{
    UNRESOLVED_AA, COMMITTED_AA
};

ostream& operator<< (ostream&, AAStatus);

/*
 * Class: AARecord
 * Responsible for the recovery of local atomic actions
 */

class AARecord : public CrashRecRecord
{
public:
    AARecord(ostream&, Boolean, const Uid&);
    ~AARecord();

    virtual Boolean resolve();
    virtual void print (ostream&);
    virtual void id (ostream&);
    virtual const CRRecordType type() const;
    virtual const Uid& value() const;

private:
    virtual void setThreshold(int);
    virtual int getThreshold();
    virtual void alert();
    virtual void calm();
    
    // AARecord state
    Uid            AAUid;
    AAStatus       status;

    static int AAThreshold;
};

#endif // LOCALAAR_H_







