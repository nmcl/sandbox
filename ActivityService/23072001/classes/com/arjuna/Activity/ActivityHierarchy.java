/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActivityHierarchy.java,v 1.1 2001/07/23 13:44:49 nmcl Exp $
 */

package com.arjuna.Activity;

import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import org.omg.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.SystemException;

class ActivityHierarchy
{

static final ActivityIdentity getId (Coordinator coord) throws SystemException 
    {
	if (coord == null)
	    throw new BAD_PARAM();
	
	try
	{
	    PropagationContext ctx = coord.get_txcontext();
	    ActivityIdentity id = new ActivityIdentity();
		    
	    id.type = 1;
	    id.timeout = ctx.timeout;
	    id.coord = null;
	    id.ctxId = new byte[ctx.currentTransaction.otid.tid.length];
		    
	    for (int i = 0; i < ctx.currentTransaction.otid.tid.length; i++)
		id.ctxId[i] = ctx.currentTransaction.otid.tid[i];
		    
	    id.pgCtx = null;
	    id.activity_specific_data = ORBInterface.orb().create_any();

	    return id;
	}
	catch (Exception e)
	{
	    throw new BAD_OPERATION(e.toString());
	}
    }
    
static final ActivityIdentity getId (Control tx) throws SystemException
    {
	if (tx == null)
	    throw new BAD_PARAM();
	
	try
	{
	    Coordinator coord = tx.get_coordinator();

	    return ActivityHierarchy.getId(coord);
	}
	catch (Exception e)
	{
	    throw new BAD_OPERATION(e.toString());
	}
    }

static final ActivityIdentity getId (ActivityCoordinatorWrapper aw) throws SystemException
    {
	if (aw == null)
	    throw new BAD_PARAM();
	
	try
	{
	    ActivityIdentity id = new ActivityIdentity();
	
	    id.type = 2;
	    id.timeout = aw.get_timeout();
	    id.coord = aw.get_coordinator();
	    id.ctxId = id.coord.get_global_id();
	    id.pgCtx = null;
	    id.activity_specific_data = ORBInterface.orb().create_any();

	    return id;
	}
	catch (Exception e)
	{
	    throw new BAD_OPERATION(e.toString());
	}	    
    }

static final ActivityContext getContext (ActivityContextStructure ctx) throws SystemException
    {
	if (ctx == null)
	    throw new BAD_PARAM();
	else
	{
	    ActivityContext theContext = new ActivityContext();
	    
	    theContext.hierarchy = new ActivityIdentity[ctx.size()];
	    theContext.invocation_specific_data = ORBInterface.orb().create_any();

	    for (int i = 0; i < ctx.size(); i++)
	    {
		ActivityContextElement e = ctx.elementAt(i);

		if (e == null)
		    throw new BAD_PARAM();
		else
		{
		    if (e.isActivity())
			theContext.hierarchy[i] = ActivityHierarchy.getId((ActivityCoordinatorWrapper) e.getEntity());
		    else
			theContext.hierarchy[i] = ActivityHierarchy.getId((Control) e.getEntity());
		}
	    }

	    return theContext;
	}
    }
	
};
