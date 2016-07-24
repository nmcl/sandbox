/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicA.h,v 1.1 1993/11/03 14:27:46 nmcl Exp $
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

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

class AbstractRecord;
class ObjectState;
class RecordList;
class ActionHierarchy;

enum ActionStatus
{
    CREATED, RUNNING, ABORTING, ABORTED,
    PREPARING, PREPARED, UNPREPARED, COMMITTING, COMMITTED,
    DONT_INTERRUPT, INVALID
};

/*
 * We deliberately derive atomic actions from the stub mangled
 * class name to ensure that we can never accidentally
 * create an atomic action that has a remote statemanager component
 * If we are running the stub generator over this file however use cpp
 * to temporarily put the 'real' name back.
 */

#ifdef STUB
#  define LocalStatemanager StateManager
#endif

//  @NoRemote, @NoMarshall	
class AtomicAction : public LocalStateManager
{
    friend class ActionHierarchy;
    
    friend SIG_FUNC_TYPE AAsignalHandler;

public:
    /* Constructors and destructor */

    AtomicAction ();
    AtomicAction (const Uid& actUid);
    virtual ~AtomicAction();

    /* THIS IS A NASTY HACK FOR CRASH RECOVERY - CHANGE ASAP! */
    void crashRecoveryCommit();

#ifdef _AIX
    // have to do this at the moment because of bugs in the xlC compiler!!

    enum ActionType {TOP_LEVEL, NESTED};
#endif

    static AtomicAction *Current ();

    /* non-virtual member functions and operators */

    Boolean add (AbstractRecord *ar); 
    
    void getHierarchy (ActionHierarchy&) const;
    int hierarchyDepth () const;
    
    Boolean isAncestor (const Uid& descendant ); 
    AtomicAction *parent ();
    ActionStatus status();
    ObjectStore *store ();
    Uid top_level_action ();

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
    
    /* virtual member functions and operators */

    virtual void merge (AbstractRecord *ar);

    /* private state variables */

    Boolean commitAllowed;
    Boolean savedIntentionList;

    /* These (genuine) lists hold the abstract records */

    RecordList *const pendingList;
    RecordList *const preparedList;
    RecordList *const readonlyList;

    /* Atomic action status variables */

    int actionDepth;
    ActionStatus actionStatus;
    ActionType actionType;
    AtomicAction *parentAction;    

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
