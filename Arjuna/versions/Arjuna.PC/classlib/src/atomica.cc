/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicA.cc,v 1.17 1994/01/28 12:44:19 nsmw Exp $
 */

/*
 *
 * Atomic Action subsystem class
 *
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstR.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

#ifndef RECLISTI_H_
#  include "RecListI.h"
#endif


/*
 * RCS identifying strings for this code and its matching header file
 */

static const char RCSid[] = "$Id: AtomicA.cc,v 1.17 1994/01/28 12:44:19 nsmw Exp $";

/* 
 * Declare some static variables that we need...
 */

AtomicAction *AtomicAction::currentAct = 0;
Boolean AtomicAction::abortAndExit = FALSE;
Boolean AtomicAction::inCritical = FALSE;
ObjectStore* AtomicAction::currentStore = 0;

/*
 * Signal handling functions. These are standard C functions declared
 * as friends to access the abortAndExit flag plus other private
 * operations.
 * We don't declare these as static member functions so that we don't
 * have to cast pointer-to-member functions to 'ordinary' pointers to
 * functions before they are passed in the signal call.
 */

/*
 * If the following operation is invoked then a signal has
 * occurred. When servicing this signal 'this' will not
 * point at the correct object so use the static variable Current
 * instead
 * A signal for which the atomic actions should be aborted has occurred.
 * The following operation simply invokes the terminate operation to 
 * terminate any current atomic actions. Since the terminate operation
 * is a virtual function, it may be refined so that control may return,
 * in which case the signal handler must be reset to allow further signals
 * to occur.
 */

#ifdef HAVE_INTSIG
int AAsignalHandler ( int DOTDOTDOT )
#else
void AAsignalHandler ( int DOTDOTDOT )
#endif
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AAsignalHandler" << endl;
#endif

    AtomicAction *currAct = AtomicAction::Current();

    /* 
     * Check Current is set in case this invocation is a relic from previous
     * AtomicAction executions
     */

    if (currAct)
    {
        if (currAct->inCritical)
	{
	    currAct->abortAndExit = TRUE;
	}
	else
	    currAct->terminate();
    }
    else
	::exit(0);			/* no atomic actions so simply die */

    /* if terminate returns then must reset the signal and continue */

    currAct->setHandler(AAsignalHandler);
}

/*
 * Public constructors and destructor
 */

