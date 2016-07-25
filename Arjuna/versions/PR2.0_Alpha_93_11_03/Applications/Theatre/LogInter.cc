/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogInter.cc,v 1.1 1993/11/03 12:23:54 nmcl Exp $
 */

#include <IV-look/2.6/InterViews/border.h>
#include <InterViews/event.h>

#include "LogMark.h"
#include "LogLevel1Event.h"
#include "LogInter.h"

const int unitSize = 20;

LogInter::LogInter()
{
    circle_queue_end = 0;
    withMark = 0;

    Log = new HBox;
    view = new Viewport(Log, CenterRight);

    Insert(view);
    view->Propagate(false);
    Insert(new HBorder);
    scroller = new HScroller(view, unitSize);
    Insert(scroller);

    for (int j = 0; j < historySize; j++)
	circle_queue[j] = 0;

    for (int k = 0; k < historySize; k++)
	AddEvent(new LogMark(0, 0), 0, 1, 0, 0);
}

LogInter::~LogInter()
{
}

void LogInter::AddEvent(Interactor* new_entry, int needMark, int endWithMark, int inLevel1, int inLevel2)
{
    Interactor* old_event = 0;
    
    if (needMark && (! withMark))
	AddEvent(new LogMark(inLevel1, inLevel2), 0, 1, inLevel1, inLevel2);
    
    old_event = circle_queue[circle_queue_end];
    circle_queue[circle_queue_end] = new_entry;
    
    Log->Insert(circle_queue[circle_queue_end]);
    
    if (old_event != 0)
    {
	Log->Remove(old_event);
	delete old_event;
    }
    
    view->Change(Log);
    
    if (GetWorld() != 0)
	view->ScrollXTo(1.0f);
    
    view->Update();
    scroller->Update();
    
    if (circle_queue_end == (historySize - 1))
	circle_queue_end = 0;
    else
	circle_queue_end++;
    
    withMark = endWithMark;
}
