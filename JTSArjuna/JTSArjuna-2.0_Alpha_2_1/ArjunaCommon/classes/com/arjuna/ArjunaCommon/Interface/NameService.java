/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: NameService.java,v 1.2.4.1 2000/04/25 08:35:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Interface;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Implementation.NameServiceImple;

import java.io.IOException;
import java.lang.NullPointerException;

public class NameService
{

public NameService (ClassName nameServiceImpleName)
    {
	Inventory invent = Inventory.inventory();

	if (invent != null)
	{
	    Object ptr = invent.createVoid(nameServiceImpleName);
	    
	    if (ptr instanceof NameServiceImple)
		_imple = (NameServiceImple) ptr;
	    else
		_imple = null;
	}
    }

public NameService (NameService nameService)
    {
	if (nameService._imple != null)
	{
	    _imple = (NameServiceImple) nameService._imple.clone();
	}
	else
	    _imple = null;
    }

public NameService (NameServiceImple nameServiceImple)
    {
	_imple = nameServiceImple;
    }

public void finalize ()
    {
	_imple = null;
    }

public int attributeType (String objName, String attr) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.attributeType(objName, attr);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String firstAttributeName (String objName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.firstAttributeName(objName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String nextAttributeName (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.nextAttributeName(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public long getLongAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.getLongAttribute(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String getStringAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.getStringAttribute(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public ObjectName getObjectNameAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.getObjectNameAttribute(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public ClassName getClassNameAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.getClassNameAttribute(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public Uid getUidAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.getUidAttribute(objName, attrName);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String setLongAttribute (String objName, String attrName, long value) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.setLongAttribute(objName, attrName, value);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String setStringAttribute (String objName, String attrName, String value) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.setStringAttribute(objName, attrName, value);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String setObjectNameAttribute (String objName, String attrName, ObjectName value) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.setObjectNameAttribute(objName, attrName, value);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String setClassNameAttribute (String objName, String attrName, ClassName value) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.setClassNameAttribute(objName, attrName, value);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String setUidAttribute (String objName, String attrName, Uid value) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.setUidAttribute(objName, attrName, value);
	else
	    throw new NullPointerException("Implementation not set");
    }

public String removeAttribute (String objName, String attrName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.removeAttribute(objName, attrName);
	else
	    throw(new NullPointerException("Implementation not set"));
    }

public String uniqueAttributeName (String objName) throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.uniqueAttributeName(objName);
	else
	    throw(new NullPointerException("Implementation not set"));
    }

public ObjectName uniqueObjectName () throws NullPointerException, IOException
    {
	if (_imple != null)
	    return _imple.uniqueObjectName();
	else
	    throw(new NullPointerException("Implementation not set"));	    
    }

public void copy (NameService toCopy)
    {
	if (this == toCopy)
	    return;
    
	_imple = (NameServiceImple) toCopy._imple.clone();
    }
	
public static ClassName name ()
    {
	return ArjunaCommonNames.Interface_NameService();
    }

public ClassName className ()
    {
	return null;
    }

public ClassName impleClassName ()
    {
	if (_imple != null)
	    return _imple.className();
	else
	    return ClassName.invalid();
    }

public NameService castup (ClassName theType)
    {
	if (theType.equals(className()))
	    return this;
	else
	    return null;
    }
    
public static NameService create (ClassName nameServiceImpleName)
    {
	NameService res = null;
	Inventory invent = Inventory.inventory();

	if (invent != null)
	{
	    NameServiceImple nameServiceImple = null;
	    Object ptr = invent.createVoid(nameServiceImpleName);

	    if (ptr instanceof NameServiceImple)
		nameServiceImple = (NameServiceImple) ptr;
	    else
		nameServiceImple = null;
	
	    if (nameServiceImple != null)
		res = new NameService(nameServiceImple);
	}

	return res;
    }

private NameServiceImple _imple;
    
}
