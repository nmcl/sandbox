/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TheatrePersis.cc,v 1.2 1995/07/18 10:55:22 nsmw Exp $
 */

#include <Common/Debug.h>
#include <Common/Uid.h>

#include <Arjuna/AtomicA.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define SEATPERSIS_SERVER
#define THEATREPERSIS_SERVER

#include "TheatrePersis_stub.h"

TheatrePersis::TheatrePersis(int num, int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	numSeats = num;
	for (int i = 0; i < 100; i++)
	    seats[i] = NULL;
        version = NULL;

	res = 1;
	for (int j = 0; (j < numSeats) && (res == 1); j++)
	{
	    res = 0;
	    seats[j] = SeatPersis::Create(res);
	}

        if (res)
            version = VersionPersis::Create(res);
    }

    if (res == 1)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

TheatrePersis::TheatrePersis(int& res, Uid& U) : LockManager(U)
{
    numSeats = 0;
    for (int i = 0; i < 100; i++)
	seats[i] = NULL;
    version = NULL;

    res = 1;
}

TheatrePersis::~TheatrePersis()
{
    SeatPersis::Destroy();
    VersionPersis::Destroy();

    terminate();
}

void TheatrePersis::Get_Seat_Status(int seat, enum Seat_Status& stat, int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
	seats[seat]->Get_Seat_Status(stat, res);

    if (res == 1)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void TheatrePersis::Reserve_Seat(int seat, int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
        seats[seat]->Reserve_Seat(res);

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void TheatrePersis::Unreserve_Seat(int seat, int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
        seats[seat]->Unreserve_Seat(res);

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void TheatrePersis::Version_Increase(int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
        version->increase(res);

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void TheatrePersis::Version_Get(int& res, int& ver)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
        version->get(res, ver);

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

/*
 *   These save_state and restore_state will only work if operation only
 *   get READ locks (They do).
 */

Boolean TheatrePersis::save_state(ObjectState& os, ObjectType t)
{
    Boolean res;

    if (t == ANDPERSISTENT)
    {
	res = os.pack(numSeats);
	for (int i = 0; (i < numSeats) && res; i++)
	    res = (seats[i]->get_uid()).pack(os);
	res = res && (version->get_uid()).pack(os);
    }
    
    if (t == RECOVERABLE)
    {
	res = os.pack(numSeats);
	for (int i = 0; (i < numSeats) && res; i++)
	    res = os.pack((long) seats[i]);
	res = res && os.pack((long) version);
    }

    return (Boolean) res;
}

Boolean TheatrePersis::restore_state(ObjectState& os, ObjectType t)
{
    Boolean res;

    if (t == ANDPERSISTENT)
    {
	int err;
	res = os.unpack(numSeats);
	for (int i = 0; (i < numSeats) && res; i++)
	{
	    Uid objUid(NIL_UID);
	    res = objUid.unpack(os);
	    if (res)
	    {
		seats[i] = SeatPersis::Create(err, objUid);
		res = (Boolean) (err == 1);
	    }
	}
        if (res)
        {
	    Uid objUid(NIL_UID);
	    res = objUid.unpack(os);
	    if (res)
	    {
		version = VersionPersis::Create(err, objUid);
		res = (Boolean) (err == 1);
            }
        }
    }

    if (t == RECOVERABLE)
    {
        long tmp;
	res = os.unpack(numSeats);
	for (int i = 0; (i < numSeats) && res; i++)
        {
	    res = os.unpack(tmp);
            if (res)
                seats[i] = (SeatPersis*) tmp;
        }
        if (res)
        {
	    res = os.unpack(tmp);
            if (res)
                version = (VersionPersis*) tmp;
        }
    }

    return (Boolean) res;
}

const TypeName TheatrePersis::type() const
{
    return "/StateManager/LockManager/TheatrePersis";
}
