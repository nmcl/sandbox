/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActionsDriver.cc,v 1.5 1995/03/06 14:40:48 ngdp Exp $
 */

#include <System/stdlib.h>
#include <Common/Boolean.h>
#include <Common/Uid.h>

#include <System/string.h>
#include <System/iostream.h>
#include "ElapsedTime.h"
#include "CpuTime.h"
#include "PerfVector.h"

#include <Arjuna/AtomicA.h>
#include <Arjuna/LockMan.h>

#include "OperActions.h"

enum OperTypes {OPERCORE, OPERNOP, OPERBE, OPERBA,
		OPERBRE, OPERBWE, OPERBRA, OPERBWA };

class PerformanceTest : public LockManager
{
public:
    PerformanceTest(ObjectType, size_t, int);
    PerformanceTest(const Uid&);
    
    ~PerformanceTest();

    void runTest(OperTypes, int, int, Boolean);
    
    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    ObjectType oType;
    size_t stateSize;
    int objCount;
    Uid *testUids;
    OperActions **testObjs;
    
    
};

PerformanceTest::PerformanceTest ( ObjectType ot, size_t size, int maxobjs )
                                 : LockManager(ot),
				   oType(ot),
				   stateSize(size),
				   objCount(maxobjs),
				   testUids(0),
				   testObjs(0)
{
    AtomicAction A;
	
    testObjs = (new OperActions *[objCount]);
    testUids = (new Uid[objCount]);
    
    for (int i = 0; i < objCount; i++)
    {
	testObjs[i] = new OperActions(stateSize, ot);
	testUids[i] = testObjs[i]->get_uid();
    }

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	/* This is to force an object store write */
	
	A.End();
    }
    else
    {
	cout << "PerformanceTest::PerformanceTest : Lock refused" << endl;
	objCount = 0;
	A.Abort();
    }
    
}


PerformanceTest::PerformanceTest ( const Uid& u )
                                 : LockManager(u),
				   oType(ANDPERSISTENT),
				   stateSize(0),
				   objCount(0),
				   testUids(0),
				   testObjs(0)
{
    AtomicAction A;
    
    A.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	A.End();
    }
    else
    {
	cout << "PerformanceTest::PerformanceTest(u) : Lock refused" << endl;
	objCount = 0;
	A.Abort();
    }
    
}

PerformanceTest::~PerformanceTest ()
{
    terminate();
    
    for (int i = 0; i < objCount; i++)
    {
	delete testObjs[i];
    }

    delete testUids;
    delete testObjs;
    
}

Boolean PerformanceTest::save_state( ObjectState& os, ObjectType )
{
    os.pack(stateSize);
    os.pack(objCount);
    os.pack(oType);
    
    for (int i = 0; i < objCount; i++)
    {
	testUids[i].pack(os);
    }
    return TRUE;
    
}

Boolean PerformanceTest::restore_state( ObjectState& os, ObjectType )
{
    int tempot;
    
    os.unpack(stateSize);
    os.unpack(objCount);
    os.unpack(tempot);
    oType = (ObjectType)tempot;

    testUids = new Uid[objCount];
    testObjs = (new OperActions *[objCount]);
    for (int i = 0; i < objCount; i++)
    {
	testUids[i].unpack(os);
	testObjs[i] = new OperActions(testUids[i]);
    }
    return TRUE;
    
}

const TypeName PerformanceTest::type() const
{
    return "/StateManager/LockManager/PerformanceTest";
}

ostream& operator<<(ostream& strm, OperTypes& OperType)
{
    switch (OperType)
    {
    case OPERCORE:
	strm << "Core Operations";
	break;
    case OPERBE:
	strm << "Begin; End";
	break;
    case OPERBA:
	strm << "Begin; Abort";
	break;
    case OPERNOP:
	strm<< "NOP";
	break;
    case OPERBRE:
	strm<< "Begin; Read Lock; End";
	break;
    case OPERBWE:
	strm<< "Begin; Write Lock; End";
	break;
    case OPERBRA:
	strm<< "Begin; Read Lock; Abort";
	break;
    case OPERBWA:
	strm<< "Begin; Write Lock; Abort";
	break;
    default:
	strm<<"??????";
    }

    return strm;
}

