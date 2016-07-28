/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowR.h,v 1.5 1994/09/02 14:22:17 ndbi Exp $
 */

#ifndef SHADOWR_H_
#define SHADOWR_H_

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
 * Enum: ShadowStatus
 * Records status of a shadow state managed by the associated ShadowRecord
 */

enum ShadowStatus
{
    UNRESOLVED_STATE, COMMITTED_STATE, ABORTED_STATE, IGNORE_STATE
};

ostream& operator<< (ostream&, ShadowStatus);

/*
 * Class: ShadowRecord
 * Responsible for the recovery of uncommitted "shadowed" states.
 * These records are created to identify all of the shadowed states found
 * by crash recovery at reboot time. Some of the states will be resolved
 * by the atomic action recovery that occurs before these records are resolved.
 * However, there will be some states that do not have an associated action
 * object. These are states belonging to actions that crashed after partially
 * completing the prepare phase.
 * Therefore, any states still shadowed after AA recovery is complete should
 * be aborted, which is the purpose of these records.
 */

class ShadowRecord : public CrashRecRecord
{
public:
    ShadowRecord(ostream&, Boolean, const Uid&, const TypeName,
		 const TypeName, const char*);
    ~ShadowRecord();

    virtual Boolean resolve();
    virtual void print (ostream&);
    virtual void id (ostream&);
    virtual const CRRecordType type() const;
    virtual const Uid& value() const;
    virtual const TypeName& getType() const;

    
private:
    virtual void setThreshold(int);
    virtual int getThreshold();
    virtual void alert();
    virtual void calm();
    
    // ShadowRecord state
    TypeName        storeType;
    char            *storeRoot;
    TypeName        objType;
    Uid             objUid;
    ShadowStatus    status;

    static int shadowThreshold;
};

#endif // SHADOWR_H_












