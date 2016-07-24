/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperParamSizeDriver.cc,v 1.1 1993/11/03 14:58:37 nmcl Exp $
 */

#include <Configure.h>

#include <System/string.h>
#include <System/iostream.h>
#include <StubGen/ClientRpc.h>
#include <StubGen/Abort.h>

#include "Time.h"

#include "OperParamSize_stub.h"

enum OperTypes {OperNOPChar0, OperNOPChar1, OperNOPChar2, OperNOPChar4,
                OperNOPChar8, OperNOPChar16, OperNOPChar32, OperNOPChar64,
                OperNOPChar128, OperNOPChar256, OperNOPChar512, OperNOPChar1024,
                OperNOPChar2048, OperNOPChar4096, OperNOPChar8192, OperNOPChar16384,
                OperNOPChar32768, OperNOPChar65536, OperNOPChar131072,
		OperNOPChar262144};

ostream& operator<<(ostream& strm, OperTypes& OperType)
{
	switch (OperType)
	{
		case OperNOPChar0 :
			strm<<"NOPChar0";
			break;
		case OperNOPChar1 :
			strm<<"NOPChar1";
			break;
		case OperNOPChar2 :
			strm<<"NOPChar2";
			break;
		case OperNOPChar4 :
			strm<<"NOPChar4";
			break;
		case OperNOPChar8 :
			strm<<"NOPChar8";
			break;
		case OperNOPChar16 :
			strm<<"NOPChar16";
			break;
		case OperNOPChar32 :
			strm<<"NOPChar32";
			break;
		case OperNOPChar64 :
			strm<<"NOPChar64";
			break;
		case OperNOPChar128 :
			strm<<"NOPChar128";
			break;
		case OperNOPChar256 :
			strm<<"NOPChar256";
			break;
		case OperNOPChar512 :
			strm<<"NOPChar512";
			break;
		case OperNOPChar1024 :
			strm<<"NOPChar1024";
			break;
		case OperNOPChar2048 :
			strm<<"NOPChar2048";
			break;
		case OperNOPChar4096 :
			strm<<"NOPChar4096";
			break;
		case OperNOPChar8192 :
			strm<<"NOPChar8192";
			break;
		case OperNOPChar16384 :
			strm<<"NOPChar16384";
			break;
		case OperNOPChar32768 :
			strm<<"NOPChar32768";
			break;
	        case OperNOPChar65536:
			strm<<"NOPChar65536";
			break;
	        case OperNOPChar131072:
			strm<<"NOPChar131072";
			break;
	        case OperNOPChar262144:
			strm<<"NOPChar262144";
			break;
		default :
			strm<<"??????";
	}

	return strm;
}

void PerformanceTest(OperTypes OperType, int NoRep, char *host)
{
        char data[262144];
    
	char c1;
	char *c2 = data;
	char *c4 = data;
	char *c8 = data;
	char *c16 = data;
	char *c32 = data;
	char *c64 = data;
	char *c128 = data;
	char *c256 = data;
	char *c512 = data;
	char *c1024 = data;
	char *c2048 = data;
	char *c4096 = data;
	char *c8192 = data;
	char *c16384 = data;
	char *c32768 = data;
	char *c65536 = data;
	char *c131072 = data;
	char *c262144 = data;
	
	int i;

	c1 = '\0';

	for (i = 0; i < 262144; i++)
	    data[i] = '\0';

	int err = 0;

	ClientRpc *RPCClient = new ClientRpc;
	RPCClient->SetTimeout(3000);
	RPCClient->SetRetry(2);
	RPCClient->SetHost(host);
	RPCClient->SetServiceName("OperParamSize");
        RpcControl *RPCCtl = new RpcControl(RPCClient);

	OperParamSize *TestObj = new OperParamSize(RPCCtl);

	Time StartTime, FinishTime, ResultTime;

	StartTime.get();

	for (i = 0; (err == 0) && (i < NoRep); i++)
	{
		switch (OperType)
		{
			case OperNOPChar0 :
				TestObj->NOPChar0();
				break;
			case OperNOPChar1 :
				TestObj->NOPChar1(c1);
				break;
			case OperNOPChar2 :
				TestObj->NOPChar2(c2);
				break;
			case OperNOPChar4 :
				TestObj->NOPChar4(c4);
				break;
			case OperNOPChar8 :
				TestObj->NOPChar8(c8);
				break;
			case OperNOPChar16 :
				TestObj->NOPChar16(c16);
				break;
			case OperNOPChar32 :
				TestObj->NOPChar32(c32);
				break;
			case OperNOPChar64 :
				TestObj->NOPChar64(c64);
				break;
			case OperNOPChar128 :
				TestObj->NOPChar128(c128);
				break;
			case OperNOPChar256 :
				TestObj->NOPChar256(c256);
				break;
			case OperNOPChar512 :
				TestObj->NOPChar512(c512);
				break;
			case OperNOPChar1024 :
				TestObj->NOPChar1024(c1024);
				break;
			case OperNOPChar2048 :
				TestObj->NOPChar2048(c2048);
				break;
			case OperNOPChar4096 :
				TestObj->NOPChar4096(c4096);
				break;
			case OperNOPChar8192 :
				TestObj->NOPChar8192(c8192);
				break;
			case OperNOPChar16384 :
				TestObj->NOPChar16384(c16384);
				break;
			case OperNOPChar32768 :
				TestObj->NOPChar32768(c32768);
				break;
			case OperNOPChar65536 :
				TestObj->NOPChar65536(c65536);
				break;
			case OperNOPChar131072 :
				TestObj->NOPChar131072(c131072);
				break;
			case OperNOPChar262144 :
				TestObj->NOPChar262144(c262144);
				break;
			default :
				err = 1;
		}
	}

	FinishTime.get();

	ResultTime = FinishTime - StartTime;

	cout<<"-------------------------------------------\n";
	cout<<"Operation : "<<OperType<<"\n";
	cout<<"Number : "<<NoRep<<"\n";
	cout<<"Time : "<<ResultTime<<" sec\n";
	cout<<"Host : "<<host<<"\n";
	cout<<"-------------------------------------------\n";

        delete TestObj;
        delete RPCCtl;
}

