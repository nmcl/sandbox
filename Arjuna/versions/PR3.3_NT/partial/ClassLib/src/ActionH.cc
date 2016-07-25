/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionH.cc,v 1.10 1995/09/18 15:15:42 ngdp Exp $
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

static const char RCSid[] = "$Id: ActionH.cc,v 1.10 1995/09/18 15:15:42 ngdp Exp $";

class ActionInfo
{
public:
    ActionInfo ();
    ActionInfo (const ActionInfo&);

    ActionInfo& operator= (const ActionInfo&);
    
    Boolean operator== (const ActionInfo&) const;
    Boolean operator!= (const ActionInfo&) const;
    
    Uid actionUid;
    AtomicAction::ActionType actionType;
};

ActionInfo::ActionInfo ()
		       : actionUid(NIL_UID),
				actionType(AtomicAction::NESTED)
{    
}

ActionInfo::ActionInfo ( const ActionInfo& other )
		       : actionUid(other.actionUid),
				actionType(other.actionType)
{    
}

ActionInfo& ActionInfo::operator= ( const ActionInfo& other )
{
    if (this != &other)
    {
		actionUid = other.actionUid;
		actionType = other.actionType;
    }

    return *this;
}

Boolean ActionInfo::operator== ( const ActionInfo& other ) const
{
    if ((actionUid == other.actionUid) &&
		(actionType == other.actionType))
	return TRUE;
		else
	return FALSE;
}

Boolean ActionInfo::operator!= ( const ActionInfo& other ) const
{
    if ((actionUid != other.actionUid) ||
		(actionType != other.actionType))
	return TRUE;
		else
	return FALSE;
}

/*
 * Public constructors and destructor
 */

ActionHierarchy::ActionHierarchy ( unsigned int depth )
				 : hierarchy(0),
				   maxHierarchyDepth(depth),
				   currentDepth(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::ActionHierarchy(" << depth << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    if (maxHierarchyDepth > 0)
    {
		hierarchy = new ActionInfo[maxHierarchyDepth];
    }
}

ActionHierarchy::ActionHierarchy ( const ActionHierarchy& copy )
				 : hierarchy(0),
				   maxHierarchyDepth(copy.maxHierarchyDepth),
				   currentDepth(copy.currentDepth)
{
      
    if (maxHierarchyDepth > 0)
    {
		hierarchy = new ActionInfo[maxHierarchyDepth];
    }
    
    for (unsigned int i = 0; i < currentDepth; i++)
		hierarchy[i] = copy.hierarchy[i];
}

ActionHierarchy::~ActionHierarchy ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::~ActionHierarchy()" << endl;
    debug_stream.unlock();
    
#endif

    if (hierarchy)
    {
	delete [] hierarchy;
	hierarchy = 0;
    }
}

/*
 * Public non-virtual member functions
 */

ostream& operator<< (ostream& strm, const ActionHierarchy& toPrint )
{
    strm << "Current depth : " << toPrint.currentDepth << endl;
    strm << "Action Uids :" << endl;
    
    for (unsigned int i = 0; i < toPrint.currentDepth; i++)
	strm << "\t" << toPrint.hierarchy[i].actionUid << endl;

    return strm;
}

	
ActionHierarchy& ActionHierarchy::operator= ( const ActionHierarchy& copy )
{
    /* Beware of A = A */
    
    if (this == &copy)
	return *this;

    if (hierarchy)
    {
	delete [] hierarchy;
    }

    currentDepth = copy.currentDepth;
    maxHierarchyDepth = copy.maxHierarchyDepth;
      
    if (maxHierarchyDepth > 0)
    {
	hierarchy = new ActionInfo[maxHierarchyDepth];
    }
    
    for (unsigned int i = 0; i < currentDepth; i++)
	hierarchy[i] = copy.hierarchy[i];

    return *this;
}
    
Boolean ActionHierarchy::operator== ( const ActionHierarchy& other ) const
{
    Boolean same = TRUE;
    
    if (currentDepth == other.depth())
    {
	for (unsigned int i = 0; i < currentDepth; i++)
	    if (hierarchy[i] != other.getActionInfo(i))
	    {
		same = FALSE;
		break;
	    }
    }
    else
	same = FALSE;
    
    return same;
}

/*
 * Add. Insert new entry growing table as needed
 */

