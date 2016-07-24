/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRController.h,v 1.1 1993/11/03 14:23:55 nmcl Exp $
 */

#ifndef CRCONTROLLER_H_
#define CRCONTROLLER_H_

#ifndef SERVERAAR_H_
#  include "ServerAAR.h"
#endif

#ifndef REPLICAR_H_
#  include "ReplicaR.h"
#endif

#ifndef LOCALAAR_H_
#  include "LocalAAR.h"
#endif

#ifndef CRRECLIST_H_
#  include "CRRecList.h"
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

/*
 *
 * Crash Recovery Controller Class
 *
 */

class CRController
{
public:
    CRController ();
    ~CRController ();

    Boolean makeRecoveryPass();

private:
    ostream& printStatus ( ostream& strm ) const;
    void getSAAWork();
    void getRepWork();
    void getLocalAAWork();
    Boolean checkAndSetupGV();
    Boolean recoverGroupView();
    Boolean getExclusiveGV();
    Boolean releaseExclusiveGV();
    Boolean enableManager();
    int getManagerPid();
    long getccount();
    Boolean forkManager(); // Old version of startManager -- not currently used

    CRRecordList *workList;
    char *recoveringHost;
    Boolean firstPass;
    Boolean gotSAAWork;
    Boolean gotRepWork;
    Boolean gotAAWork;
    Boolean groupviewRecover;
    Boolean gotExclGVAccess;

    static Boolean setupGroupView;
    static NSInterface *GV;
    static int NSIRefCount;
    static Boolean managerRunning;
};

#endif // CRCONTROLLER_H_ 
