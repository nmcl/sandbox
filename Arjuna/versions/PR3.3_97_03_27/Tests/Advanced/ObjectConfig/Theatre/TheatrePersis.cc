/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TheatrePersis.cc,v 1.1 1993/11/11 15:17:55 nmcl Exp $
 */

#ifdef DEBUG
#   ifndef DEBUG_H_
#       include <Common/Debug.h>
#   endif
#endif

#include <Arjuna/AtomicA.h>

#include <ObjectConfig/ConfIndex.h>
#include <ObjectConfig/MultiMan.h>

#include "TheatrePersis.h"

TheatrePersis::TheatrePersis(int num, int groupSize, int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    confIndex = ConfigIndex::Create();

    res = 0;

    numSeats = num;
    for (int i = 0; i < 100; i++)
    {
	seats[i]    = NULL;
        multiMan[i] = NULL;
    }

    res = 1;
    for (int j = 0; (j < numSeats) && (res == 1); j++)
    {
        res = 0;
	multiMan[j]  = MultiManager::Create();
        seats[j]     = SeatPersis::Create(res, confIndex, multiMan[j / groupSize]);
    }

    if (setlock(new Lock(WRITE), 0) == GRANTED)
        res = 1;

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
    confIndex = NULL;
    for (int i = 0; i < 100; i++)
    {
	seats[i] = NULL;
        multiMan[i] = NULL;
    }

    res = 1;
}

TheatrePersis::~TheatrePersis()
{
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

void TheatrePersis::ClusterAsSeats(int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        res = 1;
        for (int j = 0; (j < numSeats) && (res == 1); j++)
        {
             if (! confIndex->reconfig(seats[j], multiMan[j]))
                 res = 0;
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

void TheatrePersis::ClusterAsRows(int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        res = 1;
        for (int j = 0; (j < numSeats) && (res == 1); j++)
        {
             if (! confIndex->reconfig(seats[j], multiMan[j / 5]))
                 res = 0;
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

void TheatrePersis::ClusterAsTheatre(int& res)
{
    AtomicAction A;
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        res = 1;
        for (int j = 0; (j < numSeats) && (res == 1); j++)
        {
             if (! confIndex->reconfig(seats[j], multiMan[0]))
                 res = 0;
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

Boolean TheatrePersis::save_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "TheatrePersis::save_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;

    if (t == ANDPERSISTENT)
    {
	res = os.pack(numSeats) && (confIndex->get_uid()).pack(os);
	for (int i = 0; (i < numSeats) && res; i++)
	    res = (seats[i]->get_uid()).pack(os) && (multiMan[i]->get_uid()).pack(os);
    }
    
    if (t == RECOVERABLE)
    {
	res = os.pack(numSeats) && os.pack((long) confIndex);
	for (int i = 0; (i < numSeats) && res; i++)
	    res = os.pack((long) seats[i]) &&  os.pack((long) multiMan[i]);
    }

    return (Boolean) res;
}

Boolean TheatrePersis::restore_state(ObjectState& os, ObjectType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << "TheatrePersis::restore_state(ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;

    if (t == ANDPERSISTENT)
    {
	int err;
	res = os.unpack(numSeats);

        if (res)
        {
            Uid objUid(NIL_UID);
	    res = objUid.unpack(os);
	    if (res)
	    {
		confIndex = ConfigIndex::Create(objUid);
		res = (Boolean) (confIndex != NULL);
	    }
        }

	for (int i = 0; (i < numSeats) && res; i++)
	{
	    Uid objUid(NIL_UID);
	    res = objUid.unpack(os);
	    if (res)
	    {
		seats[i] = SeatPersis::Create(err, objUid, confIndex);
		res = (Boolean) (err == 1);
	    }
	    if (res)
	    {
                res = objUid.unpack(os);
	        if (res)
	        {
		    multiMan[i] = MultiManager::Create(objUid);
		    res = (Boolean) (multiMan[i] != NULL);
                }
	    }
	}
    }

    if (t == RECOVERABLE)
    {
        long l;
	res = os.unpack(numSeats);

        if (res)
	{
            res = os.unpack(l);
            confIndex = (ConfigIndex*) l;
        }

	for (int i = 0; (i < numSeats) && res; i++)
	{
	    res = os.unpack(l);
            seats[i] = (SeatPersis*) l;
            if (res)
            {
	        res = os.unpack(l);
                multiMan[i] = (MultiManager*) l;
            }
	}
    }

    return (Boolean) res;
}

const TypeName TheatrePersis::type() const
{
    return "/StateManager/LockManager/TheatrePersis";
}
