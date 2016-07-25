/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicA.cc,v 1.52 1995/10/13 09:02:32 ngdp Exp $
 */

/*
 *
 * Atomic Action subsystem class
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
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

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

#ifndef RECLISTI_H_
#  include "RecListI.h"
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

/*
 * RCS identifying strings for this code and its matching header file
 */

static const char RCSid[] = "$Id: AtomicA.cc,v 1.52 1995/10/13 09:02:32 ngdp Exp $";
static const int DEFAULT_HIERARCHY_DEPTH = 9;

#if defined(HAVE_MMAP) && defined(USE_MMAP)
static TypeName defaultStoreType = MappedStoreType;
#else
static TypeName defaultStoreType = ActionStoreType;
#endif

class ActionSetup
{
public:
    ActionSetup ();
    ~ActionSetup ();
private:
    ObjectStore *cachedStore;
};

/*
 * Object stores are reference counted. This class forces the action store to
 * be set up before main is entered to prevent excessive creation that would otherwise
 * take place as actions are created and destroyed
 * Creation MUST match that in AtomicAction::store()
 */

ActionSetup::ActionSetup ()
{
    cachedStore = ObjectStore::create(defaultStoreType);
}

ActionSetup::~ActionSetup ()
{
    if (cachedStore)
	ObjectStore::destroy(cachedStore);
}

/* 
 * Declare some static variables that we need...
 */

ActionSetup doSetup;

AtomicAction *AtomicAction::currentAct = 0;
Boolean AtomicAction::abortAndExit = FALSE;
Boolean AtomicAction::inCritical = FALSE;

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
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AAsignalHandler" << endl;
    debug_stream.unlock();
    
#endif

    /* re-establish handler */
    
    AtomicAction::setHandler(AAsignalHandler);

    AtomicAction *currAct = AtomicAction::Current();

    /* 
     * Check Current is set in case this invocation is a relic from previous
     * AtomicAction executions
     */

    if (currAct)
    {
        if (AtomicAction::inCritical)
	{
	    AtomicAction::abortAndExit = TRUE;
	}
	else
	    currAct->terminate();
    }
    else
	::exit(0);			/* no atomic actions so simply die */
}

/*
 * Public constructors and destructor
 */

