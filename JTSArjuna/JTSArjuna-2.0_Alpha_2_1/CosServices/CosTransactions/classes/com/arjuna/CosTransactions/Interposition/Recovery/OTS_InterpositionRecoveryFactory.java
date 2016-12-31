/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_InterpositionRecoveryFactory.java,v 1.1.2.2 2000/06/28 13:24:59 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interposition.Recovery;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.CosTransactions.Interposition.Arjuna.*;
import com.arjuna.CosTransactions.Interposition.Strict.*;
import com.arjuna.CosTransactions.Interposition.Restricted.*;
import com.arjuna.CosTransactions.Interposition.OSI.*;
import java.util.Vector;

public class OTS_InterpositionRecoveryFactory
{

public static synchronized void add (InterpositionRecovery toAdd)
    {
	if (toAdd != null)
	    _recoveryList.addElement(toAdd);
    }

public static synchronized void remove (InterpositionRecovery toRemove)
    {
	if (toRemove != null)
	    _recoveryList.removeElement(toRemove);
    }

public static synchronized OTS_ServerTopLevelAction create (String type, Uid u)
    {
	for (int i = 0; i < _recoveryList.size(); i++)
	{
	    InterpositionRecovery ptr = (InterpositionRecovery) _recoveryList.elementAt(i);
	    
	    if ((ptr != null) && (ptr.type().equals(type)))
		return ptr.create(u);
	}
    
	return null;
    }

public static synchronized boolean recover (String type, Uid u)
    {
	for (int i = 0; i < _recoveryList.size(); i++)
	{
	    InterpositionRecovery ptr = (InterpositionRecovery) _recoveryList.elementAt(i);
	    
	    if ((ptr != null) && (ptr.type().equals(type)))
		return ptr.recover(u);
	}

	ErrorStream.stream(ErrorStream.WARNING).err.println("OTS_InterpositionRecoveryFactory could not find recovery for "+type);
	    
	return false;
    }

public static synchronized String[] types ()
    {
	if (_recoveryList.size() == 0)
	    return null;
	
	String[] theTypes = new String[_recoveryList.size()];
	
	for (int i = 0; i < _recoveryList.size(); i++)
	{
	    InterpositionRecovery ptr = (InterpositionRecovery) _recoveryList.elementAt(i);
	    
	    if (ptr != null)
		theTypes[i] = ptr.type();
	    else
		theTypes[i] = null;
	}

	return theTypes;
    }

    static
    {
	OTS_InterpositionRecoveryFactory.add(new OTS_ServerTopLevelActionRecovery());
	OTS_InterpositionRecoveryFactory.add(new OTS_ServerStrictTopLevelActionRecovery());
	OTS_InterpositionRecoveryFactory.add(new OTS_ServerRestrictedTopLevelActionRecovery());
	OTS_InterpositionRecoveryFactory.add(new OTS_ServerOSITopLevelActionRecovery());
    }

private static Vector _recoveryList = new Vector();
    
}


    
    
