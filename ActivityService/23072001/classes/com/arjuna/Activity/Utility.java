/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Utility.javatmpl,v 1.1 2001/07/23 13:44:51 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity;

import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.io.PrintWriter;

public class Utility
{

public static String getCompletionStatus (org.omg.CosActivity.CompletionStatus cs)
    {
	switch ( cs.value() )
	{
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusSuccess  :
	    return "org.omg.CosActivity.CompletionStatus.CompletionStatusSuccess";
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusFail  :
	    return "org.omg.CosActivity.CompletionStatus.CompletionStatusFail";
	case  org.omg.CosActivity.CompletionStatus ._CompletionStatusFailOnly  :
	default:
	    return "org.omg.CosActivity.CompletionStatus.CompletionStatusFailOnly";
	}
    }

public static String getStatus (org.omg.CosActivity.Status s)
    {
	switch ( s.value() )
	{
	case  org.omg.CosActivity.Status ._StatusActive  :
	    return "org.omg.CosActivity.Status.StatusActive";
	case  org.omg.CosActivity.Status ._StatusCompleting  :	
	    return "org.omg.CosActivity.Status.StatusCompleting";
	case  org.omg.CosActivity.Status ._StatusCompleted  :
	    return "org.omg.CosActivity.Status.StatusCompleted";
	case  org.omg.CosActivity.Status ._StatusNoActivity  :
	    return "org.omg.CosActivity.Status.StatusNoActivity";
	default:
	    return "org.omg.CosActivity.Status.StatusUnknown";
	}
    }

public static void printContext (ActivityContext ctx, PrintWriter strm)
    {
	if (ctx == null)
	    strm.print("null");
	else
	{
	    for (int i = 0; i < ctx.hierarchy.length; i++)
	    {
		strm.println("\n\nContent element: "+i);
		strm.println("Context type: "+((ctx.hierarchy[i].type == 1) ? "transaction" : "activity"));
		strm.println("Timeout: "+ctx.hierarchy[i].timeout);
		strm.println("Coord: "+((ctx.hierarchy[i].coord != null) ? "set" : "unset"));
		strm.print("Context id: ");
		
		for (int j = 0; j < ctx.hierarchy[i].ctxId.length; j++)
		{
		    if ((char) ctx.hierarchy[i].ctxId[j] != (char) 0)
		        strm.print((char) ctx.hierarchy[i].ctxId[j]);
   		}

		int size = ((ctx.hierarchy[i].pgCtx != null) ? ctx.hierarchy[i].pgCtx.length : 0);
		
		strm.print("\nPropertyGroups: "+size);
	    }
	}

	strm.flush();
    }
    
public static void printToken (ActivityToken token, PrintWriter strm)
    {
	if (token == null)
	    strm.print("null");
	else
	    printContext(token.get_context(), strm);

	strm.flush();
    }

public static void printActivityInformation (ActivityInformation ai,
					     PrintWriter strm)
    {
	if (ai == null)
	    strm.print("null");
	else
	{
	    strm.print("activityId: ");
	    
	    for (int i = 0; i < ai.activityId.length; i++)
		strm.print((char) ai.activityId[i]);

	    strm.print("\nstatus: "+getCompletionStatus(ai.status));
	}

	strm.flush();
    }
    
};
