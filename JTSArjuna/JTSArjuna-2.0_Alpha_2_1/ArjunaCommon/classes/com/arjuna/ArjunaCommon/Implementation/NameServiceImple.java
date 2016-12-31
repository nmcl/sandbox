/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: NameServiceImple.java,v 1.1 2000/02/25 14:02:45 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation;

import com.arjuna.ArjunaCommon.Common.*;
import java.lang.Cloneable;

import java.io.IOException;

public abstract class NameServiceImple implements Cloneable
{
    
public abstract int attributeType (String objName, String attrName) throws IOException;
public abstract String firstAttributeName (String objName) throws IOException;
public abstract String nextAttributeName (String objName, String attrName) throws IOException;

public abstract long getLongAttribute (String objName, String attrName) throws IOException;
public abstract String getStringAttribute (String objName, String attrName) throws IOException;
public abstract ObjectName getObjectNameAttribute (String objName, String attrName) throws IOException;
public abstract ClassName getClassNameAttribute (String objName, String attrName) throws IOException;
public abstract Uid getUidAttribute (String objName, String attrName) throws IOException;

public abstract String setLongAttribute (String objName, String attrName, long value) throws IOException;
public abstract String setStringAttribute (String objName, String attrName, String value) throws IOException;
public abstract String setObjectNameAttribute (String objName, String attrName, ObjectName value) throws IOException;
public abstract String setClassNameAttribute (String objName, String attrName, ClassName value) throws IOException;
public abstract String setUidAttribute (String objName, String attrName, Uid value) throws IOException;

public abstract String removeAttribute (String objName, String attrName) throws IOException;
public abstract String uniqueAttributeName (String objName) throws IOException;
public abstract ObjectName uniqueObjectName () throws IOException;

public abstract Object clone ();

public ClassName className ()
    {
	return NameServiceImple._className;
    }

public static ClassName name ()
    {
	return NameServiceImple._className;
    }
    
private static final ClassName _className = new ClassName("NameServiceImple");
    
}
