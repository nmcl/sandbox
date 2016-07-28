/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatreInter.h,v 1.2 1994/10/31 12:07:38 nsmw Exp $
 */

#ifndef THEATREINTER_H_
#define THEATREINTER_H_

#include <IV-2_6/InterViews/box.h>
#include <IV-2_6/InterViews/banner.h>

#include "SeatInter.h"
#include "TheatreDispatch.h"

class TheatreInter :public VBox
{
public:
    TheatreInter(int& nunSeats, TheatreDispatch*);
    ~TheatreInter();

    void SetMode(char*);
    void SetLogStatus(char*);
    void SetOperation(char*);
    void SetOperResult(char*);

    void Select(int);
    void Unselect(int);
    void Reserve(int);
    void Unreserve(int);
    void Unknown(int);
  
private:
    Banner*          theatre_status;
    Banner*          operation_status;
    TheatreDispatch* TheatreDisp;

    int              num_seats;
    SeatInter        (*seats[100]);
};

#endif
