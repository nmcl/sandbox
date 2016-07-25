/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerPersis.h,v 1.5 1993/08/06 10:48:24 nmcl Exp $
 */

#ifndef TOWERPERSIS_H_
#define TOWERPERSIS_H_

#include "TowersOfHanoi.h"

#include <Common/Uid.h>
#include <Common/Boolean.h>
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
    Boolean remoteState;
public:
    TowerPersis(int& res);
    TowerPersis(int& res, Uid& U, Boolean = FALSE);
    virtual ~TowerPersis();

    void PushDisk(int size, TowerState& state, int& res);
    void PopDisk(int& size, TowerState& state, int& res);

    void GetState(TowerState& state, int& res);

    Boolean HasRemoteState () const;

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};
	
#endif
