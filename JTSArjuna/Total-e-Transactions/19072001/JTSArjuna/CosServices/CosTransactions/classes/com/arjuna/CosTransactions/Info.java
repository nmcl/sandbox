/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.3.4.1.30.2.4.2 2001/01/04 12:27:39 nmcl Exp $
 */









package com.arjuna.CosTransactions;

import com.arjuna.JavaHarness.ModuleInfo;
import java.util.Hashtable;

/**
 * Module specific implementation of the information object.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Info.javatmpl,v 1.3.4.1.30.2.4.2 2001/01/04 12:27:39 nmcl Exp $
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
	
	_attributes.put(_keys[0], "CosTransactions" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "4.5.0" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:24 2001" );
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



