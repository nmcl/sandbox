/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerDispatch.h,v 1.3 1994/10/31 13:34:28 nsmw Exp $
 */

#ifndef TOWERDISPATCH_H_
#define TOWERDISPATCH_H_

#include "TowerInter.h"

#include <Arjuna/LockMan.h>
#include <Arjuna/AtomicA.h>

#include "TowerPersis_stub.h"

class TowerDispatch : public TowerInter
{
    TowerPersis*  pinsPersis;
    ClientRpcManager*   RpcConP;

    int           state;
    int           quit;
    int           res;
    AtomicAction* A;
public:
    TowerDispatch(int);
    virtual ~TowerDispatch();

    void Run();
    virtual void Step();
    virtual void Handle(Event& e);
};
	
#endif
