/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogVoid.h,v 1.2 1994/10/31 12:07:28 nsmw Exp $
 */

#ifndef LOGVOID_H_
#define LOGVOID_H_

#include <IV-2_6/InterViews/interactor.h>

class LogVoid : public Interactor
{
    int inLevel1;
    int inLevel2;
public:
    LogVoid(int, int);
    virtual ~LogVoid();

    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
};
	
#endif
