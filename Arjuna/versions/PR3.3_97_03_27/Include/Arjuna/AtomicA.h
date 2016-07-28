/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicA.h,v 1.19 1995/09/18 15:15:22 ngdp Exp $
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
    INVALID, CLEANUP
};

/*
 * We deliberately derive atomic actions from the stub mangled
 * class name to ensure that we can never accidentally
 * create an atomic action that has a remote statemanager component
 * If we are running the stub generator over this file however use cpp
 * to temporarily put the 'real' name back.
 */

#ifdef STUB
#  define LocalStateManager StateManager
#endif

//  @NoWarnings, @NoRemote, @NoMarshall	
class AtomicAction : public LocalStateManager
{
    friend class ActionHierarchy;
    friend class ActionInfo;
    friend class ActionSetup;
    friend SIG_FUNC_TYPE AAsignalHandler;
    friend class AARecord; // Crash Recovery

public:
    enum ActionType {TOP_LEVEL, NESTED};
    enum addOutcome { ADDED, REJECTED, DUPLICATE };

    /* Constructors and destructor */

    AtomicAction ();
    AtomicAction (const Uid& actUid);
    virtual ~AtomicAction();

    static AtomicAction *Current ();
    
    /* non-virtual member functions and operators */

    addOutcome add (AbstractRecord *ar); 
    
    ActionHierarchy * const getHierarchy () const;
    unsigned int hierarchyDepth () const;
    
    Boolean isAncestor (const Uid& descendant ) const; 
    AtomicAction *const parent () const;
    ActionStatus status() const;

    Uid topLevelAction () const;
    int getCreatorPid() const;
    Boolean activate (const char * = 0);
    
    /* Allow action commit to be supressed */
    
    void preventCommit ();

    /* virtual member functions and operators */

    virtual ActionStatus Abort ();    
    virtual ActionStatus Begin ();
    virtual ActionStatus End ();

    virtual Uid getSavingUid () const;
    
    ObjectStore *const store ();
    /* inherited public virtual functions */

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
    
protected:

    AtomicAction (ActionType);
    AtomicAction (const Uid&, ActionType);
    
    /* non-virtual member functions and operators */

    void criticalEnd ();
    void criticalStart ();
    void phase2Cleanup ();
    void phase2Commit ();
    void phase2Abort ();
    PrepareOutcome prepare ();
    
    /* virtual member functions and operators */

    virtual void terminate ();

private:
    /* non-virtual member functions and operators */

    void actionInitialise ();
    void doAbort (RecordList& rl);
    void doCleanup (RecordList& rl);
    void doCommit (RecordList& rl);

    static void setHandler (SIG_ARGTYPE); /* NOTE STATIC */
    void updateState ();
    void crashRecoveryCommit ();

    /* virtual member functions and operators */

    virtual void merge (AbstractRecord *ar);

    /* private state variables */

    /* note the static declaration of the following */

    static AtomicAction* currentAct;

    ActionHierarchy *currentHierarchy;	/* BEWARE - must be pointer */
    ObjectStore* currentStore;

    Boolean commitAllowed;
    Boolean savedIntentionList;

    /* These (genuine) lists hold the abstract records */

    RecordList *const pendingList;
    RecordList *const preparedList;
    RecordList *const readonlyList;
    RecordList *const failedList;

    /* Atomic action status variables */

    ActionStatus actionStatus;
    ActionType actionType;
    AtomicAction *parentAction;    
    AbstractRecord *recordBeingHandled;
#ifndef hpux    
    int creatorPid;
#else
    pid_t creatorPid;
#endif    
    
    /* signal handling stuff */

    static Boolean abortAndExit;	/* NOTE STATIC declaration */
    static Boolean inCritical;

#ifdef SYSV
    sigset_t oldMask;
#else
    int oldMask;
#endif    
};

//  @NoRemote, @NoMarshall	
class TopLevelAction : public AtomicAction
{
public:    

    TopLevelAction ();
    virtual ~TopLevelAction ();
}
;

#include <Arjuna/AtomicA.n>

#endif
