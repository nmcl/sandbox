/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogVoid.cc,v 1.1 1993/11/11 15:18:43 nmcl Exp $
 */

#include <InterViews/canvas.h>
#include <InterViews/2.6/InterViews/shape.h>
#include <InterViews/2.6/InterViews/painter.h>

#include "LogInter.h"
#include "LogVoid.h"

LogVoid::LogVoid(int inLev1, int inLev2)
{
    inLevel1 = inLev1;
    inLevel2 = inLev2;

    shape->Rect(unitSize*2, 10*unitSize);
    shape->Rigid();
    output = new Painter;
}

LogVoid::~LogVoid()
{
}

void LogVoid::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    if (inLevel1)
    {
	output->Line(GetCanvas(), 0, 3*unitSize - 1, (unitSize / 2) - 1, 3*unitSize - 1);
	output->Line(GetCanvas(), 0, 9*unitSize - 1, (unitSize / 2) - 1, 9*unitSize - 1);
	output->Line(GetCanvas(), (3*unitSize / 2) - 1, 3*unitSize - 1, 2*unitSize - 1, 3*unitSize - 1);
	output->Line(GetCanvas(), (3*unitSize / 2) - 1, 9*unitSize - 1, 2*unitSize - 1, 9*unitSize - 1);
    }

    if (inLevel2)
    {
	output->Line(GetCanvas(), 0, 4*unitSize - 1, (unitSize / 2) - 1, 4*unitSize - 1);
	output->Line(GetCanvas(), 0, 8*unitSize - 1, (unitSize / 2) - 1, 8*unitSize - 1);
	output->Line(GetCanvas(), (3*unitSize / 2) - 1, 4*unitSize - 1, 2*unitSize - 1, 4*unitSize - 1);
	output->Line(GetCanvas(), (3*unitSize / 2) - 1, 8*unitSize - 1, 2*unitSize - 1, 8*unitSize - 1);
    }

    output->Line(GetCanvas(), 0, 6*unitSize - 1, (unitSize / 2) - 1, 6*unitSize - 1);
    output->Line(GetCanvas(), (3*unitSize / 2) - 1, 6*unitSize - 1, 2*unitSize - 1, 6*unitSize - 1);
}