AtomicAction::AtomicAction ()
                           : StateManager(),
			     commitAllowed(TRUE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionDepth(0),
			     actionStatus(CREATED),
			     actionType(NESTED),
			     savedIntentionList(FALSE),
			     parentAction(0),
			     recordBeingHandled(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction()" << endl;
#endif
}

/*
 * AtomicAction constructor with a Uid.
 * This constructor is for recreating an AtomicAction, typically during
 * crash recovery.
 */

AtomicAction::AtomicAction ( const Uid& objUid)
                           : StateManager(objUid),
			     commitAllowed(TRUE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionDepth(0),
			     actionStatus(CREATED),
			     actionType(NESTED),
			     savedIntentionList(FALSE),
			     parentAction(0),
			     recordBeingHandled(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC ;
    debug_stream << "AtomicAction::AtomicAction(" << objUid << ")" << endl;
#endif

    StateManager::activate();
}

/*
 * AtomicAction destructor. Under normal circumstances we do very little.
 * However there exists the possibility that this action is being deleted
 * while still running (user forgot to commit/abort) - in which case we
 * do an abort for him and mark all our parents as unable to commit.
 * Additionally due to scoping we may not be the current action - but
 * in that case the current action must be one of our nested actions
 * so by applying abort to it we should end up at ourselves!
 */

AtomicAction::~AtomicAction ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::~AtomicAction " << get_uid() << "" << endl;
#endif

    /* If current action is one of my children there's an error */

    if ((currentAct != 0) && (currentAct != this))
    {
	if (currentAct->isAncestor(get_uid()))
	{
	    error_stream << WARNING 
			 << "Action nesting error - deletion of action id "
			 << get_uid();
	    error_stream << " invoked while child actions active\n";
	
	    while ((currentAct != this) && (currentAct != 0))
	    {
		error_stream << "Aborting child " 
			     << currentAct->get_uid() << "\n";

		currentAct->Abort();
	    }
	}
    }
    
    if (actionStatus == RUNNING)
    {
	AtomicAction *parentAct = parent();

	/* prevent commit of parents (safety) */

	while (parentAct)
	{
	    parentAct->preventCommit();
	    parentAct = parentAct->parent();
	}
	
	error_stream << WARNING 
		     << "Destructor of still running action id " << get_uid()
		     << " invoked - Aborting" << endl;

	Abort();

    }

    if (pendingList)
	delete pendingList;

    if (preparedList)
	delete preparedList;

    if (readonlyList)
	delete readonlyList;

    if (failedList)
	delete failedList;
}

/*
 * Public non-virtual member functions
 */

/*
 * Add a record to the atomic action.
 * This function returns ADDED if the record is added. REJECTED
 * if the record cannot be added because the action is past the
 * prepare phase, and IGNORED otherwise
 */

AtomicAction::addOutcome AtomicAction::add ( AbstractRecord *A )
{
    addOutcome result = REJECTED;

    if ((actionStatus <= ABORTING) &&
	((recordBeingHandled == 0) || 
	 !(*recordBeingHandled == A)))
    {
	criticalStart();
	result = (pendingList->insert(A) ? ADDED : DUPLICATE );
	criticalEnd();
    }

    return result;
}

/* 
 * Boolean function that checks whether the Uid passed as an
 * argument is the Uid for an ancestor of the current
 * atomic action.
 */

Boolean AtomicAction::isAncestor ( const Uid& descendant )
{
    if ((parentAction) && (actionType != TOP_LEVEL))
    {
	if ((parentAction->get_uid()) == descendant)
	    return TRUE;

	return parentAction->isAncestor(descendant);
    }
    return FALSE;
}

/* 
 * Return a pointer to the parent AtomicAction
 */

AtomicAction* AtomicAction::parent ()
{
    if (actionType == NESTED)
	return parentAction;
    else
	return 0;
}

/*
 * Return the status of the AtomicAction
 */

ActionStatus AtomicAction::status ()
{
    return actionStatus;
}

/* 
 * Set up an object store and assign it to the currentStore variable.
 */

ObjectStore* AtomicAction::store ()
{
    if (!currentStore)
    {
	if (actionType == NESTED)
	    currentStore = parentAction->store();
	else
	{
	    currentStore = ObjectStore::create("LocalPersistentObjectStore");
	}
    }

    return currentStore;
}

/*
 * The following function returns the Uid of the top-level atomic
 * action.
 */

Uid AtomicAction::topLevelAction ()
{
    AtomicAction *root = this;

    while(root->parent())
	root = root->parent();

    return (root->get_uid());
}

/*
 * Public virtual member functions 
 */

/*
 * This is the user callable abort operation. It is invoked prior to the
 * start of two-phase commit and hence only processes records in the
 * pendingList (the other lists should be empty).
 */

ActionStatus AtomicAction::Abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::Abort for " << get_uid() << "" << endl;
#endif

    /* Check for superfluous invocation */

    if (actionStatus != RUNNING)
    {
	switch (actionStatus)
	{
	case CREATED:
	    error_stream << WARNING 
			 << "Abort called on non-running atomic action ";
	    break;
	case ABORTED:
	    error_stream << WARNING 
			 << "Abort called on already aborted atomic action ";
	    break;
	default:
	    error_stream << WARNING 
			 << "Abort called illegally on atomic action ";
	    break;
	}

	error_stream  << get_uid() << endl;

	return actionStatus;
    }
    
    /* Ensure I am the currently active action */

    if ((currentAct != 0) && (currentAct != this))
    {
	if (currentAct->isAncestor(get_uid()))
	{
	    /* current action is on of my children */

	    AtomicAction *parentAct = parent();

	    /* prevent commit of my parents (ensures safety) */

	    while (parentAct)
	    {
		parentAct->preventCommit();
		parentAct = parentAct->parent();
	    }

	    /* Abort my children */

	    error_stream << WARNING 
			 << "Action nesting error - Abort of action id "
			 << get_uid();
	    error_stream << " invoked while child actions active\n";
	
	    while ((currentAct != this) && (currentAct != 0))
	    {
		error_stream << "Aborting child "
			     << currentAct->get_uid() << "\n";

		currentAct->Abort();
	    }
	}
    }
    
    actionStatus = ABORTING;

    while (pendingList->size() > 0)
	doAbort(*pendingList);

    actionStatus = ABORTED;
    currentAct = parentAction;

    return (actionStatus);
}

