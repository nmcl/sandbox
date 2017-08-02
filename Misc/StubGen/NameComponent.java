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

public class NameComponent implements EditorComponent
{

public String tokenToFind ()
    {
	return "    // Constructors";
    }
    
public String tokenToUse ()
    {
	return "public String objectName = \"\";\n";
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
    
};
