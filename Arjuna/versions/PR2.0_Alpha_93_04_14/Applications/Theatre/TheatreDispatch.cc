/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TheatreDispatch.cc,v 1.3 1993/03/18 14:03:25 nmcl Exp $
 */

#include <InterViews/world.h>
#include <InterViews/event.h>
#include <InterViews/2.6/InterViews/interactor.h>

extern "C" {
void exit(int);
}
#include <iostream.h>

#include "SeatInter.h"
#include "TheatreInter.h"
#include "MenuItemDispatch.h"
#include "LogLevel1Event.h"
#include "LogLevel2Event.h"
#include "LogBaseEvent.h"
#include "LogVoid.h"

#include "Theatre.h"
#include "TheatreDispatch.h"

TheatreDispatch::TheatreDispatch() : TheatreInter(num_seats, this)
{
    int res;

    for  (int i = 0; i < 100; i++)
	selected[i] = 0;

    for  (i = 0; i < 100; i++)
	usedLevel1[i] = 0;

    for  (i = 0; i < 100; i++)
	usedLevel2[i] = 0;

    res = 0;

    ClientRpc* RpcClient = new ClientRpc("TheatrePersis");
#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
    RpcClient->SetTimeout(6000);
#endif
    if (TheatreHost != NULL)
	RpcClient->SetHost(TheatreHost);
    RpcControl* RpcCon = new RpcControl(RpcClient);
    
    if (TheatreNew)
	ThPersis = new TheatrePersis(num_seats, res, RpcCon);
    else
	ThPersis = new TheatrePersis(res, *TheatreUid, RpcCon);

    if (res == 0)
    {
	cerr << "Couldn't create Theatre Persistent\n";
	exit(1);
    }
    
    inLevel1 = 0;
    AALevel1 = 0;
    inLevel2 = 0;
    AALevel2 = 0;

    quit = false;
    actionLog = new LogInter;
    log_open = 0;

    enum Seat_Status status;
    AtomicAction A;
    A.Begin();

    for (i = 0; i < num_seats; i++)
    {
	ThPersis->Get_Seat_Status(i, status, res);

	if (res == 0)
	    Unknown(i);
	else
	{
	    switch (status)
	    {
	    case Reserved:
		Reserve(i);
		break;
	    case Unreserved:
		Unreserve(i);
		break;
	    }
	}
	Unselect(i);
    }
    A.End();
}

TheatreDispatch::~TheatreDispatch()
{
    if (ThPersis)
	delete ThPersis;
}