/*
 * Atomic action Begin operation
 */

ActionStatus AtomicAction::Begin ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::Begin() for action-id " << get_uid() << "" << endl;
#endif
    
    if (actionStatus != CREATED)
    {
	error_stream << WARNING
	             << "AtomicAction::Begin of action " << get_uid()
		     << " ignored - incorrect invocation sequence" << endl;
    } else
    {
	actionInitialise ();
	actionStatus = RUNNING;

	if ((actionType != TOP_LEVEL) &&
	    (parentAction->status() > ABORTING))
	{
	    commitAllowed = FALSE;
	    error_stream << WARNING
			 << "AtomicAction::Begin of action " << get_uid()
			 << " ignored - parent action is not running" << endl;
	}
    }
    
    return (actionStatus);
}

/* 
 * End the atomic action by committing it. This invokes the prepare()
 * operation. If this succeeds then the pendingList should be empty 
 * and the records that were formally on it will have been distributed
 * between the preparedList and the readonlyList, also if the action is
 * topLevel then the intention list will have been written to the 
 * object store. Then invoke phase2Commit and clean up the object store
 * if necessary
 *
 * If prepare() fails invoke phase2Abort. In this case the pendingList
 * may still contain records but phase2Abort takes care of these. Also
 * in this case no intention list has been written
 */

ActionStatus AtomicAction::End ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::End() for action-id " << get_uid() << "" << endl;
#endif

    /* Check for superfluous invocation */

    if (actionStatus != RUNNING)
    {
	switch (actionStatus)
	{
	case CREATED:
	    error_stream << WARNING 
			 << "End called on non-running atomic action ";
	    break;
	case COMMITTED:
	    error_stream << WARNING 
			 << "End called on already committed atomic action ";
	    break;
	default:
	    error_stream << WARNING 
			 << "End called illegally on atomic action ";
	    break;
	}

	error_stream  << get_uid() << endl;

	return actionStatus;
    }

    /* Ensure I am the currently active action */

    if ((currentAct != 0) && (currentAct != this))
    {
	if (currentAct->isAncestor(get_uid()))
	{
	    /* current action is one of my children */

	    AtomicAction *parentAct = parent();

	    /* prevent commit of my parents (ensures safety) */

	    while (parentAct)
	    {
		parentAct->preventCommit();
		parentAct = parentAct->parent();
	    }

	    /* Abort my children */

	    error_stream << WARNING 
			 << "Action nesting error - End of action id "
			 << get_uid()
			 << " invoked while child actions active" << endl;
	
	    while ((currentAct != this) && (currentAct != 0))
	    {
		error_stream << "Aborting child " 
			     << currentAct->get_uid() << endl;

		currentAct->Abort();
	    }
	}

	/* Finally abort me */

	return Abort();
    }

    if (prepare() == PREP_NOTOK)
    {
	error_stream << WARNING
		     << "AtomicAction::End() - prepare phase of action-id "
		     << get_uid() << " failed. "
		     << "Action aborting" << endl;
		  		  
	phase2Abort();		/* first phase failed */
    }
    else
    {
	
	phase2Commit();		/* first phase succeeded */
	updateState();              /* update persistent object state */
    }

    return (actionStatus);
}

