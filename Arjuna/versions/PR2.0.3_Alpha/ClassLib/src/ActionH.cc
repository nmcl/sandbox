/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionH.cc,v 1.3 1993/07/27 10:38:53 ngdp Exp $
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

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

/*
 * RCS identifying strings for this code and its matching header file
 */

static const char RCSid[] = "$Id: ActionH.cc,v 1.3 1993/07/27 10:38:53 ngdp Exp $";

struct ActionInfo
{
    Uid *actionUid;
    AtomicAction::ActionType actionType;
};

/* 
 * Declare some static variables that we need...
 */

/*
 * Public constructors and destructor
 */

ActionHierarchy::ActionHierarchy (int depth)
                           : hierarchy(0),
			     maxHierarchyDepth(depth),
			     currentDepth(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::ActionHierarchy(" << depth << ")\n" << flush;
#endif
    
    if (maxHierarchyDepth > 0)
    {
	hierarchy = new ActionInfo[maxHierarchyDepth];
    
	for (int i = 0; i < maxHierarchyDepth; i++)
	{
	    hierarchy[i].actionUid = 0;
	}
    }
}


ActionHierarchy::~ActionHierarchy ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::~ActionHierarchy()\n" << flush;
#endif

    if (hierarchy)
    {
	for (int i = 0; i < currentDepth; i++)
	{
	    delete hierarchy[i].actionUid;
	}
	delete [] hierarchy;
	hierarchy = 0;
    }
}

/*
 * Public non-virtual member functions
 */

void ActionHierarchy::add ( const Uid& actionId, AtomicAction::ActionType at )
{

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::add(" << actionId << ", " << at << ")\n" << flush;
#endif
    
    if (currentDepth < maxHierarchyDepth)
    {
	hierarchy[currentDepth].actionType = at;
	hierarchy[currentDepth++].actionUid = new Uid(actionId);
    }
    
}

Boolean ActionHierarchy::pack ( Buffer& state ) const
{
    state.pack(currentDepth);
    
    for (int i = 0; i < currentDepth; i++)
    {
	hierarchy[i].actionUid->pack(state);
	state.pack(hierarchy[i].actionType);
    }

    return TRUE;
}

Boolean ActionHierarchy::unpack ( Buffer& state )
{
    int newDepth;

    state.unpack(newDepth);

    if (maxHierarchyDepth < newDepth)
    {
	int i;
	
	/* old hierarchy is too small */

	ActionInfo *newHierarchy = new ActionInfo[newDepth];

	for (i = 0; i < currentDepth; i++)
	{
	    /* Preserve old Uid to avoid excessive creation */

	    newHierarchy[i].actionUid = hierarchy[i].actionUid;
	}

	for (i = currentDepth; i < newDepth; i++)
	{
	    /* Create any extra ones needed */

	    newHierarchy[i].actionUid = new Uid(NIL_UID);
	}

	delete [] hierarchy;

	hierarchy = newHierarchy;
	maxHierarchyDepth = newDepth;
    }
    else
    {
	/* new hiearchy is smaller - purge excess */

	int i;
	
	for (i = newDepth; i < currentDepth; i++)
	    delete hierarchy[i].actionUid;
    }

    if (newDepth > 0)
    {
	int dummy;
	
	for (int i = 0; i < newDepth; i++)
	{
	    if (hierarchy[i].actionUid == 0)
		hierarchy[i].actionUid = new Uid(NIL_UID);
	    hierarchy[i].actionUid->unpack(state);
	    state.unpack(dummy);
	    hierarchy[i].actionType = (AtomicAction::ActionType)dummy;
	}
    }

    currentDepth = newDepth;

    return TRUE;
}

Boolean ActionHierarchy::establishHierarchy ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy()\n" << flush;
#endif

    int commonRoot = 0;
    ServerAtomicAction *SAA = ServerAtomicAction::Current();
    int currentHierarchyDepth = (SAA ? SAA->hierarchyDepth() : 0);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy(): new depth = " 
		 << currentDepth << " old depth = " 
		 << currentHierarchyDepth << endl << flush;
#endif
    
    /*
     * First clean up the current hierarchy. This consists of first determining
     * if the new hierarchy is smaller than the current in which case those
     * "excess" actions need terminating
     */

    if (currentHierarchyDepth > 0)
    {
	ActionHierarchy currentHierarchy(currentHierarchyDepth);

	while ((AtomicAction::Current()) && 
	       (AtomicAction::Current()->hierarchyDepth() > currentHierarchyDepth))
	{
	    /* 
	     * This should not happen in practice since it means that
	     * a local action was still executing after the operation
	     * returned to the original caller. Still just in case...
	     */

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy(): cleaning up old action " 
		 << AtomicAction::Current()->get_uid() << endl << flush;
#endif

	    AtomicAction::Current()->Abort();
	}
	
	ServerAtomicAction::Current()->getHierarchy(currentHierarchy);
	commonRoot = findCommonPrefix(currentHierarchy);
	
	/* 
	 * Failure makes it possible for the client hierarchy
	 * to be smaller than that in the server. In this case
	 * force the "excess" actions to be correctly terminated
	 */

	for (int i = commonRoot; i < currentHierarchyDepth; i++)
	{
	    SAA = ServerAtomicAction::Current();

	    if (SAA)
	    {
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy(): finishing old action " 
		 << SAA->get_uid() << endl << flush;
#endif

		SAA->Finish();
		delete SAA;
	    }
	}
    }
    
    /*
     * Now start any new actions that exist in the client but not the server
     */

    while (commonRoot < currentDepth)
    {
	SAA = new ServerAtomicAction(*hierarchy[commonRoot].actionUid, hierarchy[commonRoot].actionType);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy(): starting up new action " 
		 << SAA->get_uid() << endl << flush;
#endif
	SAA->Begin();

	commonRoot++; 
    }

    return TRUE;
}

/*
 * Find common prefix in two hierarchies.
 */

int ActionHierarchy::findCommonPrefix ( const ActionHierarchy& oldHierarchy ) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ActionHierarchy::::findCommonPrefix()\n" << flush;
#endif

    int common = 0;

    while ((common < currentDepth) && 
	   (*hierarchy[common].actionUid == oldHierarchy.getUid(common)) &&
	   (hierarchy[common].actionType == oldHierarchy.getActionType(common)))
    {
	common++;
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ActionHierarchy::::findCommonPrefix(): prefix is " 
		 << common << endl << flush;
#endif

    return common;
}

const Uid& ActionHierarchy::getUid ( int uidIndex ) const
{
    if (uidIndex < currentDepth)
	return *hierarchy[uidIndex].actionUid;
    else
	return NIL_UID;
}

const AtomicAction::ActionType ActionHierarchy::getActionType ( int typeIndex ) const
{
    if (typeIndex < currentDepth)
	return hierarchy[typeIndex].actionType;
    else
	return AtomicAction::NESTED;
}

