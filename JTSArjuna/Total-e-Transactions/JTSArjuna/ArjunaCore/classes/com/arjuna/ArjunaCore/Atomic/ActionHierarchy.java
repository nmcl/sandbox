/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionHierarchy.java,v 1.1.4.2.4.1.2.2 2001/01/11 14:03:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

import java.io.IOException;
import java.lang.OutOfMemoryError;

/**
 * Class that represents the transaction hierarchy. This class
 * can cope with transaction hierarchies of arbitrary depth.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ActionHierarchy.java,v 1.1.4.2.4.1.2.2 2001/01/11 14:03:08 nmcl Exp $
 * @since JTS 1.0.
 */

public class ActionHierarchy
{

    /**
     * Create a new (blank) hierarchy with sufficient space for the
     * specified number of transactions. Once this space is exhausted,
     * additional space will be obtained dynamically.
     */

public ActionHierarchy (int depth)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "ActionHierarchy::ActionHierarchy("+depth+")");
	}

	hierarchy = null;
	maxHierarchyDepth = depth;
	currentDepth = 0;

	if (maxHierarchyDepth > 0)
	{
	    hierarchy = new ActionInfo[maxHierarchyDepth];

	    for (int i = 0; i < maxHierarchyDepth; i++)
		hierarchy[i] = null;
	}
    }

    /**
     * Create a new copy of the specified transaction hierarchy.
     */

public ActionHierarchy (ActionHierarchy theCopy)
    {
	hierarchy = null;
	maxHierarchyDepth = theCopy.maxHierarchyDepth;
	currentDepth = theCopy.currentDepth;
	
	if (maxHierarchyDepth > 0)
	{
	    hierarchy = new ActionInfo[maxHierarchyDepth];

	    for (int i = 0; i < maxHierarchyDepth; i++)
		hierarchy[i] = null;
	}
    
	for (int i = 0; i < currentDepth; i++)
	{
	    hierarchy[i] = new ActionInfo(theCopy.hierarchy[i]);
	}
    }

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "ActionHierarchy::~ActionHierarchy()");
	}

	if (hierarchy != null)
	    hierarchy = null;
    }

    /**
     * Print out the transaction hierarchy.
     */

public void print (PrintWriter strm)
    {
	strm.println("\tCurrent depth : "+currentDepth);

	if (currentDepth == 0)
	    strm.println("\tAction Uids : NULL");
	else
	{
	    strm.println("\tAction Uids :");	    

	    /*
	     * No need to check if hierarchy[i] is set, since currentDepth
	     * implies it is.
	     */
	
	    for (int i = 0; i < currentDepth; i++)
		strm.println("\t\t"+hierarchy[i].actionUid);
	}
    }

    /**
     * Create a copy of the specified transaction hierarchy. Any
     * hierarchy currently maintained by this object will be
     * lost in favour of the new hierarchy.
     *
     * We check for self-assignment.
     */

public synchronized void copy (ActionHierarchy c)
    {
	/* Beware of A = A */
    
	if (this == c)
	    return;

	if (hierarchy != null)
	    hierarchy = null;
	
	currentDepth = c.currentDepth;
	maxHierarchyDepth = c.maxHierarchyDepth;
      
	if (maxHierarchyDepth > 0)
	{
	    hierarchy = new ActionInfo[maxHierarchyDepth];

	    for (int i = 0; i < maxHierarchyDepth; i++)
		hierarchy[i] = null;
	}

	for (int i = 0; i < currentDepth; i++)
	    hierarchy[i] = new ActionInfo(c.hierarchy[i]);
    }

    /**
     * Overloads Object.equals.
     */
    
public final boolean equals (ActionHierarchy other)
    {
	boolean same = true;

	if (currentDepth == other.depth())
	{
	    for (int i = 0; i < currentDepth; i++)
	    {
		if (hierarchy[i].notEquals(other.getActionInfo(i)))
		{
		    same = false;
		    break;
		}
	    }
	}
	else
	    same = false;

	return same;
    }

    /**
     * Insert new entry growing table as needed.
     * Transaction id will be added as a top-level transaction.
     *
     * @return <code>true</code> if successful, <code>false</code>
     * otherwise.
     */

public final boolean add (Uid actionId)
    {
	return add(actionId, ActionType.TOP_LEVEL);
    }
    
    /**
     * Add the transaction id at the specified level.
     *
     * @return <code>true</code> if successful, <code>false</code>
     * otherwise.
     */

