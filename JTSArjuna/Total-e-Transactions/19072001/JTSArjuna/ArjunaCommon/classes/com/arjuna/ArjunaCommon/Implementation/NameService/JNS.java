/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JNS.java,v 1.1.28.2.4.1 2000/12/21 11:20:53 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation.NameService;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.ArjunaCommon.Implementation.NameServiceImple;
import com.arjuna.ArjunaCommon.Interface.NameService;
import java.util.*;

import java.io.IOException;
import java.lang.NumberFormatException;
import java.lang.StringIndexOutOfBoundsException;

/**
 * We have access to *all* attributes at this level, e.g., java.home
 * Therefore, we can use this format to impose structure on the
 * environment variables, and use the object name, e.g., if the
 * object name is foobar, then we can scan for all attributes with
 * the format foobar. and just return those.
 *
 * Assume attribute names are unique.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JNS.java,v 1.1.28.2.4.1 2000/12/21 11:20:53 nmcl Exp $
 * @since JTS 1.0.
 */

public class JNS extends NameServiceImple
{

public JNS ()
    {
    }

public int attributeType (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr == null)
	    throw new IOException("No such attribute.");
	else
	{
	    if (attr.charAt(0) == JNS.SIGNED_NUMBER)
		return ObjectName.SIGNED_NUMBER;
	    else if (attr.charAt(0) == JNS.UNSIGNED_NUMBER)
		return ObjectName.UNSIGNED_NUMBER;
	    else if (attr.charAt(0) == JNS.STRING)
		return ObjectName.STRING;
	    else if (attr.charAt(0) == JNS.OBJECTNAME)
		return ObjectName.OBJECTNAME;
	    else if (attr.charAt(0) == JNS.CLASSNAME)
		return ObjectName.CLASSNAME;
	    else if (attr.charAt(0) == JNS.UID)
		return ObjectName.UID;
	    else
		throw new IOException("Unknown attribute type.");
	}
    }

public String firstAttributeName (String objName) throws IOException
    {
	Enumeration e = PropertyManager.propertyNames();
	
	if (e.hasMoreElements())
	    return (String) e.nextElement();
	else
	    throw new IOException("No attributes.");
    }
    
public String nextAttributeName (String objName, String attrName) throws IOException
    {
	Enumeration e = PropertyManager.propertyNames();
	
	while (e.hasMoreElements())
	{
	    String s = (String) e.nextElement();
	    
	    if (s.equals(objName+"."+attrName))
	    {
		if (e.hasMoreElements())
		    return (String) e.nextElement();
	    }
	}
	
	throw new IOException("No more attributes.");
    }

public long getLongAttribute (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr != null)
	{
	    if (attr.charAt(0) == JNS.SIGNED_NUMBER)
	    {
		try
		{
		    return Long.parseLong(new String(attr.substring(1)));
		}
		catch (NumberFormatException e)
		{
		    throw new IOException("Not a number.");
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    throw new IOException("Not a number.");
		}
	    }
	    else
		throw new IOException("Not a signed number.");
	}
	else
	    throw new IOException("No such attribute.");
    }
    
public String getStringAttribute (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr != null)
	{
	    if (attr.charAt(0) == JNS.STRING)
	    {
		try
		{
		    return new String(attr.substring(1));
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    throw new IOException("No string.");
		}
	    }
	    else
		throw new IOException("Not a string.");
	}
	else
	    throw new IOException("No such attribute.");
    }

public ObjectName getObjectNameAttribute (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr != null)
	{
	    if (attr.charAt(0) == JNS.OBJECTNAME)
	    {
		try
		{
		    return new ObjectName(new String(attr.substring(1)));
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    throw new IOException("No ObjectName.");
		}
	    }
	    else
		throw new IOException("Not an ObjectName.");
	}
	else
	    throw new IOException("No such attribute.");	
    }
    
public ClassName getClassNameAttribute (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr != null)
	{
	    if (attr.charAt(0) == JNS.CLASSNAME)
	    {
		try
		{
		    return new ClassName(new String(attr.substring(1)));
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    throw new IOException("No ClassName.");
		}
	    }
	    else
		throw new IOException("Not a ClassName.");
	}
	else
	    throw new IOException("No such attribute.");	
    }
    
public Uid getUidAttribute (String objName, String attrName) throws IOException
    {
	String attr = PropertyManager.getProperty(objName+"."+attrName, null);

	if (attr != null)
	{
	    if (attr.charAt(0) == JNS.UID)
	    {
		try
		{
		    return new Uid(new String(attr.substring(1)));
		}
		catch (StringIndexOutOfBoundsException e)
		{
		    throw new IOException("No Uid.");
		}
	    }
	    else
		throw new IOException("Not a Uid.");
	}
	else
	    throw new IOException("No such attribute.");	
    }
    
public String setLongAttribute (String objName, String attrName, long value) throws IOException
    {
	PropertyManager.setProperty(objName+"."+attrName, new String(SIGNED_NUMBER+""+value));

	return null;
    }
    
public String setStringAttribute (String objName, String attrName, String value) throws IOException
    {
	PropertyManager.setProperty(objName+"."+attrName, new String(STRING+""+value));

	return null;
    }
    
public String setObjectNameAttribute (String objName, String attrName, ObjectName value) throws IOException
    {
	PropertyManager.setProperty(objName+"."+attrName, new String(OBJECTNAME+""+value.stringForm()));

	return null;
    }
    
public String setClassNameAttribute (String objName, String attrName, ClassName value) throws IOException
    {
	PropertyManager.setProperty(objName+"."+attrName, new String(CLASSNAME+""+value.stringForm()));

	return null;
    }
    
public String setUidAttribute (String objName, String attrName, Uid value) throws IOException
    {
	PropertyManager.setProperty(objName+"."+attrName, new String(CLASSNAME+""+value.stringForm()));

	return null;
    }

public String removeAttribute (String objName, String attrName) throws IOException
    {
	return PropertyManager.removeProperty(objName+"."+attrName);
    }
    
public String uniqueAttributeName (String objName) throws IOException
    {
	Uid u = new Uid();

	return u.stringForm();
    }
    
public ObjectName uniqueObjectName () throws IOException
    {
	Uid uid = new Uid();
	ObjectName uniqueName = new ObjectName(JNS.jnsName+uid.stringForm());

	return uniqueName;
    }

public Object clone ()
    {
	return new JNS();
    }

public ClassName className ()
    {
	return ArjunaCommonNames.Implementation_NameService_JNS();
    }

public static ClassName name ()
    {
	return ArjunaCommonNames.Implementation_NameService_JNS();
    }

private static final char SIGNED_NUMBER = '#';
private static final char UNSIGNED_NUMBER = '~';
private static final char STRING = '^';
private static final char OBJECTNAME = '%';
private static final char CLASSNAME = '-';
private static final char UID = '+';

private static final String jnsName = "JNS:";
    
}
