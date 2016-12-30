/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_RecoveryCreator.java,v 1.1 2000/02/25 14:05:23 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import org.omg.CosTransactions.*;

class DummyCreator extends OTS_RecoveryCreator
{
    
public RecoveryCoordinator create (Resource res, Object[] params)
    {
	return null;
    }
    
};

public abstract class OTS_RecoveryCreator
{
    
public static final OTS_RecoveryCreator getCreator ()
    {
	if (_theCreator == null)
	    _theCreator = new DummyCreator();
    
	return _theCreator;
    }

public static final void setCreator (OTS_RecoveryCreator c)
    {
	if (c == null)
	    System.err.println("WARNING - OTS_RecoveryCreator.setCreator - being passed NULL. Will ignore.");
	else
	    _theCreator = c;
    }

public static final RecoveryCoordinator createRecoveryCoordinator (Resource res, Object[] params)
    {
	return getCreator().create(res, params);
    }

protected abstract RecoveryCoordinator create (Resource res, Object[] params);

private static OTS_RecoveryCreator _theCreator = null;
 
};
