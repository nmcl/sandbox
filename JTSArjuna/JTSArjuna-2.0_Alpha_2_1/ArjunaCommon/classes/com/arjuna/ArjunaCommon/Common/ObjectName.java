/*
 * Copyright (C) 1996, 1997, 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectName.java,v 1.1 2000/02/25 14:02:38 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import com.arjuna.ArjunaCommon.Interface.NameService;
import java.io.PrintStream;

import java.lang.NullPointerException;
import java.io.IOException;

public class ObjectName
{
    /*
     * All numbers are signed, but we leave this in for compatibility.
     */
    
public static final int SIGNED_NUMBER = 0;
public static final int UNSIGNED_NUMBER = 1;
public static final int STRING = 2;
public static final int OBJECTNAME = 3;
public static final int CLASSNAME = 4;
public static final int UID = 5;

public ObjectName (String objectName)
    {
	_mech = null;
	_name = null;
	_nameService = null;

	if (objectName == null)
	{
	    _mech = _invalidString;
	    _name = _invalidString;
	}
	else
	{
	    int mechLength = objectName.indexOf(nameDelim);

	    if (mechLength != -1)
	    {
		int nameLength = objectName.length() - mechLength - nameDelim.length();

		_mech = new String(objectName.substring(0, mechLength));

		if (nameLength > 0)
		    _name = objectName.substring((mechLength+nameDelim.length()), objectName.length());
		else
		    _name = new String("");
	    }
	}

	initNameService();
    }

public ObjectName (ObjectName objectName)
    {
	_mech = null;
	_name = null;
	_nameService = null;

	this.copy(objectName);
    }

public ObjectName (NameService nameService, String objectName)
    {
	_mech = null;
	_name = null;
	_nameService = null;
	
	if ((nameService != null) && (objectName != null))
	{
	    _nameService = nameService;

	    String fullMechName = _nameService.impleClassName().stringForm();
	    int appendedString = fullMechName.indexOf(nameServiceString);

	    _mech = new String(fullMechName.substring(0, appendedString-fullMechName.length()));
	    _name = new String(objectName);
	}
	else
	{
	    if (nameService == null)
		System.out.println("ERROR: ObjectName - no name service given.");
	
	    if (objectName == null)
		System.out.println("ERROR: ObjectName - no object name give.");

	    _mech = _invalidString;
	    _name = _invalidString;
	}
    }

public void finalize ()
    {
	_mech = null;
	_name = null;
	_nameService = null;
    }
    
