/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SeatPersis.h,v 1.1 1993/11/03 12:24:18 nmcl Exp $
 */

#ifndef SEATPERSIS_H_
#define SEATPERSIS_H_

#include <Common/Uid.h>
#include <Arjuna/ObjState.h>
#include <Arjuna/LockMan.h>

enum Seat_Status { Reserved, Unreserved };

// @NoRemote, @NoMarshall
class SeatPersis :public LockManager
{
public:
    static SeatPersis* Create(int& res);
    static SeatPersis* Create(int& res, const Uid&);
    static void        Destroy();

    void Get_Seat_Status(enum Seat_Status&, int& res);
    void Reserve_Seat(int& res);
    void Unreserve_Seat(int& res);

    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    SeatPersis(int& res);
    SeatPersis(int& res, const Uid& U);
    ~SeatPersis();

    enum Seat_Status seat_stat;

    static SeatPersis *listHead;
    SeatPersis        *nextElem;
};

#endif
