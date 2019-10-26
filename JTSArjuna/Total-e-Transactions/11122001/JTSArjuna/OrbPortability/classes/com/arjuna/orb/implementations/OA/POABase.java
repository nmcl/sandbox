/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: POABase.java,v 1.1.2.1.2.1.2.1.10.1.56.1.2.1 2001/11/27 15:56:28 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.orb.POAImple;
import org.omg.PortableServer.*;
import org.omg.CORBA.Policy;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.BAD_PARAM;
import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;

public abstract class POABase implements POAImple
{

public boolean initialised ()
    {
	return _init;
    }

public boolean supportsBOA ()
    {
	return false;
    }
    
public boolean supportsPOA ()
    {
	return true;
    }

public void init (com.arjuna.orb.ORB orb) throws InvalidName, AdapterInactive, SystemException
    {
	if (!_init)
	{
	    _poa = POAHelper.narrow(orb.orb().resolve_initial_references("RootPOA"));
		    
	    _poa.the_POAManager().activate();
	    _init = true;
	}
    }

public void destroyRootPOA () throws SystemException
    {
	_poa.destroy(true, true);
	_init = false;
    }

public void destroyPOA (String adapterName) throws SystemException
    {
	if (adapterName == null)
	    throw new BAD_PARAM();
	
	org.omg.PortableServer.POA childPoa = (org.omg.PortableServer.POA) _poas.remove(adapterName);
	
	if (childPoa != null)
	{
	    childPoa.destroy(true, true);
	    childPoa = null;
	}
	else
	    throw new BAD_OPERATION();
    }

public org.omg.PortableServer.POA rootPoa () throws SystemException
    {
	return _poa;
    }
    
public void rootPoa (org.omg.PortableServer.POA thePOA) throws SystemException
    {
	_poa = thePOA;

	_init = true;
    }
 
public org.omg.PortableServer.POA poa (String adapterName) throws SystemException
    {
	return (org.omg.PortableServer.POA) _poas.get(adapterName);
    }
    
public void poa (String adapterName, org.omg.PortableServer.POA thePOA) throws SystemException
    {
	_poas.put(adapterName, thePOA);
    }

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	orb.orb().run();
    }

public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	orb.orb().run();
    }

protected org.omg.PortableServer.POA _poa = null;
protected Hashtable                  _poas = new Hashtable();
protected boolean                    _init = false;

}
