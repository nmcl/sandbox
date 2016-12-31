/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ModuleInfo.java,v 1.1 2000/04/06 09:58:37 nmcl Exp $
 */

package com.arjuna.JavaHarness;

public interface ModuleInfo
{

public void setPreString (String s);
public void getInfo ();
public String query (String attribute);
public String toString ();
public ModuleInfo[] constituents ();
 
};
