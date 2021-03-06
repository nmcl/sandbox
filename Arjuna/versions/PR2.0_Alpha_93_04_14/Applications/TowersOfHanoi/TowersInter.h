/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersInter.h,v 1.4 1993/03/18 14:08:27 nmcl Exp $
 */

#ifndef TOWERSINTER_H_
#define TOWERSINTER_H_

#include <InterViews/box.h>

#include "TowersOfHanoi.h"
#include "TowerInter.h"

class TowersInter : public VBox
{
    TowerInter   *pinsInter[4];
public:
    TowersInter();
    virtual ~TowersInter();

    void SetDisks(int num, int state[maxDisk], int numDisk);
    void SetLocked(int num, enum disksLockState locked);
    void SetInvalid(int num);
};
	
#endif
