/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbix2000_1_2.java,v 1.1.2.1 2001/01/19 15:26:29 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

public class orbix2000_1_2 extends ORBBase
{

public orbix2000_1_2 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "com.iona.corba.art.artimpl.ORBImpl", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "com.iona.corba.art.artimpl.ORBSingleton", true);
    }
    
}

