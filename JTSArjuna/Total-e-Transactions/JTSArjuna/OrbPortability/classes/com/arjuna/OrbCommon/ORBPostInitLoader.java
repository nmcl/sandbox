/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBPostInitLoader.java,v 1.1 2000/02/25 14:09:20 nmcl Exp $
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

class ORBPostInitLoader extends ORBInitLoader
{

public ORBPostInitLoader ()
    {
	super("ORBPostInitLoader", ORBPostInitLoader.PostInitPropertyName);

	initialise();
    }

public static final String PostInitPropertyName = "ORBPostInit";
    
};
