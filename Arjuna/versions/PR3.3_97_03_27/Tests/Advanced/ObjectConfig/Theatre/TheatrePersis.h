/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TheatrePersis.h,v 1.1 1993/11/11 15:17:56 nmcl Exp $
 */

#ifndef THEATREPERSIS_H_
#define THEATREPERSIS_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

#include <ObjectConfig/ConfIndex.h>

#include "SeatPersis.h"

class TheatrePersis :public LockManager
{
    int          numSeats;
    SeatPersis   *seats[100];
    MultiManager *multiMan[100];
    ConfigIndex  *confIndex;
public:
    TheatrePersis(int, int, int& res);
    TheatrePersis(int& res, Uid& U);
    ~TheatrePersis();

    void Get_Seat_Status(int seat, enum Seat_Status&, int& res);
    void Reserve_Seat(int seat, int& res);
    void Unreserve_Seat(int seat, int& res);

    void ClusterAsSeats(int& res);
    void ClusterAsRows(int& res);
    void ClusterAsTheatre(int& res);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;
};

#endif