AtomicAction::AtomicAction ()
                           : StateManager(NEITHER),
			     currentHierarchy(0),
			     currentStore(0),
			     commitAllowed(TRUE),
			     savedIntentionList(FALSE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionStatus(CREATED),
			     actionType(NESTED),
			     parentAction(0),
			     recordBeingHandled(0),
                             creatorPid(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * AtomicAction constructor with a Uid.
 * This constructor is for recreating an AtomicAction, typically during
 * crash recovery.
 */

AtomicAction::AtomicAction ( const Uid& objUid)
                           : StateManager(objUid, NEITHER),
			     currentHierarchy(0),
			     currentStore(0),
			     commitAllowed(TRUE),
			     savedIntentionList(FALSE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionStatus(CREATED),
			     actionType(NESTED),
			     parentAction(0),
			     recordBeingHandled(0),
                             creatorPid(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC ;
    debug_stream << "AtomicAction::AtomicAction(" << objUid << ")" << endl;
    debug_stream.unlock();
    
#endif
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
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::~AtomicAction " << get_uid() << "" << endl;
    debug_stream.unlock();
    
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

    if (currentStore)
		ObjectStore::destroy(currentStore);

    if (currentHierarchy)
		delete currentHierarchy;
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


unsigned int AtomicAction::hierarchyDepth () const
{
    if (currentHierarchy)
	return currentHierarchy->depth();
    else
	return 0;			/* should never happen */
}

/* 
 * Boolean function that checks whether the Uid passed as an
 * argument is the Uid for an ancestor of the current
 * atomic action.
 */

Boolean AtomicAction::isAncestor ( const Uid& descendant ) const
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

AtomicAction* const AtomicAction::parent () const
{
    if (actionType == NESTED)
	return parentAction;
    else
	return 0;
}

/*
 * Return the status of the AtomicAction
 */

ActionStatus AtomicAction::status () const
{
    return actionStatus;
}

/* 
 * Set up an object store and assign it to the currentStore variable.
 */

ObjectStore* const AtomicAction::store ()
{
    if (!currentStore)
        currentStore = ObjectStore::create(defaultStoreType);

    if (!currentStore)
    {
	error_stream << FATAL << "AtomicAction::store - cannot create store type " << defaultStoreType << endl;
	ArjunaFatal();
    }
    
    return currentStore;
}

/*
 * The following function returns the Uid of the top-level atomic
 * action.
 */

Uid AtomicAction::topLevelAction () const
{
    const AtomicAction *root = this;

    while(root->parent())
	root = root->parent();

    return (root->get_uid());
}


/* 
 * Returns the pid of the process which created this AtomicAction
 */

int AtomicAction::getCreatorPid() const
{
    return(creatorPid);
}

/*
 * Overloaded version of activate -- sets up the store, performs read_state
 * followed by restore_state
 */

Boolean AtomicAction::activate ( const char * )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::activate()"
	         << " for action-id " << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    Boolean restored = FALSE;
    
    // Set up store
    ObjectStore *aaStore = store();

    // Read object state
    ObjectState *oState = aaStore->read_committed(get_uid(), type());

    if (oState)
    {
	restored = restore_state(*oState, ANDPERSISTENT);
	delete oState;
    }
    else
    {
	char buff[1024];
	ostrstream temp(buff,1024);
	
	temp << WARNING << "Activate of atomic action with id = "
	     << get_uid() << " and type '" << type()
	     << "' unexpectedly failed" << endl << ends;

	addError(new Error(FAC_ATOMIC_ACTION, SM_ACTIVATE_FAILED, buff));
	
	restored = FALSE;
    }
    return restored;
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::Abort for " << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    char buff[256];
    ostrstream temp(buff,256);

    clearErrorList();
    
    /* Check for superfluous invocation */

    if (actionStatus != RUNNING)
    {
	switch (actionStatus)
	{
	case CREATED:
	    temp << "Abort called on non-running atomic action ";
	    break;
	case ABORTED:
	    temp << "Abort called on already aborted atomic action ";
	    break;
	default:
	    temp << "Abort called illegally on atomic action ";
	    break;
	}

	temp  << get_uid() << endl << ends;

	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER, buff));

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

	    temp << "Action nesting error - Abort of action id "
		 << get_uid() << " invoked while child actions active\n";
	
	    while ((currentAct != this) && (currentAct != 0))
	    {
		temp << "\tAborting child " << currentAct->get_uid() << "\n";

		currentAct->Abort();
	    }

	    temp << ends;
	    
	    addError(new Error(FAC_ATOMIC_ACTION, AA_NESTING_ERROR, buff));

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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::Begin() for action-id " << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    char buff[256];
    ostrstream temp(buff,256);

    clearErrorList();
    
    if (actionStatus != CREATED)
    {
	temp << "AtomicAction::Begin of action " << get_uid()
	     << " ignored - incorrect invocation sequence" << endl << ends;
	
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER, buff));
	error_stream << WARNING << buff << endl;
    } else
    {
	actionInitialise ();
	actionStatus = RUNNING;

	if ((actionType != TOP_LEVEL) &&
	    (parentAction->status() > ABORTING))
	{
	    commitAllowed = FALSE;
	    temp << "AtomicAction::Begin of action " << get_uid()
		 << " ignored - parent action is not running" << endl << ends;
	    
	    addError(new Error(FAC_ATOMIC_ACTION, AA_NESTING_ERROR, buff));

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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::End() for action-id " << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    char buff[256];
    ostrstream temp(buff,256);

    clearErrorList();
    
    /* Check for superfluous invocation */

    if (actionStatus != RUNNING)
    {
	switch (actionStatus)
	{
	case CREATED:
	    temp << "End called on non-running atomic action ";
	    break;
	case COMMITTED:
	    temp << "End called on already committed atomic action ";
	    break;
	default:
	    temp << "End called illegally on atomic action ";
	    break;
	}

	temp << get_uid() << endl << ends;
  
	addError(new Error(FAC_ATOMIC_ACTION, AA_INVALID_ORDER, buff));

	return actionStatus;
    }

    /* Ensure I am the currently active action */

    if ((currentAct != 0) && (currentAct != this))
    {
	ActionStatus pending;
	
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

	    temp << "Action nesting error - End of action id "
		 << get_uid()
		 << " invoked while child actions active" << endl;
	
	    while ((currentAct != this) && (currentAct != 0))
	    {
		temp << "\tAborting child " 
		     << currentAct->get_uid() << endl;

		currentAct->Abort();
	    }
	    temp << ends;
	}

	/* Finally abort me */
	
	pending = Abort();

	/* This has to be here as Abort clears the error list */
	
	addError(new Error(FAC_ATOMIC_ACTION, AA_NESTING_ERROR, buff));

	return pending;
    }

    if (prepare() == PREP_NOTOK)
    {
	temp << "AtomicAction::End() - prepare phase of action-id "
	     << get_uid() << " failed. "
	     << "Action aborting" << endl << ends;
		
	addError(new Error(FAC_ATOMIC_ACTION, AA_PREPARE_FAILED, buff));    
	  		  
	phase2Abort();		/* first phase failed */
    }
    else
    {
	if (actionType == TOP_LEVEL)
	{
	    char *env_value = ::getenv("AATESTING");
	    if (env_value != NULL)
	    {
			cerr << "\nATOMIC ACTION " << get_uid() << " PREPARED OK -- " 
				 << ::getpid() << "\n" << endl;
			sleep(::atoi(env_value));
	    }
	}

	phase2Commit();		/* first phase succeeded */
    }

    return (actionStatus);
}

void AtomicAction::updateState()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::updateState() for action-id " << get_uid()
		 << endl;
    debug_stream.unlock();
    
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
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
		debug_stream << "AtomicAction::End - Could not create ObjectState for failedList" << endl;
		debug_stream.unlock();
	    
#endif
            }

            if (state.notempty())
            {
                if (currentStore->write_committed(u, tn, state) == FALSE)
                {
#ifdef DEBUG
		    debug_stream.lock();
		    
		    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
		    debug_stream << "AtomicAction::End - Could not write failed list" << endl;
		    debug_stream.unlock();
		    
#endif
                }
            }
        }
        else
        {
            if (savedIntentionList)
                currentStore->remove_committed(getSavingUid(), type());
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::save_state ()" << endl;
    debug_stream.unlock();
    
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
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
	debug_stream << "AtomicAction::save_state - next record to pack is a\n"
		     << temp->typeIs() << " record (" << temp->type() << ") "
		     << "should save it? = " << temp->doSave() << endl;
	debug_stream.unlock();
    
#endif

	if (temp->doSave())
	{
	    res = os.pack((int) temp->typeIs())
			&& os.packNonMappedString(temp->type())
			&& temp->save_state(os, ot);
	}

	temp = listToSave->getFront();

	if (temp == first)
	{
	    listToSave->putFront(temp);
	    temp = 0;
	}
    }

    if (res && os.notempty())
    {
        res = ( (os.pack((int) NONE_RECORD)
		 && os.pack((int) actionStatus)
		 && os.pack((int) actionType)
		 && os.pack(creatorPid)) ? TRUE : FALSE );
    }

    return res;
}