void AtomicAction::updateState()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::updateState() for action-id " << get_uid()
		 << endl;
#endif

    /*
     * If the action is topLevel then prepare() will have written the
     * intention_list to the object_store.
     * If any of the phase2Commit processing failed then records will
     * exist on the failedList. If this is the case then we need to
	 * re-write the AtomicAction record in the object store. If the
     * failed list is empty we can simply delete the AtomicAction
     * record.
     */

    if (actionType == TOP_LEVEL)
    {
        /*
         * make sure the object store is set up for a top-level atomic action
         */

        store();

        if (failedList->size() > 0)
        {
            /*
             * Re-write the AtomicAction record with the failed list
             */

            Uid u = getSavingUid();
            TypeName tn = type();
            ObjectState state(u, tn);

            if (save_state(state, ANDPERSISTENT) == FALSE)
            {
#ifdef DEBUG
            debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
            debug_stream << "AtomicAction::End - Could not create ObjectState for failedList" << endl;
#endif
            }

            if (state.notempty())
			{
                if (currentStore->write_committed(u, tn, state) == FALSE)
                {
#ifdef DEBUG
            debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
            debug_stream << "AtomicAction::End - Could not write failed list" << endl;
#endif
                }
            }
        }
        else
        {
            if (savedIntentionList)
                currentStore->remove_state(getSavingUid(), type());
        }
    }
}

/*
 * The following routine is used by crash recovery to commit an incorrectly
 * terminated action.
 */

void AtomicAction::crashRecoveryCommit()
{
    phase2Commit();
    updateState();
}

Uid AtomicAction::getSavingUid () const
{
    return get_uid();
}

/*
 * Redefined version of save_state and restore_state from Statemanager.
 *
 * Normal operation (no crashes):
 *
 * AtomicAction::save_state is called after a successful prepare. This causes
 * and AtomicAction object to be saved in the object store. This object
 * contains primarily the "intentions list" of the AtomicAction.
 * After successfully completing phase 2 of the commit protocol, the
 * AtomicAction object is deleted from the store.
 *
 * Failure cases:
 *
 * If a server crashes after successfully preparing, then upon recovery the
 * action must be resolved (either committed or aborted) depending upon 
 * whether the co-ordinating atomic action committed or aborted.
 * Upon server recovery, the crash recovery mechanism detects ServerAtomicAction
 * objects in the object store and attempts to activate the AtomicAction object
 * of the co-ordinating action. If this is successful then the SAA is
 * committed else aborted.
 *
 * If, when processing phase 2 of the commit protocol, the co-ordinator
 * experiences a failure to commit from one of the records then the
 * AtomicAction object is NOT deleted. It is rewritten when a new state
 * which contains a list of the records that failed during phase 2 commit. This
 * list is called the "failedList".
 *
 * The crash recovery manager will detect local AtomicAction objects in
 * addition to SAA objects in the objectstore. An attempt will be made
 * to commit these actions. If the action contained a call to a now dead
 * server, this action can never be resolved and the AA object can never
 * be removed. However, if the action is purely local then after the 
 * processing is complete the removed by crash recovery.
 */

