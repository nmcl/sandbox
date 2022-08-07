/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBClassLoader.java,v 1.1 2000/02/25 14:09:16 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.lang.ClassLoader;
import java.util.Hashtable;

import java.lang.ClassNotFoundException;

class ORBClassLoader extends ClassLoader
{

public ORBClassLoader ()
    {
	loadedClasses = new Hashtable();
    }
    
protected Class loadClass (String className, boolean resolve) throws ClassNotFoundException
    {
	Class c = (Class) loadedClasses.get(className);

	if (c == null)
	{
	    c = findSystemClass(className);

	    // put it into hash table for later.
	    
	    loadedClasses.put(className, c);
	}

	// c must be set to get here!
	
	if (resolve)
	    resolveClass(c);

	return c;
    }
    
private Hashtable loadedClasses;
    
};
