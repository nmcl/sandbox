/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoveryClassLoader.java,v 1.1.2.1 2000/07/27 16:12:10 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import java.lang.ClassLoader;
import java.util.Hashtable;

import java.lang.ClassNotFoundException;

public class RecoveryClassLoader extends ClassLoader
{

    public RecoveryClassLoader ()
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
