/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappingEntry.java,v 1.1 1997/05/31 13:32:00 nmcl Exp $
 */

package RMISystem.rmi;

class MappingEntry
{

public MappingEntry (String objName, String jExec)
    {
	_objectName = objName;
	_javaExec = jExec;
    }

public String objectName ()
    {
	return _objectName;
    }

public String javaExec ()
    {
	return _javaExec;
    }

private String _objectName;
private String _javaExec;
    
};
