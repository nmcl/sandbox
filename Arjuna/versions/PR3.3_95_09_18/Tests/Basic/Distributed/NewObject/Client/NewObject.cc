/*
 * Copyright (C) 1993-1998,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NewObject.cc,v 1.3 1995/02/22 14:56:01 ngdp Exp $
 */

#include <Config/Configure.h>

#include <System/iostream.h>
#include <Common/Boolean.h>
#include <Arjuna/AtomicA.h>

#include "ObjTest_stub.h"

int main(int, char**)
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
