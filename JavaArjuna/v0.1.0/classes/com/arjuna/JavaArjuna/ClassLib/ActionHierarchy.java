/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionHierarchy.java,v 1.7 1998/07/06 13:26:10 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.Common.Uid;
import com.arjuna.JavaArjuna.Common.InputBuffer;
import com.arjuna.JavaArjuna.Common.OutputBuffer;

import java.io.PrintStream;
import java.io.IOException;

public class ActionHierarchy
{

public ActionHierarchy (int depth)
    {
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

    /*
     * https://github.com/nmcl/sandbox/issues/75
public void finalize ()
    {
	if (hierarchy != null)
	    hierarchy = null;
    }
    */

public void print (PrintStream strm)
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
    
public final boolean equals (ActionHierarchy other)
    {
	boolean same = true;

	if (currentDepth == other.depth())
	{
	    for (int i = 0; i < currentDepth; i++)
		if (hierarchy[i].notEquals(other.getActionInfo(i)))
		{
		    same = false;
		    break;
		}
	}
	else
	    same = false;

	return same;
    }

    /*
     * Add. Insert new entry growing table as needed
     */

public final boolean add (Uid actionId)
    {
	return add(actionId, BasicAction.TOP_LEVEL);
    }
    
public final boolean add (Uid actionId, int at)
    {
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

	    hierarchy[currentDepth].actionUid = new Uid(actionId);
	    hierarchy[currentDepth++].actionType = at;
	}

	return result;
    }

    /*
     * Scan hierarchy looking for a particular Uid starting at deepest
     * and going up. Do NOT go past any top level action.
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
		if (hierarchy[i].actionType == BasicAction.TOP_LEVEL)
		    break;
	    }
	}

	return result;
    }

public void pack (OutputBuffer state) throws IOException
    {
	state.packInt(currentDepth);

	for (int i = 0; i < currentDepth; i++)
	{
	    hierarchy[i].actionUid.pack(state);
	    state.packInt(hierarchy[i].actionType);
	}
    }

    /*
     * CAREFULLY unpack the 'new' hierarchy. We unpack into
     * a temporary to ensure that the current hierarchy is not corrupted
     */

public void unpack (InputBuffer state) throws IOException
    {
	int newDepth = 0;
	ActionHierarchy newHier = null;
	ActionInfo temp = new ActionInfo();

	newDepth = state.unpackInt();
	
	newHier = new ActionHierarchy(newDepth);

	if (newHier == null)
	    throw new IOException("Memory exhausted.");
	
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

public final void forgetDeepest ()
    {
	if (currentDepth > 0)
	{
	    hierarchy[--currentDepth] = null;
	}
    }

    /*
     * Find common prefix in two hierarchies.
     */

public final int findCommonPrefix (ActionHierarchy oldHierarchy)
    {
	int common = 0;
	int max = oldHierarchy.depth();
    

	while ((common < currentDepth) &&
	       (common < max) &&
	       (hierarchy[common].equals(oldHierarchy.getActionInfo(common))))
	{
	    common++;
	}

	return common;
    }

public synchronized final int depth ()
    {
	return currentDepth;
    }

public synchronized final Uid getDeepestActionUid ()
    {
	if (currentDepth > 0)
	    return hierarchy[currentDepth-1].actionUid;
	else
	    return Uid.nullUid();
    }

public synchronized final ActionInfo getActionInfo (int typeIndex)
    {
	return hierarchy[typeIndex];
    }

public static final int DEFAULT_HIERARCHY_DEPTH = 9;
    
private ActionInfo[] hierarchy;
private int maxHierarchyDepth;
private int currentDepth;

}
