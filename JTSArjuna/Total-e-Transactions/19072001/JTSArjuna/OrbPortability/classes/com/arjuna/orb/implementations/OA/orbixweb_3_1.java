/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbixweb_3_1.java,v 1.1.2.3 2000/11/14 11:55:14 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.orb.BOAImple;
import IE.Iona.OrbixWeb._CORBA;
import IE.Iona.OrbixWeb._OrbixWeb;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

public class orbixweb_2_1 extends BOABase
{

public synchronized void init (com.arjuna.orb.ORB o) throws InvalidName, SystemException
    {
	_boa = o.orb().BOA_init(new String[0], new java.util.Properties());
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String s,
			       Properties p) throws InvalidName, SystemException
    {
	_boa = o.orb().BOA_init(s, p);
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String[] s,
			       Properties p) throws InvalidName, SystemException
    {
	_boa = o.orb().BOA_init(s, p);
	_init = true;
    }

public synchronized void shutdown (String name) throws SystemException
    {
	((org.omg.CORBA.BOA) _boa).deactivate_impl(name);
	_init = false;
    }
    
public synchronized void destroy () throws SystemException
    {
	shutdown(null);
    }

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	int timeout = -1;
	String waitTime = PropertyManager.getProperty(ORBEnvironment.SERVER_TIMEOUT);

	if (waitTime != null)
	{
	    try
	    {
		Integer t = Integer.valueOf(waitTime);
		timeout = t.intValue();
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("Invalid server timeout value "+waitTime);
	    }
	}

	if (timeout != -1)
	    _CORBA.Orbix.impl_is_ready(name, timeout);
	else
	    _CORBA.Orbix.impl_is_ready(name);
    }
    
public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	int timeout = -1;
	String waitTime = PropertyManager.getProperty(ORBEnvironment.SERVER_TIMEOUT);

	if (waitTime != null)
	{
	    try
	    {
		Integer t = Integer.valueOf(waitTime);
		timeout = t.intValue();
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("Invalid server timeout value "+waitTime);
	    }
	}

	if (timeout != -1)
	    _CORBA.Orbix.impl_is_ready(timeout);
	else
	    _CORBA.Orbix.impl_is_ready();
    }

}
