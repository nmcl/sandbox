/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatrePersis.h,v 1.1 1993/11/03 12:24:36 nmcl Exp $
 */

#ifndef THEATREPERSIS_H_
#define THEATREPERSIS_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

#include "SeatPersis.h"
#include "VersionPersis.h"

class TheatrePersis :public LockManager
{
    SeatPersis*    seats[100];
    VersionPersis* version;
    int            numSeats;
public:
    TheatrePersis(int, int& res);
    TheatrePersis(int& res, Uid& U);
    ~TheatrePersis();

    void Get_Seat_Status(int seat, enum Seat_Status&, int& res);
    void Reserve_Seat(int seat, int& res);
    void Unreserve_Seat(int seat, int& res);

    void Version_Increase(int& res);
    void Version_Get(int& res, int& version);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