public final boolean add (Uid actionId, int at)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "ActionHierarchy::add("+actionId+", "+at+")");
	}

	boolean result = true;

	if (currentDepth >= maxHierarchyDepth)
	{
	    ActionInfo[] newHier = null;
	    int newDepth = (maxHierarchyDepth + 1) * 2; /*  Ensure non zero */

	    newHier = new ActionInfo[newDepth];

	    if (newHier != null)
	    {
		maxHierarchyDepth = newDepth;
	    
		for (int i = 0; i < currentDepth; i++)
		    newHier[i] = hierarchy[i];

		for (int i = currentDepth; i < newDepth; i++)
		    newHier[i] = null;

		hierarchy = newHier;
		newHier = null;
	    }
	    else
		result = false;
	}

	if (result)
	{
	    if (hierarchy[currentDepth] == null)
		hierarchy[currentDepth] = new ActionInfo();

	    /*
	     * What if at == TOPLEVEL and currentDepth != 0?!
	     */

	    hierarchy[currentDepth].actionUid = new Uid(actionId);
	    hierarchy[currentDepth++].actionType = at;
	}

	return result;
    }

    /**
     * Scan hierarchy looking for a particular Uid starting at deepest
     * and going up. Do NOT go past any top level action.
     *
     * @return <code>true</code> if is ancestor, <code>false</code>
     * otherwise.
     */

public final boolean isAncestor (Uid target)
    {
	boolean result = false;

	for (int i = currentDepth - 1; (i >= 0) && (!result); i--)
	{
	    if (hierarchy[i].actionUid.equals(target))
	    {
		result = true;
		break;
	    }
	    else
	    {
		if (hierarchy[i].actionType == ActionType.TOP_LEVEL)
		    break;
	    }
	}

	return result;
    }

    /**
     * Pack the hierarchy into the buffer.
     */

public void pack (OutputBuffer state) throws IOException
    {
	state.packInt(currentDepth);

	for (int i = 0; i < currentDepth; i++)
	{
	    hierarchy[i].actionUid.pack(state);
	    state.packInt(hierarchy[i].actionType);
	}
    }

    /**
     * CAREFULLY unpack the 'new' hierarchy. We unpack into
     * a temporary to ensure that the current hierarchy is not corrupted.
     */

public void unpack (InputBuffer state) throws IOException
    {
	int newDepth = 0;
	ActionHierarchy newHier = null;
	ActionInfo temp = new ActionInfo();

	newDepth = state.unpackInt();

	try
	{
	    newHier = new ActionHierarchy(newDepth);
	}
	catch (OutOfMemoryError ex)
	{
	    throw new IOException("Memory exhausted.");
	}
	
	for (int i = 0; i < newDepth; i++)
	{
	    temp.actionUid.unpack(state);
	    temp.actionType = state.unpackInt();
	    
	    newHier.add(temp.actionUid, temp.actionType);
	}

	/*
	 * We don't need to copy this if we got here. We can
	 * simply assign to it, and let the garbage collector
	 * figure things out.
	 */

	hierarchy = newHier.hierarchy;
	currentDepth = newHier.currentDepth;
	maxHierarchyDepth = newHier.maxHierarchyDepth;

	newHier.hierarchy = null;
	newHier = null;
    }

    /**
     * Remove the deepest nested transaction from the hierarchy.
     */

public final void forgetDeepest ()
    {
	if (currentDepth > 0)
	{
	    hierarchy[--currentDepth] = null;
	}
    }

    /**
     * Find common prefix in two hierarchies.
     *
     * @return the index of the first common ancestor.
     */

public final int findCommonPrefix (ActionHierarchy oldHierarchy)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "ActionHierarchy::findCommonPrefix()");
	}
	
	int common = 0;
	int max = oldHierarchy.depth();

	while ((common < currentDepth) &&
	       (common < max) &&
	       (hierarchy[common].equals(oldHierarchy.getActionInfo(common))))
	{
	    common++;
	}
	
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "ActionHierarchy::::findCommonPrefix(): prefix is "+common);
	}
	
	return common;
    }

    /**
     * Returns the current depth of the hierarchy.
     *
     * @return the hierarchy depth.
     */

public synchronized final int depth ()
    {
	return currentDepth;
    }

    /**
     * Returns the transaction id of the most deeply nested transaction.
     *
     * @return the <code>Uid</code> of the deepest transaction.
     */

public synchronized final Uid getDeepestActionUid ()
    {
	if (currentDepth > 0)
	    return hierarchy[currentDepth-1].actionUid;
	else
	    return Uid.nullUid();
    }

    /**
     * Returns the ActionInfo for the transaction at the
     * specified level.
     */

public synchronized final ActionInfo getActionInfo (int typeIndex)
    {
	return hierarchy[typeIndex];
    }

public static final int DEFAULT_HIERARCHY_DEPTH = 9;
    
private ActionInfo[] hierarchy;
private int          maxHierarchyDepth;
private int          currentDepth;

}
