/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerInter.h,v 1.2 1994/10/31 13:34:31 nsmw Exp $
 */

#ifndef TOWERINTER_H_
#define TOWERINTER_H_

#include <InterViews/color.h>
#include <InterViews/pattern.h>
#include <IV-2_6/InterViews/interactor.h>

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

    Pattern *lightgray;
    Pattern *gray;
    Pattern *solid;

    const Color *black;
    const Color *white;

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