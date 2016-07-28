/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SeatPersis.cc,v 1.2 1994/09/14 08:56:08 ngdp Exp $
 */

#include <System/iostream.h>

#ifdef DEBUG
#   ifndef DEBUG_H_
#       include <Common/Debug.h>
#   endif
#endif

#include <Arjuna/AtomicA.h>

#include "SeatPersis.h"

SeatPersis *SeatPersis::listHead = NULL;

SeatPersis *SeatPersis::Create(int& res, ConfigIndex *confIndex, MultiManager *multiManager)
{
    SeatPersis *tempPtr = new SeatPersis(res, confIndex, multiManager);

    tempPtr->nextElem = listHead;
    listHead          = tempPtr;

    return tempPtr;
}

SeatPersis *SeatPersis::Create(int& res, const Uid& uid, ConfigIndex *confIndex)
{
    Boolean done = FALSE;

    SeatPersis *tempPtr = listHead;
    while ((! done) && (tempPtr != NULL))
    {
        if (uid == tempPtr->get_uid())
            done = TRUE;
        else
            tempPtr = tempPtr->nextElem;
    }

    if (tempPtr == NULL)
    {
        tempPtr           = new SeatPersis(res, uid, confIndex);
        tempPtr->nextElem = listHead;
        listHead          = tempPtr;
    }
    else
	res = 1;	

    return tempPtr;
}

void SeatPersis::Destroy()
{
    SeatPersis *tempPtr = listHead;

    while (tempPtr != NULL)
    {
        tempPtr = listHead->nextElem;
        delete listHead;
        listHead = tempPtr;
    }
}

SeatPersis::SeatPersis(int& res, ConfigIndex *confIndex,
		       MultiManager *multiMan)
		      : Manager(confIndex, multiMan)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    seat_stat = Unreserved;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
	res = 1;

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

SeatPersis::SeatPersis(int& res, const Uid& U, ConfigIndex *confIndex) : Manager(U, confIndex)
{
    seat_stat = Unreserved;
    res = 1;
}

SeatPersis::~SeatPersis()
{
    terminate();
}

void SeatPersis::Get_Seat_Status(enum Seat_Status& stat, int& res)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        stat = seat_stat;
	res = 1;
    }

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void SeatPersis::Reserve_Seat(int& res)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (seat_stat == Reserved)
	    res = 2;
	else
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		seat_stat = Reserved;
		res = 1;
	    }
    }

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void SeatPersis::Unreserve_Seat(int& res)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (seat_stat == Unreserved)
	    res = 2;
	else
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		seat_stat = Unreserved;
		res = 1;
	    }
    }

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

Boolean SeatPersis::save_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "SeatPersis::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;

    res = os.pack((int) seat_stat);

    return res;
}

Boolean SeatPersis::restore_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "SeatPersis::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean  res;
    int   tmp;

    res = os.unpack(tmp);
    seat_stat = (enum Seat_Status) tmp;

    return res;
}

const TypeName SeatPersis::type() const
{
    return "/StateManager/LockManager/SeatPersis";
}
