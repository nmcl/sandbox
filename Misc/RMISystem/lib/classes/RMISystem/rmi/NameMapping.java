/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameMapping.java,v 1.2 1997/06/08 14:22:54 nmcl Exp $
 */

package RMISystem.rmi;

import java.io.*;

import java.lang.ClassNotFoundException;
import java.io.FileNotFoundException;
import java.io.IOException;

public class NameMapping
{

public NameMapping ()
    {
	_mapFileName = NameMapping.defaultMap;
	_mapFile = null;
	_list = null;

	loadMapping();
    }
    
public NameMapping (String mapFile)
    {
	_mapFileName = mapFile;
	_mapFile = null;
	_list = null;

	loadMapping();
    }
	
public synchronized String getJavaCodeName (String objectName)
    {
	if (_list == null)
	    return null;
	
	for (int i = 0; i < _list.length; i++)
	{
	    if (_list[i].objectName().compareTo(objectName) == 0)
		return _list[i].javaExec();
	}

	return null;
    }

public synchronized boolean setJavaCodeName (String objectName, String javaExec)
    {
	int index = -1;
	
	if (_list != null)
	{
	    /*
	     * Check we're not already registered.
	     */
	    
	    for (int i = 0; i < _list.length; i++)
	    {
		if (_list[i] != null)
		{
		    if (_list[i].objectName().compareTo(objectName) == 0)
			return false;
		}
		else
		    index = i;  // remember slot
	    }
	}
	else
	{
	    _list = new MappingEntry[1];
	    index = 0;
	}
	
	if (index < 0)  // no spare slot!
	{
	    MappingEntry[] newList = new MappingEntry[_list.length+10]; // some extra slop

	    for (int i = 0; i < _list.length; i++)
	    {
		newList[i] = _list[i];
		_list[i] = null;
	    }

	    for (int j = _list.length; j < newList.length; j++)
		newList[j] = null;

	    index = _list.length;
	    _list = newList;
	}

	_list[index] = new MappingEntry(objectName, javaExec);

	return true;
    }

public synchronized boolean removeJavaCodeName (String objectName)
    {
	if (_list != null)
	{
	    /*
	     * Check we're registered.
	     */
	    
	    for (int i = 0; i < _list.length; i++)
	    {
		if (_list[i].objectName().compareTo(objectName) == 0)
		{
		    _list[i] = null;
		    return true;
		}
	    }
	}

	return false;
    }    

public synchronized void list (PrintStream strm)
    {
	strm.print("Mapping file: ");
	
	if (_list == null)
	    strm.print("empty.");
	else
	{
	    strm.println();
	    
	    for (int i = 0; i < _list.length; i++)
		strm.println("< "+_list[i].objectName()+", "+_list[i].javaExec()+" >");
	}
    }
    
public synchronized boolean clearMapping ()
    {
	if (_mapFile == null)
	    return false;
	else
	{
	    _list = null;
	    return _mapFile.delete();
	}
    }

public synchronized boolean loadMapping ()
    {
	_mapFile = new File(_mapFileName); // assume absolute path
	_list = null;

	try
	{
	    FileInputStream i = new FileInputStream(_mapFile);
	    ObjectInput o = new ObjectInputStream(i);
	    int number = o.read();

	    if (number > 0)
	    {
		_list = new MappingEntry[number];
	    
		for (int j = 0; j < number; j++)
		{
		    String objName = (String) o.readObject();
		    String execName = (String) o.readObject();
		
		    _list[j] = new MappingEntry(objName, execName);
		}
	    }

	    return true;
	}
	catch (ClassNotFoundException e)
	{
	    // can't find String?!
	}
	catch (FileNotFoundException e)
	{
	    System.err.println(e);
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}

	return false;
    }
    
public synchronized boolean saveMapping ()
    {
	try
	{
	    FileOutputStream f = new FileOutputStream(_mapFile);
	    ObjectOutput s = new ObjectOutputStream(f);

	    if (_list != null)
	    {
		s.write(_list.length);

		for (int i = 0; i < _list.length; i++)
		{
		    s.writeObject(_list[i].objectName());
		    s.writeObject(_list[i].javaExec());
		}

		s.flush();
	    }

	    return true;
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}

	return false;
    }

public static final String defaultMap = "/tmp/mapfile";
    
private String _mapFileName;
private File _mapFile;
private MappingEntry[] _list;

};
