/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersDispatch.cc,v 1.2 1994/01/06 12:29:31 ngdp Exp $
 */

#include <System/iostream.h>

#include <InterViews/event.h>
#include <InterViews/sensor.h>

#include "TowersOfHanoi.h"
#include "TowersDispatch.h"

#include <StubGen/ClientRpc.h>
#include <Arjuna/AtomicA.h>

TowersDispatch::TowersDispatch(int isReplicated)
{
    res          = 0;
    workStack    = 0;
    state        = 0;
    quit         = false;
    A            = 0;

    workStack = NULL;
    for (int i = 0; i < 4; i++)
	pinsPersis[i] = NULL;

    res = 0;

    ClientRpc* RpcClientWS = new ClientRpc("WorkStack");
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    RpcClientWS->SetTimeout(8000);
    RpcClientWS->SetRetry(3);
#endif
#endif
    if (WSHost != NULL)
        RpcClientWS->SetHost(WSHost);
    RpcConWS = new RpcControl(RpcClientWS);

    if (TowersNew)
	workStack    = new WorkStack(4*(maxDisk + 1), res, RpcConWS);
    else
	workStack    = new WorkStack(res, *WSUid, FALSE, RpcConWS);

    if (res == 0)
    {
	cerr << "Unable to create work stack\n";
	clean_and_exit(1); 
    }

    pinsPersis[0] = 0;
    pinsPersis[1] = 0;
    pinsPersis[2] = 0;
    pinsPersis[3] = 0;

    res = 0;
    ClientRpc* RpcClientP1 = new ClientRpc("TowerPersis");
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    RpcClientP1->SetTimeout(8000);
    RpcClientP1->SetRetry(3);
#endif
#endif
    if (Pin1Host != NULL)
        RpcClientP1->SetHost(Pin1Host);
    RpcConP1 = new RpcControl(RpcClientP1);

    if (TowersNew)
	pinsPersis[0] = new TowerPersis(res, RpcConP1);
    else
	pinsPersis[0] = new TowerPersis(res, *Pin1Uid, FALSE, RpcConP1);

    if (res == 0)
    {
	cerr << "Unable to create pin 1\n";
	clean_and_exit(1); 
    }

    res = 0;
    ClientRpc* RpcClientP2 = new ClientRpc("TowerPersis");
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    RpcClientP2->SetTimeout(8000);
    RpcClientP2->SetRetry(3);
#endif
#endif
    if (Pin2Host != NULL)
        RpcClientP2->SetHost(Pin2Host);
    RpcConP2 = new RpcControl(RpcClientP2);

    if (TowersNew)
	pinsPersis[1] = new TowerPersis(res, RpcConP2);
    else
	pinsPersis[1] = new TowerPersis(res, *Pin2Uid, isReplicated, RpcConP2);

    if (res == 0)
    {
	cerr << "Unable to create pin 2\n";
	clean_and_exit(1); 
    }
    
    res = 0;
    ClientRpc* RpcClientP3 = new ClientRpc("TowerPersis");
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    RpcClientP3->SetTimeout(8000);
    RpcClientP3->SetRetry(3);
#endif
#endif
    if (Pin3Host != NULL)
        RpcClientP3->SetHost(Pin3Host);
    RpcConP3 = new RpcControl(RpcClientP3);

    if (TowersNew)
	pinsPersis[2] = new TowerPersis(res, RpcConP3);
    else
	pinsPersis[2] = new TowerPersis(res, *Pin3Uid, isReplicated, RpcConP3);

    if (res == 0)
    {
	cerr << "Unable to create pin 3\n";
	clean_and_exit(1); 
    }

    res = 0;
    ClientRpc* RpcClientP4 = new ClientRpc("TowerPersis");
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    RpcClientP4->SetTimeout(8000);
    RpcClientP4->SetRetry(3);
#endif
#endif
    if (Pin4Host != NULL)
        RpcClientP4->SetHost(Pin4Host);
    RpcConP4 = new RpcControl(RpcClientP4);

    if (TowersNew)
	pinsPersis[3] = new TowerPersis(res, RpcConP4);
    else
	pinsPersis[3] = new TowerPersis(res, *Pin4Uid, FALSE, RpcConP4);

    if (res == 0)
    {
	cerr << "Unable to create pin 4 (in Hand)\n";
	clean_and_exit(1); 
    }

    TowerState tState;

    if (TowersNew)
    {
	for (int i = maxDisk; i > 0; i--)
	{
	    res = 0;
	    pinsPersis[0]->PushDisk(i, tState, res);
	    if (res == 0)
	    {
		cerr << "(Init Pin) Unable to Push Disk " << i << "\n";
		clean_and_exit(1); 
	    }
	}
    }

    res = 0;
    pinsPersis[0]->GetState(tState, res);
    if (res == 0)
	SetInvalid(0);
    else
    {
	SetLocked(0, noneL);
	SetDisks(0, tState.state, tState.num);
    }

    res = 0;
    pinsPersis[1]->GetState(tState, res);
    if (res == 0)
	SetInvalid(1);
    else
    {
	SetLocked(1, noneL);
	SetDisks(1, tState.state, tState.num);
    }

    res = 0;
    pinsPersis[2]->GetState(tState, res);
    if (res == 0)
	SetInvalid(2);
    else
    {
	SetLocked(2, noneL);
	SetDisks(2, tState.state, tState.num);
    }

    res = 0;
    pinsPersis[3]->GetState(tState, res);
    if (res == 0)
	SetInvalid(3);
    else
    {
	SetLocked(3, noneL);
	SetDisks(3, tState.state, tState.num);
    }

    if (TowersNew)
    {
	struct Work work;
	work.start   = 1;
	work.pinTo   = 0; work.pinFrom = 1;
	work.pinTmp  = 2; work.number  = maxDisk;

	res = 0;
	workStack->PushWork(work, res);
	if (res == 0)
	{
	    cerr << "(Init Work) Unable to Push Work\n";
	    clean_and_exit(1); 
	}
    }
    
    input = new Sensor;

    input->Catch(DownEvent);
    input->CatchTimer(1, 0);
    input->Catch(KeyEvent);
}

