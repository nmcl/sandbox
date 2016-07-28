/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersInter.h,v 1.2 1994/10/31 13:34:37 nsmw Exp $
 */

#ifndef TOWERSINTER_H_
#define TOWERSINTER_H_

#include <IV-2_6/InterViews/box.h>

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
