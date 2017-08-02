/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

public class RefComponent implements EditorComponent
{

public RefComponent ()
    {
	myString = " \npublic void finalize ()\n";
	myString += "{\n\ttry\n\t{\n\t\tthis.unref(this.objectName);\n";
	myString += "\t}\n\tcatch (Exception e)\n\t{\n\t}\n}\n";
    }
    
public String tokenToFind ()
    {
	return "    // Methods from remote interfaces";
    }
    
public String tokenToUse ()
    {
	return myString;
    }

public boolean replace ()
    {
	return false;
    }

public boolean insertBefore ()
    {
	return true;
    }

public String componentName ()
    {
	return null;
    }
    
private String myString;
    
};
