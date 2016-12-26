/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbacus_4_0.java,v 1.1.2.2.4.1.10.1 2001/03/07 15:19:18 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

public class orbacus_4_0 extends ORBBase
{

public orbacus_4_0 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "com.ooc.CORBA.ORBSingleton", true);
	PropertyManager.setProperty("ooc.orb.conc_model", "threaded");
    }

public synchronized void init () throws SystemException
    {
	PropertyManager.setProperty("ooc.orb.conc_model", "threaded");

	super.init();

	((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);
    }
 
public synchronized void init (Applet a, Properties p) throws SystemException
    {
	PropertyManager.setProperty("ooc.orb.conc_model", "threaded");

	super.init(a, p);

	((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);
    }
 
public synchronized void init (String[] s, Properties p) throws SystemException
    {
	PropertyManager.setProperty("ooc.orb.conc_model", "threaded");

	super.init(s, p);

	((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);
    }

public synchronized void shutdown () throws SystemException
    {
	_orb.shutdown(false);
	_init = false;
    }

public synchronized void destroy () throws SystemException
    {
	shutdown();
    }

public synchronized void orb (org.omg.CORBA.ORB o) throws SystemException
    {
	_orb = o;
	_init = true;

	((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);
    }
 
}

