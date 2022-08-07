/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: vbroker_3_3.java,v 1.1.2.3.2.1 2000/11/21 11:45:21 nmcl Exp $
 */

package com.arjuna.orb.implementations.OA;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.orb.BOAImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

public class vbroker_3_3 extends BOABase
{

public synchronized void init (com.arjuna.orb.ORB o) throws InvalidName, SystemException
    {
	_boa = ((com.visigenic.vbroker.orb.ORB) o.orb()).BOA_init();
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String s,
			       Properties p) throws InvalidName, SystemException
    {
	_boa = ((com.visigenic.vbroker.orb.ORB) o.orb()).BOA_init(s, p);
	_init = true;
    }
    
public synchronized void init (com.arjuna.orb.ORB o, String[] s,
			       Properties p) throws InvalidName, SystemException
    {
	if (DebugController.enabled())
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OA.initBOA(String[]) not supported by ORB. Using initBOA(String)");
	}

	if (s != null)
	    init(o, s[0], p);
	else
	    init(o, (String) null, p);
    }

public void run (com.arjuna.orb.ORB orb, String name) throws SystemException
    {
	((org.omg.CORBA.BOA) _boa).impl_is_ready();
    }

public void run (com.arjuna.orb.ORB orb) throws SystemException
    {
	((org.omg.CORBA.BOA) _boa).impl_is_ready();
    }

}
