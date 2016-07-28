/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SeatInter.h,v 1.1 1993/11/11 15:18:51 nmcl Exp $
 */

#ifndef SEATINTER_H_
#define SEATINTER_H_

class SeatInter;

#include <InterViews/2.6/InterViews/interactor.h>

#include "TheatreDispatch.h"

enum seat_status { Seat_Empty, Seat_Full, Seat_Unknown,
		   Seat_Select_Empty, Seat_Select_Full, Seat_Select_Unknown};

class SeatInter : public Interactor
{
    int              status;
    int              seat_number;
    TheatreDispatch* TheatreDisp;
public:
    SeatInter(int, TheatreDispatch*);
    virtual ~SeatInter();

    void Select();
    void Unselect();
    void Reserve();
    void Unreserve();
    void Unknown();
    
    virtual void Redraw(Coord x1, Coord y1, Coord x2, Coord y2);
    virtual void Handle(Event& e);
};
	
#endif
