/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecR.h,v 1.1 1993/11/03 14:23:58 nmcl Exp $
 */

#ifndef CRASHRECR_H_
#define CRASHRECR_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

/*
 *
 * Crash Recovery Record Class
 *
 */

enum RecoveryStatus
{
    NOT_COMPLETE, COMPLETE
};

class CrashRecRecord
{
    friend class CRRecordList;

public:
    virtual ostream& print (ostream & strm) const;
    virtual ostream& id (ostream & strm) const = 0;
    virtual Boolean resolve();

    void setRecoveryStatus(RecoveryStatus newStatus);
    RecoveryStatus getRecoveryStatus();

protected:
    /* Constructor and destructor */

    CrashRecRecord();
    virtual ~CrashRecRecord();

protected:
    static Boolean rpc_handler ();
    Boolean (*old_handler) ();

private:
    RecoveryStatus recoveryStatus;
    CrashRecRecord *next;
    CrashRecRecord *previous;
};

#endif // CRASHRECR_H_
