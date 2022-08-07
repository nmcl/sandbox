/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: jacorb_1_2.java,v 1.1.2.1 2001/02/01 15:31:46 nmcl Exp $
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
import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;

public class jacorb_1_2 extends POABase
{

public jacorb_1_2 ()
    {
	PropertyManager.setProperty("jacorb.poa.monitoring", "off");
	PropertyManager.setProperty("jacorb.poa.thread_pool_max", "20");
	PropertyManager.setProperty("jacorb.poa.thread_pool_min", "10");
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

}

