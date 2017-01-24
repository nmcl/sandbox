/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBPostInit.java,v 1.4 1998/11/12 08:52:36 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.util.Properties;
import java.util.Enumeration;
import java.lang.ClassLoader;

import java.lang.ClassNotFoundException;

/*
 * This class allows the programmer (and us!) to register classes
 * which must be instantiated after ORB initialisation.
 */

class ORBPostInit extends ORBInitLoader
{

public ORBPostInit ()
    {
	super("ORBPostInit", ORBPostInit.PostInitPropertyName);

	initialise();
    }

public static final String PostInitPropertyName = "ORBPostInit";
    
};
