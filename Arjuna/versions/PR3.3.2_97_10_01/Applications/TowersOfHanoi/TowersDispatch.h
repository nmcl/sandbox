/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersDispatch.h,v 1.3 1994/10/31 13:34:35 nsmw Exp $
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
    ClientRpcManager  *RpcConWS;

    int          state;
    int          quit;
    int          res;
    int          pinTmp;
    AtomicAction* A;

    TowerPersis *pinsPersis[4];
    ClientRpcManager  *RpcConP1;
    ClientRpcManager  *RpcConP2;
    ClientRpcManager  *RpcConP3;
    ClientRpcManager  *RpcConP4;

    void clean_and_exit(int code);
public:
    TowersDispatch(int);
    virtual ~TowersDispatch();

    void Run();
    virtual void Step();
    virtual void Handle(Event& e);
};
	
#endif
