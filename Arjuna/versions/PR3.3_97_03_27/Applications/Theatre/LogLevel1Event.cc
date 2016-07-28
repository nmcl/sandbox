/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogLevel1Event.cc,v 1.2 1994/10/31 12:07:20 nsmw Exp $
 */

#include <InterViews/canvas.h>
#include <IV-2_6/InterViews/shape.h>
#include <IV-2_6/InterViews/painter.h>

#include "LogInter.h"
#include "LogLevel1Event.h"

LogLevel1Event::LogLevel1Event(enum level1EventType et)
{
    eventType  = et;

    shape->Rect(4*unitSize, 10*unitSize);
    shape->Rigid();
    output = new Painter;
}

LogLevel1Event::~LogLevel1Event()
{
}

void LogLevel1Event::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    output->Line (GetCanvas(),     0,            6*unitSize - 1, 4*unitSize - 1, 6*unitSize - 1);
    switch (eventType)
    {
    case L1BeginEvent:
	output->Line   (GetCanvas(), 0,   unitSize, 0,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2),   unitSize, 0,  unitSize);
	output->Text   (GetCanvas(), "Top-Level", (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Begin",     (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 4*unitSize, 6*unitSize - 1, 4*unitSize, 3*unitSize);
	break;
    case L1CommitEvent:
	output->Line   (GetCanvas(), 4*unitSize - 1,   unitSize, 4*unitSize - 1,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2) + 3*unitSize - 1,   unitSize, 4*unitSize - 1,  unitSize);
	output->Text   (GetCanvas(), "Top-Level",             (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Commit",                (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 0,          6*unitSize - 1, 4*unitSize - 1, 3*unitSize);
	break;
    case L1AbortEvent:
	output->Line   (GetCanvas(), 4*unitSize - 1,   unitSize, 4*unitSize - 1,  2*unitSize -1);
	output->Line   (GetCanvas(), (unitSize / 2) + 3*unitSize - 1,   unitSize, 4*unitSize - 1,  unitSize);
	output->Text   (GetCanvas(), "Top-Level",             (unitSize / 2), unitSize -1);
	output->Text   (GetCanvas(), "Abort ",                (unitSize / 2), 0);
	output->Ellipse(GetCanvas(), 0,          6*unitSize - 1, 4*unitSize - 1, 3*unitSize);
	break;
    }
}


