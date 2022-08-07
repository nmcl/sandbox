/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ORBBase.java,v 1.1.2.1.4.1.10.1 2001/03/07 15:19:18 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

/**
 * The base class from which all ORB implementations are derived. Each
 * such implementation may be responsible for ensuring that the right
 * ORB specific properties (such as org.omg.CORBA.ORBClass) are set.
 */

public class ORBBase implements ORBImple
{

public synchronized boolean initialised ()
    {
	return _init;
    }

public synchronized void init () throws SystemException
    {
	_orb = org.omg.CORBA.ORB.init();
	_init = true;
    }
 
public synchronized void init (Applet a, Properties p) throws SystemException
    {
	_orb = org.omg.CORBA.ORB.init(a, p);
	_init = true;
    }
 
public synchronized void init (String[] s, Properties p) throws SystemException
    {
	_orb = org.omg.CORBA.ORB.init(s, p);
	_init = true;
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
 
public synchronized org.omg.CORBA.ORB orb () throws SystemException
    {
	return _orb;
    }

public synchronized void orb (org.omg.CORBA.ORB o) throws SystemException
    {
	_orb = o;
	_init = true;
    }

protected org.omg.CORBA.ORB _orb = null;
protected boolean           _init = false;

}
