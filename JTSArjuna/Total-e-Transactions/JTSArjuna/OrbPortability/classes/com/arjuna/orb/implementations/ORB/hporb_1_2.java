/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: hporb_1_2.java,v 1.1.2.1 2002/06/17 12:55:53 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

public class hporb_1_2 extends ORBBase
{

public hporb_1_2 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "com.hp.mw.hporb.ORB", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "com.hp.mw.hporb.ORBSingleton", true);
    }
    
}

