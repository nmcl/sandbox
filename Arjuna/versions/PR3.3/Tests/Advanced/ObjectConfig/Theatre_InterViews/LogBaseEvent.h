/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogBaseEvent.h,v 1.1 1993/11/11 15:18:31 nmcl Exp $
 */

#ifndef LOGBASEEVENT_H_
#define LOGBASEEVENT_H_

#include <InterViews/2.6/InterViews/interactor.h>

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
