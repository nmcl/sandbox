/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: LogLevel1Event.h,v 1.2 1994/10/31 12:07:21 nsmw Exp $
 */

#ifndef LOGLEVEL1EVENT_H_
#define LOGLEVEL1EVENT_H_

#include <IV-2_6/InterViews/interactor.h>

enum level1EventType { L1BeginEvent, L1CommitEvent, L1AbortEvent };

class LogLevel1Event : public Interactor
{
    enum level1EventType eventType;
public:
    LogLevel1Event(enum level1EventType);
    virtual ~LogLevel1Event();

    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
