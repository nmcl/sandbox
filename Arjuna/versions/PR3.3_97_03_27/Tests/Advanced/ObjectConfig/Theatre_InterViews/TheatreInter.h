/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatreInter.h,v 1.1 1993/11/11 15:19:04 nmcl Exp $
 */

#ifndef THEATREINTER_H_
#define THEATREINTER_H_

#include <InterViews/2.6/InterViews/box.h>
#include <IV-look/2.6/InterViews/banner.h>

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
