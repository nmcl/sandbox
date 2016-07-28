/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogBaseEvent.cc,v 1.3 1996/11/18 14:45:49 nsmw Exp $
 */

#include <stdio.h>

#include <InterViews/canvas.h>
#include <IV-2_6/InterViews/shape.h>
#include <IV-2_6/InterViews/painter.h>

#include "LogInter.h"
#include "LogBaseEvent.h"

LogBaseEvent::LogBaseEvent(enum baseEventType et, int inLev1, int inLev2, int seat_num, int oper)
{
    inLevel1    = inLev1;
    inLevel2    = inLev2;
    seat_number = seat_num;
    operation   = oper;

    eventType  = et;

    shape->Rect(2*unitSize, 10*unitSize);
    shape->Rigid();
    output = new Painter;
}

LogBaseEvent::~LogBaseEvent()
{
}

void LogBaseEvent::Redraw(Coord x1, Coord y1, Coord x2, Coord y2)
{
    if (inLevel1)
    {
        output->Line (GetCanvas(),              0, 3*unitSize - 1, 3*unitSize - 1, 3*unitSize - 1);
        output->Line (GetCanvas(),              0, 9*unitSize - 1, 3*unitSize - 1, 9*unitSize - 1);
    }

    if (inLevel2)
    {
        output->Line (GetCanvas(), 0, 4*unitSize - 1, 3*unitSize - 1, 4*unitSize - 1);
        output->Line (GetCanvas(), 0, 8*unitSize - 1, 3*unitSize - 1, 8*unitSize - 1);
    }

    char label[16];
    char type;
    switch (operation)
    {
    case 0:
	type = 'R';
	break;
    case 1:
	type = 'U';
	break;
    case 2:
	type = 'G';
	break;
    }

    output->Ellipse(GetCanvas(), unitSize - 1,       6*unitSize - 1, unitSize - 1,   unitSize);
    output->Ellipse(GetCanvas(), unitSize - 1,       6*unitSize - 1, unitSize/2 - 1, unitSize/2);
    output->Line(GetCanvas(),    0,                  6*unitSize - 1, unitSize/2 - 1, 6*unitSize - 1);
    output->Line(GetCanvas(),    (3*unitSize/2) - 1, 6*unitSize - 1, 2*unitSize - 1,   6*unitSize - 1);

    output->Line (GetCanvas(), 2*unitSize - 1, unitSize, 2*unitSize - 1,  2*unitSize -1);
    output->Line (GetCanvas(), (unitSize / 2) + unitSize - 1, unitSize, 2*unitSize - 1, unitSize);

    switch (eventType)
    {
    case BaseCommitEvent:
	sprintf(label, "%c %d", type, seat_number);
	output->Text(GetCanvas(), label, 0, unitSize -1);
	output->Text(GetCanvas(), "Commit", 0, 0);
	break;
    case BaseAbortEvent:
	sprintf(label, "%c %d", type, seat_number);
	output->Text(GetCanvas(), label, 0, unitSize -1);
	output->Text(GetCanvas(), "Abort", 0, 0);
	break;
    }
}

