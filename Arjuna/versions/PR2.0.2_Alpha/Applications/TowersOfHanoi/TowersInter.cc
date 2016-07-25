/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersInter.cc,v 1.1 1993/11/03 12:25:38 nmcl Exp $
 */

#include <InterViews/glue.h>
#include <InterViews/banner.h>

#include "TowersInter.h"

TowersInter::TowersInter()
{
    pinsInter[0] = new TowerInter(maxDisk);
    pinsInter[1] = new TowerInter(maxDisk);
    pinsInter[2] = new TowerInter(maxDisk);
    pinsInter[3] = new TowerInter(1);

    Insert(new HBox(new HGlue(5, 0),
		    new VBox(pinsInter[0], new Banner("", "Pin 1", "")),
		    new HGlue(5, 0),
		    new VBox(pinsInter[1], new Banner("", "Pin 2", "")),
		    new HGlue(5, 0),
		    new VBox(pinsInter[2], new Banner("", "Pin 3", "")),
		    new HGlue(5, 0)));

    Insert(new HBox(new HGlue,
		    new VBox(pinsInter[3], new Banner("", "In Hand", "")),
		    new HGlue));
}

TowersInter::~TowersInter()
{
   if (pinsInter[0])
       delete pinsInter[0];
   if (pinsInter[1])
       delete pinsInter[1];
   if (pinsInter[2])
       delete pinsInter[2];
   if (pinsInter[3])
       delete pinsInter[3];
}

void TowersInter::SetDisks(int num, int state[maxDisk], int numDisk)
{
    pinsInter[num]->SetDisks(state, numDisk);
}

void TowersInter::SetLocked(int num, enum disksLockState locked)
{
    pinsInter[num]->SetLocked(locked);
}

void TowersInter::SetInvalid(int num)
{
    pinsInter[num]->SetInvalid();
}
