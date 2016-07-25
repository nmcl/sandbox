/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogLevel2Event.cc,v 1.1 1993/11/03 12:24:00 nmcl Exp $
 */

#include <InterViews/canvas.h>
#include <InterViews/2.6/InterViews/shape.h>
#include <InterViews/2.6/InterViews/painter.h>

#include "LogInter.h"
#include "LogLevel2Event.h"

LogLevel2Event::LogLevel2Event(enum level2EventType et, int inLev1)
{
    eventType  = et;
    inLevel1   = inLev1;

    shape->Rect(3*unitSize, 10*unitSize);
    shape->Rigid();
    output = new Painter;
}

LogLevel2Event::~LogLevel2Event()
{
}

void LogLevel2Event::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    if (inLevel1)
    {
	output->Line (GetCanvas(), 0,              3*unitSize - 1, 3*unitSize - 1, 3*unitSize - 1);
	output->Line (GetCanvas(), 0,              9*unitSize - 1, 3*unitSize - 1, 9*unitSize - 1);
    }

    output->Line (GetCanvas(),     0,              6*unitSize - 1, 3*unitSize - 1, 6*unitSize - 1);

    switch (eventType)
    {
    case L2BeginEvent:
	output->Line   (GetCanvas(), 0,   unitSize, 0,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2),   unitSize, 0,  unitSize);
	output->Text   (GetCanvas(), "Nested",   (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Begin",    (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 3*unitSize, 6*unitSize - 1, 3*unitSize,   2*unitSize);
	break;
    case L2CommitEvent:
	output->Line   (GetCanvas(), 3*unitSize - 1,   unitSize, 3*unitSize - 1,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2) + 2*unitSize - 1,   unitSize, 3*unitSize - 1,  unitSize);
	output->Text   (GetCanvas(), "Nested",                (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Commit",                (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 0,          6*unitSize - 1, 3*unitSize - 1, 2*unitSize);
	break;
    case L2AbortEvent:
	output->Line   (GetCanvas(), 3*unitSize - 1,   unitSize, 3*unitSize - 1,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2) + 2*unitSize - 1,   unitSize, 3*unitSize - 1,  unitSize);
	output->Text   (GetCanvas(), "Nested",                (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Abort",                 (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 0,          6*unitSize - 1, 3*unitSize - 1, 2*unitSize);
	break;
    }
}