Boolean AtomicAction::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::restore_state ()" << endl;
    debug_stream.unlock();
    
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
     * Now unpack the prepared list.
     * Note: This may either be a full intentions list or just the failedList,
     * either way, restore it as the prepared list.
     */

    if (res)
    {
        res = os.unpack(record_type);
    }
    

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
                res = record->restore_state(os, ot) &&
		    preparedList->insert(record);
	}

        /* Tidy up before we leave. */
	if (tName)
        {
#ifndef GCC_ARRAY_DELETE_BUG
	  ::delete [] tName;
#else
	  ::delete tName;
#endif
	  tName = 0;
        }

        res = ( res && os.unpack(record_type) );
	
    }

    res = ( (res
	     && os.unpack(tempActionStatus)
	     && os.unpack(tempActionType)
	     && os.unpack(creatorPid)) ? TRUE : FALSE );

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
    return ("/StateManager/AtomicAction");
}

/*
 * Protected non-virtual member functions
 */


AtomicAction::AtomicAction ( ActionType at )
                           : StateManager(NEITHER),
			     currentHierarchy(0),
			     currentStore(0),
			     commitAllowed(TRUE),
			     savedIntentionList(FALSE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionStatus(CREATED),
			     actionType(at),
			     parentAction(0),
			     recordBeingHandled(0),
                             creatorPid(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction(" << at
		 << ")" << endl;
    debug_stream.unlock();
    
#endif
}

AtomicAction::AtomicAction ( const Uid& u, ActionType at )
                           : StateManager(u, NEITHER),
			     currentHierarchy(0),
			     currentStore(0),
			     commitAllowed(TRUE),
			     savedIntentionList(FALSE),
			     pendingList(new RecordList),
			     preparedList(new RecordList),
			     readonlyList(new RecordList),
			     failedList(new RecordList),
			     actionStatus(CREATED),
			     actionType(at),
			     parentAction(0),
			     recordBeingHandled(0),
                             creatorPid(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "AtomicAction::AtomicAction(" << u << ", " << at
		 << ")" << endl;
    debug_stream.unlock();
    
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
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::criticalEnd()" << endl;
    debug_stream.unlock();
    
#endif

    inCritical = FALSE;

#ifndef WIN32
    
#if defined(SYSV)|| defined(__ultrix)
    ::sigprocmask(SIG_SETMASK, &oldMask, 0);
#else
    ::sigsetmask(oldMask);
#endif

#endif
    
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
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::criticalStart()" << endl;
    debug_stream.unlock();
    
#endif
 
    inCritical = TRUE;

#ifndef WIN32

#if defined(SYSV)|| defined(__ultrix)    
    sigset_t inSet;

    ::sigfillset(&inSet);
    ::sigprocmask(SIG_BLOCK, &inSet, &oldMask);
#else
    oldMask = ::sigblock(~0);
#endif

#endif
}

/*
 * Cleanup phase for actions.
 * If an action is in the PREPARED state when a terminate signal is
 * delivered (ie the coordinator node has crashed) then we need to
 * cleanup. This is essentially the same as phase2Abort but we call
 * cleanup ops rather than abort ops and let the records take care
 * of appropriate cleanup.
 *
 * The pendingList is processed because it may not be empty - since
 * prepare() stops processing the list at the first PREP_NOTOK result.
 *
 * The read_only list is processed to ensure that actions are aborted
 * immediately and any servers killed at that point since they need not
 * hang around. This contrasts with commit where readonlyList entries 
 * are simply merged with the parent list or discarded
 */

void AtomicAction::phase2Cleanup ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::phase2Cleanup() for action-id " 
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    criticalStart();

    actionStatus = CLEANUP;
    
    while ((pendingList->size() > 0) ||
	   (readonlyList->size() > 0) ||
	   (preparedList->size() > 0))
    {
	doCleanup(*preparedList);
	doCleanup(*readonlyList);
	doCleanup(*pendingList);
    }
    
    currentAct = parentAction;

    criticalEnd();
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::phase2Commit() for action-id "
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    if (pendingList->size() > 0)
	addError(new Error(FAC_ATOMIC_ACTION, AA_PHASE2_ERROR, "non-empty pendingList at phase2Commit"));

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
    updateState();
    
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::phase2Abort() for action-id " 
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::prepare () for action-id " << get_uid() << "" << endl;
	  
    debug_stream.unlock();
    
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

		/*
		 * Propagate any error info in the record to this action
		 */


		ErrorList& eList = recordBeingHandled->getErrorList();
		Error *error = 0;

		while ((error = eList.pop()))
		{
		    addError(error);
		}

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

    if ((actionType == TOP_LEVEL) &&
	(preparedList->size() > 0))
    {
	/* Only do this if we have some records worth saving! */
	
	Uid u = getSavingUid();
	TypeName tn = type();
        ObjectState state(u, tn);

	if (save_state(state, ANDPERSISTENT) == FALSE)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
	    debug_stream << "AtomicAction::Prepare - creating intention list failed" << endl;
	    debug_stream.unlock();
	    
#endif
	    criticalEnd();

	    return(PREP_NOTOK);
        }

	if (state.notempty())
	{
	    if (currentStore->write_committed(u, tn, state) == FALSE)
	    {
#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
		debug_stream << "AtomicAction::Prepare - intention list write failed" << endl;
		debug_stream.unlock();
		
#endif
		criticalEnd();

		return(PREP_NOTOK);
	    }
	    else
		savedIntentionList = TRUE;
	}
    }

    criticalEnd();

    if ((preparedList->size() == 0) && (readonlyList->size() >= 0))
	return(READONLY);
    else
	return(PREP_OK);
}

