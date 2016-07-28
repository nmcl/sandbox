/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperInitTermDriver.cc,v 1.4 1995/02/22 14:56:04 ngdp Exp $
 */

#include <Config/Configure.h>

#include <System/string.h>
#include <System/iostream.h>

#include "ElapsedTime.h"
#include "CpuTime.h"

#include <StubGen/ClientRpc.h>
#include <StubGen/ClientRpcMan.h>

void PerformanceTest(int norep)
{
    int i;
	
    ElapsedTime startTime, finishTime, initTime, termTime;
    CpuTime startCpu, finishCpu, initCpu, termCpu;

    startTime.get(); startCpu.get();

    for (i = 0; i < norep; i++)
    {
	ElapsedTime st, et;
	CpuTime scpu, ecpu;
	
	st.get(); scpu.get();

	ClientRpc* RpcClient = new ClientRpc("OperParamSize");
	ClientRpcManager* RpcCon = new ClientRpcManager(RpcClient);
	
	et.get(); ecpu.get();
	initTime = initTime + (et - st);
	initCpu = initCpu + (ecpu - scpu);
	
	st.get(); scpu.get();

	delete RpcCon;
	delete RpcClient;
	
	et.get(); ecpu.get();
	termTime = termTime + (et - st);
	termCpu = termCpu + (ecpu - scpu);

    }

    
    finishTime.get(); finishCpu.get();

    cout <<"-------------------------------------------\n";
    cout << "Iterations : " << norep << "\n";
    cout << "Overall Time : " <<  finishTime - startTime << "\n";
    cout << "Overall Cpu Time : " <<  finishCpu - startCpu << "\n";
    cout << "Initiate Elapsed Time: " << initTime << "\n";
    cout << "Terminate Elapsed Time: " << termTime << "\n";
    cout << "Initiate Cpu Time:: " << initCpu << "\n";
    cout << "Terminate Cpu Time:: " << termCpu << "\n";
    cout  << "-------------------------------------------\n" << flush;
}

int ParseArg(int argc, char *argv[], int &norep)
{
    int err = 0;
	
    for (int i = 1; (i < argc) && (err == 0); i++)
    {
	if ((i < argc) && (strcmp(argv[i], "-num") == 0))
	{
	    i++;
	    if (i < argc)
		norep = atoi(argv[i]);
	    else
	    {
		err = 1;
		cout<<"No number given.\n";
	    }			
	}
	else
	{
	    err = 1;
	    cout<<"Invalid parameter ["<<argv[i]<<"].\n";
	}
    }
    
    return err;
}

int main(int argc, char *argv[])
{
    int err = 0;
    
    int		norep = 100;
    
    err = ParseArg(argc, argv, norep);
    
    if (err == 0)
	PerformanceTest(norep);
    
    return 0;
}