public int attributeType (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.attributeType(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public String firstAttributeName () throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.firstAttributeName(_name);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public String nextAttributeName (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.nextAttributeName(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public long getLongAttribute (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.getLongAttribute(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public String getStringAttribute (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.getStringAttribute(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public ObjectName getObjectNameAttribute (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.getObjectNameAttribute(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public ClassName getClassNameAttribute (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.getClassNameAttribute(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public Uid getUidAttribute (String attrName) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.getUidAttribute(_name, attrName);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public void setLongAttribute (String attrName, long value) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    _name = _nameService.setLongAttribute(_name, attrName, value);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public void setStringAttribute (String attrName, String value) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    _name = _nameService.setStringAttribute(_name, attrName, value);
	else
	    throw(new NullPointerException("NameService implementation not set"));	
    }

public void setObjectNameAttribute (String attrName, ObjectName value) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    _name = _nameService.setObjectNameAttribute(_name, attrName, value);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public void setClassNameAttribute (String attrName, ClassName value) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    _name = _nameService.setClassNameAttribute(_name, attrName, value);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public void setUidAttribute (String attrName, Uid value) throws NullPointerException, IOException
    {
	if (_nameService != null)
	    _name = _nameService.setUidAttribute(_name, attrName, value);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public boolean removeAttribute (String attrName) throws IOException
    {
	if (_nameService != null)
	{
	    _name = _nameService.removeAttribute(_name, attrName);

	    if (_name != null)
		return true;
	}
	
	return false;
    }

public String uniqueAttributeName () throws NullPointerException, IOException
    {
	if (_nameService != null)
	    return _nameService.uniqueAttributeName(_name);
	else
	    throw(new NullPointerException("NameService implementation not set"));
    }

public static ObjectName uniqueObjectName (String mech) throws NullPointerException, IOException
    {
	NameService nameService = null;
	ObjectName objectName = ObjectName.invalid();

	if ((mech != null) && (mech.compareTo(_invalidString) != 0))
	{
	    String nameServiceMech = mech + nameServiceString;
	    ClassName nameServiceMechClassName = new ClassName(nameServiceMech);
	    nameService = NameService.create(nameServiceMechClassName);

	    if (nameService != null)
		objectName = nameService.uniqueObjectName();
	    
	    nameService = null;
	}

	return objectName;
    }

public synchronized void copy (ObjectName objectName)
    {
	if (objectName == this)
	    return;
	
	_nameService = null;
        _mech = null;
        _name = null;

	if (objectName._mech != null)
	{
	    _mech = new String(objectName._mech);

	    if (objectName._nameService != null)
		_nameService = new NameService(objectName._nameService);
	}

	if (objectName._name != null)
	    _name = new String(objectName._name);

	initNameService();
    }

public boolean equals (ObjectName objectName)
    {
	return (((_name == objectName._name) || 
		((_name != null) && (objectName._name != null) && (_name.compareTo(objectName._name) == 0))) &&
	    ((_mech == objectName._mech) ||
	     ((_mech != null) && (objectName._mech != null) && (_mech.compareTo(objectName._mech) == 0))));
    }

public boolean notEquals (ObjectName objectName)
    {
	return (((_name != objectName._name) && 
		((_name == null) || (objectName._name == null) || (_name.compareTo(objectName._name) != 0))) ||
	    ((_mech != objectName._mech) &&
	     ((_mech == null) || (objectName._mech == null) || (_mech.compareTo(objectName._mech) != 0))));
    }

public String getMechanismName ()
    {
	return _mech;
    }

public String stringForm ()
    {
	return new String(_mech + nameDelim + _name);
    }

public static ObjectName invalid ()
    {
	return _invalid;
    }

public void print (PrintStream strm)
    {
	strm.print("<ObjectName:"+_mech+nameDelim+_name+">");
    }

public String toString ()
    {
	return stringForm();
    }

public static void print (int t)
    {
	switch (t)
	{
	case ObjectName.SIGNED_NUMBER:
	    System.out.print("SIGNED_NUMBER");
	    break;
	case ObjectName.UNSIGNED_NUMBER:
	    System.out.print("UNSIGNED_NUMBER");
	    break;
	case ObjectName.STRING:
	    System.out.print("STRING");
	    break;
	case ObjectName.OBJECTNAME:
	    System.out.print("OBJECTNAME");
	    break;
	case ObjectName.CLASSNAME:
	    System.out.print("CLASSNAME");
	    break;
	case ObjectName.UID:
	    System.out.print("UID");
	    break;
	}
    }
    
private void initNameService ()
    {
	if (_nameService != null)
	    return;

	if ((_mech != null) && (_mech.compareTo(_invalidString) != 0))
	{
	    String nameServiceMech = new String(_mech + nameServiceString);
	    ClassName nameServiceMechClassName = new ClassName(nameServiceMech);
	    
	    _nameService = NameService.create(nameServiceMechClassName);
	    
	    nameServiceMech = null;
	}
	else
	    _nameService = null;
    }

private String _mech;
private String _name;
private NameService _nameService;
    
private static final String _invalidString = "invalid";
private static final String nameServiceString = "NameServiceImple";
private static final String nameDelim = ":";
private static final String nullString = "";
private static final ObjectName _invalid = new ObjectName("invalid:invalid");

}
