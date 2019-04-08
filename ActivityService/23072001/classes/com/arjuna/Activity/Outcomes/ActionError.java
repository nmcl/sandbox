/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ActionError.java,v 1.1 2001/07/23 13:44:55 nmcl Exp $
 */

package com.arjuna.Activity.Outcomes;

import com.arjuna.OrbCommon.ORBInterface;
import org.omg.CosActivity.Outcome;
import org.omg.CORBA.*;

import org.omg.CORBA.SystemException;

public class ActionError
{

public static Outcome getOutcome (SystemException ex)
    {
	Any a = null;
	
	if (ex != null)
	{
	    a = ORBInterface.orb().create_any();
	    
	    a.insert_Value(ex);
	}

	if (a == null)
	    return new Outcome("ActionError", null);
	else
	    return new Outcome("ActionSystemException", a);
    }
    
};