void TowersDispatch::Run()
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

TowersDispatch::~TowersDispatch()
{

    if (workStack)
	delete workStack;

    if (RpcConWS)
	delete RpcConWS;

    if (pinsPersis[0])
	delete pinsPersis[0];
    if (pinsPersis[1])
	delete pinsPersis[1];
    if (pinsPersis[2])
	delete pinsPersis[2];
    if (pinsPersis[3])
	delete pinsPersis[3];

    if (RpcConP1)
	delete RpcConP1;
    if (RpcConP2)
	delete RpcConP2;
    if (RpcConP3)
	delete RpcConP3;
    if (RpcConP4)
	delete RpcConP4;
}

void TowersDispatch::Handle(Event& e)
{
    int doneWork;
    
    switch (e.eventType)
    {
    case DownEvent:
	quit = true;
	break;
    case TimerEvent:
	switch (state)
	{
	case 0:
	case 1:
	    doneWork = false;
	    if (state == 0)
	    {
		A = new AtomicAction();
		A->Begin();
	    }
	    
	    res = 1;
	    while ((! doneWork) && (res == 1))
	    {
		Work work;
		
		res = 0;
		workStack->PopWork(work, res);
		
		if (res == 1)
		{
		    if (work.start)
		    {
			Work task1, task2;
			
			task2.start   = 1;
			task2.pinTo   = (work.pinTo  + 2) % 3; task2.pinFrom = (work.pinFrom + 2) % 3;
			task2.pinTmp  = (work.pinTmp + 2) % 3; task2.number  = work.number;
			
			res = 0;
			workStack->PushWork(task2, res);
			
			if (res == 1)
			{
			    task1.start   = 0;
			    task1.pinTo   = (work.pinTo  + 2) % 3; task1.pinFrom = (work.pinFrom + 2) % 3;
			    task1.pinTmp  = (work.pinTmp + 2) % 3; task1.number  = work.number;
			    
			    res = 0;
			    workStack->PushWork(task1, res);
			}
		    }
		    else
		    {
			if (work.number == 1)
			{
			    int size;
			    TowerState tState;
			    
			    pinTmp = work.pinTmp;
			    res = 0;
			    pinsPersis[work.pinFrom]->PopDisk(size, tState, res);
			    if (res == 1)
			    {
				SetDisks(work.pinFrom, tState.state, tState.num);
				SetLocked(work.pinFrom, writeL);
				res = 0;
				pinsPersis[work.pinTo]->PushDisk(size, tState, res);
				if (res == 1)
				{
				    SetDisks(work.pinTo, tState.state, tState.num);
				    SetLocked(work.pinTo, writeL);
				}
				else
				    SetLocked(work.pinTo, confL);
			    }
			    else 
				SetLocked(work.pinFrom, confL);
			    
			    doneWork   = true;
			}
			else
			{
			    Work task1, task1x, task2, task3, task4, task4x;
			    
			    if (work.number == 2)
			    {
				task4x.start   = 0;
				task4x.pinTo   = work.pinTo;   task4x.pinFrom = 3;
				task4x.pinTmp  = work.pinFrom; task4x.number  = 1;
				
				res = 0;
				workStack->PushWork(task4x, res);
				
				if (res == 1)
				{
				    task4.start    = 0;
				    task4.pinTo    = 3;            task4.pinFrom  = work.pinTmp;
				    task4.pinTmp   = work.pinFrom; task4.number   = 1;
				    
				    res = 0;
				    workStack->PushWork(task4, res);
				}
			    }
			    else
			    {
				task4.start   = 0;
				task4.pinTo   = work.pinTo;   task4.pinFrom = work.pinTmp;
				task4.pinTmp  = work.pinFrom; task4.number  = work.number - 1;
				
				res = 0;
				workStack->PushWork(task4, res);
			    }
			    
			    if (res == 1)
			    {
				task3.start   = 0;
				task3.pinTo   = work.pinTo;   task3.pinFrom = 3;
				task3.pinTmp  = work.pinTmp;  task3.number  = 1;
				
				res = 0;
				workStack->PushWork(task3, res);
			    }
			    
			    if (res == 1)
			    {
				task2.start   = 0;
				task2.pinTo   = 3;            task2.pinFrom = work.pinFrom;
				task2.pinTmp  = work.pinTmp;  task2.number  = 1;
				
				res = 0;
				workStack->PushWork(task2, res);
			    }
			    
			    if (res == 1)
			    {
				if (work.number == 2)
				{
				    task1x.start   = 0;
				    task1x.pinTo   = work.pinTmp;  task1x.pinFrom = 3;
				    task1x.pinTmp  = work.pinTo;   task1x.number  = 1;
				    
				    res = 0;
				    workStack->PushWork(task1x, res);
				    
				    if (res == 1)
				    {
					task1.start    = 0;
					task1.pinTo    = 3;            task1.pinFrom  = work.pinFrom;
					task1.pinTmp   = work.pinTo;   task1.number   = 1;
					
					res = 0;
					workStack->PushWork(task1, res);
				    }
				}
				else
				{
				    task1.start    = 0;
				    task1.pinTo    = work.pinTmp;  task1.pinFrom  = work.pinFrom;
				    task1.pinTmp   = work.pinTo;   task1.number   = work.number - 1;
				    
				    res = 0;
				    workStack->PushWork(task1, res);
				}
			    }
			}
		    }
		}
	    }
	    
	    if (res == 0)
		state = 3;
	    else 
		state = state + 1;
	    Draw();
	    break;
	case 2:
	{
	    TowerState tState;

	    res = 0;
	    pinsPersis[pinTmp]->GetState(tState, res);
	    if (res == 1)
	    {
		SetDisks(pinTmp, tState.state, tState.num);
		SetLocked(pinTmp, readL);
	    }
	    else
		SetLocked(pinTmp, confL);
	}
	    state = 3;
	    Draw();
	    break;
	case 3:
	{
	    if (res == 0)
	    {
		A->Abort();
		
		for (int k = 0; k < 4; k++)	
		    SetInvalid(k);
	    }
	    else
	    {
		if (A->End() != COMMITTED)
		    cerr << "Atomic Action: End (Finish) BAD\n";
	    }
	    delete A;
	    A = 0;
	    
	    for (int j = 0; j < 4; j++)	
		SetLocked(j, noneL);
	    
	    Draw();
	    state = 0;
	    if (quit)
		QuitRunning(e);
	}
	    break;
	default:
	    cerr << "TowersDispatch::Handle State default\n";
	    state = 0;
	    break;
	}
	break;
    default:
	TowersInter::Handle(e);
	break;
    }
}

void TowersDispatch::clean_and_exit(int code)
{
    if (workStack != NULL)
	delete workStack;

    for (int i = 0; i < 4; i++)
	if (pinsPersis[i] != NULL)
	    delete pinsPersis[i];

    exit(code);
}
