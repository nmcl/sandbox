/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRController.h,v 1.10 1994/08/18 15:24:08 ndbi Exp $
 */

#ifndef CRCONTROLLER_H_
#define CRCONTROLLER_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CRRECLIST_H_
#  include <CRRecList.h>
#endif

#ifndef CONNECTMAN_H_
#  include <ConnectMan.h>
#endif

#ifndef PHASE2SLEEP
#  define PHASE2SLEEP 300
#endif

/*
 * Class: CrashRecoveryController (CRController)
 * Manages and controls the recovery operations performed by the recovery
 * doctors (i.e. AADoctor, RepDoctor...)
 */

class CRController
{
public:
    CRController(ostream& strm);
    ~CRController();
    void recover();

private:
    Boolean makeRecoveryPass();
    void printStatus(ostream&);
    void banner(ostream&, Boolean = FALSE);
    void phaseTwoSleep();

    // CRController state
    static char*        host;
    static int          refCount;
    static ConnectMan*  connection;

    ostream&            outStrm;
    CRRecordList*       workList;
};

// Functions to call in exceptional circumstanes.
Boolean warnAdmin();
Boolean calmAdmin();

#endif // CRCONTROLLER_H_ 