Boolean AtomicAction::save_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::save_state ()" << endl;
#endif
    
    /*
     * This routine is called:
     * a) After a successful prepare - to save the intentions list
     * b) After a failure during phase 2 of commit - to overwrite the
     *    intentions list by the failedList
     * c) By the save_state operation of ServerAtomicAction
     */

    RecordList *listToSave;
    Boolean res = TRUE;

    /*
     * If we have a failedList then we are re-writing an AtomicAction object
     * after a failure during phase 2 commit
     */

    if (failedList->size() > 0)
    {
        listToSave = failedList;
    }
    else
    {
        listToSave = preparedList;
    }

    AbstractRecord *first = listToSave->getFront();
    AbstractRecord *temp = first;

    while (res && (temp != 0))
    {
	listToSave->putRear(temp);

        /*
	 * First check to see if we need to call save_state. If we do then
	 * we must first save the record type (and enum) and then save the
	 * unique identity of the record (a string). The former is needed
	 * to determine what type of record we are restoring, while the
	 * latter is required to re-create the actual record.
	 */

        /*
         * PERSISTENCE records are saved so that the state modifications
         * of objects in a server atomic action may be committed or aborted
         * by the crash recovery mechanism.
         *
         * RPCCALL records are saved so that an AtomicAction that only 
         * manipulates remote objects will have a record in the object store
         * that crash recovery can examine when deciding whether a Server-
         * AtomicAction should be committed or aborted.
         */

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::save_state - next record to pack is a\n"
                 << temp->typeIs() << " record (" << temp->type() << ") "
                 << "should save it? = " << temp->doSave() << endl;
#endif

	if (temp->doSave())
	    res = os.pack(temp->typeIs()) && os.pack(temp->type())
	          && temp->save_state(os, ot);

	temp = listToSave->getFront();

	if (temp == first)
	{
	    listToSave->putFront(temp);
	    temp = 0;
	}
    }

    if (res && os.notempty())
    {
        res = ( (os.pack(NONE_RECORD) && os.pack(actionStatus)
                                      && os.pack(actionType)) ? TRUE : FALSE );
    }

    return res;
}

Boolean AtomicAction::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::restore_state ()" << endl;
#endif

    Boolean res = TRUE;
    int record_type;
    int tempActionStatus;
    int tempActionType;

    /*
     * This routine is called:
     * a) by the AAStatus module of the crash recovery system for recovery
     *    of local AAs
     * b) from restore_state of ServerAtomicAction which also invoked
     *    by the crash recovery system
     */

    /*
     * First unpack the prepared list.
     * Note: This may either be a full intentions list or just the failedList,
     * either way, restore it as the prepared list.
     */

    res = os.unpack(record_type);

    while ( res && (record_type != NONE_RECORD) )
    {
        TypeName tName = 0;
	res = os.unpack(tName);
	if (res)
	{
            AbstractRecord* record = AbstractRecord::create(tName);
	    if (record == 0)
	        res = FALSE;
	    else
                res = record->restore_state(os, ot) && preparedList->insert(record);
	}

        /* Tidy up before we leave. */
	if (tName)
        {
#ifndef __GNUG__
	  ::delete [] tName;
#else
	  ::delete tName;
#endif
        }

        res = ( res && os.unpack(record_type) );
    }

    res = ( (res && os.unpack(tempActionStatus) && 
             os.unpack(tempActionType)) ? TRUE : FALSE );

    if (res)
    {
        actionStatus = (ActionStatus) tempActionStatus;
        actionType = (ActionType) tempActionType;
        savedIntentionList = TRUE;
    }


    return (res);
}


const TypeName AtomicAction::type () const
{
#ifndef MSDOS
    return ("/StateManager/AtomicAction");
#else
    return("/StateMan/AtomicA");
#endif
}

/*
 * Protected non-virtual member functions
 */


