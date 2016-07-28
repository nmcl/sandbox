/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashMan.cc,v 1.5 1994/09/13 09:28:43 ngdp Exp $
 */

/* 
 * Crash Recovery Manager
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef CRCONTROLLER_H_
#  include <CRController.h>
#endif

main()
{

    // ofstream outStrm("/dev/console");

    CRController controller(cout);
    controller.recover();

    return 0;
    
}