/*
 * Protected virtual operations 
 */

/*
 * Terminate is called whenever a signal has been caught during processing.
 * It attempts to cleanup the action as best it can and exit.
 * NOTE: All phase1 and phase2 actions are performed in critical sections
 * so that orphan and terminate signals are blocked. This ensures we complete
 * the phase as appropriate and do not have to consider them here. Thus
 * some action states will never get seen
 */

void AtomicAction::terminate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream << "AtomicAction::terminate()" << endl;
    debug_stream.unlock();
    
#endif

    error_stream << "AtomicAction - caught terminate signal" << endl;

    while (currentAct)
    {
	switch (currentAct->status())
	{
	case RUNNING:
	    error_stream << "Aborting " << currentAct->get_uid() << endl;
	    currentAct->Abort();
	    break;
	case PREPARED:
	    error_stream << "Cleanup for " << currentAct->get_uid() << endl;
	    currentAct->phase2Cleanup();
	    break;
	case CREATED:
	case INVALID:
	default:
	    currentAct = parentAction;
	    break;
	}
    }
    
    error_stream << "Process terminating...." << endl;
    
    /* 
     * If here then all atomic actions were aborted successfully
     * so the current process can exit 
     */

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PROTECTED;
    debug_stream <<"AtomicAction::terminate - actions aborted, exiting" << endl;
    debug_stream.unlock();
    