AtomicAction::AtomicAction ( ActionType at )
                           : StateManager(NEITHER),
			     commitAllowed(TRUE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionDepth(0),
			     actionStatus(CREATED),
			     actionType(at),
			     savedIntentionList(FALSE),
			     parentAction(0),
			     recordBeingHandled(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction(" << at
		 << ")" << endl;
#endif
}

AtomicAction::AtomicAction ( const Uid& u, ActionType at )
                           : StateManager(u),
			     commitAllowed(TRUE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionDepth(0),
			     actionStatus(CREATED),
			     actionType(at),
			     savedIntentionList(FALSE),
			     parentAction(0),
			     recordBeingHandled(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction(" << u << ", " << at
		 << ")" << endl;
#endif
}

/*
 * If the abortAndExit variable is set a signal must have occurred
 * during the critical region which is terminated by the criticalEnd
 * operation. If this is the case then the non_critical_signal_handler
 * is invoked.
 */

void AtomicAction::criticalEnd ()
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::criticalEnd()" << endl;
#endif

    inCritical = FALSE;

    if (abortAndExit == TRUE)
	terminate();
}

/*
 * The following function defines the start of a critical region
 * by setting the critical flag. If the signal handler is
 * called the class variable abortAndExit is set. The value of
 * this variable is checked in the corresponding operation to
 * end the critical region.
 */

void AtomicAction::criticalStart ()
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::criticalStart()" << endl;
#endif
 
    abortAndExit = FALSE;
    inCritical = TRUE;
}

/*
 * Second phase of the two-phase commit protocol for committing actions.
 * This operation first invokes the doCommit operation on the
 * preparedList. This ensures that the appropriate commit operation
 * is performed on each entry which is then either deleted (top_level)
 * or merged into the parent's pendingList.
 * 
 * Processing of the readonlyList is different in that if the action
 * is top_level then all records in the readonlyList are deleted without
 * further processing. If nested the records must be merged. This is
 * an optimisation to avoid unnecessary processing.
 *
 * Note that at this point the pendingList SHOULD be empty due to the
 * prior invocation of prepare().
 */

void AtomicAction::phase2Commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::phase2Commit() for action-id "
	         << get_uid() << "" << endl;
#endif

    if (pendingList->size() > 0)
	error_stream << WARNING << "non-empty pendingList at phase2Commit";

    criticalStart();

    actionStatus = COMMITTING;

    doCommit(*preparedList);		/* process the preparedList */

    /* The readonlyList requires special attention */

    if (readonlyList->size() > 0)
    {
	while ((recordBeingHandled = readonlyList->getFront()))
	{
	    if (actionType == TOP_LEVEL)
		delete recordBeingHandled;
	    else
		merge(recordBeingHandled);
	}
    }

    actionStatus = COMMITTED;
    currentAct = parentAction;

    criticalEnd();
}

/*
 * Second phase of the two phase commit protocol for aborting actions.
 * Actions are aborted by invoking the doAbort operation on the
 * preparedList, the readonlyList, and the pendingList.
 *
 * The pendingList is processed because it may not be empty - since
 * prepare() stops processing the list at the first PREP_NOTOK result.
 *
 * The read_only list is processed to ensure that actions are aborted
 * immediately and any servers killed at that point since they need not
 * hang around. This contrasts with commit where readonlyList entries 
 * are simply merged with the parent list or discarded
 */

void AtomicAction::phase2Abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::phase2Abort() for action-id " 
	         << get_uid() << "" << endl;
#endif

    criticalStart();

    actionStatus = ABORTING;
    
    while ((pendingList->size() > 0) ||
	   (readonlyList->size() > 0) ||
	   (preparedList->size() > 0))
    {
	doAbort(*preparedList);
	doAbort(*readonlyList);
	doAbort(*pendingList);
    }
    
    actionStatus = ABORTED;
    currentAct = parentAction;

    criticalEnd();
}

/*
 * Phase one of a two phase commit protocol.
 * This function returns the ouctome of the prepare operation.
 * If all goes well it will be PREP_OK, if not PREP_NOTOK.
 * The value READONLY may also be returned if all the records indicate
 * that they are readonly records. Such records do not take part in the
 * second phase commit processing.
 */

PrepareOutcome AtomicAction::prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::prepare () for action-id " << get_uid() << "" << endl;
	  
