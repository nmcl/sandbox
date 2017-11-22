/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogBaseEvent.h,v 1.2 1994/10/31 12:07:16 nsmw Exp $
 */

#ifndef LOGBASEEVENT_H_
#define LOGBASEEVENT_H_

#include <IV-2_6/InterViews/interactor.h>

enum baseEventType {  BaseCommitEvent, BaseAbortEvent };

class LogBaseEvent : public Interactor
{
    enum baseEventType eventType;
    int inLevel1;
    int inLevel2;
    int seat_number;
    int operation;
public:
    LogBaseEvent(enum baseEventType, int, int, int, int);
    virtual ~LogBaseEvent();

    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
