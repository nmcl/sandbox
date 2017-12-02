/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: LogMark.cc,v 1.2 1994/10/31 12:07:24 nsmw Exp $
 */

#include <InterViews/canvas.h>
#include <IV-2_6/InterViews/shape.h>
#include <IV-2_6/InterViews/painter.h>

#include "LogInter.h"
#include "LogMark.h"

LogMark::LogMark(int inLev1, int inLev2)
{
    inLevel1 = inLev1;
    inLevel2 = inLev2;

    shape->Rect(unitSize, 10*unitSize);
    shape->Rigid();
    output = new Painter;
}

LogMark::~LogMark()
{
}

void LogMark::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    if (inLevel1)
    {
	output->Line(GetCanvas(), 0, 3*unitSize - 1, unitSize - 1, 3*unitSize - 1);
	output->Line(GetCanvas(), 0, 9*unitSize - 1, unitSize - 1, 9*unitSize - 1);
    }

    if (inLevel2)
    {
	output->Line(GetCanvas(), 0, 4*unitSize - 1, unitSize - 1, 4*unitSize - 1);
	output->Line(GetCanvas(), 0, 8*unitSize - 1, unitSize - 1, 8*unitSize - 1);
    }

    output->Line(GetCanvas(), 0, 6*unitSize - 1, unitSize - 1, 6*unitSize - 1);
}

