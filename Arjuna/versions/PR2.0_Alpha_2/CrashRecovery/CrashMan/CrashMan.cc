/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashMan.cc,v 1.1 1993/11/03 14:17:01 nmcl Exp $
 */

/* 
 * Crash Recovery Manager
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

/*
 * This is not necesarily needed here but prevents include file nesting
 * errors with CFRONT 3.0.1
 */

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef CRCONTROLLER_H_
#  include "CRController.h"
#endif

main()
{
    CRController crController;
    Boolean stillWork = TRUE;
    const int sleeptime = 30;

    while (stillWork)
    {
        if ( stillWork = crController.makeRecoveryPass() )
        {
            cout << "\"I'm sleepy\" said Dougal. \"Wake me up in "
                 << sleeptime << " secs\"" << endl;
            sleep(sleeptime);
        }
    }
}
