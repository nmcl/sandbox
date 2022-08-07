/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.3.2.1.2.1.26.1.2.1.2.2.2.1.50.1 2001/08/01 14:18:24 jcoleman Exp $
 */









package com.arjuna.JDBC2;

import com.arjuna.JavaHarness.ModuleInfo;
import java.util.Hashtable;

/**
 * Module specific implementation of the information object.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Info.javatmpl,v 1.3.2.1.2.1.26.1.2.1.2.2.2.1.50.1 2001/08/01 14:18:24 jcoleman Exp $
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
	
	_attributes.put(_keys[0], "JDBC2" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "1.0.1" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:31 2001" );
	_attributes.put(_keys[5], "none" );

	String supportedVersions = "JNDI";
	






	supportedVersions += ", Sequelink_5_1";


	_attributes.put(_keys[6], supportedVersions);
    }    

public String[] keys ()
    {
	return _keys;
    }

    /*
     * Do not re-order!
     */

private String[]  _keys = { "MODULE", "SOURCE IDENTIFIER", "BUILD INFORMATION", "VERSION", "DATE", "NOTES", "SUPPORTED DRIVERS" };
   
}

