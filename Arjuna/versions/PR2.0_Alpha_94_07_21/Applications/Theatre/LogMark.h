/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: LogMark.h,v 1.1 1993/11/03 12:24:05 nmcl Exp $
 */

#ifndef LOGMARK_H_
#define LOGMARK_H_

#include <InterViews/2.6/InterViews/interactor.h>

class LogMark : public Interactor
{
    int inLevel1;
    int inLevel2;
public:
    LogMark(int, int);
    virtual ~LogMark();

    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
