/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.1.4.2.2.2.4.2 2001/01/04 12:32:58 nmcl Exp $
 */









package com.arjuna.ArjunaCore;

import com.arjuna.JavaHarness.ModuleInfo;
import java.util.Hashtable;

/**
 * Module specific implementation of the com.arjuna.JavaHarness.ModuleInfo
 * interface. Can be used to query the module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Info.javatmpl,v 1.1.4.2.2.2.4.2 2001/01/04 12:32:58 nmcl Exp $
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
	
	_attributes.put(_keys[0], "ArjunaCore" );
	_attributes.put(_keys[1], "unkown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "4.7.0" );
	_attributes.put(_keys[4], "Thu Sep 20 13:40:17 2001" );
	_attributes.put(_keys[5], "none" );
	_attributes.put(_keys[6], com.arjuna.ArjunaCore.Common.Configuration.objectStoreRoot());
	_attributes.put(_keys[7], com.arjuna.ArjunaCore.Common.Configuration.lockStoreRoot());
    }

public String[] keys ()
    {
	return _keys;
    }

    /*
     * Do not re-order!
     */

private String[]  _keys = { "MODULE", "SOURCE IDENTIFIER", "BUILD INFORMATION", "VERSION", "DATE", "NOTES", "OBJECTSTORE ROOT", "LOCKSTORE ROOT" };

}



