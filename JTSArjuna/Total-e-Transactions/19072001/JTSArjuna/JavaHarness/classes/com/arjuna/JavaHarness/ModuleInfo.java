/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ModuleInfo.java,v 1.1.28.1.4.2 2001/01/04 12:26:03 nmcl Exp $
 */

package com.arjuna.JavaHarness;

import java.util.Hashtable;

import java.lang.IllegalArgumentException;

/**
 * Each module within a product line provides an implementation
 * of this interface, which can be used to obtain information
 * about the module. General module information may be obtained simply
 * by calling toString. However, more specific information can be
 * obtained by querying the implementation about specific module values.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ModuleInfo.java,v 1.1.28.1.4.2 2001/01/04 12:26:03 nmcl Exp $
 * @since JTS 2.0.
 */

public abstract class ModuleInfo
{

    /**
     * When toString() is called the parameter in this method is pre-pended
     * to the string output. This method should obviously be called prior
     * to using toString().
     */

public void setPreString (String s)
    {
	_preString = s;
    }

/** 
 * Prints information about the module to standard output.
 */

public void getInfo ()
    {
	System.out.println(toString());
    }

/**
 * Returns information about the specified module attribute. If the
 * attribute does not exist then the IllegalArgumentException is thrown.
 */

public String query (String attribute) throws IllegalArgumentException
    {
	return (String) _attributes.get(attribute);
    }

/**
 * Overrides Object.toString().
 */
 
public String toString ()
    {
	String val = null;
	String[] theKeys = keys();
	
	for (int i = 0; i < theKeys.length; i++)
	{
	    String attr = _preString+theKeys[i]+": "+(String) _attributes.get(theKeys[i]);

	    if (val == null)
		val = attr;
	    else
		val = val+"\n"+attr;
	}

	return val;
    }

/**
 * If ths module contains sub-modules, then their ModuleInfo implementations
 * will be returned.
 */
 
public ModuleInfo[] constituents ()
    {
	return null;
    }

/**
 * Returns the queryable values for this module.
 */
 
public abstract String[] keys ();

protected String    _preString;
protected Hashtable _attributes;
    
}
