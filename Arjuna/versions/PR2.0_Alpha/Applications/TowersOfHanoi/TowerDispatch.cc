/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerDispatch.cc,v 1.3 1993/03/18 14:08:33 nmcl Exp $
 */

#include <Common/Error.h>

#include <InterViews/event.h>
#include <InterViews/sensor.h>

#include "TowerView.h"
#include "TowerDispatch.h"

TowerDispatch::TowerDispatch() : TowerInter(maxDisk)
{
    int  res;

    quit  = false;
    state = 0;
    pinsPersis = 0;

    res = 0;

    ClientRpc* RpcClientP = new ClientRpc("TowerPersis");
    if (PinHost != NULL)
        RpcClientP->SetHost(PinHost);
    RpcControl* RpcConP = new RpcControl(RpcClientP);

    pinsPersis = new TowerPersis(res, *PinUid, RpcConP);

    if (res == 0)
    {
	error_stream << "Unable to create pin\n";
	delete pinsPersis;
	exit(1); 
    }

    TowerState tState;

    res = 0;
    pinsPersis->GetState(tState, res);
    if (res == 0)
	SetInvalid();
    else
    {
	SetLocked(noneL);
	SetDisks(tState.state, tState.num);
    }

    input = new Sensor;

    input->Catch(DownEvent);
    input->CatchTimer(1, 0);
}

void TowerDispatch::Run()
{
    Event e;
    Draw();

    do
    {
	Read(e);
	if (e.target != 0)
	    e.target->Handle(e);
    }
    while (e.target != 0);
}

TowerDispatch::~TowerDispatch()
{
    if (pinsPersis)
	delete pinsPersis;
}

void TowerDispatch::Handle(Event& e)
{
    TowerState tState;

    switch (e.eventType)
    {
    case DownEvent:
	quit = true;
	break;
    case TimerEvent:
	switch (state)
	{
	case 0:
	    A = new AtomicAction;
	    A->Begin();

	    res = 0;
	    pinsPersis->GetState(tState, res);
	    if (res == 1)
	    {
		SetDisks(tState.state, tState.num);
		SetLocked(readL);
	    }
	    else
		SetLocked(confL);
	    Draw();
	    state = 1;
	    break;
	case 1:
	    if (res == 1)
		A->End();
	    else
	    {
		A->Abort();
		SetInvalid();
	    }
	    delete A;
	    SetLocked(noneL);
	    Draw();
	    if (quit)
		QuitRunning(e);
	    state = 2;
	    break;
	case 2:
	    state = 3;
	    break;
	case 3:
	    state = 4;
	    break;
	case 4:
	    state = 0;
	    break;
	}
    default:
	TowerInter::Handle(e);
	break;
    }
}