void TheatreDispatch::MenuDo(enum DispatchOper oper_num)
{
    switch (oper_num)
    {
    case new_oper:
	SetOperation("New");
	SetOperResult("Unimplemented");
	break;
    case open_oper:
	SetOperation("Open");
	SetOperResult("Unimplemented");
	break;
    case close_oper:
	SetOperation("Close");
	SetOperResult("Unimplemented");
	break;
	
    case blockbooking_begin_oper:
	SetOperation("Block Booking Begin");
	SetOperResult("");
	if (! inLevel1)
	{
	    SetMode("Block Booking");
	    AALevel1 = new AtomicAction;
	    AALevel1->Begin();
	    inLevel1 = 1;
	    actionLog->AddEvent(new LogLevel1Event(L1BeginEvent), 1, 1, 0, inLevel2);
	    SetOperResult("Done");
	}
	else
	    SetOperResult("Not Done");
	break;
    case blockbooking_commit_oper:
	SetOperation("Block Booking Commit");
	SetOperResult("");
	if (inLevel1 && (! inLevel2))
	{
	    SetMode("");
	    AALevel1->End();
	    delete AALevel1;
	    AALevel1 = 0;
	    inLevel1 = 0;
	    actionLog->AddEvent(new LogLevel1Event(L1CommitEvent), 0, 0, inLevel1, inLevel2);
	    for  (int i = 0; i < num_seats; i++)
		usedLevel1[i] = 0;
	    SetOperResult("Done");
	}
	else
	    SetOperResult("Not Done");
	break;
    case blockbooking_abort_oper:
	SetOperation("Block Booking Abort");
	SetOperResult("");
	if (inLevel1 && (! inLevel2))
	{
	    SetMode("");
	    AALevel1->Abort();
	    delete AALevel1;
	    AALevel1 = 0;
	    inLevel1 = 0;
	    actionLog->AddEvent(new LogLevel1Event(L1AbortEvent), 0, 0, inLevel1, inLevel2);
	    for  (int i = 0; i < num_seats; i++)
		if (usedLevel1[i])
		{
		    Unknown(i);
		    usedLevel1[i] = 0;
		}
	    SetOperResult("Done");
	}
	else
	    SetOperResult("Not Done");
	break;

    case subblockbooking_begin_oper:
	SetOperation("Sub-Block Booking Begin");
	SetOperResult("");
	if (inLevel1 && ! inLevel2)
	{
	    SetMode("Sub-Block Booking");
	    AALevel2 = new AtomicAction;
	    AALevel2->Begin();
	    inLevel2 = 1;
	    actionLog->AddEvent(new LogLevel2Event(L2BeginEvent, inLevel1), 1, 1, inLevel1, 0);
	    SetOperResult("Done");
	}
	else
	    SetOperResult("Not Done");
	break;
    case subblockbooking_commit_oper:
	SetOperation("Block Booking Commit");
	if (inLevel1 && inLevel2)
	{
	    SetMode("Sub-Block Booking");
	    AALevel2->End();
	    delete AALevel2;
	    AALevel2 = 0;
	    inLevel2 = 0;
	    actionLog->AddEvent(new LogLevel2Event(L2CommitEvent, inLevel1), 0, 0, inLevel1, inLevel2);
	    for  (int i = 0; i < num_seats; i++)
		if (usedLevel2[i])
		{
		    usedLevel1[i] = 1;
		    usedLevel2[i] = 0;
		}
	    SetOperResult("Done");	
	}
	else
	    SetOperResult("Not Done");
	break;
    case subblockbooking_abort_oper:
	SetOperation("Sub-Block Booking Abort");
	SetOperResult("");
	if (inLevel1 && inLevel2)
	{
	    SetMode("Block Booking");
	    AALevel2->Abort();
	    delete AALevel2;
	    AALevel2 = 0;
	    inLevel2 = 0;
	    actionLog->AddEvent(new LogLevel2Event(L2AbortEvent, inLevel1), 0, 0, inLevel1, inLevel2);
	    for  (int i = 0; i < num_seats; i++)
		if (usedLevel2[i])
		{
		    Unknown(i);
		    usedLevel2[i] = 0;
		}
	    SetOperResult("Done");
	}
	else
	    SetOperResult("Not Done");
	break;

    case reserve_oper:
    {
	int res    = 0; 
	int totres = 1; 

	SetOperation("Reserving");
	SetOperResult("");
	for (int i = 0; i < num_seats; i++)
	    if (selected[i])
	    {
		enum baseEventType eventType;
		ThPersis->Reserve_Seat(i, res);
		if (res == 0)
		    eventType = BaseAbortEvent;
		else
		    eventType = BaseCommitEvent;

		actionLog->AddEvent(new LogBaseEvent(eventType, inLevel1, inLevel2, i, 0), 1, 0, inLevel1, inLevel2);
		
		switch (res)
		{
		case 0:
		    Unknown(i);
		    Unselect(i);
		    selected[i] = 0;
		    totres = 0;
		    break;
		case 1:
		    Reserve(i);
		    Unselect(i);
		    selected[i] = 0;
		    break;
		case 2:
		    Reserve(i);
		    Select(i);
		    selected[i] = 1;
		    totres = 0;
		    break;
		}

		if (inLevel1)
		{
		    if (inLevel2)
			usedLevel2[i] = 1;
		    else
			usedLevel1[i] = 1;
		}
	    }

	if (totres)
	    SetOperResult("Done");
	else
	    SetOperResult("Not Done");
    }
	break;
    case unreserve_oper:
    {
	int res = 0;
	int totres = 1;

	SetOperation("Unreserving");
	SetOperResult("");
	for (int i = 0; i < num_seats; i++)
	    if (selected[i])
	    {
		enum baseEventType eventType;
		ThPersis->Unreserve_Seat(i, res);
		if (res == 0)
		    eventType = BaseAbortEvent;
		else
		    eventType = BaseCommitEvent;

		actionLog->AddEvent(new LogBaseEvent(eventType, inLevel1, inLevel2, i, 1), 1, 0, inLevel1, inLevel2);
		
		switch (res)
		{
		case 0:
		    Unknown(i);
		    Unselect(i);
		    selected[i] = 0;
		    totres = 0;
		    break;
		case 1:
		    Unreserve(i);
		    Unselect(i);
		    selected[i] = 0;
		    break;
		case 2:
		    Unreserve(i);
		    Select(i);
		    selected[i] = 1;
		    totres = 0;
		    break;
		}

		if (inLevel1)
		{
		    if (inLevel2)
			usedLevel2[i] = 1;
		    else
			usedLevel1[i] = 1;
		}
	    }

	if (totres)
	    SetOperResult("Done");
	else
	    SetOperResult("Not Done");
    }
	break;
    case redisplay_oper:
    {
	int res    = 0;
	int totres = 0;
	enum Seat_Status status;

	SetOperation("Redisplay");
	SetOperResult("");

	for (int i = 0; i < num_seats; i++)
	{
	    if (selected[i])
	    {
		enum baseEventType eventType;
		ThPersis->Get_Seat_Status(i, status, res);
		if (res == 0)
		    eventType = BaseAbortEvent;
		else
		    eventType = BaseCommitEvent;

		actionLog->AddEvent(new LogBaseEvent(eventType, inLevel1, inLevel2, i, 2), 1, 0, inLevel1, inLevel2);

		if (res == 0)
		    Unknown(i);
		else
		{
		    switch (status)
		    {
		    case Reserved:
			Reserve(i);
			break;
		    case Unreserved:
			Unreserve(i);
			break;
		    }
		}
	
		if (inLevel1)
		{
		    if (inLevel2)
			usedLevel2[i] = 1;
		    else
			usedLevel1[i] = 1;
		}
		Unselect(i);
		selected[i] = 0;
	    }
	}

	SetOperResult("Done");
    }
	break;
    case redisplay_all_oper:
    {
	int res    = 0;
	int totres = 0;	
	enum Seat_Status status;

	SetOperation("Redisplay All");
	SetOperResult("");

	if (inLevel1 || inLevel2)
	    SetOperResult("Not Done");
	else
	{
	    AtomicAction A;
	    A.Begin();
	    for (int i = 0; i < num_seats; i++)
	    {
		ThPersis->Get_Seat_Status(i, status, res);

		if (res == 0)
		    Unknown(i);
		else
		{
		    switch (status)
		    {
		    case Reserved:
			Reserve(i);
			break;
		    case Unreserved:
			Unreserve(i);
			break;
		    }
		}
		Unselect(i);
		selected[i] = 0;
	    }
	    
	    actionLog->AddEvent(new LogLevel1Event(L1BeginEvent), 1, 1, 0, 0);
	    actionLog->AddEvent(new LogBaseEvent(BaseCommitEvent, 1, 0, 0, 2), 1, 0, 1, 0);
	    actionLog->AddEvent(new LogVoid(1, 0), 0, 1, 1, 0);
	    actionLog->AddEvent(new LogBaseEvent(BaseCommitEvent, 1, 0, num_seats - 1, 2), 1, 0, 1, 0);
	    actionLog->AddEvent(new LogLevel1Event(L1CommitEvent), 0, 0, 0, 0);
	    A.End();

	    SetOperResult("Done");
	}
    }
	break;
	
    case log_open_oper:
	if (! log_open)
	{
	    GetWorld()->InsertApplication(actionLog);
	    log_open = 1;
	}

	SetLogStatus("Log open");
	break;
    case log_close_oper:
	if (log_open)
	{
	    GetWorld()->Remove(actionLog);
	    log_open = 0;
	}

	SetLogStatus("Log close");
	break;

    case nop_oper:
	break;
    case quit_oper:
	SetOperation("Redisplay");
	if (inLevel1 || inLevel2)
	    SetOperResult("Not Done");
	else
	    quit = true;
	break;
    default:
	break;
    }
}

void TheatreDispatch::SeatDo(int seat_num)
{
    selected[seat_num] = ! selected[seat_num];
    if (selected[seat_num])
	Select(seat_num);
    else
	Unselect(seat_num);
}

void TheatreDispatch::Run()
{
    Event e;

    do
    {
        Read(e);
        if (e.target != 0)
            e.target->Handle(e);
    }
    while (! quit);
}
