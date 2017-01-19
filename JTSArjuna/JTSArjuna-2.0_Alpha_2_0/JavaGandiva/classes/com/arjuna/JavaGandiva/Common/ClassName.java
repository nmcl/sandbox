/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassName.java,v 1.3 1998/07/06 13:30:44 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Common;

import java.io.IOException;

public class ClassName
{

public ClassName ()
    {
	this.copy(ClassName.invalid());
    }
    
public ClassName (String className)
    {
	_className = null;
    
	if (className != null)
	    _className = new String(className);
	else
	    this.copy(ClassName.invalid());
    }
    
public ClassName (ClassName name)
    {
	_className = null;

	this.copy(name);
    }

public static ClassName invalid()
    {
	return _invalid;
    }

public String stringForm ()
    {
	return _className;
    }

public void pack (OutputBuffer buff) throws IOException
    {
	buff.packString(_className);
    }

public void unpack (InputBuffer buff) throws IOException
    {
	_className = buff.unpackString();
    }
    
public void copy (ClassName className)
    {
	if (this == className)
	    return;
	
	if (className._className != null)
	    _className = new String(className._className);
	else
	    System.out.println("ClassName.copy - no class name available!");
    }

public boolean equals (ClassName className)
    {
	if (_className != className._className)
	    return (_className.compareTo(className._className) == 0);
	else
	    return true;
    }

public boolean notEquals (ClassName className)
    {
	if (_className == className._className)
	    return false;
	else
	    return (_className.compareTo(className._className) != 0);
    }

public String toString ()
    {
	return new String("<ClassName:"+_className+">");
    }

private String _className;
    
private static final ClassName _invalid = new ClassName("$Invalid");

}
