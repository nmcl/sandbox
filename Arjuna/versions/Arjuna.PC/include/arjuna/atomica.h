/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicA.h,v 1.5 1994/01/20 12:51:36 ngdp Exp $
 */

#ifndef ATOMICA_H_
#define ATOMICA_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * AtomicAction subsystem class
 *
 */

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef PREPOUT_H_
#  include <Arjuna/PrepOut.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif


class AbstractRecord;
class ObjectState;
class RecordList;


/* 
 * Beware. This enum is ordered deliberately
 */

enum ActionStatus
{
    RUNNING, PREPARING, ABORTING, ABORTED,
    PREPARED, COMMITTING, COMMITTED, CREATED, 
    INVALID
};


//  @NoRemote, @NoMarshall	
class AtomicAction : public StateManager
{
    friend SIG_FUNC_TYPE AAsignalHandler;
    friend class AARecord; // Crash Recovery

public:
    enum addOutcome { ADDED, REJECTED, DUPLICATE };

    /* Constructors and destructor */

    AtomicAction ();
    AtomicAction (const Uid& actUid);
    virtual ~AtomicAction();

#ifdef _AIX
    // have to do this at the moment because of bugs in the xlC compiler!!

    enum ActionType {TOP_LEVEL, NESTED};
#endif

    static AtomicAction *Current ();

    /* non-virtual member functions and operators */

    addOutcome add (AbstractRecord *ar); 

    int hierarchyDepth () const;
    
    Boolean isAncestor (const Uid& descendant ); 
    AtomicAction *parent ();
    ActionStatus status();
    ObjectStore *store ();
    Uid topLevelAction ();

    /* virtual member functions and operators */

    virtual ActionStatus Abort ();    
    virtual ActionStatus Begin ();
    virtual ActionStatus End ();

    virtual Uid getSavingUid () const;

    /* inherited public virtual functions */

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
    
protected:

#ifndef _AIX
    // have to do this at the moment because of bugs in the xlC compiler!!

    enum ActionType {TOP_LEVEL, NESTED};
#endif

    AtomicAction (ActionType);
    AtomicAction (const Uid&, ActionType);

    /* non-virtual member functions and operators */

    void criticalEnd ();
    void criticalStart ();
    void phase2Commit ();
    void phase2Abort ();
    PrepareOutcome prepare ();

    /* virtual member functions and operators */

    virtual void terminate ();

    /* note the static declaration of the following */

    static AtomicAction* currentAct;

     /* Should be private */

    static ObjectStore* currentStore;

private:
    /* non-virtual member functions and operators */

    void actionInitialise ();
    void doAbort (RecordList& rl);
    void doCommit (RecordList& rl);
    void preventCommit ();
    void setHandler (SIG_ARGTYPE);
    void updateState ();
    void crashRecoveryCommit ();
    
    /* virtual member functions and operators */

    virtual void merge (AbstractRecord *ar);

    /* private state variables */

    Boolean commitAllowed;
    Boolean savedIntentionList;

    /* These (genuine) lists hold the abstract records */

    RecordList *const pendingList;
    RecordList *const preparedList;
    RecordList *const readonlyList;
    RecordList *const failedList;

    /* Atomic action status variables */

    int actionDepth;
    ActionStatus actionStatus;
    ActionType actionType;
    AtomicAction *parentAction;    
    AbstractRecord *recordBeingHandled;
    
    /* signal handling stuff */

    static Boolean abortAndExit;	/* NOTE STATIC declaration */
    static Boolean inCritical;
    SIG_ARGTYPE currentHandler, oldHandler;
};

//  @NoRemote, @NoMarshall	
class TopLevelAction : public AtomicAction
{
public:    

    TopLevelAction ();
    ~TopLevelAction ();
}
;

#include <Arjuna/AtomicA.n>

#endif
