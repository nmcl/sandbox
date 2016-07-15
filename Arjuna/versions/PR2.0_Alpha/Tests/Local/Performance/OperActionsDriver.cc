/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActionsDriver.cc,v 1.2 1993/03/18 15:07:52 n048z Exp $
 */

#include <Common/Boolean.h>

#include <System/string.h>
#include <System/iostream.h>
#include "Time.h"

#include <Arjuna/AtomicA.h>

#include "OperActions.h"

enum OperTypes {OperNOP, OperBRE, OperBWE, OperBRA, OperBWA };

ostream& operator<<(ostream& strm, OperTypes& OperType)
{
	switch (OperType)
	{
		case OperNOP :
			strm<<"NOP";
			break;
		case OperBRE :
			strm<<"Begin; Read Lock; End";
			break;
		case OperBWE :
			strm<<"Begin; Write Lock; End";
			break;
		case OperBRA :
			strm<<"Begin; Read Lock; Abort";
			break;
		case OperBWA :
			strm<<"Begin; Write Lock; Abort";
			break;

		default :
			strm<<"??????";
	}

	return strm;
}

void PerformanceTest(OperTypes OperType, int NoRep, size_t stateSize, ObjectType ot, Boolean nested)
{
    AtomicAction A;
    OperActions	TestObj(stateSize, ot);
    
    int err = 0;

    Time StartTime, FinishTime, ResultTime;

    if (nested)
	A.Begin();

    switch (OperType)
    {
        case OperNOP :
	    TestObj.NOP();
	    break;
	case OperBRE :
	    TestObj.BRE();
	    break;
	case OperBWE :
	    TestObj.BWE();
	    break;
	case OperBRA :
	    TestObj.BRA();
	    break;
	case OperBWA :
	    TestObj.BWA();
	    break;
	default :
	    err = 1;
    }

    StartTime.get();

    for (int i = 0; i < NoRep; i++)
    {
	switch (OperType)
	{
	    case OperNOP :
		TestObj.NOP();
		break;
	    case OperBRE :
		TestObj.BRE();
		break;
	    case OperBWE :
		TestObj.BWE();
		break;
	    case OperBRA :
		TestObj.BRA();
		break;
	    case OperBWA :
		TestObj.BWA();
		break;
		default :
		err = 1;
	}
    }

    FinishTime.get();

    if (nested)
	if(A.End() != COMMITTED)
	    cout<<"Unable to commit top-level action\n";

    ResultTime = FinishTime - StartTime;

    cout<<"-------------------------------------------\n";
    cout<<"Operation : "<<OperType;
    if (nested)
	cout<<" (Nested)\n";
    else
	cout<<" (Top-Level)\n";
    cout<<"Object State Size "<<stateSize<<"\n";
    cout<<"Object type "<<ot<<"\n";
    cout<<"Number : "<<NoRep<<"\n";
    cout<<"Time : "<<ResultTime<<" sec\n";
    cout<<"-------------------------------------------\n" << flush;
}

int AtoOper(OperTypes *OperTypePtr, char* arg)
{
	int err = 0;

	if (strcmp("NOP", arg) == 0)
		*OperTypePtr = OperNOP;
	else if (strcmp("BRE", arg) == 0)
		*OperTypePtr = OperBRE;
	else if (strcmp("BWE", arg) == 0)
		*OperTypePtr = OperBWE;
	else if (strcmp("BRA", arg) == 0)
		*OperTypePtr = OperBRA;
	else if (strcmp("BWA", arg) == 0)
		*OperTypePtr = OperBWA;
	else
	{
		err = 1;
		cout<<"Unknown operation specified.\n";
	}

	return err;
}

int ParseArg(int argc, char *argv[], OperTypes *OperTypePtr, int *NoRepPtr,
             size_t *stateSizePtr, ObjectType *otPtr, Boolean *nestedPtr)
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
		else if ((i < argc) && (strcmp(argv[i], "-size") == 0))
		{
			i++;
			if (i < argc)
				*stateSizePtr = atoi(argv[i]);
			else
			{
				err = 1;
				cout<<"No size given.\n";
			}			
		}
		else if ((i < argc) && (strcmp(argv[i], "-rec") == 0))
			*otPtr = RECOVERABLE;
		else if ((i < argc) && (strcmp(argv[i], "-per") == 0))
			*otPtr = ANDPERSISTENT;
		else if ((i < argc) && (strcmp(argv[i], "-top") == 0))
			*nestedPtr = FALSE;
		else if ((i < argc) && (strcmp(argv[i], "-nest") == 0))
			*nestedPtr = TRUE;
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

	OperTypes	OperType  = OperBRE;
	int		NoRep     = 100;
	size_t          stateSize = 1000;
        ObjectType      ot        = ANDPERSISTENT;
        Boolean         nested    = TRUE;

	err = ParseArg(argc, argv, &OperType, &NoRep, &stateSize, &ot, &nested);

	if (err == 0)
 		PerformanceTest(OperType, NoRep, stateSize, ot, nested);

	return 0;
}
