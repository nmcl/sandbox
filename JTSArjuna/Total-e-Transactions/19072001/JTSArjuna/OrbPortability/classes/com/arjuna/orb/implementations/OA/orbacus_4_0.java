/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbacus_4_0.java,v 1.1.2.2.2.1.2.1.10.1 2001/03/07 15:18:18 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
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

public class orbacus_4_0 implements POAImple
{

public orbacus_4_0 ()
    {
	/*
	 * Just in case someone uses the raw orb init routine and
	PropertyManager.setProperty("ooc.orb.oa.conc_model", "thread_per_request");
    }
    
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
	    PropertyManager.setProperty("ooc.orb.oa.conc_model", "thread_per_request");

	    _poa = POAHelper.narrow(orb.orb().resolve_initial_references("RootPOA"));
		    
	    _poa.the_POAManager().activate();

	    ((com.ooc.CORBA.ORB) orb.orb()).run_init();  // start up listening thread for co-located applications

	    _init = true;
	}
    }

public void createPOA (String adapterName,
		       Policy[] policies) throws AdapterAlreadyExists, InvalidPolicy, AdapterInactive, SystemException
    {
	if (_poa == null)
	{
	    ErrorStream.warning().println("OA.createPOA called without root POA.");

	    throw new AdapterInactive();
	}

	org.omg.PortableServer.POA childPoa = _poa.create_POA(adapterName, _poa.the_POAManager(), policies);

	childPoa.the_POAManager().activate();

	super._poas.put(adapterName, childPoa);
    }
    
public void rootPoa (org.omg.PortableServer.POA thePOA) throws SystemException
    {
	_poa = thePOA;

	((com.ooc.CORBA.ORB) ORBInterface.orb()).run_init();  // start up listening thread for co-located applications

	_init = true;
    }

}

