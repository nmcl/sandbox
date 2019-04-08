/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SignalSetList.java,v 1.1 2001/07/23 13:44:54 nmcl Exp $
 */

package com.arjuna.Activity.ActivityCoordinator;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.Activity.Signals.*;
import com.arjuna.Activity.SignalSets.*;
import org.omg.CosTransactions.*;
import org.omg.CosActivity.CompletionStatus;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

import org.omg.CosActivity.SignalSetAlreadyRegistered;
import org.omg.CosActivity.SignalSetUnknown;
import org.omg.CosActivity.ActionNotFound;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;
import java.lang.ArrayIndexOutOfBoundsException;

/*
 * This object maintains the lists of Actions and the names of the SignalSets
 * they have been registered with, and the global Actions (these are Actions
 * that receive every Signal sent).
 */

class SignalSetList
{

public SignalSetList (Uid uid)
    {
	_signalSets = new Hashtable();

	/*
	 * Register the system defined SignalSets. These may never actually
	 * be used.
	 */

	try
	{
	    /*
	     * Pass the activity's identity to the Synchronization
	     * SignalSet so it can add the ActivityInformation structure
	     * to the Signals.
	     */

	    java.lang.Object[] params = new java.lang.Object[1];
	    
	    params[0] = uid;
	    
	    _signalSets.put(SystemDefined.Synchronization, SignalSetFactory.getFactory().create(SystemDefined.Synchronization, params));
	    _signalSets.put(SystemDefined.ChildLifetime, SignalSetFactory.getFactory().create(SystemDefined.ChildLifetime, params));
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }

public synchronized boolean haveSignalSet (String signal_set_name)
    {
	if (_signalSets.get(signal_set_name) != null)
	    return true;
	else
	{
	    /*
	     * We don't register all pre-defined SignalSets because
	     * some of them (e.g., ChildLifetime) are actually
	     * multiple instances of the same thing, which requires
	     * a bit more work.
	     */

	    return SystemDefined.member(signal_set_name);
	}
    }
    
public synchronized SignalSet getSignalSet (String signal_set_name,
					    java.lang.Object[] params) throws SignalSetUnknown, SystemException
    {
	if (_signalSets == null)
	    throw new SignalSetUnknown();
	else
	{
	    SignalSet ss = (SignalSet) _signalSets.get(signal_set_name);
	    
	    if (ss == null)
	    {
		ss = SignalSetFactory.getFactory().create(signal_set_name, params);
		
		if (ss == null)
		    throw new SignalSetUnknown();
	    }
	    
	    return ss;
	}
    }
    
public synchronized void addSignalSet (SignalSet signal_set) throws SignalSetAlreadyRegistered, SystemException
    {
	if (signal_set == null)
	    throw new BAD_PARAM();
	
	String signal_set_name = signal_set.signal_set_name();
	
	if (_signalSets.get(signal_set_name) != null)
	    throw new SignalSetAlreadyRegistered();

	if (SystemDefined.member(signal_set_name))
	    throw new SignalSetAlreadyRegistered();
	else
	    _signalSets.put(signal_set_name, signal_set);
    }

public synchronized void removeSignalSet (String signal_set_name) throws SignalSetUnknown, SystemException
    {
	if (signal_set_name == null)
	    throw new BAD_PARAM();

	if (SystemDefined.member(signal_set_name))
	    throw new BAD_OPERATION();
	
	SignalSet ss = (SignalSet) _signalSets.remove(signal_set_name);
	
	if (ss == null)
	    throw new SignalSetUnknown();
    }

private Hashtable     _signalSets;

};