#endif

    PrepareOutcome p;

    actionStatus = PREPARING;

    /* If we cannot commit - say the prepare failed */

    if (!commitAllowed)
    {
	actionStatus = PREPARED;
	return PREP_NOTOK;
    }
    
    /*
     * make sure the object store is set up for a top-level atomic action
     */

    if (actionType == TOP_LEVEL)
    {
	store(); 
    }

    criticalStart();

    /*
     * Here is the start of the hard work. Walk down the pendingList
     * invoking the appropriate prepare operation. If it succeeds 
     * put the record on either the preparedList or the read_only
     * list and continue until the pendingList is exhausted..
     *
     * If prepare fails on any record stop processing immediately
     * and put the offending record back on the pendingList
     */

    while ((recordBeingHandled = pendingList->getFront()))
	{
	p = ((actionType == TOP_LEVEL)
	     ? recordBeingHandled->topLevelPrepare()
	     : recordBeingHandled->nestedPrepare());

	if (p == PREP_OK)
	{
	    if (!preparedList->insert(recordBeingHandled))
		delete recordBeingHandled;
	}
	else
	{
	    if (p == READONLY)
	    {
		if (!readonlyList->insert(recordBeingHandled))
		delete recordBeingHandled;
	    }
	    else
	    {
		/*
                 * Prepare on this record failed - we are in trouble.
                 * Add the record back onto the pendingList and return
                 */

		pendingList->putFront(recordBeingHandled);
		recordBeingHandled = 0;
		
		actionStatus = PREPARED;

		criticalEnd();
		
		return(PREP_NOTOK);
	    }
	}
    }

    actionStatus = PREPARED;

    /* 
     * If we are here then everything went okay so save the intention list
     * in the ObjectStore in case of a node crash providing that its not
     * empty
     */

    if (actionType == TOP_LEVEL)
    {
	Uid u = getSavingUid();
	TypeName tn = type();
        ObjectState state(u, tn);

	if (save_state(state, ANDPERSISTENT) == FALSE)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
	    debug_stream << "AtomicAction::Prepare - creating intention list failed" << endl;
#endif
	    criticalEnd();

	    return(PREP_NOTOK);
        }

	if (state.notempty())
	{
	    if (currentStore->write_committed(u, tn, state) == FALSE)
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
		debug_stream << "AtomicAction::Prepare - intention list write failed" << endl;
#endif
		criticalEnd();

		return(PREP_NOTOK);
	    }
	    else
		savedIntentionList = TRUE;
	}
    }

    criticalEnd();

    if ((preparedList->size() == 0) && (readonlyList->size() > 0))
	return(READONLY);
    else
	return(PREP_OK);
}

/*
 * Protected virtual operations 
 */

/*
 * Terminate is called whenever a signal has been caught during processing.
 * It attempts to cleanup the action as best it can and exit. THIS IS
 * ALSO PROBABLY THE INCORRECT THING TO DO - FIX IT!!!
 */

void AtomicAction::terminate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::terminate()" << endl;
#endif

    error_stream << "AtomicAction - caught signal aborting all atomic actions"
		 << endl;

    while (currentAct)
    {
	error_stream << "Aborting " << currentAct->get_uid() << endl;
	currentAct->Abort();
    }

    error_stream << "Process terminating...." << endl;

    /* 
     * If here then all atomic actions were aborted successfully
     * so the current process can exit 
     */

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream <<"AtomicAction::terminate - actions aborted, exiting" << endl;
#endif

    ::exit(0);				/* THIS IS WRONG! */
}

/*
 * Private non-virtual functions
 */

/* 
 * action_initialise determines whether the AtomicAction is a nested,
 * top-level, or a top-level nested atomic action
 */

void AtomicAction::actionInitialise ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::actionInitialise() for action-id "
	         << get_uid() << "" << endl;
