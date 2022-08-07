/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: jbroker_2_0.java,v 1.1.2.3 2000/11/14 11:55:14 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.orb.BOAImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

public class jbroker_2_0 extends BOABase
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

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	orb.run();
    }
    
public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	orb.run();
    }

}
