/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbix2000_1_2.java,v 1.1.2.1 2001/01/19 15:26:28 nmcl Exp $
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
import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;

public class orbix2000_1_2 extends POABase
{

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

