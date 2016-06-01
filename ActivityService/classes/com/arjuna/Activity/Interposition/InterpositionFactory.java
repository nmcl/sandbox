/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: InterpositionFactory.java,v 1.1 2001/07/23 13:44:55 nmcl Exp $
 */

package com.arjuna.Activity.Interposition;

import org.omg.CosActivity.*;
import org.omg.CORBA.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.NO_IMPLEMENT;

public class InterpositionFactory
{

public ActivityToken create (ActivityContext ctx) throws InvalidToken, SystemException
    {
	if (ctx == null)
	    throw new InvalidToken();

	/*
	 * We don't use the implementation_specific_data portion of the
	 * ActivityContext so we must remember to null them out.
	 */

	// not implemented

	throw new NO_IMPLEMENT();
    }
	
};
