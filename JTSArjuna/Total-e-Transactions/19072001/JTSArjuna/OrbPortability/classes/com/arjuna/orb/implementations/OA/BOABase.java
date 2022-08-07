/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BOABase.java,v 1.1.2.1.14.1 2001/03/07 15:17:31 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.orb.BOAImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

public abstract class BOABase implements BOAImple
{

public synchronized boolean initialised ()
    {
	return _init;
    }

public synchronized boolean supportsBOA ()
    {
	return true;
    }
    
public synchronized boolean supportsPOA ()
    {
	return false;
    }

public synchronized void shutdown (String name) throws SystemException
    {
	_init = false;
    }
    
public synchronized void destroy () throws SystemException
    {
	shutdown(null);
    }

public synchronized java.lang.Object boa () throws SystemException
    {
	return _boa;
    }
    
public synchronized void boa (java.lang.Object b) throws SystemException
    {
	_boa = b;

	_init = true;
    }

protected java.lang.Object _boa = null;
protected boolean          _init = false;

}
