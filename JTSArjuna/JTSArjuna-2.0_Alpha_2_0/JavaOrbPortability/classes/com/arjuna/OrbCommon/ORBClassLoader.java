/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBClassLoader.java,v 1.3 1998/07/15 15:30:11 nmcl Exp $
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