#endif

    ::exit(0);
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::actionInitialise() for action-id "
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    if (currentAct)			/* ie not top_level */
    {
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
	debug_stream << "Action " << get_uid()
	             << " with parent status " << currentAct->actionStatus 
		     << "" << endl;
	debug_stream.unlock();
	
#endif
	currentHierarchy = new ActionHierarchy(*currentAct->getHierarchy());
	
    }
    else
    {
	currentHierarchy = new ActionHierarchy(DEFAULT_HIERARCHY_DEPTH);

        /* this is a top-level atomic action so set the signal handler */

	/* block a number of signals */

#ifndef WIN32

#if defined(SYSV) || defined(__ultrix)    
	sigset_t inSet;

	::sigemptyset(&inSet);
	::sigaddset(&inSet,SIGHUP);

	::sigprocmask(SIG_BLOCK, &inSet, 0);
#else
	(void) ::sigblock(0|sigmask(SIGHUP));
#endif

#endif

	setHandler(AAsignalHandler);
    }
    
    currentHierarchy->add(get_uid(), actionType);
    
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
	    debug_stream.lock();
	    
	    error_stream << WARNING
		         << "Running Top Level Action (" << get_uid() 
			 << ") from within nested action (" 
			 << currentAct->get_uid() << ")" << endl;
	    debug_stream.unlock();
	    
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

    creatorPid = ::getpid();
}