#endif

    if (currentAct)			/* ie not top_level */
    {
	actionDepth = currentAct->hierarchyDepth();
	
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
	debug_stream << "Action " << get_uid()
	             << " with parent status " << currentAct->actionStatus 
		     << "" << endl;
#endif

    }
    else
	{
#ifndef MSDOS
	/* this is a top-level atomic action so set the signal handler */

	/* block a number of signals */

#ifdef SYSV
	sigset_t inSet;

	::sigemptyset(&inSet);
	::sigaddset(&inSet,SIGHUP);
	::sigaddset(&inSet,SIGQUIT);
	::sigaddset(&inSet,SIGTERM);

	::sigprocmask(SIG_BLOCK, &inSet, 0);
#else
	(void) ::sigblock(0|sigmask(SIGHUP)|sigmask(SIGQUIT)|sigmask(SIGTERM));
#endif

	setHandler(AAsignalHandler);
#endif
    }
    
    actionDepth++;
    
    switch(actionType)
    {
    case TOP_LEVEL:
	if (currentAct)
	{
	    /*
	     * do not want to print warning all the time as this is what nested
	     * top-level actions are used for.
	     */
#ifdef DEBUG
	    error_stream << WARNING
		         << "Running Top Level Action (" << get_uid() 
			 << ") from within nested action (" 
			 << currentAct->get_uid() << ")" << endl;
#endif
	}
	break;
    case NESTED :
	if (!currentAct)
	    actionType = TOP_LEVEL;
	break;
    }

    criticalStart();

    parentAction = currentAct;
    currentAct = this;

    criticalEnd();
}

/*
 * Walk down a record list extracting records and calling the appropriate
 * abort function. Discard records when done.
 */

void AtomicAction::doAbort ( RecordList& list_toprocess )
{
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
	debug_stream << "AtomicAction::doAbort (" << (void *)&list_toprocess <<")" << endl;
#endif

    if (list_toprocess.size() > 0)
    {
	while (recordBeingHandled = list_toprocess.getFront())
	{
	    if (actionType == TOP_LEVEL)
		recordBeingHandled->topLevelAbort();
	    else
		recordBeingHandled->nestedAbort();

	    if ((actionType != TOP_LEVEL) && recordBeingHandled->propagateOnAbort())
		merge(recordBeingHandled);
	    else
		delete recordBeingHandled;
	}
    }
}

/*
 * Walk down a record list extracting records and calling the appropriate
 * commit function. Discard or merge records as appropriate
 */

void AtomicAction::doCommit ( RecordList& rl )
{
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
	debug_stream << "AtomicAction::doCommit (" << (void *)&rl <<")" << endl;
#endif

    if (rl.size() > 0)
    {
	while (recordBeingHandled = rl.getFront())
	{
	    if (actionType == TOP_LEVEL)
	    {
                if (recordBeingHandled->topLevelCommit())
                {
                    /*
                     * Record successfully committed, we can delete it now
                     */

		    delete recordBeingHandled;
                }
                else
                {
                    /*
                     * The commit failed. Add this record to the failed list
                     * to indicate this.
                     */

                    failedList->insert(recordBeingHandled);
                }
	    }
	    else
	    {
		recordBeingHandled->nestedCommit();
		merge(recordBeingHandled);
	    }
	}
    }
}

void AtomicAction::setHandler ( SIG_ARGTYPE handler )
{
    oldHandler = ::signal(SIGINT, handler);
}



/*
 * Private virtual functions
 */

/* 
 * Operation to merge a record into those held by the parent AtomicAction.
 * This is accomplished by invoking the add operation of the parent
 * AtomicAction. If the add operation does not return ADDED, the record is
 * deleted
 */

void AtomicAction::merge ( AbstractRecord *A )
{
    addOutcome as;
    
    if ((as = parentAction->add(A)) != ADDED)
    {
	delete A;
	if (as == REJECTED)
	    error_stream << WARNING 
			 << "(internal) AtomicAction::merge - record rejected"
			 << endl;
    }
}

#ifdef NO_INLINES
#  define ATOMICA_CC_
#  include "Arjuna/AtomicA.n"
#  undef ATOMICA_CC_
#endif
