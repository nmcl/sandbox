/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActions.cc,v 1.7 1995/06/28 08:51:19 ngdp Exp $
 */

#include <System/iostream.h>
#include <System/memory.h>
#include <Arjuna/Lock.h>
#include <Arjuna/AtomicA.h>

#define OPERACTIONS_SERVER
#define LOCKMANAGER_SERVER
#define STATEMANAGER_SERVER
#include "OperActions_stub.h"

OperActions::OperActions(size_t s, ObjectType ot)
			: LockManager(ot),
			  size(s),
			  value(new char[size])
{
    AtomicAction A;

    ::memset(value, 'a', size);

    A.Begin();
    if (setlock(new Lock(WRITE), 0) != GRANTED)
    {
	A.Abort();
	
	cerr << "OperActions::OperActions( . . . ): Unable to set lock\n" << flush;
    }
    else
	if (A.End() != COMMITTED)
	    cerr << "OperActions::OperActions( . . . ): Unable to commit\n" << flush;
}

OperActions::OperActions ( const Uid& u ) 
                         : LockManager(u), size(0), value(0)
{
}

OperActions::~OperActions()
{
    terminate();

    if (value != NULL)
	delete value;
}

PerformanceVector OperActions::core ( )
{ 
    PerformanceVector v;
    char *p = 0;

#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif

    activate();

    p = (char *)value;

#ifdef DO_PVEC
    endElapsed.get();
    endCpu.get();

    v.setElement(OBJSTORELOAD, endCpu - startCpu, endElapsed - startElapsed);
#endif
    
    modified();
    p[size/2]++;
#ifdef DO_PVEC
    startElapsed.get();
    startCpu.get();
#endif
    deactivate(0,TRUE);
#ifdef DO_PVEC
    endElapsed.get();
    endCpu.get();

    v.setElement(OBJSTOREUNLOAD, endCpu - startCpu, endElapsed - startElapsed);
#endif   
    return v;
    
}

PerformanceVector OperActions::nop()
{
    PerformanceVector v;
    
    return v;
}

PerformanceVector OperActions::be ()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC

    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif
    A.Begin();
    A.End ();
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;
}

PerformanceVector OperActions::ba ()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif
    A.Begin();
    A.Abort();
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;
}

PerformanceVector OperActions::bre()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif

    A.Begin();
    if (setlock(new Lock(READ), 0) != GRANTED)
	cerr << "OperActions::bre( . . . ): Unable to set lock\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(LOCK, endCpu - startCpu, endElapsed - startElapsed);

    startElapsed.get();
    startCpu.get();
#endif
    if (A.End() != COMMITTED)
	cerr << "OperActions::bre( . . . ): Unable to commit\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;
    
}

PerformanceVector OperActions::bwe()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();

#endif
    A.Begin();

    if (setlock(new Lock(WRITE), 0) != GRANTED)
	cerr << "OperActions::bwe( . . . ): Unable to set lock\n" << flush;

#ifdef DO_PVEC
    endElapsed.get();
    endCpu.get();

    v.setElement(LOCK, endCpu - startCpu, endElapsed - startElapsed);

    startElapsed.get();
    startCpu.get();
#endif 
    if (A.End() != COMMITTED)
	cerr << "OperActions::bwe( . . . ): Unable to commit\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;
}

PerformanceVector OperActions::bra()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif
    A.Begin();
    if (setlock(new Lock(READ), 0) != GRANTED)
	cerr << "OperActions::bra( . . . ): Unable to set lock\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();
    
    v.setElement(LOCK, endCpu - startCpu, endElapsed - startElapsed);

    startElapsed.get();
    startCpu.get();
#endif
    if (A.Abort() != ABORTED)
	cerr << "OperActions::bra( . . . ): Unable to commit\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;

}

PerformanceVector OperActions::bwa()
{
    PerformanceVector v;
    AtomicAction A;
#ifdef DO_PVEC
    ElapsedTime startElapsed, endElapsed;
    CpuTime startCpu, endCpu;

    startElapsed.get();
    startCpu.get();
#endif
    A.Begin();
    if (setlock(new Lock(WRITE), 0) != GRANTED)
	cerr << "OperActions::bwa( . . . ): Unable to set lock\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();
    
    v.setElement(LOCK, endCpu - startCpu, endElapsed - startElapsed);

    startElapsed.get();
    startCpu.get();
#endif
    if (A.Abort() != ABORTED)
	cerr << "OperActions::bwa( . . . ): Unable to commit\n" << flush;
#ifdef DO_PVEC

    endElapsed.get();
    endCpu.get();

    v.setElement(ACTION, endCpu - startCpu, endElapsed - startElapsed);
#endif
    return v;
}

Boolean OperActions::save_state(ObjectState& os, ObjectType)
{
    return os.packMappedBlock(value, size);
}

Boolean OperActions::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(value, size);
}

const TypeName OperActions::type() const
{
    return "/StateManager/LockManager/OperActions";
}
