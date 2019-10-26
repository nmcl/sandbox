/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbacus_3_2.java,v 1.1.2.3.14.1 2001/03/07 15:18:18 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.orb.BOAImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

public class orbacus_3_2 extends BOABase
{

public synchronized void init (com.arjuna.orb.ORB o) throws InvalidName, SystemException
    {
	_boa = ((com.ooc.CORBA.ORB) o.orb()).BOA_init(new String[0], new java.util.Properties());
	((com.ooc.CORBA.BOA) _boa).conc_model(com.ooc.CORBA.BOA.ConcModel.ConcModelThreadPerRequest);
	((com.ooc.CORBA.BOA) _boa).init_servers();

	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String s,
			       Properties p) throws InvalidName, SystemException
    {
	if (DebugController.enabled())
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("initBOA(String) not supported by ORB. Using initBOA(String[])");
	}

	String[] args = new String[1];
	args[0] = 2;

	initBOA(args, p);
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String[] s,
			       Properties p) throws InvalidName, SystemException
    {
	_boa = ((com.ooc.CORBA.ORB) o.orb()).BOA_init(s, p);

	((com.ooc.CORBA.BOA) _boa).conc_model(com.ooc.CORBA.BOA.ConcModel.ConcModelThreadPerRequest);
	((com.ooc.CORBA.BOA) _boa).init_servers();

	_init = true;
    }

public synchronized void shutdown (String name) throws SystemException
    {
	((com.ooc.CORBA.BOA) _boa).deactivate_impl(null);
	_init = false;
    }
    
public synchronized void destroy () throws SystemException
    {
	shutdown(null);
    }

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	((com.ooc.CORBA.BOA) _boa).impl_is_ready(null);
    }
    
public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	((com.ooc.CORBA.BOA) _boa).impl_is_ready(null);
    }

public synchronized void boa (java.lang.Object b) throws SystemException
    {
	_boa = b;

	((com.ooc.CORBA.BOA) _boa).conc_model(com.ooc.CORBA.BOA.ConcModel.ConcModelThreadPerRequest);
	((com.ooc.CORBA.BOA) _boa).init_servers();

	_init = true;
    }

}