Boolean ActionHierarchy::add ( const Uid& actionId, AtomicAction::ActionType at )
{

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::add(" << actionId << ", " << at << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean result = TRUE;
    
    if (currentDepth >= maxHierarchyDepth)
    {
	ActionInfo *newHier = 0;
	unsigned int newDepth = (maxHierarchyDepth + 1) * 2; /*  Ensure non zero */
	
	newHier = new ActionInfo[newDepth];

	if (newHier)
	{
	    maxHierarchyDepth = newDepth;
	    
	    for (unsigned int i = 0; i < currentDepth; i++)
	    {
		newHier[i] = hierarchy[i];
	    }

	    delete [] hierarchy;

	    hierarchy = newHier;
	}
	else
	    result = FALSE;
    }

    if (result)
    {
	hierarchy[currentDepth].actionType = at;
	hierarchy[currentDepth++].actionUid = actionId;
    }

    return result;
}

/*
 * Scan hierarchy looking for a particular Uid starting at deepest
 * and going up. Do NOT go past any top level action
 */

Boolean ActionHierarchy::isAncestor ( const Uid& target ) const
{
    Boolean result = FALSE;

    for (int i = currentDepth - 1; (i >= 0) && (!result); i--)
    {
	if (hierarchy[i].actionUid == target)
	{
	    result = TRUE;
	    break;
	}
	else
	    if (hierarchy[i].actionType == AtomicAction::TOP_LEVEL)
		break;
    }

    return result;
}

Boolean ActionHierarchy::pack ( Buffer& state ) const
{
    Boolean result = state.pack(currentDepth);

    if (result)
    {
		for (unsigned int i = 0; (i < currentDepth) && result; i++)
		{
			result = hierarchy[i].actionUid.pack(state);
			if (result)
				result = state.pack((int) hierarchy[i].actionType);
		}
    }
    
    return result;
}

/*
 * CAREFULLY unpack the 'new' hierarchy. We unpack into
 * a temporary to ensure that the current hierarchy is not corrupted
 */

Boolean ActionHierarchy::unpack ( Buffer& state )
{
    unsigned int newDepth = 0;
    Boolean result = FALSE;
    ActionHierarchy *newHier = 0;

    if (state.unpack(newDepth))
    {
	ActionInfo temp;
	unsigned int dummy;
	
	newHier = new ActionHierarchy(newDepth);
	result = (newHier != 0);
	
	for (unsigned int i = 0; (i < newDepth) && result; i++)
	{
	    result = temp.actionUid.unpack(state);
	    if (result)
		result = state.unpack(dummy);
	    if (result)
	    {
		temp.actionType = (AtomicAction::ActionType)dummy;
		newHier->add(temp.actionUid, temp.actionType);
	    }
	}
    }

    if (newHier)
    {
	if (result)
	    *this = *newHier;

	delete newHier;
    }
    
    return result;
}

Boolean ActionHierarchy::establishHierarchy ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy()" << endl;
    debug_stream.unlock();
    
#endif

    unsigned int commonRoot = 0;
    ServerAtomicAction *SAA = ServerAtomicAction::currentSAA();
    unsigned int currentHierarchyDepth = (SAA ? SAA->hierarchyDepth() : 0);

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
    debug_stream << "ActionHierarchy::establishHierarchy(): new depth = " 
		 << currentDepth << " old depth = " 
		 << currentHierarchyDepth << endl;
    debug_stream.unlock();
    
#endif
    
    /*
     * First clean up the current hierarchy. This consists of first determining
     * if the new hierarchy is smaller than the current in which case those
     * "excess" actions need terminating
     */

    if (currentHierarchyDepth > 0)
    {
	ActionHierarchy *currentHierarchy = 0;

	while ((AtomicAction::Current()) && 
	       (AtomicAction::Current()->hierarchyDepth() > currentHierarchyDepth))
	{
	    /* 
	     * This should not happen in practice since it means that
	     * a local action was still executing after the operation
	     * returned to the original caller. Still just in case...
	     */

#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
	    debug_stream << "ActionHierarchy::establishHierarchy(): cleaning up old action " 
			 << AtomicAction::Current()->get_uid() << endl;
	    debug_stream.unlock();
	    
#endif

	    AtomicAction::Current()->Abort();
	}
	
	currentHierarchy = ServerAtomicAction::currentSAA()->getHierarchy();
	commonRoot = findCommonPrefix(*currentHierarchy);
	
	/* 
	 * Failure makes it possible for the client hierarchy
	 * to be smaller than that in the server. In this case
	 * force the "excess" actions to be correctly terminated
	 */

	for (unsigned int i = commonRoot; i < currentHierarchyDepth; i++)
	{
	    SAA = ServerAtomicAction::currentSAA();

	    if (SAA)
	    {
#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
		debug_stream << "ActionHierarchy::establishHierarchy(): finishing old action " 
			     << SAA->get_uid() << endl;
		debug_stream.unlock();
		
#endif

		SAA->finish();
		delete SAA;
	    }
	}

    }
    
    /*
     * Now start any new actions that exist in the client but not the server
     */

    while (commonRoot < currentDepth)
    {
	SAA = new ServerAtomicAction(hierarchy[commonRoot].actionUid, hierarchy[commonRoot].actionType);

#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PUBLIC;
	debug_stream << "ActionHierarchy::establishHierarchy(): starting up new action " 
		     << SAA->get_uid() << endl;
	debug_stream.unlock();
	
#endif
	SAA->Begin();

	commonRoot++; 
    }

    return TRUE;
}

void ActionHierarchy::forgetDeepest ()
{
    if (currentDepth > 0)
    {
	hierarchy[--currentDepth].actionUid = NIL_UID;
    }
    
}

/*
 * Find common prefix in two hierarchies.
 */

unsigned int ActionHierarchy::findCommonPrefix ( const ActionHierarchy& oldHierarchy ) const
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "ActionHierarchy::::findCommonPrefix()" << endl;
    debug_stream.unlock();
    
#endif

    unsigned int common = 0;
    unsigned int max = oldHierarchy.depth();
    

    while ((common < currentDepth) &&
	   (common < max) &&
	   (hierarchy[common] == oldHierarchy.getActionInfo(common)))
    {
	common++;
    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ATOMIC_ACTION << VIS_PRIVATE;
    debug_stream << "ActionHierarchy::::findCommonPrefix(): prefix is " 
		 << common << endl;
    debug_stream.unlock();
    
#endif

    return common;
}

unsigned int ActionHierarchy::depth () const
{
    return currentDepth;
}

const Uid& ActionHierarchy::getDeepestActionUid ( ) const
{
    if (currentDepth > 0)
	return hierarchy[currentDepth-1].actionUid;
    else
	return NIL_UID;
}

const ActionInfo& ActionHierarchy::getActionInfo ( unsigned int typeIndex ) const
{
    return hierarchy[typeIndex];
}

