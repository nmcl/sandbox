/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.1.2.1.4.1.20.1.4.1 2001/01/04 12:26:55 nmcl Exp $
 */









package com.arjuna.CosRecovery;

import com.arjuna.JavaHarness.ModuleInfo;
import java.util.Hashtable;

/**
 * Info object provides build information for this module
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
	
	_attributes.put(_keys[0], "CosRecovery" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "0.0.8" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:26 2001" );
	_attributes.put(_keys[5], "none" );
    }

public String[] keys ()
    {
	return _keys;
    }

    /*
     * Do not re-order!
     */

private String[]  _keys = { "MODULE", "SOURCE IDENTIFIER", "BUILD INFORMATION", "VERSION", "DATE", "NOTES" };
    
}



