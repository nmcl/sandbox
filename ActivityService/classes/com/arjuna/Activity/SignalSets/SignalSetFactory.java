/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SignalSetFactory.java,v 1.1 2001/07/23 13:44:56 nmcl Exp $
 */

package com.arjuna.Activity.SignalSets;

import com.arjuna.Activity.SignalSets.*;
import org.omg.CosActivity.SignalSet;
import org.omg.CORBA.*;
import java.util.Hashtable;

import org.omg.CosActivity.SignalSetAlreadyRegistered;
import org.omg.CosActivity.SignalSetUnknown;
import org.omg.CORBA.BAD_PARAM;
import java.lang.ExceptionInInitializerError;

public class SignalSetFactory
{

public static SignalSetFactory getFactory ()
    {
	return _theFactory;
    }

public boolean haveSignal (String signalSetName)
    {
	if (signalSetName == null)
	    return false;
	
	if (_creators.get(signalSetName) == null)
	    return false;
	else
	    return true;
    }
    
public SignalSet create (String signalSetName, java.lang.Object[] params) throws SignalSetUnknown
    {
	SignalSetCreator sc = (SignalSetCreator) _creators.get(signalSetName);
	
	if (sc == null)
	    throw new SignalSetUnknown();
	else
	    return sc.create(params);
    }
    
public void addCreator (SignalSetCreator c) throws SignalSetAlreadyRegistered, org.omg.CORBA.SystemException
    {
	if (c == null)
	    throw new BAD_PARAM();
	else
	{
	    if (_creators.get(c.name()) != null)
		throw new SignalSetAlreadyRegistered();
	    else
		_creators.put(c.name(), c);
	}
    }
    
public void removeCreator (SignalSetCreator c) throws SignalSetUnknown, org.omg.CORBA.SystemException
    {
	if (c == null)
	    throw new BAD_PARAM();
	else
	{
	    if (_creators.remove(c.name()) == null)
		throw new SignalSetUnknown();
	}
    }
    
protected SignalSetFactory ()
    {
	_creators = new Hashtable();
    }
    
private Hashtable _creators;

private static SignalSetFactory _theFactory;
    
    static
    {
	_theFactory = new SignalSetFactory();

	/*
	 * Now add the default, system signal sets.
	 */

	try
	{
	    _theFactory.addCreator(new SynchronizationCreator());
	    _theFactory.addCreator(new ChildLifetimeCreator());
	}
	catch (Exception e)
	{
	    throw new ExceptionInInitializerError(e);
	}
    }
    
};
