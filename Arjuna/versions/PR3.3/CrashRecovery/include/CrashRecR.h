/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecR.h,v 1.8 1995/01/20 10:19:08 ngdp Exp $
 */

#ifndef CRASHRECR_H_
#define CRASHRECR_H_

#ifndef IOSTREAM_H_
# include <System/iostream.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CRRECTYPE_H_
#  include <CRRecType.h>
#endif

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

enum RecoveryStatus
{
    NOT_COMPLETE, COMPLETE
};

ostream& operator<< (ostream&, RecoveryStatus);

/*
 * Class: Crash Recovery Record (CrashRecRecord)
 * Base class of recovery records. The inherited classes are responsible
 * for performing specific recovery operations. These records are managed
 * by the CRRecordList class.
 */

class CrashRecRecord
{
    friend class CRRecordList;

public:
    virtual Boolean resolve() = 0;
    virtual void id(ostream&) = 0;
    virtual const CRRecordType type() const = 0;
    virtual const Uid& value() const = 0;
    virtual void print(ostream&);

    void setRecoveryStatus(RecoveryStatus);
    RecoveryStatus getRecoveryStatus();
    Boolean operator== (const CrashRecRecord *) const;

protected:
    CrashRecRecord(ostream&, Boolean);
    virtual ~CrashRecRecord();

    static Boolean rpc_handler(RPC_Status, Int32);
    Boolean (*old_handler)(RPC_Status, Int32);
    ostream& outStrm;
    Boolean foundAtReboot;
    static Boolean compareHosts(const char*, const char*);

    void checkAlarms();
    void resetAlarms();
    int numberAttempts();
    
private:
    void setNext(CrashRecRecord *);
    void setPrevious(CrashRecRecord *);

    virtual void setThreshold(int) = 0;
    virtual int getThreshold() = 0;
    virtual void alert() = 0;
    virtual void calm() = 0;

    RecoveryStatus recoveryStatus;
    int noAttempts;
    Boolean alerted;
    CrashRecRecord *next;
    CrashRecRecord *previous;
};

#endif /* CRASHRECR_H_ */
