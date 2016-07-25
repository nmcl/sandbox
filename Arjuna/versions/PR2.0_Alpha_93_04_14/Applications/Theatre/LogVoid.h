/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: LogVoid.h,v 1.3 1993/03/18 14:02:46 nmcl Exp $
 */

#ifndef LOGVOID_H_
#define LOGVOID_H_

#include <InterViews/2.6/InterViews/interactor.h>

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
