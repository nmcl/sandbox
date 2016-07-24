/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperInitTermDriver.cc,v 1.1 1993/11/03 14:58:32 nmcl Exp $
 */

#include <Configure.h>

#include <System/string.h>
#include <System/iostream.h>
#include "Time.h"

#include <StubGen/ClientRpc.h>
#include <StubGen/RpcControl.h>

void PerformanceTest(int NoRep)
{
        int i;

	Time StartTime, FinishTime, ResultTime;

	StartTime.get();

	for (i = 0; i < NoRep; i++)
	{
            ClientRpc* RpcClient = new ClientRpc("OperParamSize");
            RpcControl* RpcCon = new RpcControl(RpcClient);

            delete RpcCon;
        }

	FinishTime.get();

	ResultTime = FinishTime - StartTime;

	cout<<"-------------------------------------------\n";
	cout<<"Number : "<<NoRep<<"\n";
	cout<<"Time : "<<ResultTime<<" sec\n";
	cout<<"-------------------------------------------\n";
}

int ParseArg(int argc, char *argv[], int *NoRepPtr)
{
	int err = 0;
	
	for (int i = 1; (i < argc) && (err == 0); i++)
	{
                if ((i < argc) && (strcmp(argv[i], "-num") == 0))
		{
			i++;
			if (i < argc)
				*NoRepPtr = atoi(argv[i]);
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

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	int err = 0;

	int		NoRep = 100;

	err = ParseArg(argc, argv, &NoRep);

	if (err == 0)
 		PerformanceTest(NoRep);

	return 0;
}
