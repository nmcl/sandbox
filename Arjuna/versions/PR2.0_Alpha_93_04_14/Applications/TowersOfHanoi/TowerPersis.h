/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerPersis.h,v 1.4 1993/03/18 14:08:20 nmcl Exp $
 */

#ifndef TOWERPERSIS_H_
#define TOWERPERSIS_H_

#include "TowersOfHanoi.h"

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

// @NoRemote, @AutoMarshall
struct TowerState
{
    int num;
    int state[maxDisk];
};

class TowerPersis : public LockManager
{
    int disks[maxDisk];
    int diskNumber;
public:
    TowerPersis(int& res);
    TowerPersis(int& res, Uid& U);
    virtual ~TowerPersis();

    void PushDisk(int size, TowerState& state, int& res);
    void PopDisk(int& size, TowerState& state, int& res);

    void GetState(TowerState& state, int& res);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};
	
#endif