/*
 * Walk down a record list extracting records and calling the appropriate
 * abort function. Discard records when done.
 */

void AtomicAction::doAbort ( RecordList& list_toprocess )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::doAbort (" << (void *)&list_toprocess <<")" << endl;
    debug_stream.unlock();
	
#endif

    if (list_toprocess.size() > 0)
    {
	Boolean ok = FALSE;
	
	while ((recordBeingHandled = list_toprocess.getFront()))
	{   
	    if (actionType == TOP_LEVEL)
		ok = recordBeingHandled->topLevelAbort();
	    else
		ok = recordBeingHandled->nestedAbort();

	    if ((actionType != TOP_LEVEL) && recordBeingHandled->propagateOnAbort())
		merge(recordBeingHandled);
	    else
	    {
		if (!ok)
		{
		    /* Preserve error messages */
		    ErrorList& eList = recordBeingHandled->getErrorList();
		    Error *error = 0;

		    while ((error = eList.pop()))
			addError(error);
		}

		delete recordBeingHandled;
	    }
	}
    }
}

/*
 * Walk down a record list extracting records and calling the appropriate
 * cleanup function. Discard records when done.
 * NOTE: We only need to do cleanup at top level since cleanup at nested
 * level would be subsumed when the parent action is forced to abort
 */

void AtomicAction::doCleanup ( RecordList& list_toprocess )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::doCleanup (" << (void *)&list_toprocess <<")" << endl;
    debug_stream.unlock();
	
#endif

    if (list_toprocess.size() > 0)
    {
	Boolean ok = FALSE;
	
	while ((recordBeingHandled = list_toprocess.getFront()))
	{
	    if (actionType == TOP_LEVEL)
		ok = recordBeingHandled->topLevelCleanup();
	    else
		ok = recordBeingHandled->nestedAbort();

	    if ((actionType != TOP_LEVEL) && recordBeingHandled->propagateOnAbort())
		merge(recordBeingHandled);
	    else
	    {
		if (!ok)
		{
		    /* Preserve error messages */
		    ErrorList& eList = recordBeingHandled->getErrorList();
		    Error *error = 0;
		    
		    while ((error = eList.pop()))
			addError(error);   
		}

		delete recordBeingHandled;
	    }
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "AtomicAction::doCommit (" << (void *)&rl <<")" << endl;
    debug_stream.unlock();
	
#endif

    if (rl.size() > 0)
    {
	Boolean ok = FALSE;
	
	while ((recordBeingHandled = rl.getFront()))
	{
	    if (actionType == TOP_LEVEL)
	    {
                if ((ok = recordBeingHandled->topLevelCommit()))
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
		ok = recordBeingHandled->nestedCommit();
		merge(recordBeingHandled);
	    }
	    if (!ok)
	    {
		    /* Preserve error messages */
		    ErrorList& eList = recordBeingHandled->getErrorList();
		    Error *error = 0;

		    while ((error = eList.pop()))
			addError(error);
	    }
	    
	}
    }
}

void AtomicAction::setHandler ( SIG_ARGTYPE handler )
{
    ::signal(SIGINT, handler);
#ifndef WIN32
    ::signal(SIGQUIT, handler);
#endif
    ::signal(SIGTERM, handler);
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
	    addError(new Error(FAC_ATOMIC_ACTION, AR_ADD_FAILED,
			       "(internal) AtomicAction::merge - record rejected\n"));
    }
}

#ifdef NO_INLINES
#  define ATOMICA_CC_
#  include "Arjuna/AtomicA.n"
#  undef ATOMICA_CC_
#endif
