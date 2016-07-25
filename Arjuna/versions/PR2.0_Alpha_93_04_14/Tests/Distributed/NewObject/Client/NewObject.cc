/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NewObject.cc,v 1.4 1993/03/18 15:06:25 n048z Exp $
 */

#include <Configure.h>

#include <System/iostream.h>
#include <Common/Boolean.h>
#include <Arjuna/AtomicA.h>

#include "ObjTest_stub.h"

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int, char**)
#else
int main(int, char**)
#endif
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
