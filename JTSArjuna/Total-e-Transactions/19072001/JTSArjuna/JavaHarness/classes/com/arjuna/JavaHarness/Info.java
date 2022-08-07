/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.3.2.1.28.1.4.2 2001/01/04 12:26:03 nmcl Exp $
 */










package com.arjuna.JavaHarness;

import java.util.Hashtable;

/**
 * The JavaHarness implementation of com.arjuna.JavaHarness.ModuleInfo
 * interface. Can be used to query the module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Info.javatmpl,v 1.3.2.1.28.1.4.2 2001/01/04 12:26:03 nmcl Exp $
 * @since JTS 2.0.
 * @see com.arjuna.JavaHarness.ModuleInfo
 */

public class Info extends ModuleInfo
{

public Info ()
    {
	this("");
    }
    
public Info (String s)
    {
	_preString  = s;

	_attributes = new Hashtable();

	_attributes.put(_keys[0], "JavaHarness" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "1.4.3" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:15 2001" );
	_attributes.put(_keys[5], "none" );
	_attributes.put(_keys[6], "1_3" );
    }
    
public String[] keys ()
    {
	return _keys;
    }

    /*
     * Do not re-order!
     */

private String[]  _keys = { "MODULE", "SOURCE IDENTIFIER", "BUILD INFORMATION", "VERSION", "DATE", "NOTES", "JDK VERSION" };
    
}
