/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogLevel2Event.h,v 1.3 1993/03/18 14:02:42 nmcl Exp $
 */

#ifndef LOGLEVEL2EVENT_H_
#define LOGLEVEL2EVENT_H_

#include <InterViews/2.6/InterViews/interactor.h>

enum level2EventType { L2BeginEvent, L2CommitEvent, L2AbortEvent };

class LogLevel2Event : public Interactor
{
    enum level2EventType eventType;
    int                  inLevel1;
public:
    LogLevel2Event(enum level2EventType, int);
    virtual ~LogLevel2Event();

    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
