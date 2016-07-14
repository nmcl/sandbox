/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerDispatch.h,v 1.3 1993/03/18 14:08:13 nmcl Exp $
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
    int           state;
    int           quit;
    int           res;
    AtomicAction* A;
public:
    TowerDispatch();
    virtual ~TowerDispatch();

    void Run();
    virtual void Handle(Event& e);
};
	
#endif
