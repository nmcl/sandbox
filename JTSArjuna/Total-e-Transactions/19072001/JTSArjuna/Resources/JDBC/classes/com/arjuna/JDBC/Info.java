/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.3.2.1.2.2.26.2.4.2.2.2 2001/02/02 12:35:57 nmcl Exp $
 */









package com.arjuna.JDBC;

import com.arjuna.JavaHarness.ModuleInfo;
import java.util.Hashtable;

/**
 * Module specific implementation of the information object.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Info.javatmpl,v 1.3.2.1.2.2.26.2.4.2.2.2 2001/02/02 12:35:57 nmcl Exp $
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
	
	_attributes.put(_keys[0], "JDBC" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "4.0.0" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:29 2001" );
	_attributes.put(_keys[5], "none" );
	
	String supportedVersions = "";
	


	if (!supportedVersions.equals(""))
	    supportedVersions += ", ";

	supportedVersions += "Sequelink_5_1";


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


