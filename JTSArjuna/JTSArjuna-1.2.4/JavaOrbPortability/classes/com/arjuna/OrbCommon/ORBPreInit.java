/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBPreInit.java,v 1.4 1998/11/12 08:52:36 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.util.Properties;
import java.util.Enumeration;
import java.lang.ClassLoader;

import java.lang.ClassNotFoundException;

/*
 * This class allows the programmer (and us!) to register classes
 * which must be instantiated prior to ORB initialisation.
 */

class ORBPreInit extends ORBInitLoader
{

public ORBPreInit ()
    {
	super("ORBPreInit", ORBPreInit.PreInitPropertyName);

	initialise();
    }

public static final String PreInitPropertyName = "ORBPreInit";
    
};
