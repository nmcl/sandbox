/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ClassName.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 */

package com.arjuna.ats.arjuna.gandiva;

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.state.*;
import java.io.IOException;

import com.arjuna.ats.arjuna.logging.tsLogger;

/**
 * Each implementation type may provide an instance of this class
 * to uniquely identify itself to its users and the Inventory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ClassName.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 * @since JTS 1.0.
 */

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
    
    /** 
     * @message com.arjuna.ats.arjuna.gandiva.ClassName_1 [com.arjuna.ats.arjuna.gandiva.ClassName_1] - ClassName.copy - no class name available!
     */

public void copy (ClassName className)
    {
	if (this == className)
	    return;
	
	if (className._className != null)
	    _className = new String(className._className);
	else
	{
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.gandiva.ClassName_1");
	}
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
