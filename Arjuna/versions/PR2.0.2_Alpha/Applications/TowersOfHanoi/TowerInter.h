/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerInter.h,v 1.1 1993/11/03 12:25:27 nmcl Exp $
 */

#ifndef TOWERINTER_H_
#define TOWERINTER_H_

#include <InterViews/interactor.h>

#include "TowersOfHanoi.h"

extern const int vUnit;
extern const int hUnit;

enum disksLockState { confL, readL, writeL, noneL };

class TowerInter : public Interactor
{
    int maxHight;
    int disks[maxDisk];
    int diskNumber;
    enum disksLockState disksLockSt;
    int  invalid;

    void DrawDisk(int level, int size);
public:
    TowerInter(int mH);
    virtual ~TowerInter();

    void SetDisks(int disks[maxDisk], int);
    void SetLocked(enum disksLockState);
    void SetInvalid();
    
    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
