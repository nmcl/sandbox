/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Info.javatmpl,v 1.1 2001/07/23 13:44:51 nmcl Exp $
 */









package com.arjuna.Activity;

import com.arjuna.JavaHarness.ModuleInfo;
import java.io.*;
import java.util.*;

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
	
	_attributes.put(_keys[0], "XOTS" );
	_attributes.put(_keys[1], "unknown" );
	_attributes.put(_keys[2], "Arjuna Solutions Ltd. [unknown]" );
	_attributes.put(_keys[3], "1.0.0" );
	_attributes.put(_keys[4], "Thu Sep 20 13:59:42 2001" );
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
