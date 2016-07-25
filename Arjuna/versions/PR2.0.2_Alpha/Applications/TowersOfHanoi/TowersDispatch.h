/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersDispatch.h,v 1.1 1993/11/03 12:25:36 nmcl Exp $
 */

#ifndef TOWERSDISPATCH_H_
#define TOWERSDISPATCH_H_

#include "TowersInter.h"

#include <Arjuna/LockMan.h>
#include <Arjuna/AtomicA.h>

#include "TowerPersis_stub.h"
#include "WorkStack_stub.h"

class TowersDispatch : public TowersInter
{
    WorkStack*   workStack;
    RpcControl  *RpcConWS;

    int          state;
    int          quit;
    int          res;
    int          pinTmp;
    AtomicAction* A;

    TowerPersis *pinsPersis[4];
    RpcControl  *RpcConP1;
    RpcControl  *RpcConP2;
    RpcControl  *RpcConP3;
    RpcControl  *RpcConP4;

    void clean_and_exit(int code);
public:
    TowersDispatch(int);
    virtual ~TowersDispatch();

    void Run();
    virtual void Handle(Event& e);
};
	
#endif
