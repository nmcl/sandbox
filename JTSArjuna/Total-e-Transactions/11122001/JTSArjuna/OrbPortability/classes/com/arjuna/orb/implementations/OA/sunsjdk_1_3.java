/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: sunsjdk_1_3.java,v 1.1.2.3 2000/11/14 11:55:14 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.orb.BOAImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.SystemException;

public class sunsjdk_1_3 extends BOABase
{

public synchronized void init (com.arjuna.orb.ORB o) throws InvalidName, SystemException
    {
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String s,
			       Properties p) throws InvalidName, SystemException
    {
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String[] s,
			       Properties p) throws InvalidName, SystemException
    {
	_init = true;
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
	throw new BAD_OPERATION("Not supported by ORB.");
    }
    
public synchronized void boa (java.lang.Object b) throws SystemException
    {
	throw new BAD_OPERATION("Not supported by ORB.");
    }

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	try
	{
	    java.lang.Object sync = new java.lang.Object();

	    synchronized (sync)
	    {
		sync.wait();
	    }
	}
	catch (Exception e)
	{
	}
    }

public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	try
	{
	    java.lang.Object sync = new java.lang.Object();

	    synchronized (sync)
	    {
		sync.wait();
	    }
	}
	catch (Exception e)
	{
	}
    }

}
