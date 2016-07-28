/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActionsDriver.cc,v 1.9 1995/07/12 09:02:22 ngdp Exp $
 */

#include <System/stdlib.h>
#include <Common/Boolean.h>

#include <System/string.h>
#include <System/iostream.h>
#include <Common/Utility.h>
#include "ElapsedTime.h"
#include "CpuTime.h"
#include "PerfVector.h"

#include <Arjuna/AtomicA.h>
#include <Arjuna/LockMan.h>
#include <StubGen/ClientRpc.h>

#include "OperActions_stub.h"

enum OperTypes {OPERCORE, OPERNOP, OPERBE, OPERBA,
		OPERBRE, OPERBWE, OPERBRA, OPERBWA };

class PerformanceTest : public LockManager
{
public:
    PerformanceTest(ObjectType, size_t, int, char *);
    PerformanceTest(const Uid&);
    
    ~PerformanceTest();

    void runTest(OperTypes, int, int, Boolean);
    
    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    char *host;
    ObjectType oType;
    size_t stateSize;
    int objCount;
    Uid *testUids;
    OperActions **testObjs;
};

PerformanceTest::PerformanceTest ( ObjectType ot, size_t size, int maxobjs, 
				   char *hostname )
                                 : LockManager(ot),
				   host(0),
				   oType(ot),
				   stateSize(size),
				   objCount(maxobjs),
				   testUids(0),
				   testObjs(0)
{
    AtomicAction A;

    if (hostname)
	host = clone(hostname);
	
    testObjs = (new OperActions *[objCount]);
    testUids = (new Uid[objCount]);

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	for (int i = 0; i < objCount; i++)
	{
	    ClientRpcManager *rpcCon = new ClientRpcManager("OperActions",1);
	    
	    if (host)
	    {
		ClientRpc* rpcClient = rpcCon->getRpcHandle();
		rpcClient->setHost(host);
	    }
	    
   
	    testObjs[i] = new OperActions(stateSize, ot, rpcCon);
	    testUids[i] = testObjs[i]->get_uid();
	}
	
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
				   host(0),
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

    if (host)
	delete host;
    
    delete testUids;
    delete testObjs;
    
}

Boolean PerformanceTest::save_state( ObjectState& os, ObjectType )
{
    os.packMappedString(host);
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

    if (host)
    {
	delete host;
	host = 0;
    }
    
    os.unpack(host);
    
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
	     Boolean& nested, Boolean& newTest, char *&host )
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
	    if (noObjs > 25)
	    {
		err = 1;
		cout << "Would create too many processes 25 max\n";
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
		Uid newUid = Uid(argv[i]);
		newTest = FALSE;
		
		testUid = newUid;
	    }
	    
	    else
	    {
		err = 1;
		cout<<"No uid given.\n";
	    }			
	}
	else if ((i < argc) && (strcmp(argv[i], "-host") == 0))
	{
	    i++;
	    if (i < argc)
	    {
		host = clone(argv[i]);
	    }
	    
	    else
	    {
		err = 1;
		host = 0;
		
		cout<<"No host given.\n";
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

int main(int argc, char*argv[])
{
    int err = 0;
    
    OperTypes	OperType  = OPERCORE;
    int		NoRep     = 1;
    int noObjs = 25;
    Uid u(NIL_UID);
    Boolean newTest = TRUE;
    char *host = 0;
    
    
    size_t          stateSize = 1024;
    ObjectType      ot        = ANDPERSISTENT;
    Boolean         nested    = FALSE;
    
    err = ParseArg(argc, argv, u, OperType, NoRep, stateSize, ot, 
		   noObjs, nested, newTest, host);
    
    if (err == 0)
    {
	if (newTest)
	{
	    PerformanceTest pt(ot, stateSize, noObjs, host);
	    
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