void PerformanceTest::runTest(OperTypes OperType, int NoRep, int noObjects, 
			      Boolean nested)
{
    AtomicAction A;
    int err = 0;
    int i, j;

    ElapsedTime startTime, finishTime, resultTime;
    CpuTime startCpu, finishCpu, overallCpu;
    PerformanceVector v;

    if (noObjects > objCount)
    {
	cout << "Too many objects requested: run with -new to increase. Using " << objCount << endl;
	noObjects = objCount;
    }
    
    if (nested)
	A.Begin();

    startTime.get(); startCpu.get();

    for (i = 0; i < NoRep; i++)
	for (j = 0; j < noObjects; j++)
	{
	    switch (OperType)
	    {
	    case OPERBE:
		v += testObjs[j]->be();
		break;
	    case OPERBA:
		v += testObjs[j]->ba();
		break;
	    case OPERCORE:
		v += testObjs[j]->core();
		break;
	    case OPERNOP :
		v += testObjs[j]->nop();
		break;
	    case OPERBRE :
		v += testObjs[j]->bre();
		break;
	    case OPERBWE :
		v += testObjs[j]->bwe();
		break;
	    case OPERBRA :
		v += testObjs[j]->bra();
		break;
	    case OPERBWA :
		v += testObjs[j]->bwa();
		break;
	    default :
		err = 1;
	    }
	}
    
    finishTime.get(); finishCpu.get();

    if (nested)
	if(A.End() != COMMITTED)
	    cout<<"Unable to commit top-level action\n";

    resultTime = finishTime - startTime;
    overallCpu = finishCpu - startCpu;

    cout <<"-------------------------------------------\n";
    cout <<"Operation : "<<OperType;
    if (nested)
	cout <<" (Nested)\n";
    else
	cout <<" (Top-Level)\n";
    cout << "Number of Objects involved : " << noObjects << endl;
    cout <<"Object State Size : "<< stateSize << "\n";
    cout << "Object type : " << oType << "\n";
    cout << "Iterations : " << NoRep << "\n";
    cout << "Overall Time : " <<  resultTime << "\n";
    cout << "Overall Cpu Time : " <<  overallCpu << "\n";
    cout << "Performance Vector : \n"  <<  v;
    
    cout  << "-------------------------------------------\n" << flush;

}

int AtoOper(OperTypes& OperTypePtr, char* arg)
{
	int err = 0;

	if (strcmp("CORE", arg) == 0)
	    OperTypePtr = OPERCORE;
	else if (strcmp("BE", arg) == 0)
		OperTypePtr = OPERBE;
	else if (strcmp("BA", arg) == 0)
		OperTypePtr = OPERBA;
	else if (strcmp("NOP", arg) == 0)
		OperTypePtr = OPERNOP;
	else if (strcmp("BRE", arg) == 0)
		OperTypePtr = OPERBRE;
	else if (strcmp("BWE", arg) == 0)
		OperTypePtr = OPERBWE;
	else if (strcmp("BRA", arg) == 0)
		OperTypePtr = OPERBRA;
	else if (strcmp("BWA", arg) == 0)
		OperTypePtr = OPERBWA;
	else
	{
		err = 1;
		cout<<"Unknown operation specified.\n";
	}

	return err;
}

int ParseArg(int argc, char *argv[], Uid& testUid, OperTypes& OperType, 
	     int& NoRep,size_t &stateSize, ObjectType& ot, int& noObjs, 
	     Boolean& nested, Boolean& newTest)
{
    int err = 0;
	
    for (int i = 1; (i < argc) && (err == 0); i++)
    {
	if ((i < argc) && (strcmp(argv[i], "-oper") == 0))
	{
	    i++;
	    if (i < argc)
		err = AtoOper(OperType, argv[i]);
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
		NoRep = atoi(argv[i]);
	    else
	    {
		err = 1;
		cout<<"No number given.\n";
	    }			
	}
	else if ((i < argc) && (strcmp(argv[i], "-obj") == 0))
	{
	    i++;
	    if (i < argc)
		noObjs = atoi(argv[i]);
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
		stateSize = atoi(argv[i]);
	    else
	    {
		err = 1;
		cout<<"No size given.\n";
	    }			
	}
	else if ((i < argc) && (strcmp(argv[i], "-uid") == 0))
	{
	    i++;
	    if (i < argc)
	    {
		newTest = FALSE;
		
		testUid = *new Uid(argv[i]);
	    }
	    
	    else
	    {
		err = 1;
		cout<<"No uid given.\n";
	    }			
	}
	else if ((i < argc) && (strcmp(argv[i], "-rec") == 0))
	    ot = RECOVERABLE;
	else if ((i < argc) && (strcmp(argv[i], "-per") == 0))
	    ot = ANDPERSISTENT;
	else if ((i < argc) && (strcmp(argv[i], "-top") == 0))
	    nested = FALSE;
	else if ((i < argc) && (strcmp(argv[i], "-nest") == 0))
	    nested = TRUE;
	else if ((i < argc) && (strcmp(argv[i], "-new") == 0))
	    newTest = TRUE;
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
    
    OperTypes	OperType  = OPERCORE;
    int		NoRep     = 1;
    int noObjs = 100;
    Uid u(NIL_UID);
    Boolean newTest = TRUE;
    
    size_t          stateSize = 1024;
    ObjectType      ot        = ANDPERSISTENT;
    Boolean         nested    = FALSE;
    
    err = ParseArg(argc, argv, u, OperType, NoRep, stateSize, ot, 
		   noObjs, nested, newTest);
    
    if (err == 0)
    {
	if (newTest)
	{
	    PerformanceTest pt(ot, stateSize, noObjs);
	    
	    cout << "Test uid is : " << pt.get_uid() << endl;
	    
	    pt.runTest(OperType, NoRep, noObjs, nested);
	}
	else
	{
	    PerformanceTest pt(u);
	    
	    pt.runTest(OperType,NoRep, noObjs, nested);
	}
    }
    
    return 0;
}
