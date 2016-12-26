/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_RecoveryCreator.java,v 1.1.8.2.2.1.6.1.10.1.26.1 2001/05/24 12:35:52 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import org.omg.CosTransactions.*;

import org.omg.CORBA.NO_IMPLEMENT;
import org.omg.CORBA.SystemException;

/*
 * Default visibility.
 */

class DummyCreator extends OTS_RecoveryCreator
{
    
public RecoveryCoordinator create (Resource res, Object[] params) throws SystemException
    {
	throw new NO_IMPLEMENT();
    }

public void destroy (RecoveryCoordinator rc) throws SystemException
    {
    }

public void destroyAll (Object[] params) throws SystemException
    {
    }
    
};

/**
 * This abstract class is used to allow dynamic registration of creators
 * for RecoveryCoordinators. This means that we can provide ORB specific
 * implementations without having to have implementation specific
 * information within the transaction service.
 */

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
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_RecoveryCreator.setCreator - being passed NULL. Will ignore.");
	}
	else
	    _theCreator = c;
    }

public static final RecoveryCoordinator createRecoveryCoordinator (Resource res, Object[] params) throws SystemException
    {
	return getCreator().create(res, params);
    }

public static final void destroyRecoveryCoordinator (RecoveryCoordinator rc) throws SystemException
    {
	getCreator().destroy(rc);
    }

public static final void destroyAllRecoveryCoordinators (Object[] params) throws SystemException
    {
	getCreator().destroyAll(params);
    }
    
protected abstract RecoveryCoordinator create (Resource res, Object[] params) throws SystemException;
protected abstract void destroy (RecoveryCoordinator rc) throws SystemException;
protected abstract void destroyAll (Object[] params) throws SystemException;
    
private static OTS_RecoveryCreator _theCreator = null;
 
};
