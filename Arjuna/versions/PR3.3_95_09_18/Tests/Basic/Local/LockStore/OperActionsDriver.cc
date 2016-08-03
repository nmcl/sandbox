/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActionsDriver.cc,v 1.3 1995/03/06 14:40:37 ngdp Exp $
 */

#include <System/stdlib.h>
#include <Common/Boolean.h>
#include <Common/Uid.h>

#include <System/string.h>
#include <System/iostream.h>

#include <Arjuna/AtomicA.h>
#include <Arjuna/LockMan.h>

#include "OperActions.h"

class PerformanceTest : public LockManager
{
public:
    PerformanceTest(int);
    PerformanceTest(const Uid&);
    
    ~PerformanceTest();

    void runTest(int, int);
    
    virtual Boolean save_state(ObjectState&, ObjectType);
    virtual Boolean restore_state(ObjectState&, ObjectType);
    virtual const TypeName type() const;

private:
    int objCount;
    Uid *testUids;
    OperActions **testObjs;
    
    
};

PerformanceTest::PerformanceTest ( int maxobjs )
                                 : LockManager(ANDPERSISTENT),
				   objCount(maxobjs),
				   testUids(0),
				   testObjs(0)
{
    AtomicAction A;
	
    testObjs = (new OperActions *[objCount]);
    testUids = (new Uid[objCount]);
    
    for (int i = 0; i < objCount; i++)
    {
	testObjs[i] = new OperActions();
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
    os.pack(objCount);
    
    for (int i = 0; i < objCount; i++)
    {
	testUids[i].pack(os);
    }
    return TRUE;
    
}

Boolean PerformanceTest::restore_state( ObjectState& os, ObjectType )
{
    os.unpack(objCount);

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

void PerformanceTest::runTest(int noDelete, int noObjects)
{
    AtomicAction A;
    int i, j;

    /* randomly delete/recreate some objects */

    for (j = 0; j < noDelete; j++)
    {
	i = rand() % noObjects;

	delete testObjs[i];

	testObjs[i] = new OperActions();
	testUids[i] = testObjs[i]->get_uid();
	
    }
    
    for (j = 0; j < noObjects; j++)
    {
	testObjs[j]->anop();
    }
    
}

int ParseArg(int argc, char *argv[], Uid& testUid, 
	     int& noDelete, int& noObjs)
{
    int err = 0;
	
    for (int i = 1; (i < argc) && (err == 0); i++)
    {
	if ((i < argc) && (strcmp(argv[i], "-num") == 0))
	{
	    i++;
	    if (i < argc)
		noDelete = atoi(argv[i]);
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
	else if ((i < argc) && (strcmp(argv[i], "-uid") == 0))
	{
	    i++;
	    if (i < argc)
	    {
		testUid = *new Uid(argv[i]);
	    }
	    
	    else
	    {
		err = 1;
		cout<<"No uid given.\n";
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
    
    int		noDelete     = 5;
    int noObjs = 10;
    Uid u(NIL_UID);
    
    err = ParseArg(argc, argv, u, noDelete, noObjs);
    
    if (err == 0)
    {
	PerformanceTest pt(noObjs);
	    
	pt.runTest(noDelete, noObjs);

    }
    
    return 0;
}
