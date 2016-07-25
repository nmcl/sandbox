/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TowerInter.cc,v 1.1 1993/11/03 12:25:25 nmcl Exp $
 */

#include <System/iostream.h>
#include <Common/Error.h>

#include <InterViews/canvas.h>
#include <InterViews/shape.h>
#include <InterViews/pattern.h>
#include <InterViews/painter.h>
#include <InterViews/color.h>

#include "TowersOfHanoi.h"
#include "TowerInter.h"

const int vUnit   = 16;
const int hUnit   = 16;

void TowerInter::DrawDisk(int level, int size)
{
    if (invalid)
	output->SetPattern (lightgray);
    else
	output->SetColors  (white, black);

    output->FillEllipse(GetCanvas(), hUnit*(maxDisk + 1) - 1,        vUnit*level,
			             hUnit*size,                     (vUnit*size)/maxDisk);

    output->SetPattern (solid);
    output->SetColors  (black, white);
    output->Ellipse    (GetCanvas(), hUnit*(maxDisk + 1) - 1,        vUnit*level,
			             hUnit*size,                     (vUnit*size)/maxDisk);
    
    if (invalid)
	output->SetPattern (lightgray);
    else
	output->SetColors  (white, black);

    output->FillRect   (GetCanvas(), hUnit*(maxDisk + 1 - size) - 1, vUnit*level,
			             hUnit*(maxDisk + 1 + size) - 1, vUnit*(level + 1) - 1);
    
    output->SetPattern (solid);
    output->SetColors  (black, white);
    output->Line       (GetCanvas(), hUnit*(maxDisk + 1 - size) - 1, vUnit*level,
			             hUnit*(maxDisk + 1 - size) - 1, vUnit*(level + 1));
    output->Line       (GetCanvas(), hUnit*(maxDisk + 1 + size) - 1, vUnit*level,
			             hUnit*(maxDisk + 1 + size) - 1, vUnit*(level + 1));
    
    if (invalid)
	output->SetPattern (lightgray);
    else
	output->SetColors  (white, black);

    output->FillEllipse(GetCanvas(), hUnit*(maxDisk + 1) - 1,        vUnit*(level + 1),
			             hUnit*size,                     (vUnit*size)/maxDisk);

    output->SetPattern (solid);
    output->SetColors  (black, white);
    output->Ellipse    (GetCanvas(), hUnit*(maxDisk + 1) - 1,        vUnit*(level + 1),
			             hUnit*size,                     (vUnit*size)/maxDisk);
}

TowerInter::TowerInter(int mH)
{
    maxHight    = mH;
    
    diskNumber  = 0;
    disksLockSt = noneL;
    invalid     = true;
    
    shape->Rect(2*hUnit*(maxDisk + 1), vUnit*(maxHight + 2));
    shape->Rigid();
    output = new Painter;
}

TowerInter::~TowerInter()
{
}

void TowerInter::SetDisks(int state[maxDisk], int num)
{
    diskNumber = num;
    for (int i = 0; i < num; i++)
	disks[i] = state[i];
    invalid     = false;
}
  
void TowerInter::SetLocked(enum disksLockState lockSt)
{
    disksLockSt = lockSt;
}

void TowerInter::SetInvalid()
{
    invalid     = true;
}

void TowerInter::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    output->SetColors(white, black);

    output->FillRect   (GetCanvas(), 0,                          0,
                                     2*hUnit*(maxDisk + 1) - 1,  vUnit*(maxHight + 2) - 1);
    output->SetColors  (black, white);

    Coord y[4];
    y[0] = 0;   y[1] = 2*vUnit - 1;   y[2] = 2*vUnit - 1;           y[3] = 0;
    Coord x[4];
    x[0] = 0;   x[1] = 2*hUnit - 1;   x[2] = 2*hUnit*maxDisk - 1;   x[3] = 2*hUnit*(maxDisk + 1) - 1;

    switch (disksLockSt)
    {
    case noneL:
	break;
    case readL:
	output->SetPattern(lightgray);
	output->FillPolygon(GetCanvas(), x, y, 4);
	output->SetPattern(solid);
	break;
    case writeL:
	output->SetPattern(gray);
	output->FillPolygon(GetCanvas(), x, y, 4);
	output->SetPattern(solid);
	break;
    case confL:
	output->FillPolygon(GetCanvas(), x, y, 4);
	break;
    };
    output->Polygon(GetCanvas(), x, y, 4);

    for (int i = 0; i < diskNumber; i++)
	if (i < maxHight)
	{
	    if (disks[i] <= maxDisk)
		DrawDisk(i + 1, disks[i]);
	    else
		error_stream << "Disk to big: Level " << i << " Size " << disks[i] << "\n";
	}
	else
	    error_stream << "Extra Disk's: Level " << i << "\n";
}
