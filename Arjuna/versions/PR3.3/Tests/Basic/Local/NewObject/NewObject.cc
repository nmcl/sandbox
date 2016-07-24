/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NewObject.cc,v 1.4 1997/03/26 15:33:38 nmcl Exp $
 */

#include <System/iostream.h>

#include <Common/Boolean.h>
#include <Arjuna/AtomicA.h>

#include "ObjTest.h"

main()
{
    int res = 0;
    ObjTest* x;
    AtomicAction A;
    
    A.Begin();
    cout << "A.Begin : Started\n";

    cout << "Creating new persistent object\n\n" << flush;
    x = new ObjTest(res, FALSE);

    cout << "\nCommitting top-level action (should call save_state)\n" << flush;
    if (A.End() != COMMITTED)
	cout << "A.End() : Not Done\n";
    else
	cout << "A.End() : Done\n";

    delete x;
    return 0;
}
