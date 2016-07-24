/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperParamSizeDriver.cc,v 1.7 1995/02/22 14:56:06 ngdp Exp $
 */

#include <Config/Configure.h>

#include <System/string.h>
#include <System/iostream.h>
#include <StubGen/ClientRpc.h>
#include <StubGen/Abort.h>

#include "ElapsedTime.h"
#include "CpuTime.h"

#include "OperParamSize_stub.h"

void PerformanceTest(int norep, int size, char *host)
{
    int i;
    Param param1(size);
	
    ElapsedTime startTime, finishTime, resultTime;
    CpuTime startCpu, finishCpu, overallCpu;

    ClientRpc *RPCClient = new ClientRpc;
    RPCClient->setTimeout(3000);
    RPCClient->setRetry(2);
    RPCClient->setHost(host);
    RPCClient->setServiceName("OperParamSize");
    ClientRpcManager *RPCCtl = new ClientRpcManager(RPCClient);
    
    OperParamSize *TestObj = new OperParamSize(RPCCtl);

    startTime.get(); startCpu.get();

    for (i = 0; i < norep; i++)
    {
	TestObj->nullOp(param1);
    }

    
    finishTime.get(); finishCpu.get();

    resultTime = finishTime - startTime;
    overallCpu = finishCpu - startCpu;

    cout <<"-------------------------------------------\n";
    cout <<"Parameter Size : "<< size << "\n";
    cout << "Iterations : " << norep << "\n";
    cout << "Overall Time : " <<  resultTime << "\n";
    cout << "Overall Cpu Time : " <<  overallCpu << "\n";
    
    cout  << "-------------------------------------------\n" << flush;


    delete TestObj;
    delete RPCCtl;
    delete RPCClient;
}


int ParseArg(int argc, char *argv[], int& norep, int & size, char *&host)
{
	int err = 0;
	
	for (int i = 1; (i < argc) && (err == 0); i++)
	{
	    if ((i < argc) && (strcmp(argv[i], "-host") == 0))
	    {
		i++;
		if (i < argc)
		    host = argv[i];
		else
		{
		    err = 1;
		    cout<<"No host name given.\n";
		}			
	    }
	    else if ((i < argc) && (strcmp(argv[i], "-num") == 0))
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
	    else if ((i < argc) && (strcmp(argv[i], "-size") == 0))
	    {
		i++;
		if (i < argc)
		    size = atoi(argv[i]);
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

Boolean OnTimeout(RPC_Status, Int32)
{
    cerr << "RPC has timed out!" << endl;

    return FALSE;
}

int main(int argc, char *argv[])
{
	int err = 0;

	int		norep = 100;
	int size = 1024;
	
	char            *host = NULL;

        setHandler(OnTimeout);
	err = ParseArg(argc, argv, norep, size, host);

	if (err == 0)
 		PerformanceTest(norep, size, host);

	return 0;
}