int AtoOper(OperTypes *OperTypePtr, char* arg)
{
	int err = 0;

	if (strcmp("NOPChar0", arg) == 0)
		*OperTypePtr = OperNOPChar0;
	else if (strcmp("NOPChar1", arg) == 0)
		*OperTypePtr = OperNOPChar1;
	else if (strcmp("NOPChar2", arg) == 0)
		*OperTypePtr = OperNOPChar2;
	else if (strcmp("NOPChar4", arg) == 0)
		*OperTypePtr = OperNOPChar4;
	else if (strcmp("NOPChar8", arg) == 0)
		*OperTypePtr = OperNOPChar8;
	else if (strcmp("NOPChar16", arg) == 0)
		*OperTypePtr = OperNOPChar16;
	else if (strcmp("NOPChar32", arg) == 0)
		*OperTypePtr = OperNOPChar32;
	else if (strcmp("NOPChar64", arg) == 0)
		*OperTypePtr = OperNOPChar64;
	else if (strcmp("NOPChar128", arg) == 0)
		*OperTypePtr = OperNOPChar128;
	else if (strcmp("NOPChar256", arg) == 0)
		*OperTypePtr = OperNOPChar256;
	else if (strcmp("NOPChar512", arg) == 0)
		*OperTypePtr = OperNOPChar512;
	else if (strcmp("NOPChar1024", arg) == 0)
		*OperTypePtr = OperNOPChar1024;
	else if (strcmp("NOPChar2048", arg) == 0)
		*OperTypePtr = OperNOPChar2048;
	else if (strcmp("NOPChar4096", arg) == 0)
		*OperTypePtr = OperNOPChar4096;
	else if (strcmp("NOPChar8192", arg) == 0)
		*OperTypePtr = OperNOPChar8192;
	else if (strcmp("NOPChar16384", arg) == 0)
		*OperTypePtr = OperNOPChar16384;
	else if (strcmp("NOPChar32768", arg) == 0)
		*OperTypePtr = OperNOPChar32768;
	else if (strcmp("NOPChar65536", arg) == 0)
		*OperTypePtr = OperNOPChar65536;
	else if (strcmp("NOPChar131072", arg) == 0)
		*OperTypePtr = OperNOPChar131072;
	else if (strcmp("NOPChar262144", arg) == 0)
		*OperTypePtr = OperNOPChar262144;
	else
	{
		err = 1;
		cout<<"Unknown operation specified.\n";
	}

	return err;
}

int ParseArg(int argc, char *argv[], OperTypes *OperTypePtr, int *NoRepPtr, char **host)
{
	int err = 0;
	
	for (int i = 1; (i < argc) && (err == 0); i++)
	{
		if ((i < argc) && (strcmp(argv[i], "-oper") == 0))
		{
			i++;
			if (i < argc)
				err = AtoOper(OperTypePtr, argv[i]);
			else
			{
				err = 1;
				cout<<"No operation given.\n";
			}
		}
		else if ((i < argc) && (strcmp(argv[i], "-host") == 0))
		{
			i++;
			if (i < argc)
				*host = argv[i];
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

Boolean OnTimeout()
{
    cerr << "RPC has timed out!" << endl;

    return FALSE;
}

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	int err = 0;

	OperTypes	OperType = OperNOPChar0;
	int		NoRep = 100;
	char            *host = NULL;

        setHandler(OnTimeout);
	err = ParseArg(argc, argv, &OperType, &NoRep, &host);

	if (err == 0)
 		PerformanceTest(OperType, NoRep, host);

	return 0;
}
